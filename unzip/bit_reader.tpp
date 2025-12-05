#ifndef BIT_READER_TPP
#define BIT_READER_TPP

#include "bit_reader.h"

namespace shed_zip{
    BitReader::BitReader(const shed_std::Vvector<uint8_t>& data):buffer(data),byte_pos(0),bit_buffer(0),bit_count(0){}

    void BitReader::ensure_bits(int bits){
        // 位数不足则填充，足够了不用
        while(bit_count < bits && byte_pos < buffer.size()){
            // 把新字节拼接到bit_buffer,跳过已经填充的地方
            bit_buffer |= (static_cast<uint32_t>(buffer[byte_pos]) << bit_count);
            bit_count += 8;
            byte_pos++;
        }
    }

    uint32_t BitReader::read_bits(int bits){
        ensure_bits(bits);
        // 取低bit位
        uint32_t ret = bit_buffer & ((1 << bits) - 1);
        // 抹掉已经写了的部分
        bit_buffer >>= bits;
        // 有效位数也剪掉
        bit_count -= bits;
        return ret;
    }

    uint32_t BitReader::peek_bits(int bits){
        ensure_bits(bits);
        return bit_buffer &((1 << bits) - 1);
    }

    void BitReader::drop_bits(int bits){
        ensure_bits(bits);
        bit_buffer >>= bits;
        bit_count -= bits;
    }

    void BitReader::align_to_byte(){
        // 丢弃bit_buffer 中剩余的bit,重置为byte边界
        bit_buffer = 0;
        bit_count = 0;
    }

    bool BitReader::has_bits(int bits){
        ensure_bits(bits);
        return bit_count >= bits;
    }

    int BitReader::get_byte_pos() const{
        return byte_pos - (bit_count / 8);
    }
}

#endif