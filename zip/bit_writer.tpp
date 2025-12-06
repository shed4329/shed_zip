#ifndef BIT_WRITER_TPP
#define BIT_WRITER_TPP

#include "bit_writer.h"

namespace shed_zip{
    BitWriter::BitWriter():bit_buffer(0),bit_count(0){}

    void BitWriter::reset(){
        buffer.clear();
        bit_buffer = 0;
        bit_count = 0;
    }

    shed_std::Vvector<uint8_t>& BitWriter::get_buffer(){
        flush_byte_align();
        return buffer;
    }

    void BitWriter::write_bits(uint32_t value, int bits){
        // 加到高位
        bit_buffer |= (value << bit_count);
        bit_count += bits;

        // 只要满8bits就输出
        while(bit_count >= 8){
            buffer.push_back(static_cast<uint8_t>(bit_buffer& 0xFF));
            bit_buffer >>= 8;
            bit_count -= 8;
        }
    }

    void BitWriter::write_huffman_code(uint32_t code, int bits) {
        // 反轉位元順序
        uint32_t reversed = 0;
        for (int i = 0; i < bits; ++i) {
            if ((code >> i) & 1) {
                reversed |= (1 << (bits - 1 - i));
            }
        }
        write_bits(reversed, bits);
    }

    void BitWriter::flush_byte_align() {
        if (bit_count > 0) {
            buffer.push_back(static_cast<uint8_t>(bit_buffer & 0xFF));
            bit_buffer = 0;
            bit_count = 0;
        }
    }
}

#endif