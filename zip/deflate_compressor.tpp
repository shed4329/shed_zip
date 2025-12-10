#ifndef DEFLATE_COMPRESSOR_TPP
#define DEFLATE_COMPRESSOR_TPP

#include "deflate_compressor.h"

namespace shed_zip{

    // 這些表格對應 RFC 1951 的定義
    static const int DYN_LEN_EXTRA[] = { 0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0 };
    static const int DYN_LEN_BASE[] = { 3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258 };
    static const int DYN_DIST_EXTRA[] = { 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13 };
    static const int DYN_DIST_BASE[] = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577 };

    DeflateCompressor::DeflateCompressor(ZipConfig cfg):config(cfg){}

    shed_std::Vvector<uint8_t> DeflateCompressor::compress(const shed_std::Vvector<uint8_t>& input){
        BitWriter writer;
        LZ77Matcher lz77(config);

        // 重置狀態
        token_buffer.clear();
        freq_collector.reset();

        int pos = 0;
        int limit = (int)input.size();

        // 切分block，32KB時觸發
        int block_size_limit = 32768;

        while(pos < limit){
            if(token_buffer.size() >= block_size_limit){
                flush_block(writer,false);
                token_buffer.clear();
                freq_collector.reset();
            }

            Match match = lz77.find_longest_match(input,pos);

            if(match.found){
                // 記錄匹配
                token_buffer.push_back(DeflateToken::make_match(match.length,match.distance));
                freq_collector.add_match(match.length,match.distance);
                
                // LZ77 Lazy update
                 if (config.level >= 5) {
                    for (int i = 1; i < match.length && pos + i + 2 < limit; ++i) {
                        lz77.insert_hash(input, pos + i);
                    }
                }
                pos += match.length;
            }else {
                // 記錄字面量
                token_buffer.push_back(DeflateToken::make_literal(input[pos]));
                freq_collector.add_literal(input[pos]);
                pos++;
            }
        }
        
        freq_collector.add_eob(); // 標記 Block 結束
        flush_block(writer, true); // 輸出 Final Block

        writer.flush_byte_align();
        return writer.get_buffer();

    }

    void DeflateCompressor::flush_block(BitWriter& writer, bool is_final) {
        // 這裡我們強制使用 Dynamic Huffman (BTYPE=10)
        // 除非數據量極小導致建樹失敗或開銷太大（這裡簡單判斷）
       write_dynamic_block(writer,is_final);
    }

    void DeflateCompressor::write_dynamic_block(BitWriter& writer,bool is_final){
        // 1.寫入Header 
        uint32_t header = (is_final ? 1 : 0) | (2 << 1); 
        writer.write_bits(header, 3);

        // 2.構建兩顆主樹
        HuffmanTree lit_len_tree;
        lit_len_tree.build_tree(freq_collector.get_lit_len_freqs(), 15);

        HuffmanTree dist_tree;
        dist_tree.build_tree(freq_collector.get_dist_freqs(), 15);

        // 3.獲取bit長度並修改
        const auto& ll_lens = lit_len_tree.get_bit_lengths();
        const auto& d_lens = dist_tree.get_bit_lengths();

        int hlit = 286; 
        while (hlit > 257 && ll_lens[hlit-1] == 0) hlit--; 

        int hdist = 30;
        while (hdist > 1 && d_lens[hdist-1] == 0) hdist--;

        // 4. RLE 壓縮位元長度表 (合併 Lit/Len 和 Dist)
        // 這是 Deflate 為了壓縮 "樹結構" 本身所做的設計
        shed_std::Vvector<uint16_t> rle_syms;
        shed_std::Vvector<uint16_t> rle_exts;
        shed_std::Vvector<int> rle_ext_bits;

        shed_std::Vvector<int> all_lens;
        for(int i=0; i<hlit; ++i) all_lens.push_back(ll_lens[i]);
        for(int i=0; i<hdist; ++i) all_lens.push_back(d_lens[i]);

        // RLE 算法
        for (int i = 0; i < all_lens.size(); ) {
            int val = all_lens[i];
            int run = 1;
            // 計算連續重複次數 (最多 138)
            while (i + run < all_lens.size() && all_lens[i+run] == val && run < 138) run++;

            if (val == 0) {
                // 針對 0 的特殊壓縮 (Code 17, 18)
                if (run >= 11) { // Code 18: 11-138 個 0
                    rle_syms.push_back(18); rle_exts.push_back(run-11); rle_ext_bits.push_back(7);
                    i += run;
                } else if (run >= 3) { // Code 17: 3-10 個 0
                    rle_syms.push_back(17); rle_exts.push_back(run-3); rle_ext_bits.push_back(3);
                    i += run;
                } else {
                    // 不足 3 個 0，直接寫 0
                    rle_syms.push_back(0); rle_exts.push_back(0); rle_ext_bits.push_back(0);
                    i++;
                }
            } else {
                // 針對非 0 值的壓縮 (Code 16)
                // 第一個值必須直接寫入
                rle_syms.push_back(val); rle_exts.push_back(0); rle_ext_bits.push_back(0);
                i++; run--;
                
                // 剩下的重複值
                while (run >= 3) {
                    int step = (run > 6) ? 6 : run; // Code 16: 重複 3-6 次
                    rle_syms.push_back(16); rle_exts.push_back(step-3); rle_ext_bits.push_back(2);
                    run -= step; i += step;
                }
                // 剩下不足 3 次的，直接寫入
                while (run > 0) {
                    rle_syms.push_back(val); rle_exts.push_back(0); rle_ext_bits.push_back(0);
                    i++; run--;
                }
            }
        }

        // 5. 構建 Code Length Tree (CL Tree)
        // 統計 RLE 符號 (0-18) 的頻率
        shed_std::Vvector<uint32_t> cl_freqs(19);
        cl_freqs.fill(0);
        for (int i=0; i<rle_syms.size(); ++i) cl_freqs[rle_syms[i]]++;
        
        HuffmanTree cl_tree;
        cl_tree.build_tree(cl_freqs, 7); // CL 樹最大深度限制為 7
        const auto& cl_lens = cl_tree.get_bit_lengths();
        const auto& cl_codes = cl_tree.get_codes();

        // 6. 寫入 Dynamic Header
        // 6.1 Counts
        writer.write_bits(hlit - 257, 5);
        writer.write_bits(hdist - 1, 5);

        // 6.2 HCLEN (Code Length Count)
        // CL 樹的長度表是按照這個奇怪的順序寫入的：
        const int cl_order[] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
        int hclen = 19;
        while (hclen > 4 && cl_lens[cl_order[hclen-1]] == 0) hclen--;
        writer.write_bits(hclen - 4, 4);

        // 6.3 寫入 CL Tree 的長度表
        for (int i = 0; i < hclen; ++i) {
            writer.write_bits(cl_lens[cl_order[i]], 3);
        }

        // 6.4 寫入 RLE 編碼後的 Lit/Dist 樹結構 (使用 CL Tree 編碼)
        for (int i = 0; i < rle_syms.size(); ++i) {
            int sym = rle_syms[i];
            writer.write_huffman_code(cl_codes[sym], cl_lens[sym]);
            
            if (rle_ext_bits[i] > 0) {
                writer.write_bits(rle_exts[i], rle_ext_bits[i]);
            }
        }

        // 7. 寫入實際壓縮數據 (Compressed Block Data)
        const auto& ll_codes = lit_len_tree.get_codes();
        const auto& d_codes = dist_tree.get_codes();
        const auto& ll_final_lens = lit_len_tree.get_bit_lengths(); // 避免變數名衝突
        const auto& d_final_lens = dist_tree.get_bit_lengths();

        for (int i=0; i<token_buffer.size(); ++i) {
            const auto& token = token_buffer[i];
            if (token.distance == 0) {
                // Literal
                writer.write_huffman_code(ll_codes[token.value], ll_final_lens[token.value]);
            } else {
                // Match
                // 7.1 Length
                int len_code = freq_collector.get_length_code(token.value);
                writer.write_huffman_code(ll_codes[len_code], ll_final_lens[len_code]);
                
                // Length Extra Bits
                int len_idx = len_code - 257;
                if (DYN_LEN_EXTRA[len_idx] > 0) {
                    writer.write_bits(token.value - DYN_LEN_BASE[len_idx], DYN_LEN_EXTRA[len_idx]);
                }

                // 7.2 Distance
                int dist_code = freq_collector.get_dist_code(token.distance);
                writer.write_huffman_code(d_codes[dist_code], d_final_lens[dist_code]);
                
                // Distance Extra Bits
                if (DYN_DIST_EXTRA[dist_code] > 0) {
                    writer.write_bits(token.distance - DYN_DIST_BASE[dist_code], DYN_DIST_EXTRA[dist_code]);
                }
            }
        }

        // 8. 寫入 End of Block (256)
        writer.write_huffman_code(ll_codes[256], ll_final_lens[256]);
    }



    // uint32_t DeflateCompressor::estimate_store_size(){
    //     // TODO:store 最大只能存65535 bytes
    //     // Header ( 3 nits) + Padding(0-7bits) + LEN/NLEN(32bits) + Data(bytes * 8)
    //     return 3+7+32+(uint32_t)literal_buffer.size()*8;
    // }

    // uint32_t DeflateCompressor::estimate_fixed_size(){
    //     uint32_t bits = 3; // Header
    //     // 遍历频率表计算
    //     const auto& ll_freqs = freq_collector.get_lit_len_freqs();
    //     const auto& d_freqs = freq_collector.get_dist_freqs();

    //     // TODO 实现长度计算
    //     return 0xFFFFFFFF;
    // }

    // uint32_t DeflateCompressor::estimate_dynamic_size(const HuffmanTree& l_tree,const HuffmanTree& d_tree){
    //     uint32_t bits = 3; // header
    //     // Header Size(HLIT,HDIST,HCLEN) + Code Length Tree + RLE Data
    //     // 先估计为100bytes吧

    //     const auto& ll_lens = l_tree.get_bit_lengths();
    //     const auto& ll_freqs = freq_collector.get_lit_len_freqs();
    //     for(int i=0; i<286; ++i) bits += ll_freqs[i] * ll_lens[i];
        
    //     const auto& d_lens = d_tree.get_bit_lengths();
    //     const auto& d_freqs = freq_collector.get_dist_freqs();
    //     for(int i=0; i<30; ++i) bits += d_freqs[i] * d_lens[i];

    //     return bits + 800;// 先估计这么多吧
    // }

    // void DeflateCompressor::write_store_block(BitWriter& writer,bool is_final){
    //     // BTYPE 00
    //     uint32_t header = (is_final ? 1 : 0) | (0 << 1);
    //     writer.write_bits(header, 3);
    //     writer.flush_byte_align(); 

    //      uint16_t len = (uint16_t)literal_buffer.size();
    //     uint16_t nlen = ~len;

    //     writer.write_bits(len, 16);
    //     writer.write_bits(nlen, 16);

    //     // 直接寫入字節
    //     for(int i=0; i<literal_buffer.size(); ++i) {
    //         writer.write_bits(literal_buffer[i], 8);
    //     }
    // }
}

#include "deflate_compressor.tpp"

#endif // DEFLATE_COMPRESSOR_TPP