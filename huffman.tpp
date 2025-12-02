#ifndef HUFFMAN_TPP
#define HUFFMAN_TPP

#include "huffman.h"

namespace shed_zip{
    // see RFC 1951 standard
    // TODO: understand this
    // 用于存储LZ77匹配长度的编码，长度值不是直接存储，而应该符号+额外bit
    // 这样设计用于节约空间
    const int HuffmanEncoder::length_extra_bits[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0
    };
    // 长度的基准值
    const int HuffmanEncoder::length_base[] = {
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258
    };
    // 距离补位
    const int HuffmanEncoder::dist_extra_bits[] = {
        0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
        7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13
    };
    // 距离补位
    const int HuffmanEncoder::dist_base[] = {
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577
    };

    HuffmanEncoder::HuffmanEncoder(): bit_buffer(0),bit_count(0){}

    void HuffmanEncoder::reset(){
        buffer.clear();
        bit_buffer = 0;
        bit_count = 0;
    }

    shed_std::Vvector<uint8_t>& HuffmanEncoder::get_buffer(){
        flush_byte_align();
        return buffer;
    }

    void HuffmanEncoder::write_bits(uint32_t value, int bits){
        // 把bit_buffer当作32位寄存器，这里是把value放在已经填充的数据的高位
        bit_buffer |= (value << bit_count);
        // 更新已填充的位置
        bit_count += bits;
        // 8位8位地刷出
        while(bit_count >= 8){
            buffer.push_back(static_cast<uint8_t>(bit_buffer & 0xFF));
            bit_buffer >>= 8;
            bit_count -= 8;
        }
    }

    void HuffmanEncoder::write_huffman_code(uint32_t code, int bits){
        // 翻转huffman编码 （流是LSB，填充的时候是MSB）
        uint32_t reversed = 0;
        for(int i = 0; i <bits ; ++i){
            // 一位一位地翻转
            if((code >> i) & 1) reversed |= (1 << (bits - 1 - i));
        }
        write_bits(reversed,bits);
    }

    // 首位工作，处理残留bit
    void HuffmanEncoder::flush_byte_align(){
        if(bit_count > 0){
            // 不足补0
            buffer.push_back(static_cast<uint8_t>(bit_buffer & 0xFF));
            bit_buffer = 0;
            bit_count = 0;
        }
    }

    // 压缩块的开头必须有3bit的块信息，1.这是否是最后一个块 2.这个块用什么编码方式压缩
    void HuffmanEncoder::write_block_header(bool is_final){
        // bit0: BFINAL 最后一块标志
        // bit1,bit2: BTYPE,块类型标志，这里固定为01
        // for deflate: 00 = store, 01 = 静态huffman ,10 = 动态huffman编码, 11 = 保留（未使用）
        uint32_t header = (is_final ? 1: 0) | (1 << 1);
        write_bits(header,3);
    }

    void HuffmanEncoder::write_literal(uint8_t literal){
        uint32_t code;
        int bits;
        int val = literal;
        // 这是固定码表
        // 对于0-143的字面量，用8位固定编码
        if(val <= 143) {
            code = 0x30 + val; 
            bits = 8;
        }else{
            // 144-255，采用9位固定编码
            code = 0x190 + val - 144;
            bits = 9;
        }
        write_huffman_code(code,bits);
    }

    void HuffmanEncoder::write_eob(){
        // 块结束位256，对于固定huaffman， 256 是 000 0000 （7bit）
        write_huffman_code(0,7);
    }

    void HuffmanEncoder::write_match(int length, int distance){
        // 1.长度码
        int len_code_idx = 0;
        // 分段映射，有点像找汇编的段基址
        for(int i = 0; i < 28; i++){
            if(length < length_base[i+1]){
                len_code_idx = i;
                break;
            }
            // 没有的话就默认给个28吧
            if(i == 28) len_code_idx = 28;
        }

        // 生成长度哈夫曼符号，计算公式257+len_code_idx
        int symbol = 257 + len_code_idx;
        uint32_t h_code;
        int h_bits;

        // 固定码表的编码规则
        if(symbol <= 279){
            h_code = symbol - 256;
            h_bits = 7;
        }else{
            h_code = 0xC0 + (symbol - 280);
            h_bits = 8;
        }

        write_huffman_code(h_code,h_bits);
        
        // 如果有额外位还要写入，写入数值 和 位数
        if(length_extra_bits[len_code_idx] > 0){
            write_bits(length - length_base[len_code_idx],length_extra_bits[len_code_idx]);
        }

        // 2.距离编码
        int dist_code_idx = 0;
        for(int i = 0;i<30;++i){
            if(distance < dist_base[i+1]){
                dist_code_idx = i;
                break;
            }
        }

        // 固定位置编码5字节
        write_huffman_code(dist_code_idx,5);

        if(dist_extra_bits[dist_code_idx] > 0){
            write_bits(distance - dist_base[dist_code_idx],dist_extra_bits[dist_code_idx]);
        }
    }


}


#endif // HUFFMAN_TPP