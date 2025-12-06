#ifndef DEFLATE_TOKEN
#define DEFLATE_TOKEN

#include "../zip_config.h"

namespace shed_zip{
    // 由于动态huffman模式不能变压缩边写入，所以需要缓存LZ77的输出
    // 这表示LZ输出流的一个元素（字面量/匹配）
    struct DeflateToken{
        // distance == 0： value 字面量(0-255)
        // distance > 0 :  value 长度(3-258)   distance 长度(1-32768)
        uint16_t value;
        uint16_t distance;

        static DeflateToken make_literal(uint8_t lit){
            return {static_cast<uint16_t>(lit),0};
        }

        static DeflateToken make_match(uint16_t len,uint16_t dist){
            return {len,dist};
        }
    };
}// namespace shed_zip

#endif // DEFLATE_TOKEN