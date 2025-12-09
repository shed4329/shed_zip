#ifndef HUFFMAN_DECODER_TPP
#define HUFFMAN_DECODER_TPP

#include "huffman_decoder.h"
#include "huffman_table.h"

namespace shed_zip{
    uint32_t HuffmanDecoder::reverse_bits(uint32_t val,int bits){
        uint32_t res = 0;
        for(int i = 0; i < bits ; i++){
            // 经典翻转置位
            if((val >> i) & 1) res |= (1 << (bits - 1 -i));
        }
        return res;
    }

    int HuffmanDecoder::decode_fixed_literal(BitReader& reader){
        // 固定huffman码表(See RFC 1951)
        //    000 0000 -    001 0111： 256 - 279
        //   0011 0000 -   1011 1111:  0   - 143
        //   1100 0000 -   1100 0111:  280 - 287
        // 1 1001 0000 - 1 1111 1111： 144 - 255
        
        // 先偷看7 bits
        uint32_t bits7 = reader.peek_bits(7);
        uint32_t huff7 = reverse_bits(bits7,7);

        if(huff7 <= 0x17){
            reader.drop_bits(7);
            return 256+huff7;
        }

        // 偷看8 bits
        uint32_t bits8 = reader.peek_bits(8);
        uint32_t huff8 = reverse_bits(bits8,8);

        if(huff8 >= 0x30 && huff8 <= 0xBF){
            reader.drop_bits(8);
            return huff8 - 0x30; // 0 - 143
        }else if(huff8 >= 0xC0 && huff8 <= 0xC7){
            reader.drop_bits(8);
            return huff8 - 0xC0 + 280;// 280-287
        }

        uint32_t bits9 = reader.peek_bits(9);
        uint32_t huff9 = reverse_bits(bits9,9);

        if(huff9 >= 0x190 && huff9 <= 0x1FF){
            reader.drop_bits(9);
            return 144 + huff9 - 0x190; // 144 - 255
        }

        return -1;//error
    }

    int HuffmanDecoder::decode_fixed_distance(BitReader& reader){
        // 固定距离编码总共是 5 bits
        // 0 0000 - 1 1111 ： 0 - 31
        uint32_t bits = reader.read_bits(5);
        return reverse_bits(bits,5);
    }
}

#endif // HUFFMAN_DECODER_TPP