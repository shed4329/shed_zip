#ifndef DEFLATE_COMPRESSOR_H
#define DEFLATE_COMPRESSOR_H

#include "../zip_config.h"
#include "huffman.h"
#include "lz77.h"
#include "bit_writer.h"
#include "huffman_tree.h"
#include "deflate_token.h"
#include "frequency_collector.h"

namespace shed_zip{
    class DeflateCompressor{
        public:
            DeflateCompressor(ZipConfig cfg = ZipConfig());

            shed_std::Vvector<uint8_t> compress(const shed_std::Vvector<uint8_t>& input);

            // Store 压缩,BTYPE = 00
            void write_store_block(BitWriter& writer, bool is_final);
            // Fixed Huffman 压缩,BTYPE = 01;
            void write_fixed_huffman(BitWriter& writer,bool is_final);
            // Dynamic Huffman 压缩，BTYPE = 10;
            void write_dynamic_huffmna(BitWriter& writer,bool is_final);

            // 估计各种压缩方法的长度，用于确认
            uint32_t estimate_dynamic_size(const HuffmanTree& lit_tree,const HuffmanTree& dist_tree);
            uint32_t estimate_fixed_size();
            uint32_t estimate_store_size();
        private:
            ZipConfig config;
            // 缓冲区
            shed_std::Vvector<DeflateToken> token_buffer; 
            shed_std::Vvector<uint8_t> literal_buffer; // 用于 Store 的原始数据备份
            FrequencyCollector freq_collector;
            void flush_block(BitWriter& writer, bool is_final);
            
    };
    
}

#include "deflate_compressor.tpp"

#endif // DEFLATE_COMPRESSOR_H