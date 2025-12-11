#ifndef IISTREAM_H
#define IISTREAM_H

#include "IiostreamBase.h"

namespace shed_std{
    class Iistream : public IiostreamBase{
        private:

            inline bool is_blank_char(int character) {
                // 覆盖所有标准空白字符：空格、水平制表、换行、回车、换页、垂直制表
                return (character == ' '   || 
                        character == '\t'  || 
                        character == '\n'  || 
                        character == '\r'  || 
                        character == '\f'  || 
                        character == '\v');
            }

            inline char _to_lower(char character) {
                if (character >= 'A' && character <= 'Z') {
                    return character + ('a' - 'A');  // 大写转小写
                }
                return character;
            }

            inline bool _is_alpha(char character) {
                if (character >= 'A' && character <= 'Z') {
                    return true;
                }else if(character>='a'&&character<='z'){
                    return true;
                }
                return false;
            }

            inline int _str_cmp(const char* a, const char* b) {
                while (*a != '\0' && *b != '\0' && *a == *b) {
                    a++;
                    b++;
                }
                return *a - *b;  // 0表示相等，非0表示不等
            }
        public:
            // 构造函数，接受缓冲区指针
            explicit Iistream(IiostreamBuf* buf):IiostreamBase(buf){}

            explicit operator bool() const{
                return is_good();
            }

            static constexpr int MAX_READING_LENGTH = 1024;

            // 读取单个字符
            int get_char(){
                if(is_good()){
                    int character = _buf -> stream_bump_char();
                    if( character == -1){
                        _buf->setState(Iiostream_state::IO_EOF);
                    }
                    return character;
                }
                return -1;
            }
            
            // 读取n个字符到s,是否启用长度上限1024，启用时超出截断，移除上限对大文件二进制读取有帮助
            int read_n(char* s, int n,bool enable_max = true){
                if(!is_good() || s == nullptr || n < 0){
                    // 无效
                    return -1;
                }

                if(enable_max){
                    if(n > MAX_READING_LENGTH){
                        n = MAX_READING_LENGTH;
                    }
                }

                int read_len = 0;

                for(int i = 0 ; i< n; ++i){
                    int c = _buf->stream_bump_char();
                    if(c == -1){
                        _buf->setState(Iiostream_state::IO_EOF);
                        break;
                    }
                    ++read_len;
                    s[i] = static_cast<char>(c);
                }
                return read_len;
            }


            // 读取一行（不含换行符）
            Iistream& get_line(char* buf,int max_len = MAX_READING_LENGTH){
                if(!is_good() || !buf || max_len <= 0) return *this;
                int i = 0;
                while(i<max_len-1){// 留一个给\0
                    int character = _buf->stream_bump_char();
                    if(character == -1){
                        _buf->setState(Iiostream_state::IO_EOF);
                        break;
                    }
                    if(character == '\n'){
                        break;
                    }
                    buf[i++] = (char)character;
                }
                buf[i] = '\0';
                return *this;
            }

            // 运算符重载，读取整数,跳过之前的空格，制表符和换行符
            Iistream& operator>>(int& num){
                if(!is_good()) return *this;

                num = 0;
                bool negative = false;
                int character;

                // 跳过空白字符
                do{
                    character = _buf->stream_bump_char();
                    if(character == -1){
                        _buf->setState(Iiostream_state::IO_EOF);
                        return *this;
                    }
                }while(character==' '|| character=='\t' || character == '\n');

                // 处理符号
                if(character == '-'){
                    negative = true;
                    character = _buf -> stream_bump_char();
                }else if(character == '+'){
                    character = _buf -> stream_bump_char();
                }

                // 处理数字
                if( character<'0' || character >'9'){// 非数字字符，设置失败状态
                    _buf->setState(Iiostream_state::IO_FAIL);
                    return *this;
                }

                while(character>= '0' && character<='9' ){
                    num = num*10+(character-'0');
                    character = _buf->stream_bump_char();
                    if(character == -1){
                        break;
                    } 
                }

                if(negative){
                    num = -num;
                }

                return *this;
            }

            // 运算符重载,跳过之前的空格，制表符和换行符
            Iistream& operator>>(char* str){
                if(!is_good() || !str) {
                    return *this;
                }
            

                int character;
                // 跳过空白字符
                do{
                    character = _buf->stream_bump_char();
                    if(character == -1){
                        _buf->setState(Iiostream_state::IO_EOF);
                        return *this;
                    }
                }while(is_blank_char(character));

                // 读取非空字符串
                int i = 0;
                while(!is_blank_char(character) && i<MAX_READING_LENGTH){
                    str[i++] = (char)character;
                    character = _buf->stream_bump_char();
                }
                str[i] = '\0';
                return *this;
            }

            Iistream& operator>>(bool& val){
                if(!is_good()){
                    return *this;
                }

                val = false;
                int character;
                 do{
                    character = _buf->stream_bump_char();
                    if(character == -1){
                        _buf->setState(Iiostream_state::IO_EOF);
                        return *this;
                    }
                }while(is_blank_char(character));

                // 情况1:输入0或1
                if(character == '0' || character == '1'){
                    val = (character == '1');// 1 -> true; 0 -> false;
                    return *this;
                }

                // 情况2：输入true或false
                char buf[64] = {0};
                int i = 0;
                while(_is_alpha(character) && i<63){
                    buf[i++] = _to_lower(char(character));
                    character = _buf->stream_bump_char();
                    if(character == -1) break;
                }
                buf[i] = '\0';

                // 比较是否为 "true" 或 "false"（不依赖strcmp）
                bool is_true = (_str_cmp(buf, "true") == 0);
                bool is_false = (_str_cmp(buf, "false") == 0);

                if (is_true) {
                    val = true;
                } else if (is_false) {
                    val = false;
                } else {
                    _buf->setState(Iiostream_state::IO_FAIL);  // 格式错误
                }

                return *this;
            }

            Iistream& operator>>(double& val){
                if (!is_good()) return *this;

                val = 0.0;
                bool negative = false;
                bool has_integer = false;   // 是否有整数部分
                bool has_fraction = false;  // 是否有小数部分
                int character;

                // 跳过前置空白
                do {
                    character = _buf->stream_bump_char();
                    if (character == -1) {
                        _buf->setState(Iiostream_state::IO_EOF);
                        return *this;
                    }
                } while (is_blank_char(character));

                // 处理正负号
                if (character == '-') {
                    negative = true;
                    character = _buf->stream_bump_char();
                } else if (character == '+') {
                    character = _buf->stream_bump_char();
                }

                // 读取整数部分
                if(character>='0'&&character<='9'){
                     has_integer = true;
                    while (character>='0'&&character<='9') {
                        val = val * 10 + (character - '0');  // 整数累加（'0'的ASCII值减去得到数字）
                        character = _buf->stream_bump_char();
                        if (character == -1){
                             break;
                        }
                    }
                }

                // 处理小数部分（如 .45 或 123.45）
                if (character == '.') {
                    character = _buf->stream_bump_char();
                    if (!(character>='0'&&character<='9')) {  // 小数点后必须有数字（如 "123." 是错误）
                        _buf->setState(Iiostream_state::IO_FAIL);
                        return *this;
                    }

                    has_fraction = true;
                    double fraction = 0.1;  // 小数权重（0.1, 0.01, ...）
                    while (character>='0'&&character<='9') {
                        val += (character-'0')*fraction;
                        fraction *= 0.1;
                        character = _buf->stream_bump_char();
                        if (character == -1){
                             break;
                        }
                    }
                }

                if (!has_integer && !has_fraction) {
                    _buf->setState(Iiostream_state::IO_FAIL);  // 如仅输入 "+" 或 "-"
                    return *this;
                }
                if(negative){
                    val = -val;
                }
                return *this;
            }
    };
}
#endif