#include "deflate_compressor.h"
#include "shed_std/Vvector.h"
#include "shed_std/Sstring.h"
#include "shed_std/Oostream.h"
#include "shed_std/Cconsole_output.h"

// 辅助函数，将数字转换为Hex输出
void print_hex_byte(shed_std::Oostream& out, unsigned char byte) {
    const char* hex_chars = "0123456789ABCDEF";
    out << hex_chars[(byte >> 4) & 0xF];
    out << hex_chars[byte & 0xF];
    out << " ";
}

int func(){
     shed_zip::DeflateCompressor compressor;

    // 测试文本
    shed_std::Sstring text = "HELLO HELLO HELLO HELLO WORLD! THIS IS A TEST OF THE SHED_ZIP DEFLATE COMPRESSOR.";
    
    shed_std::Vvector<unsigned char> input_data;
    for (int i = 0; i < text.size(); ++i) {
        input_data.push_back(static_cast<unsigned char>(text[i]));
    }

    shed_std::Cconsole_output << "Original Text: " << text << shed_std::end_line;
    shed_std::Cconsole_output << "Original Size: "<< shed_std::end_line;
    shed_std::Cconsole_output<<input_data.size()<< shed_std::end_line;
    shed_std::Cconsole_output << " bytes"<< shed_std::end_line;

    // 执行压缩
    shed_std::Vvector<unsigned char> compressed = compressor.compress(input_data);

    shed_std::Cconsole_output << "Compressed Size: "<< shed_std::end_line;
    shed_std::Cconsole_output<<compressed.size() << shed_std::end_line;
    shed_std::Cconsole_output << " bytes"<< shed_std::end_line;
    
    shed_std::Cconsole_output << "Raw Deflate Output (Hex): " << shed_std::end_line;
    for (int i = 0; i < compressed.size(); ++i) {
        print_hex_byte(shed_std::Cconsole_output, compressed[i]);
    }
    shed_std::Cconsole_output<< shed_std::end_line;

    return 0;
}


/**
 * 具体的测试结果，可以用这个网站解密
 * https://www.toolhelper.cn/EncodeDecode/Compress
 * 选择Deflate算法
 */
int main() {
    try{
        func();
    }catch(shed_std::Eexception& e){
        shed_std::Cconsole_output << e.what() << shed_std::end_line;
    }
}