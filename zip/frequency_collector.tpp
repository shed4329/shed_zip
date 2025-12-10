#ifndef FREQUENCY_COLLECTOR_TPP
#define FREQUENCY_COLLECTOR_TPP

#include "frequency_collector.h"

namespace shed_zip{
    FrequencyCollector::FrequencyCollector():lit_len_freqs(286),dist_freqs(30){
        lit_len_freqs.fill(0);
        dist_freqs.fill(0);
    }

    void FrequencyCollector::reset(){
        lit_len_freqs.fill(0);
        dist_freqs.fill(0);
    }

    void FrequencyCollector::add_literal(uint8_t lit){
        lit_len_freqs[lit]++;
    }

    void FrequencyCollector::add_eob(){
        lit_len_freqs[256]++;
    }

    void FrequencyCollector::add_match(uint16_t len,uint16_t dist){
        int len_code = get_length_code(len);
        lit_len_freqs[len_code]++;

        int dist_code = get_dist_code(dist);
        dist_freqs[dist_code]++;
    }

    // 這些方法以後全部統一，查表的東西
    int FrequencyCollector::get_length_code(int len) const{
        if(len <= 10) return 257 + len -3;
        if(len <= 18) return 265 + (len - 11) / 2;
        if(len <= 34) return 269 + (len - 19) / 4;
        if(len <= 66) return 273 + (len - 35) / 8;
        if(len <= 130) return 277 + (len - 67)/16;
        if(len <= 257) return 281 + (len -131)/32;
        return 285;
    }

    int FrequencyCollector::get_dist_code(int dist) const{
        if (dist <= 256) {
            if (dist <= 4) return dist - 1;
            if (dist <= 8) return (dist - 5) / 2 + 4;
            if (dist <= 16) return (dist - 9) / 4 + 6;
            if (dist <= 32) return (dist - 17) / 8 + 8;
            if (dist <= 64) return (dist - 33) / 16 + 10;
            if (dist <= 128) return (dist - 65) / 32 + 12;
            return (dist - 129) / 64 + 14;
        } else {
            if (dist <= 512) return (dist - 257) / 128 + 16;
            if (dist <= 1024) return (dist - 513) / 256 + 18;
            if (dist <= 2048) return (dist - 1025) / 512 + 20;
            if (dist <= 4096) return (dist - 2049) / 1024 + 22;
            if (dist <= 8192) return (dist - 4097) / 2048 + 24;
            if (dist <= 16384) return (dist - 8193) / 4096 + 26;
            return (dist - 16385) / 8192 + 28;
        }
    }

}

#endif // FREQUENCY_COLLECTOR_TPP