#ifndef UNZIP_EXTRACTOR_H
#define UNZIP_EXTRACTOR_H

#include "zip_config.h"
#include "inflate_decompressor.h"

namespace shed_zip{
    class UnzipExtractor{
        public:
            // 自动检测ZIP或GZIP并解压
            // 对于ZIP，目前只解压第一个找到的文档
            // TODO:多文档提取
            shed_std::Vvector<uint8_t> extract(const shed_std::Vvector<uint8_t>& file_data);
            // 解析GZIP
            shed_std::Vvector<uint8_t> extract_gzip(const shed_std::Vvector<uint8_t>& data);
            // 解析ZIP
            shed_std::Vvector<uint8_t> extract_zip(const shed_std::Vvector<uint8_t>& data);

            DecompressStatus get_status() const { return status; }
        private:
            DecompressStatus status;
            // 辅助读多字节
            static uint32_t read_u32(const shed_std::Vvector<uint8_t>& data, int& offset);
            static uint16_t read_u16(const shed_std::Vvector<uint8_t>& data, int& offset);
            uint32_t calculate_crc32(const shed_std::Vvector<uint8_t>& data);
    }; 
}

#include "unzip_extractor.tpp"

#endif // UNZIP_EXTRACTOR_H