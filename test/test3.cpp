#include "../zip_config.h"
#include "../shed_std/Cconsole_output.h"
#include "../shed_std/Ffile_output.h"
#include "../zip/zip_archiver.h"

/**
 * 測試二進制寫入壓縮包
 */
int main(){
    shed_std::Ffile_output fout;
    
    shed_std::Cconsole_output << "Starting gzip creating test..."<<shed_std::end_line;

    shed_std::Sstring text = "This is text message from shed_zip test for testing binary writing";
    shed_std::Vvector<shed_zip::uint8_t> original;
    for(int i=0;i<text.size();++i) original.push_back((shed_zip::uint8_t)text[i]);

    shed_zip::ZipConfig cfg(6);
    shed_zip::ZipArchiver archiver(cfg);

    auto gz_data = archiver.create_gzip(original,"test.txt");

    shed_std::Cconsole_output <<"Compressed size: " << (int)gz_data.size() << " bytes" <<shed_std::end_line;

    if(fout.open("data.gz",false)){
        shed_std::Sstring str;
        for(auto it = gz_data.begin();it!=gz_data.end();++it){
            str.push_back(*it);
        }
        fout.put_chars(reinterpret_cast<const char*>(str.c_string()), gz_data.size(), false);
        fout.close();
        shed_std::Cconsole_output << "Successfully wrote to data.gz\n";
    }else{
        shed_std::Cconsole_output << "Failed to open data.gz for writing.\n";
    }

    auto zip_data = archiver.create_zip(original,"test.txt");

    shed_std::Cconsole_output <<"Compressed size: " << (int)zip_data.size() << " bytes" <<shed_std::end_line;

    if(fout.open("data.zip",false)){
        shed_std::Sstring str;
        for(auto it = zip_data.begin();it!=zip_data.end();++it){
            str.push_back(*it);
        }
        fout.put_chars(reinterpret_cast<const char*>(str.c_string()), zip_data.size(), false);
        fout.close();
        shed_std::Cconsole_output << "Successfully wrote to data.zip\n";
    }else{
        shed_std::Cconsole_output << "Failed to open data.zip for writing.\n";
    }
}