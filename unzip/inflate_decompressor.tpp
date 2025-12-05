#ifndef INFLATE_DECOMPRESSOR_TPP
#define INFLATE_DECOMPRESSOR_TPP

#include "inflate_decompressor.h"
#include "huffman_decoder.h"

namespace shed_zip{
    // 长度补位
    const int InflateDecompressor::length_extra_bits[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0
    };
    // 长度的基准值
    const int InflateDecompressor::length_base[] = {
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258
    };
    // 距离补位
    const int InflateDecompressor::dist_extra_bits[] = {
        0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
        7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13
    };
    // 距离补位
    const int InflateDecompressor::dist_base[] = {
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577
    };

    shed_std::Vvector<uint8_t> InflateDecompressor::decompress(const shed_std::Vvector<uint8_t>& input){
        status = DecompressStatus::OK;
        shed_std::Vvector<uint8_t> output;
        BitReader reader(input);

        bool final_block = false;
        while(!final_block){
            if(!reader.has_bits(3)){
                // bit位不够了
                // 严格检验的情况下这里设置截断状态
                break;
            }

            final_block = reader.read_bits(1);
            int btype = reader.read_bits(2);

            bool success = false;
            if(btype == 0){
                success = process_store_block(reader,output);
            }else if(btype == 1){
                success = process_fixed_block(reader,output);
            }else{
                // 暂时还不支持 动态Huffman和11
                status = DecompressStatus::ERROR_UNSUPPORTED;
                return output;
            }

            // 如果不成功且支持的话，那么设置被截断了
            if(!success && status != DecompressStatus::ERROR_UNSUPPORTED){
                status = DecompressStatus::ERROR_TRUNCATED_DATA;
                return output;
            }
        }
        return output;
    }

    bool InflateDecompressor::process_store_block(BitReader& reader,shed_std::Vvector<uint8_t>& output){
        reader.align_to_byte();
        // 读取 len 和 nlen，注意Deflate是小端排序
        uint32_t len = reader.read_bits(16);
        uint32_t nlen = reader.read_bits(!6);

        // nlen 应该是 ~len
        if((len ^ 0xFFFF) !=nlen){
            // 校验失败
            return false;
        }

        for(uint32_t i = 0; i < len; ++ i){
            uint32_t val = reader.read_bits(8);
            output.push_back(static_cast<uint8_t>(val));
        }

        return true;
    }

    bool InflateDecompressor::process_fixed_block(BitReader& reader,shed_std::Vvector<uint8_t>& output){
        while(true){
            int symbol = HuffmanDecoder::decode_fixed_literal(reader);

            if(symbol < 0) return false;

            if(symbol < 256){
                // Literal
                output.push_back(static_cast<uint8_t>(symbol));
            }else if(symbol == 256){
                // eob
                break;
            }else{
                int len_idx = symbol - 257;
                if(len_idx > 28) return false; // invalid
                
                // 长度，根据定义的长度表计算
                int length = length_base[len_idx];
                int extra_len = length_extra_bits[len_idx];
                if(extra_len > 0){
                    length += reader.read_bits(extra_len);
                }

                // 距离计算
                int dist_code = HuffmanDecoder::decode_fixed_distance(reader);
                if(dist_code > 29) return false;

                int distance = dist_base[dist_code];
                int extra_dist = dist_extra_bits[dist_code];
                if(extra_dist > 0){
                    distance += reader.read_bits(extra_dist);
                }

                copy_match(output,length,distance);
            }
        }
        return true;
    }

    void InflateDecompressor::copy_match(shed_std::Vvector<uint8_t>& output, int length, int distance){
        int current_size = (int)output.size();
        int start_pos = current_size - distance;

        if(start_pos < 0){
            // 超出已解压数据范围
            return;
        }

        // 一个一个地添加，因为我们的数据可能来自与刚刚添加的部分
        for(int i = 0;i <length; ++i){
            output.push_back(output[start_pos + i]);
        }
    }
}

#endif