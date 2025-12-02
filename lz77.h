#ifndef LZ77_H
#define LZ77_H

#include "zip_config.h"
#include "shed_std/Hhashmap.h"

namespace shed_zip{
    struct Match{
        bool found;
        int length;
        int distance;
    };

    class LZ77Matcher{
        public:
            LZ77Matcher(const ZipConfig& cfg);

            // 寻找最长匹配
            Match find_longest_match(const shed_std::Vvector<uint8_t>& data, int current_pos);

            // 插入Hash(用于Lazy Update)
            void insert_hash(const shed_std::Vvector<uint8_t>& data,int pos);
        private:
            ZipConfig config;
            shed_std::Hhashmap<uint32_t,int> head; // Key： 3-byte Hash, Value:pos

            static constexpr int MIN_MATCH = 3;
            static constexpr int MAX_MATCH = 258;
    };
} // namespace shed_zip

#endif // LZ77_H