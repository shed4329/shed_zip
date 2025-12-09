#ifndef HUFFMAN_TABLE_TPP
#define HUFFMAN_TABLE_TPP

#include "huffman_table.h"

namespace shed_zip{
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
        shed_std::Vvector<int> bl_count(max_len+1);
    }
}// namespace shed_zip
 
#endif // HUFFMAN_TABLE_TPP