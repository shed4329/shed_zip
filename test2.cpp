#include "zip_config.h"
#include "zip_archiver.h"
#include "unzip_extractor.h"
#include "shed_std/Oostream.h"
#include "shed_std/Cconsole_output.h"

int main() {

    shed_std::Sstring text = "ShedStd Auto-Detect Test! Will it deflate raw?";
    shed_std::Vvector<shed_zip::uint8_t> original;
    for(int i=0; i<text.size(); ++i) original.push_back((shed_zip::uint8_t)text[i]);

    // 1. 正常 GZIP 測試
    shed_zip::ZipConfig cfg(6);
    shed_zip::ZipArchiver archiver(cfg);
    auto gz_data = archiver.create_gzip(original, "test.txt");

    shed_zip::UnzipExtractor extractor;
    auto res1 = extractor.extract(gz_data);
    
    shed_std::Cconsole_output << "GZIP Extract: " << (extractor.get_status() == shed_zip::DecompressStatus::OK ? "OK" : "FAIL") << "\n";

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
    } else {
        shed_std::Cconsole_output << "FAIL! Code: " << (int)extractor.get_status() << "\n";
    }

    return 0;
}