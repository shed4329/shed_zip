#ifndef BIT_READER
#define BIT_READER

#include "../zip_config.h"

#include "../shed_std/Vvector.h"

namespace shed_zip{
    class BitReader{
        public:
            BitReader(const shed_std::Vvector<uint8_t>& data);

            // 读取n个bit（最多32）
            // 读取的同时指针会移动
            uint32_t read_bits(int bits);

            // 偷看n个bit，但是不移动指针（用于huffman解码器判定）
            uint32_t peek_bits(int bits);

            // 跳过n个bit
            void drop_bits(int bits);

            // 跳到下一个 Byte 边界（适用于 BYTPE = 00 的 Store 模式）
            void align_to_byte();

            // 检查是否有足够的 bit 可读
            bool has_bits(int bits);

            // 获取当前读取的 Byte 位置
            int get_byte_pos() const;
        private:
            const shed_std::Vvector<uint8_t>& buffer;
            int byte_pos;           // 指针
            uint32_t bit_buffer;    // 缓冲区
            int bit_count;          // 缓冲区有效位计数
            
            // 内部函数：填充 bit_buffer 直到至少有n个bit或数据耗尽
            void ensure_bits(int bits);
    };
} // namespace shed_zip

#include "bit_reader.tpp"

#endif // BIT_READER