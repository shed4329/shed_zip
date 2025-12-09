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

    // 新增：Code Length 樹的長度讀取順序
    const int InflateDecompressor::cl_code_order[] = {
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
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
            }else if(btype == 2){
                success = process_dynamic_block(reader,output);
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

    bool InflateDecompressor::process_dynamic_block(BitReader& reader, shed_std::Vvector<uint8_t>& output) {
        // 1. 讀取 Header (HLIT, HDIST, HCLEN)
        if (!reader.has_bits(5 + 5 + 4)) return false;
        int hlit = reader.read_bits(5) + 257;   // Literal/Length codes 數量 (257-286)
        int hdist = reader.read_bits(5) + 1;    // Distance codes 數量 (1-32)
        int hclen = reader.read_bits(4) + 4;    // Code Length codes 數量 (4-19)

        // 2. 讀取 Code Length 樹的長度表
        shed_std::Vvector<int> cl_lengths(19);
        cl_lengths.fill(0);
        
        for (int i = 0; i < hclen; ++i) {
            if (!reader.has_bits(3)) return false;
            cl_lengths[cl_code_order[i]] = reader.read_bits(3);
        }

        // 3. 構建 Code Length 樹 (用於解碼後續的樹)
        HuffmanTable cl_table;
        if (!cl_table.build(cl_lengths)) return false;

        // 4. 解碼 Literal/Length 樹和 Distance 樹的長度表
        // 這兩個表是連續存儲的，總共需要讀取 hlit + hdist 個長度
        shed_std::Vvector<int> all_lengths; 
        all_lengths.reserve(hlit + hdist); // 預留空間
        
        int total_codes = hlit + hdist;
        while (all_lengths.size() < total_codes) {
            int sym = cl_table.decode(reader);
            if (sym < 0) return false; // 解碼錯誤

            if (sym <= 15) {
                // 0-15: 直接表示長度
                all_lengths.push_back(sym);
            } else if (sym == 16) {
                // 16: 複製前一個長度 3-6 次
                if (all_lengths.empty()) return false; // 沒有前一個
                int prev = all_lengths.back();
                if (!reader.has_bits(2)) return false;
                int repeat = reader.read_bits(2) + 3;
                for (int i = 0; i < repeat; ++i) all_lengths.push_back(prev);
            } else if (sym == 17) {
                // 17: 重複 0 (3-10 次)
                if (!reader.has_bits(3)) return false;
                int repeat = reader.read_bits(3) + 3;
                for (int i = 0; i < repeat; ++i) all_lengths.push_back(0);
            } else if (sym == 18) {
                // 18: 重複 0 (11-138 次)
                if (!reader.has_bits(7)) return false;
                int repeat = reader.read_bits(7) + 11;
                for (int i = 0; i < repeat; ++i) all_lengths.push_back(0);
            } else {
                return false; // 異常符號
            }
        }

        if (all_lengths.size() != total_codes) return false; // 長度不匹配

        // 5. 分割並構建主樹
        shed_std::Vvector<int> ll_lengths;
        for (int i = 0; i < hlit; ++i) ll_lengths.push_back(all_lengths[i]);
        
        shed_std::Vvector<int> dist_lengths;
        for (int i = hlit; i < total_codes; ++i) dist_lengths.push_back(all_lengths[i]);

        HuffmanTable ll_table;
        HuffmanTable dist_table;

        if (!ll_table.build(ll_lengths)) return false;
        if (!dist_table.build(dist_lengths)) return false; // 只有當 hdist > 0 時才需要 build 成功? 規範說只有1個距離碼且長度0時才允許空

        // 6. 解碼實際壓縮數據 (類似 fixed block，但使用動態表)
        while (true) {
            int symbol = ll_table.decode(reader);
            if (symbol < 0) return false;

            if (symbol < 256) {
                // Literal
                output.push_back((uint8_t)symbol);
            } else if (symbol == 256) {
                // End of Block
                break;
            } else {
                // Match (Length + Distance)
                int len_idx = symbol - 257;
                if (len_idx > 28) return false;

                // 讀取長度
                int length = length_base[len_idx];
                int extra_len = length_extra_bits[len_idx];
                if (extra_len > 0) {
                    if (!reader.has_bits(extra_len)) return false;
                    length += reader.read_bits(extra_len);
                }

                // 讀取距離
                int dist_code = dist_table.decode(reader);
                if (dist_code < 0 || dist_code > 29) return false;

                int distance = dist_base[dist_code];
                int extra_dist = dist_extra_bits[dist_code];
                if (extra_dist > 0) {
                    if (!reader.has_bits(extra_dist)) return false;
                    distance += reader.read_bits(extra_dist);
                }

                copy_match(output, length, distance);
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