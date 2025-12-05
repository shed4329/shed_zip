#ifndef ZIP_ARCHIVER_TPP
#define ZIP_ARCHIVER_TPP

#include "zip_archiver.h"

namespace shed_zip{
   ZipArchiver::ZipArchiver(ZipConfig cfg):config(cfg){}
   
   uint32_t ZipArchiver::calculate_crc32(const shed_std::Vvector<uint8_t>& data){
        // CRC32 的计算流程
        // CRC初始值设定为0xFFFFFFFF
        // 对于每个字节，先与当前CRC异或，然后逐位右移8次，最低位为1时xor多项式0xEDB88320
        // 处理完毕后，对CRC按位取反
        uint32_t crc = 0xFFFFFFFF;
        for(int i = 0;i<data.size();++i){
            uint8_t byte = data[i];
            crc = crc ^ byte;
            for(int j = 0; j <8;++j){
                uint32_t mask = -(int)(crc & 1);
                crc = (crc >> 1) ^ (0xEDB88320 & mask);
            }
        }
        return ~crc;
   }

   void ZipArchiver::write_u32(shed_std::Vvector<uint8_t>& buf, uint32_t val) {
        buf.push_back(val & 0xFF);
        buf.push_back((val >> 8) & 0xFF);
        buf.push_back((val >> 16) & 0xFF);
        buf.push_back((val >> 24) & 0xFF);
    }

    void ZipArchiver::write_u16(shed_std::Vvector<uint8_t>& buf, uint16_t val) {
        buf.push_back(val & 0xFF);
        buf.push_back((val >> 8) & 0xFF);
    }

    shed_std::Vvector<uint8_t> ZipArchiver::create_gzip(const shed_std::Vvector<uint8_t>& data,const shed_std::Sstring& filename){
        shed_std::Vvector<uint8_t> out;

        // GZIP Header
        out.push_back(0x1F);
        out.push_back(0x8B);
        out.push_back(0x08); // Deflate
        out.push_back(0x00); // Flags
        write_u32(out,0);    // Time
        out.push_back(0x00); //  XF:
        out.push_back(0x03); // OS:Unix

        // Body
        DeflateCompressor compressor(config);
        shed_std::Vvector<uint8_t> compressed = compressor.compress(data);

        for(int i=0; i<compressed.size(); ++i) out.push_back(compressed[i]);

        // Footer
        uint32_t crc = calculate_crc32(data);
        write_u32(out, crc);
        write_u32(out, (uint32_t)data.size());

        return out;
    }

    shed_std::Vvector<uint8_t> ZipArchiver::create_zip(const shed_std::Vvector<uint8_t>& data,const shed_std::Sstring& filename){
        shed_std::Vvector<uint8_t> out;
        
        uint16_t method = config.force_store ? 0 : 8;
        shed_std::Vvector<uint8_t> payload;

        if (config.force_store) {
            payload = data;
        } else {
            DeflateCompressor compressor(config);
            payload = compressor.compress(data);
        }

        uint32_t crc = calculate_crc32(data);
        uint32_t uncompressed_size = (uint32_t)data.size();
        uint32_t compressed_size = (uint32_t)payload.size();
        uint16_t filename_len = (uint16_t)filename.size();

        // 1. Local File Header
        write_u32(out,0x04034b50); // signature
        write_u16(out,20);         // Version
        write_u16(out, 0);          // Flags
        write_u16(out, method);     
        write_u32(out, 0);          // Time
        write_u32(out, crc);
        write_u32(out, compressed_size);
        write_u32(out, uncompressed_size);
        write_u16(out, filename_len);
        write_u16(out, 0);          // Extra
        for(int i=0; i<filename.size(); ++i) out.push_back((uint8_t)filename[i]);
        for(int i=0; i<payload.size(); ++i) out.push_back(payload[i]);

        // 2.Central Directory
        uint32_t cd_start = (uint32_t)out.size();
        write_u32(out, 0x02014b50);
        write_u16(out, 20);
        write_u16(out, 20);
        write_u16(out, 0);
        write_u16(out, method);
        write_u32(out, 0);
        write_u32(out, crc);
        write_u32(out, compressed_size);
        write_u32(out, uncompressed_size);
        write_u16(out, filename_len);
        write_u16(out, 0);
        write_u16(out, 0);
        write_u16(out, 0);
        write_u16(out, 0);
        write_u32(out, 0);
        write_u32(out, 0); // Local header offset (0 for single file)

        for(int i=0; i<filename.size(); ++i) out.push_back((uint8_t)filename[i]);

        uint32_t cd_size = (uint32_t)out.size() - cd_start;

         // 3. EOCD
        write_u32(out, 0x06054b50);
        write_u16(out, 0);
        write_u16(out, 0);
        write_u16(out, 1);
        write_u16(out, 1);
        write_u32(out, cd_size);
        write_u32(out, cd_start);
        write_u16(out, 0);

        return out;
    }
} // namespace shed_zip

#endif