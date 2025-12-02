#ifndef LZ77_TPP
#define LZ77_TPP

#include "lz77.h"

namespace shed_zip{
    LZ77Matcher::LZ77Matcher(const ZipConfig& cfg):config(cfg){}

    void LZ77Matcher::insert_hash(const shed_std::Vvector<unit8_t>& data,int pos){
        if(pos + 2 >= (int)data.size()) return;
        uint32_t h = (data[pos] << 16) | (data[pos+1] << 8) | data[pos+2];
        head[h] = pos;
    }

    Match LZ77Matcher::find_longest_match(const shed_std::Vvector<unit8_t>& data, int current_pos){
        Match m = {false,0,0};
        int limit = (int)data.size();

        if(current_pos + MIN_MATCH >= limit) return m;

        uint32_t h = (data[current_pos] << 16) | (data[current_pos+1] << 8) | data[current_pos+2];
        // 不一致
        int prev_pos = head[h];
    }
}

#endif