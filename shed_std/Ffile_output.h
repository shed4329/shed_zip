#ifndef FFILE_OUTPUT_H
#define FFILE_OUTPUT_H

#include "Oostream.h"
#include "IiostreamBufFileOutput.h"

namespace shed_std{
    // 文件输出流
    class Ffile_output : public Oostream{
        private:
            IiostreamBufFileOutput _buf;
            static constexpr int MAX_LENGTH = 65535;
            int string_length_n(const char* str,int n = MAX_LENGTH){
                if(str == nullptr) return 0;
                int len = 0;
                for(int i=0;i<n;++i){
                    if(str[i] == '\0') break;
                    ++len;
                }
                return len;
            }
        public:
            // 构造函数,绑定缓冲区
            Ffile_output() : Oostream(&_buf){}

            // 打开文件
            bool open(const char* filename, bool is_append){
                return _buf.open(filename,is_append);
            }

            // 关闭文件（自动刷新缓冲区）
            void close(){
                _buf.flush();
                _buf.close();
            }

            // 检查文件是否打开
            bool is_open() const{
                return _buf.state() == Iiostream_state::IO_GOOD;
            }

            // 写入单个字符
            void put(char c){
                _buf.stream_put_char(c);
            }

            // 写入字符串
            void write(const char* str,int len = -1){
                if(len == -1){
                    len = string_length_n(str);
                }
                _buf.stream_put_n_char(str,len);
            }

            // 刷新缓冲区
            void flush(){
                _buf.flush();
            }

            // 禁用拷贝（避免文件句柄重复释放）
            Ffile_output(const Ffile_output&) = delete;
            Ffile_output& operator=(const Ffile_output&) = delete;

            // 支持移动（方便传递所有权）
            Ffile_output(Ffile_output&&) = default;
            Ffile_output& operator=(Ffile_output&&) = default;

            // 析构函数：确保关闭文件
            ~Ffile_output() override {
                close();
            }


    };
}
#endif // FFILE_OUTPUT_H