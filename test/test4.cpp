#include "../zip_config.h"
#include "../shed_std/Sstring.h"
#include "../shed_std/Vvector.h"
#include "../shed_std/Cconsole_output.h"
#include "../zip/zip_archiver.h"
#include "../shed_std/Ffile_output.h"
#include "../shed_std/Ffile_input.h"
#include "../unzip/unzip_extractor.h"
#include "../shed_std/Eexception.h"

/**
 * 测试压缩文件的压缩和读取
 */


 void func(){
    shed_std::Sstring text = "shed_zip is a zip libarary developed base on shed_std by shed4329!";
    shed_std::Vvector<shed_zip::uint8_t> original;
    for(int i = 0;i<text.size();++i) original.push_back((shed_zip::uint8_t)text[i]);

    // GZIP
    shed_std::Cconsole_output << "GZIP TEST"<<shed_std::end_line;
    shed_zip::ZipConfig cfg(6);
    shed_zip::ZipArchiver archiver(cfg);
    auto gz_data = archiver.create_gzip(original,"test.txt");

    shed_std::Ffile_output fout;
    shed_std::Ffile_input  fin;

    if(fout.open("test4.gz",false)){
        shed_std::Sstring str;
        for(int i = 0;i<gz_data.size();++i) str.push_back(gz_data[i]);
        fout.put_chars(str.c_string(),str.size(),false);
        if(fout.is_good()){
            shed_std::Cconsole_output<<"create gzip sucessfully!"<<shed_std::end_line;
        }else{
            shed_std::Cconsole_output<<"create gzip FAILED!"<<shed_std::end_line;
        } 
    }else{
        shed_std::Cconsole_output<<"fopen FAILED!"<<shed_std::end_line;
    } 

    fout.close();

    shed_zip::UnzipExtractor extractor;

    if(fin.open("test4.gz")){
        char buffer[1024];
        //shed_std should provide some file API?
        int len = fin.read_n(buffer,1024,false);
        shed_std::Vvector<shed_zip::uint8_t> read_gzip_raw;
        for(int i = 0;i<len;++i) read_gzip_raw.push_back(buffer[i]);

        auto res_gzip = extractor.extract(read_gzip_raw);

        for (int i = 0; i < res_gzip.size(); ++i) {
            shed_std::Cconsole_output << (char)res_gzip[i]; // Cast uint8_t to char for text output
        }

        shed_std::Cconsole_output << shed_std::end_line;
    }
}

int main(){
    try{
        func();
    }catch(shed_std::Eexception& e){
        shed_std::Cconsole_output << e.what() << shed_std::end_line;
    }
}

