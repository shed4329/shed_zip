#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "../zip_config.h"
#include "../shed_std/Vvector.h"
#include "../shed_std/Hheap.h"
#include "../shed_std/Ccomparator.h"

namespace shed_zip{
    struct HuffmanNode{
        int id;
        int symobol;
        uint32_t freq; // 0-285 or -1
        int left;
        int right;
        int parent;
        int depth;

        HuffmanNode(int i = 0, int s = -1, uint32_t f = 0):id(i),symobol(s),freq(f),left(-1),right(-1),parent(-1),depth(0){}
    };

    // Hheap里放的轻量级节点
    struct FreqNode
    {
        uint32_t freq;
        int node_index;
        int depth;

        // a >b ,则b上浮（小根堆）
        bool operator>(const FreqNode& other) const{
            if(freq != other.freq){
                return freq > other.freq;
            }
            return depth > other.depth; // 频率相同的，深度深的是为大
        }
    };
    
    class HuffmanTree{
        public:
            HuffmanTree();

            void reset();

            // 构建树
            // frequencies: 索引 = 符号， 值 = 频率
            // max_bits: 限制最大位元长度(Deflate = 15)
            void build_tree(const shed_std::Vvector<uint32_t>& frequencies, int max_bits = 15);

            // 结果获取
            const shed_std::Vvector<int>& get_bit_lengths() const;
            const shed_std::Vvector<uint16_t>& get_codes() const;

        private:
            shed_std::Vvector<HuffmanNode> nodes;
            shed_std::Vvector<int> bit_lengths;
            shed_std::Vvector<uint16_t> codes;
            
            // 小根堆
            shed_std::Hheap<FreqNode,shed_std::greater<FreqNode>> pq;

            void compute_depths(int node_idx,int current_depth);
            void gen_codes(const shed_std::Vvector<uint32_t>& freqs,int max_symbol);
    };
}

#endif