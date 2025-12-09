#ifndef HUFFMAN_TABLE_TPP
#define HUFFMAN_TABLE_TPP

#include "huffman_table.h"


namespace shed_zip{
    // 每个符号的huffman码长数组  ->   查找的vvector
    // 符号0的码长3，符号1的码长5之类的
    bool HuffmanTable::build(const shed_std::Vvector<int>& lengths){
        // 1.统计各个长度的数量
        // 获取max_len
        int max_len = 0;
        for(int i = 0;i<lengths.size();++i){
            if(lengths[i] > max_len) max_len = lengths[i];
        }

        if(max_len == 0){
            table_bits = 0;
            return true; // 空树
        }

        if(max_len > 15) return false; // Deflate 限制不超过15

        // 2.记录每个长度的起始编码(Canonical Huffman)
        // 码长为2，3的符号有多少个
        shed_std::Vvector<int> bl_count(max_len+1);
        bl_count.fill(0);
        for(int i = 0; i < lengths.size();++i){
            if(lengths[i] > 0) bl_count[lengths[i]]++;
        }

        // 短的码字对应短的长度
        shed_std::Vvector<int> next_code(max_len+1);
        int code = 0;
        bl_count[0] = 0;
        for(int bits = 1; bits <= max_len; bits++){
            code = (code + bl_count[bits-1]) << 1;
            // 确定code，例如symbol len = 1有2个，len = 2 有 1 个，len = 3有1个
            // len  = 1, 0,len = 2, (0+2)<<1 = 100,len=3,(100 + 001)<<1 = 1010
            next_code[bits] = code;
        }

        // 填充查找表
        table_bits = max_len;
        int table_size = 1 << table_bits;
        table = shed_std::Vvector<Entry>(table_size);
        
        // 不同的len映射到2^max_len的数组里
        for(int i = 0; i <lengths.size();++i){
            int len = lengths[i];
            if(len == 0) continue; // 跳过码长为0的

            int current_code = next_code[len];
            next_code[len]++;

            // Deflate是LSB，Huffman是MSB
            int rev_code = HuffmanDecoder::reverse_bits(current_code,len);

            // 填充所有可能的后续位
            for(int j = rev_code; j < table_size; j+= (1 << len)){
                table[j].symbol = (uint16_t)i;
                table[j].bits = (uint8_t)len;
            }
        }
        return true;
    }

    int HuffmanTable::decode(BitReader& reader) const{
        if(table_bits == 0) return -1;

        // 够看足够的bit
        uint32_t bits = reader.peek_bits(table_bits);

        // 查表
        if(bits >= (uint32_t) table.size()) return -1;
        Entry entry = table[(int) bits];

        if(entry.bits == 0) return -1;

        // 消耗实际使用的位数
        reader.drop_bits(entry.bits);
        return entry.symbol;
    }
}// namespace shed_zip
 
#endif // HUFFMAN_TABLE_TPP