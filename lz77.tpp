#ifndef LZ77_TPP
#define LZ77_TPP

#include "lz77.h"

namespace shed_zip{
    LZ77Matcher::LZ77Matcher(const ZipConfig& cfg):config(cfg){}

    void LZ77Matcher::insert_hash(const shed_std::Vvector<uint8_t>& data,int pos){
        // 如果不够三字节，直接返回，不插入了
        if(pos + 2 >= (int)data.size()) return;
        // 计算hash值，直接凭借
        uint32_t h = (data[pos] << 16) | (data[pos+1] << 8) | data[pos+2];
        // 贪婪更新
        head[h] = pos;
    }

    Match LZ77Matcher::find_longest_match(const shed_std::Vvector<uint8_t>& data, int current_pos){
        // 默认未匹配
        Match m = {false,0,0};
        int limit = (int)data.size();
        // 剩余字节不够了，返回空
        if(current_pos + MIN_MATCH >= limit) return m;
        // 计算hash值
        uint32_t h = (data[current_pos] << 16) | (data[current_pos+1] << 8) | data[current_pos+2];
        // 查hash表
        auto it = head.find(h);

        // 找到重复项
        if(it!=head.end()){
            int prev_pos = it->second; // 最近一次出现的位置
            int dist = current_pos - prev_pos;//到当前位置的距离

            // 验证匹配有效性
            // 1.dist > 0 ：确保不是和自己匹配
            if(dist > 0 && dist <= config.window_size){
                // 逐字节比较，找最长重复长度
                int len = 0;
                // 开始进行字节比较
                // 不越界。不超过最大匹配，当前字节 == 历史字节
                while(current_pos + len < limit && len < MAX_MATCH && data[current_pos + len] == data[prev_pos + len]){
                    len++;//长度+1
                }

                // 只有长度大于最小匹配长度才算成功
                if(len >= MIN_MATCH){
                    m.found = true;
                    m.length = len;
                    m.distance = dist;
                }
            }
        }

        // 更新当前位置的hash，贪婪更新，总是覆盖旧的位置
        head[h] = current_pos;

        return m;
    }
}

#endif