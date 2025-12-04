#ifndef UNZIP_EXTRACTOR_TPP
#define UNZIP_EXTRACTOR_TPP

#include "unzip_extractor.h"

namespace shed_zip{
    uint32_t UnzipExtractor::read_u32(const shed_std::Vvector<uint8_t>& data,int& offset){
        if(offset + 4 > data.size()) return 0;//根本没位置
        uint32_t val = data[offset] | (data[offset+1] << 8) | (data[offset+2] << 16) | (data[offset+3] << 24);
        offset += 4;
        return val;
    }

    uint16_t UnzipExtractor::read_u16(const shed_std::Vvector<uint8_t>& data, int& offset) {
        if (offset + 2 > data.size()) return 0;
        uint16_t val = data[offset] | (data[offset+1] << 8);
        offset += 2;
        return val;
    }

    uint32_t UnzipExtractor::calculate_crc32(const shed_std::Vvector<uint8_t>& data){
        uint32_t crc = 0xFFFFFFFF;
        for(int i=0; i<data.size(); ++i) {
            crc ^= data[i];
            for(int j=0; j<8; ++j) {
                uint32_t mask = -(int)(crc & 1);
                crc = (crc >> 1) ^ (0xEDB88320 & mask);
            }
        }
        return ~crc;
    }

    shed_std::Vvector<uint8_t> UnzipExtractor::extract(const shed_std::Vvector<uint8_t>& file_data){
        status = DecompressStatus::OK;
        if (file_data.size() < 2) return {};

        // 检查signature魔数
        // GZIP: 1F 8B
        if (file_data[0] == 0x1F && file_data[1] == 0x8B) {
            return extract_gzip(file_data);
        }
        // ZIP: 50 4B 03 04 (PK 03 04)
        if (file_data[0] == 0x50 && file_data[1] == 0x4B) {
            return extract_zip(file_data);
        }

        // 尝试使用Raw Deflate
        InflateDecompressor inflater;
        shed_std::Vvector<uint8_t> result = inflater.decompress(file_data);
        status = inflater.get_last_status();
        return result;
    }

    shed_std::Vvector<uint8_t> UnzipExtractor::extract_gzip(const shed_std::Vvector<uint8_t>& data){
        int pos = 0;
        if(read_u16(data,pos) != 0x8B1F){
            status = DecompressStatus::ERROR_BAD_HEADER;
            return shed_std::Vvector<uint8_t>();
        }

        uint8_t cm = data[pos++];
        uint8_t flags = data[pos++];
        pos += 6;

        if(flags & 0x04){uint16_t x1 = read_u16(data,pos);pos += x1;}
        if(flags & 0x08){
            while(pos < data.size()&&data[pos]!=0){
                pos++;
            }
            pos++;
        }
        if(flags & 0x10){
            while(pos < data.size() && data[pos] != 0){
                pos++; 
            }
            pos++;
        }
        if (flags & 0x02) pos += 2; 


        shed_std::Vvector<uint8_t> payload;
        if(data.size() < 8){status = DecompressStatus::ERROR_TRUNCATED_DATA;return shed_std::Vvector<uint8_t>();}
        int payload_end = data.size() - 8;

        for(int i = pos; i < payload_end; ++i){
            payload.push_back(data[i]);
        }

        InflateDecompressor inflater;
        shed_std::Vvector<uint8_t> decompressed = inflater.decompress(payload);
        status = inflater.get_last_status();
        
        if (status != DecompressStatus::OK) return {};

        // GZIP CRC校验
        int footer_pos = payload_end;
        uint32_t file_crc = read_u32(data, footer_pos);
        uint32_t calc_crc = calculate_crc32(decompressed);
        
        if (file_crc != calc_crc) {
            status = DecompressStatus::ERROR_BAD_CRC;
        }

        return decompressed;
    }

    shed_std::Vvector<uint8_t> UnzipExtractor::extract_zip(const shed_std::Vvector<uint8_t>& data){
        int pos = 0;
        if(read_u32(data,pos) != 0x04034b50){
            status = DecompressStatus::ERROR_BAD_HEADER;
            return {};
        }

        pos += 2;
        uint16_t flags = read_u16(data, pos);
        uint16_t method = read_u16(data, pos);
        pos += 4; 
        uint32_t header_crc = read_u32(data, pos);
        uint32_t comp_size = read_u32(data, pos);
        pos += 4; 
        uint16_t name_len = read_u16(data, pos);
        uint16_t extra_len = read_u16(data, pos);

        pos += name_len;
        pos += extra_len;

        shed_std::Vvector<uint8_t> payload;
        for(uint32_t i=0; i<comp_size; ++i) {
            if (pos < data.size()) payload.push_back(data[pos++]);
        }

        shed_std::Vvector<uint8_t> result;
        if(method == 0){
            result = payload;
        }else if(method == 8){
            InflateDecompressor inflater;
            result = inflater.decompress(payload);
            status = inflater.get_last_status();
        }else{
            status = DecompressStatus::ERROR_UNSUPPORTED;
        }

        if(status == DecompressStatus::OK && !(flags & 0x08)){
            if ( calculate_crc32(result) != header_crc){
                status = DecompressStatus::ERROR_BAD_CRC;
            }
        }

        return result;
    }
}//namespace shed_zip

#include "unzip_extractor.tpp"

#endif