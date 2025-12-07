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

    uint32_t DeflateCompressor::estimate_store_size(){
        // TODO:store 最大只能存65535 bytes
        // Header ( 3 nits) + Padding(0-7bits) + LEN/NLEN(32bits) + Data(bytes * 8)
        return 3+7+32+(uint32_t)literal_buffer.size()*8;
    }

    uint32_t DeflateCompressor::estimate_fixed_size(){
        uint32_t bits = 3; // Header
        // 遍历频率表计算
        const auto& ll_freqs = freq_collector.get_lit_len_freqs();
        const auto& d_freqs = freq_collector.get_dist_freqs();

        // TODO 实现长度计算
        return 0xFFFFFFFF;
    }

    uint32_t DeflateCompressor::estimate_dynamic_size(const HuffmanTree& l_tree,const HuffmanTree& d_tree){
        uint32_t bits = 3; // header
        // Header Size(HLIT,HDIST,HCLEN) + Code Length Tree + RLE Data
        // 先估计为100bytes吧

        const auto& ll_lens = l_tree.get_bit_lengths();
        const auto& ll_freqs = freq_collector.get_lit_len_freqs();
        for(int i=0; i<286; ++i) bits += ll_freqs[i] * ll_lens[i];
        
        const auto& d_lens = d_tree.get_bit_lengths();
        const auto& d_freqs = freq_collector.get_dist_freqs();
        for(int i=0; i<30; ++i) bits += d_freqs[i] * d_lens[i];

        return bits + 800;// 先估计这么多吧
    }

    void DeflateCompressor::write_store_block(BitWriter& writer,bool is_final){
        // BTYPE 00
        uint32_t header = (is_final ? 1 : 0) | (0 << 1);
        writer.write_bits(header, 3);
        writer.flush_byte_align(); 

         uint16_t len = (uint16_t)literal_buffer.size();
        uint16_t nlen = ~len;

        writer.write_bits(len, 16);
        writer.write_bits(nlen, 16);

        // 直接寫入字節
        for(int i=0; i<literal_buffer.size(); ++i) {
            writer.write_bits(literal_buffer[i], 8);
        }
    }
}

#include "deflate_compressor.tpp"

#endif // DEFLATE_COMPRESSOR_TPP