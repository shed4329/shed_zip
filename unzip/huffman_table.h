#ifndef HUFFMAN_TABLE_H
#define HUFFMAN_TABLE_H

#include "zip_config.h"
#include "bit_reader.h"

namespace shed_zip{
    class HuffmanTable{
        public:
            HuffmanTable():table_bits(0){}

            // 从长度构建解码表
            bool build(const shed_std::Vvector<int>& lengths);

            // 从BitReader解码下一个符号
            int decode(BitReader& reader) const;

        private:
            struct Entry{
                uint16_t symbol; // 解码处的符号
                uint8_t bits;    // 该符号可用的bits数目
            };

            int table_bits; // 表的大小(2^table_bits)
            shed_std::Vvector<Entry> table;// 快速查找表
        
    };
} // namespace shed_zip

#endif