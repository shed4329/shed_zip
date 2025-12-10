#ifndef HUFFMAN_TREE_TPP
#define HUFFMAN_TREE_TPP

#include "huffman_tree.h"

namespace shed_zip{
    void HuffmanTree::reset() {
        nodes.clear();
        pq.clear();
        bit_lengths.clear();
        codes.clear();
    }

    void HuffmanTree::build_tree(const shed_std::Vvector<uint32_t>& frequencies, int max_bits){
        reset();
        int num_symbols = (int)frequencies.size();
        bit_lengths.resize(num_symbols);
        bit_lengths.fill(0);
        codes.resize(num_symbols);
        codes.fill(0);

        // 1.初始化,把所有存在的符号压入小根堆
        for(int i = 0;i<num_symbols ; ++i){
            if(frequencies[i] > 0){
                int id = (int)nodes.size();
                nodes.push_back(HuffmanNode(id,i,frequencies[i]));
                // 推入小根堆
                FreqNode fn;
                fn.freq = frequencies[i];
                fn.node_index = id;
                fn.depth = 0;
                pq.push(fn);
            }
        }

        if(pq.empty()) return;

        // 特殊情况：只有一个符号
        if(pq.size() == 1){
            FreqNode top = pq.top();
            pq.pop();
            bit_lengths[nodes[top.node_index].symobol] = 1;
            codes[nodes[top.node_index].symobol] = 0;
            return;
        }

        // 构建huffman，反复合并最小的树
        while(pq.size()>1){
            // 取出最小
            FreqNode left = pq.top();
            pq.pop();

            // 取出次小
            FreqNode right = pq.top();
            pq.pop();

            int new_idx = (int)nodes.size();
            uint32_t sum_freq = left.freq + right.freq;

            HuffmanNode parent(new_idx,-1,sum_freq);
            parent.left = left.node_index;
            parent.right = right.node_index;

            // 简单计算子树深度打破平局
            int d = (left.depth > right.depth ? left.depth : right.depth) + 1;
            parent.depth = d;

            nodes[left.node_index].parent = new_idx;
            nodes[right.node_index].parent = new_idx;

            nodes.push_back(parent);

            // 推回heap
            FreqNode new_fn;
            new_fn.freq = sum_freq;
            new_fn.node_index = new_idx;
            new_fn.depth = d;
            pq.push(new_fn);
        }

        // 计算每个符号的长度，遍历树
        int root_idx = pq.top().node_index;
        compute_depths(root_idx,0);

        // 长度限制处理，简单截断
        // 这里的做法简化了，不正确 TODO
        for(int i = 0;i < (int)nodes.size();++i){
            if(nodes[i].symobol != -1){
                if(nodes[i].depth > max_bits){
                    nodes[i].depth = max_bits;
                }
                bit_lengths[nodes[i].symobol] = nodes[i].depth;
            }
        }

        // 生成 Canonical Codes
        gen_codes(frequencies,num_symbols-1);
    }

    void HuffmanTree::compute_depths(int node_idx,int current_depth){
        if(node_idx == -1) return;

        if(nodes[node_idx].left == -1 && nodes[node_idx].right == -1){
            nodes[node_idx].depth = current_depth;
            return;
        }

        compute_depths(nodes[node_idx].left,current_depth+1);
        compute_depths(nodes[node_idx].right,current_depth+1);
    }

    void HuffmanTree::gen_codes(const shed_std::Vvector<uint32_t>& freqs,int max_symbol){
        int max_len = 15;
        shed_std::Vvector<int> bl_count(max_len+1);
        bl_count.fill(0);

        // 统计长度频率
        for(int i=0;i<=max_symbol;++i){
            int len = bit_lengths[i];
            if(len > 0){
                if(len > max_len) len = max_len;
                bl_count[len]++;
            }
        }

        // 计算起始编码
        shed_std::Vvector<uint16_t> next_code(max_len +1);
        next_code.fill(0);
        uint16_t code = 0;
        bl_count[0] = 0;

        for(int bits = 1;bits <= max_len ; ++bits){
            code = (code + bl_count[bits - 1]) << 1;
            next_code[bits] = code;
        }

        // 分配编码
        for(int n = 0;n<=max_symbol;n++){
            int len = bit_lengths[n];
            if(len!=0){
                codes[n] = next_code[len];
                next_code[len]++;
            }
        }
    }

    const shed_std::Vvector<int>& HuffmanTree::get_bit_lengths() const {
        return bit_lengths;
    }

    const shed_std::Vvector<uint16_t>& HuffmanTree::get_codes() const {
        return codes;
    }
}

#endif // HUFFMAN_TREE_TPP