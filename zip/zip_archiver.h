#ifndef ZIP_ARCHIVER_H
#define ZIP_ARCHIVER_H

#include "../zip_config.h"
#include "deflate_compressor.h"

namespace shed_zip{
    class ZipArchiver{
        public:
            ZipArchiver(ZipConfig cfg = ZipConfig());

            // 创建.zip格式数据
            shed_std::Vvector<uint8_t> create_zip(const shed_std::Vvector<uint8_t>& data, const shed_std::Sstring& filename);

            // 创建.gzip格式数据
            shed_std::Vvector<uint8_t> create_gzip(const shed_std::Vvector<uint8_t>& data, const shed_std::Sstring& filename);
        private:
            ZipConfig config;
            static uint32_t calculate_crc32(const shed_std::Vvector<uint8_t>& data);
            void write_u32(shed_std::Vvector<uint8_t>& buf,uint32_t val);
            void write_u16(shed_std::Vvector<uint8_t>& buf,uint16_t val);
    };
}
#include "zip_archiver.tpp"

#endif