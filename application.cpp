#include "shed_std/iostream.h"
#include "shed_std/Eexception.h"
#include "shed_std/Sstring.h"
#include "zip_config.h"
#include "zip/zip_archiver.h"
#include "unzip/unzip_extractor.h"

/**
 * 压缩文件
 * @param source_file_name  源文件名字
 * @param in_archive_name   压缩文件中的名字
 * @param dest_file_name    压缩好的文件名字
 * @param size              文件大小(单位KB)
 * @param format_zip        是否是zip格式，默认为true，false的情况压缩为gzip
 */
void compress(shed_std::Sstring& source_file_name,shed_std::Sstring& in_archive_name,shed_std::Sstring& dest_file_name,int size,bool format_zip = true){
    shed_std::Ffile_output fout;
    shed_std::Ffile_input fin;

    // 1.读取二进制文件
    shed_std::Vvector<shed_zip::uint8_t> source_hex_read;
    if(fin.open(source_file_name.c_string())){
        for(int i = 0; i < size;++i){
            char buffer[1024];
            int len = fin.read_n(buffer,1024,false);
            for(int j = 0 ; j<len;++j) source_hex_read.push_back(buffer[j]);
        }
    }else{
        shed_std::Cconsole_output << "无法打开文件"<<shed_std::end_line;
        return;
    }
    if(source_hex_read.empty()){
        shed_std::Cconsole_output << "读取到文件为空!"<<shed_std::end_line;
        return;
    }

    // 2.压缩
    shed_zip::ZipConfig cfg(6);
    shed_zip::ZipArchiver archiver(cfg);
    shed_std::Vvector<shed_zip::uint8_t> zip_data;
    if(format_zip){
        zip_data = archiver.create_zip(source_hex_read,in_archive_name.c_string());
    }else{
        zip_data = archiver.create_gzip(source_hex_read,in_archive_name.c_string());
    }

    if(zip_data.empty()){
        shed_std::Cconsole_output << "压缩后数据为空!"<<shed_std::end_line;
    }

    // 3.写入到压缩文件
    if(fout.open(dest_file_name.c_string(),false)){
        shed_std::Sstring tmp;
        for(int i = 0;i<zip_data.size();++i) tmp.push_back(zip_data[i]);
        fout.put_chars(tmp.c_string(),tmp.size(),false);
         if(fout.is_good()){
            shed_std::Cconsole_output<<"压缩文件创建成功!"<<shed_std::end_line;
        }else{
            shed_std::Cconsole_output<<"压缩文件创建失败!"<<shed_std::end_line;
        } 
    }else{
        shed_std::Cconsole_output << "无法打开文件!"<<shed_std::end_line;
    }
    // 4.关闭流
    fout.close();
    fin.close();
}

/**
 * 解压缩文件，会自动尝试zip和gzip还有纯deflate三种操作
 * @param source_file_name 需要解压的文件
 * @param dest_file_name   解压缩出来的文件
 * @param size             压缩文件大小
 */
void extract(shed_std::Sstring& source_file_name,shed_std::Sstring& dest_file_name,int size){
    shed_std::Ffile_input fin;
    shed_std::Ffile_output fout;
    
    // 1.读取文件
    shed_std::Vvector<shed_zip::uint8_t> zip_hex_data;
    if(fin.open(source_file_name.c_string())){
        for(int i = 0; i<size;++i){
            char buffer[1024];
            int len = fin.read_n(buffer,1024,false);
            for(int j = 0;j<len;++j) zip_hex_data.push_back(buffer[j]);
        }
    }else{
        shed_std::Cconsole_output << "无法打开文件" <<shed_std::end_line;
        return;
    }

    if(zip_hex_data.empty()){
        shed_std::Cconsole_output << "文件为空"<<shed_std::end_line;
        return;
    }

    // 2.解压文件
    shed_zip::UnzipExtractor extractor;
    auto extracted_data = extractor.extract(zip_hex_data);
    
    if(extracted_data.empty()){
        shed_std::Cconsole_output << "解压后文件大小为空"<<shed_std::end_line;
    }

    // 3.输出到文件
    if(fout.open(dest_file_name.c_string(),false)){
        shed_std::Sstring tmp;
        for(int i = 0;i<extracted_data.size();++i) tmp.push_back(extracted_data[i]);
        fout.put_chars(tmp.c_string(),tmp.size(),false);
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


/***
 * 已知错误：
 * 输入与数字不匹配的情况的时候，缓冲区垃圾导致一直读取不到有用的信息
 */
void show(){
    shed_std::Cconsole_output << "====================================" <<shed_std::end_line;
    shed_std::Cconsole_output << "欢迎使用本压缩软件"<<shed_std::end_line;

    while(true){
        shed_std::Cconsole_output << "压缩[C]  解压[E] 关于[A] 退出[Q]"<<shed_std::end_line;
        char c[10]={0};
        shed_std::Cconsole_input >> c;
        
        // 压缩分支
        if(c[0] == 'c' || c[0] == 'C'){
            shed_std::Cconsole_output << "请输入文件名:";
            shed_std::Cconsole_output.flush();
            shed_std::Sstring file_name;
            shed_std::Cconsole_input >> file_name;
            shed_std::Cconsole_output << shed_std::end_line;

            shed_std::Cconsole_output << "请输入源文件大小(KB),文件大小不应当超过64KB:";
            shed_std::Cconsole_output.flush();
            int size;
            shed_std::Cconsole_input >> size;
            shed_std::Cconsole_output << shed_std::end_line;

            if(size <= 0){
                shed_std::Cconsole_output << "文件大小不能小于等于0" << shed_std::end_line; 
                continue;
            }

            shed_std::Cconsole_output << "请输入压缩文档内名称:";
            shed_std::Cconsole_output.flush();
            shed_std::Sstring in_archive_name;
            shed_std::Cconsole_input >> in_archive_name;
            shed_std::Cconsole_output << shed_std::end_line;

            shed_std::Cconsole_output << "请输入压缩文档名:";
            shed_std::Cconsole_output.flush();
            shed_std::Sstring archive_name;
            shed_std::Cconsole_input >> archive_name;
            shed_std::Cconsole_output << shed_std::end_line;

            // 选择压缩格式
            shed_std::Cconsole_output << "请选择压缩格式(Zip[Z]/Gzip[G]，默认Zip):";
            shed_std::Cconsole_output.flush();
            shed_std::Sstring fmt;
            shed_std::Cconsole_input >> fmt;
            shed_std::Cconsole_output << shed_std::end_line;
            
            bool is_zip = true;
            if(fmt[0] == 'g' || fmt[0] == 'G'){
                is_zip = false;
            }

            // 调用压缩函数
            compress(file_name, in_archive_name, archive_name, size, is_zip);

        // 解压分支
        }else if(c[0] == 'e' || c[0] == 'E'){
            shed_std::Cconsole_output << "请输入需要解压的文件名:";
            shed_std::Cconsole_output.flush();
            shed_std::Sstring source_file;
            shed_std::Cconsole_input >> source_file;

            shed_std::Cconsole_output << "请输入压缩文件大小(KB):";
            shed_std::Cconsole_output.flush();
            int size = 0;
            shed_std::Cconsole_input >> size;

            if(size <= 0){
                shed_std::Cconsole_output << "文件大小不能小于等于0" << shed_std::end_line;
                shed_std::Cconsole_input.clear(); // 清理失败标志位
                continue;
            }

            shed_std::Cconsole_output << "请输入解压后的文件名:";
            shed_std::Cconsole_output.flush();
            shed_std::Sstring dest_file;
            shed_std::Cconsole_input >> dest_file;

            // 调用解压函数
            extract(source_file, dest_file, size);

        // 关于分支
        }else if(c[0] == 'a' || c[0] == 'A'){
            shed_std::Cconsole_output << "====================================" << shed_std::end_line;
            shed_std::Cconsole_output << "本压缩软件支持Zip/Gzip格式的压缩与解压" << shed_std::end_line;
            shed_std::Cconsole_output << "版本:1.0" << shed_std::end_line;
            shed_std::Cconsole_output << "shed_std v0.16 beta 3"<<shed_std::end_line;
            shed_std::Cconsole_output << "shed_zip v0.02"<<shed_std::end_line;
            shed_std::Cconsole_output << "====================================" << shed_std::end_line;

        // 退出分支
        }else if(c[0] == 'q' || c[0] == 'Q'){
            shed_std::Cconsole_output << "感谢使用，再见！" << shed_std::end_line;
            break; // 退出循环，结束程序

        // 无效输入分支
        }else{
            shed_std::Cconsole_output << "输入无效，请输入 C/E/A/Q 中的一个！" << shed_std::end_line;
        }
    }
}

// 主函数（补充入口）
int main() {
    try{
        show();
    }catch (shed_std::Eexception& e){
        shed_std::Cconsole_output << e.what() << shed_std::end_line;
    }
    return 0;
}