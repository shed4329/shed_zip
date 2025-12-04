#include "zip_config.h"
#include "zip_archiver.h"
#include "unzip_extractor.h"
#include "shed_std/Oostream.h"
#include "shed_std/Cconsole_output.h"

// 辅助函数，将数字转换为Hex输出
void print_hex_byte(shed_std::Oostream& out, unsigned char byte) {
    const char* hex_chars = "0123456789ABCDEF";
    out << hex_chars[(byte >> 4) & 0xF];
    out << hex_chars[byte & 0xF];
    out << " ";
}

int main() {

    shed_std::Sstring text = "ShedZip Auto-Detect Test! Will it deflate raw?";
    shed_std::Vvector<shed_zip::uint8_t> original;
    for(int i=0; i<text.size(); ++i) original.push_back((shed_zip::uint8_t)text[i]);

    // 1. 正常 GZIP 測試
    shed_zip::ZipConfig cfg(6);
    shed_zip::ZipArchiver archiver(cfg);
    auto gz_data = archiver.create_gzip(original, "test.txt");

    for (int i = 0;i<gz_data.size(); ++i) {
        print_hex_byte(shed_std::Cconsole_output,gz_data[i]);
    }

    shed_zip::UnzipExtractor extractor;
    auto res1 = extractor.extract(gz_data);
    
    shed_std::Cconsole_output << "GZIP Extract: " << (extractor.get_status() == shed_zip::DecompressStatus::OK ? "OK" : "FAIL") << shed_std::end_line;

    if (extractor.get_status() == shed_zip::DecompressStatus::OK) {
        shed_std::Cconsole_output << "Content: ";
        // Iterate through the vector and print each character
        for (int i = 0; i < res1.size(); ++i) {
            shed_std::Cconsole_output << (char)res1[i]; // Cast uint8_t to char for text output
        }
        shed_std::Cconsole_output << shed_std::end_line;
    }

    // 2. Raw Deflate 測試 (Fallback)
    // 我們手動創建一個 Raw Deflate 串流 (不帶 GZIP header)
    shed_zip::DeflateCompressor raw_comp(cfg);
    auto raw_deflate_data = raw_comp.compress(original);

    // 嘗試用 Unzip 去解這個沒有 Header 的數據
    auto res2 = extractor.extract(raw_deflate_data);

    shed_std::Cconsole_output << "Raw Deflate Fallback: ";
    if (extractor.get_status() == shed_zip::DecompressStatus::OK) {
        shed_std::Cconsole_output << "OK! (Correctly fell back to inflate)\n";
        shed_std::Cconsole_output << "Recovered size: " << (int)res2.size() << "\n";

        shed_std::Cconsole_output << "Content: ";
        for (int i = 0; i < res2.size(); ++i) {
            shed_std::Cconsole_output << (char)res2[i];
        }
        shed_std::Cconsole_output << shed_std::end_line;
    } else {
        shed_std::Cconsole_output << "FAIL! Code: " << (int)extractor.get_status() << "\n";
    }

    return 0;
}