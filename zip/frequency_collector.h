#ifndef FREQUENCY_COLLECTOR_H
#define FREQUENCY_COLLECTOR_H

#include "../zip_config.h"

namespace shed_zip{
    class FrequencyCollector{
        public:
            FrequencyCollector();

            void reset();

            // 统计函数
            void add_literal(uint8_t literal);
            void add_match(uint16_t len, uint16_t dist);
            void add_eob(); // End of Block(256)

            // 获取频率表
            const shed_std::Vvector<uint32_t>& get_lit_len_freqs() const { return lit_len_freqs; }
            const shed_std::Vvector<uint32_t>& get_dist_freqs() const { return dist_freqs; }

            // 辅助:将长度/距离转换为对应的code（用于统计）
            int get_length_code(int len) const;
            int get_dist_code(int dist) const;
        private:
            // 0-255： Literal，256:EOB, 257-285:Length Codes
            shed_std::Vvector<uint32_t> lit_len_freqs; // size 286

            // 0-29: Distance Codes
            shed_std::Vvector<uint32_t> dist_freqs; // size 30

           
    };
}

#include "frequency_collector.tpp"

#endif // FREQUENCY_COLLECTOR_H