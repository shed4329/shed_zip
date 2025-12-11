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
 * 测试jpg的压缩和读取
 */


void func(){
    // 1.讀取jpg
    shed_std::Ffile_output fout;
    shed_std::Ffile_input  fin;

    //讀取二進制文件
    shed_std::Vvector<shed_zip::uint8_t> image_hex_read;
    if(fin.open("chart.svg")){
        // 64KB
        for(int i = 0; i< 64*1024/1024; ++i){
            char buffer[1024];
            //shed_std should provide some file API?
            int len = fin.read_n(buffer,1024,false);
            for(int j = 0;j<len;++j) image_hex_read.push_back(buffer[j]);
        } 
    }else{
        shed_std::Cconsole_output << "找不到文件" <<shed_std::end_line;
    }
    shed_std::Cconsole_output << "读取的文件大小="<<image_hex_read.size()<<shed_std::end_line;
    // 2.壓縮
    shed_zip::ZipConfig cfg(6);
    shed_zip::ZipArchiver archiver(cfg);
    auto zip_data = archiver.create_gzip(image_hex_read,"chart.svg");

    // 3.寫入到壓縮文件
    if(fout.open("test5.zip",false)){
        shed_std::Sstring str;
        for(int i = 0;i<zip_data.size();++i) str.push_back(zip_data[i]);
        fout.put_chars(str.c_string(),str.size(),false);
        if(fout.is_good()){
            shed_std::Cconsole_output<<"压缩文件创建成功!"<<shed_std::end_line;
        }else{
            shed_std::Cconsole_output<<"压缩文件创建失败!"<<shed_std::end_line;
        } 
    }else{
        shed_std::Cconsole_output << "无法打开文件!"<<shed_std::end_line;
    }
    // 关闭流
    fout.close();
    fin.close();

    // 4.读取zip
    shed_std::Vvector<shed_zip::uint8_t> zip_hex_read;
    if(fin.open("test5.zip")){
        // 64KB
        for(int i = 0; i< 8192*1024/1024; ++i){
            char buffer[1024];
            //shed_std should provide some file API?
            int len = fin.read_n(buffer,1024,false);
            for(int j = 0;j<len;++j) zip_hex_read.push_back(buffer[j]);
        } 
    }else{
        shed_std::Cconsole_output << "找不到文件" <<shed_std::end_line;
    }

    // 5.解压缩
    shed_zip::UnzipExtractor extractor;
    auto extracted_res = extractor.extract(zip_hex_read);
    shed_std::Cconsole_output << extracted_res.size() <<shed_std::end_line;

    // 6.写出到文件
    if(fout.open("chart2.svg",false)){
        shed_std::Sstring str;
        for(int i = 0;i<extracted_res.size();++i) str.push_back(extracted_res[i]);
        fout.put_chars(str.c_string(),str.size(),false);
        if(fout.is_good()){
            shed_std::Cconsole_output<<"写出到文件成功"<<shed_std::end_line;
        }else{
            shed_std::Cconsole_output<<"写出到文件失败"<<shed_std::end_line;
        } 
    }else{
         shed_std::Cconsole_output<<"无法创建文件"<<shed_std::end_line;
    }

    fin.close();
    fout.close();
}

int main(){
    try{
        func();
    }catch(shed_std::Eexception& e){
        shed_std::Cconsole_output << e.what() << shed_std::end_line;
    }
}

