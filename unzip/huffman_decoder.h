#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include "../zip_config.h"
#include "bit_reader.h"


namespace shed_zip{
    class HuffmanDecoder{
        public:
            // 解码下一个固定huffman编码符号（literal 或 length code)
            // return: 0 -255 literal, 256 EOB, 257+ length code
            static int decode_fixed_literal(BitReader& reader);

            // 解码距离编码
            static int decode_fixed_distance(BitReader& reader);

            // 辅助:翻转 n个 bit（将LSB数据转换为MSB Huffman）
            static uint32_t reverse_bits(uint32_t val, int bits);
        private:
            
    };
}

#include "huffman_decoder.tpp"

#endif // HUFFMAN_DECODER_H