#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "../zip_config.h"

namespace shed_zip{
    class HuffmanEncoder{
        public:
            HuffmanEncoder();
            // 将 Literal(0-255) 写入字节流
            void write_literal(uint8_t literal);

            // 将 长度与距离对 写入字节流
            void write_match(int length, int distance);

            // 写入区块结束符号(end of block)
            void write_eob();

            // 写入区块标题
            void write_block_header(bool in_final);
            // 取得编码后的字节组流
            shed_std::Vvector<uint8_t>& get_buffer();
            // 清空缓冲区
            void reset();
            // 刷新缓冲区
            void flush_byte_align();



        private:
            shed_std::Vvector<uint8_t> buffer;
            uint32_t bit_buffer;
            int bit_count;

            // 内部字节写入函数
            void write_bits(uint32_t value, int bits);
            void write_huffman_code(uint32_t code, int bits);
            

            // 查找表
            static const int length_extra_bits[];
            static const int length_base[];
            static const int dist_extra_bits[];
            static const int dist_base[];
    };

    
}

#include "huffman.tpp"

#endif // HUFFMAN_H