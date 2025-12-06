#ifndef BIT_WRITER
#define BIT_WRITER

#include "../zip_config.h"
#include "../shed_std/Vvector.h"

namespace shed_zip{
    class BitWriter{
        public:
            BitWriter();

            // 写入数值，LSB First
            // value:要写入的整数
            // bits:bit数(0-32)
            void write_bits(uint32_t value,int bits);

            // 写入Huffman编码
            // Huffman码在逻辑上是MSB Fisrt，但是在Deflate 流中是LSB First 存储
            // 这个函数会自动做bit reverse
            void write_huffman_code(uint32_t code,int bits);

            // 对齐到 Byte边界（flush剩余bit）
            void flush_byte_align();

            // 获取底层缓冲区
            shed_std::Vvector<uint8_t>& get_buffer();

            // 清空状态
            void reset();        
        private:
                shed_std::Vvector<uint8_t> buffer;
                uint32_t bit_buffer;
                int bit_count;
    };
} // namespace shed_zip

#include "bit_writer.tpp"

#endif