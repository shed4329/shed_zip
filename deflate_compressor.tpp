#ifndef DEFLATE_COMPRESSOR_TPP
#define DEFLATE_COMPRESSOR_TPP

#include "deflate_compressor.h"

namespace shed_zip{
    DeflateCompressor::DeflateCompressor(ZipConfig cfg):config(cfg){}

    shed_std::Vvector<uint8_t> DeflateCompressor::compress(const shed_std::Vvector<uint8_t>& input){
        // TODO:这个地方直接Store需要更改
        HuffmanEncoder huffman;
        LZ77Matcher lz77(config);

        // 写入Deflate标头
        huffman.write_block_header(true);

        int pos = 0;
        int limit = (int)input.size();

        while(pos < limit){
            Match match = lz77.find_longest_match(input,pos);

            if(match.found){
                huffman.write_match(match.length,match.distance);

                // Lazy Update：跳过的字节也需要插入hash table
                // 压缩等级越高，需要越勤地更新hash
                if(config.level >= 5){
                    // 插入位置是否能构成三字节
                    for(int i = 1;i<match.length && pos + i+2 <limit ; ++i){
                        lz77.insert_hash(input,pos+i);
                    }
                }

                pos += match.length;
            }else{
                huffman.write_literal(input[pos]);
                pos++;
            }
        }

        huffman.write_eob();
        huffman.flush_byte_align();

        return huffman.get_buffer();

    }
}

#include "deflate_compressor.tpp"

#endif // DEFLATE_COMPRESSOR_TPP