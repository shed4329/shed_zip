#ifndef INFLATE_DECOMPRESSOR_H
#define INFLATE_DECOMPRESSOR_H

#include "../zip_config.h"
#include "bit_reader.h"

namespace shed_zip{
    class InflateDecompressor{
        public:
            // 执行解压缩
            // 返回解压缩之后的数据，如果出错，buffer中可能有部分数据
            shed_std::Vvector<uint8_t> decompress(const shed_std::Vvector<uint8_t>& input);

            DecompressStatus get_last_status() const {return status;}

        private:
            DecompressStatus status;


            // 处理未压缩块（BYTPE 00）
            bool process_store_block(BitReader& reader,shed_std::Vvector<uint8_t>& output);

            // 处理固定huffman编码(BTYPE 01)
            bool process_fixed_block(BitReader& reader,shed_std::Vvector<uint8_t>& output);
            // 处理动态huffman编码(BYTPE 10)
            bool process_dynamic_block(BitReader& reader, shed_std::Vvector<uint8_t>& output);

            // LZ77 复制逻辑
            void copy_match(shed_std::Vvector<uint8_t>& output, int length, int distance);

            // 查找表
            static const int length_extra_bits[];
            static const int length_base[];
            static const int dist_extra_bits[];
            static const int dist_base[];
            static const int cl_code_order[];
    };
} // namesapce shed_zip

#include "inflate_decompressor.tpp"
#endif // INFLATE_DECOMPRESSOR_H