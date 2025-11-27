#ifndef IIOSTREAMBASE_H
#define IIOSTREAMBASE_H


#include "IiostreamBuf.h"
namespace shed_std{
    class IiostreamBase{
        protected:
            IiostreamBuf* _buf; // 缓冲区指针
            // 保护构造函数
            explicit IiostreamBase(IiostreamBuf* buf):_buf(buf){}

            // 禁止拷贝构造和赋值
            IiostreamBase(const IiostreamBase& other) = delete;
            IiostreamBase& operator=(const IiostreamBase& other) = delete;
            // TODO:移动构造函数和赋值
        public:
            // 析构函数
            virtual ~IiostreamBase() = default;
            // 获取缓冲区
            IiostreamBuf* read_buffer() const{
                return _buf;
            }

            // 状态检查函数
            bool is_good() const{
                return _buf && _buf->state() == Iiostream_state::IO_GOOD;
            }

            bool is_fail() const{
                // 因为状态是位图，应该取关注的位检查
                return _buf && (_buf->state() & Iiostream_state::IO_FAIL)!=0;
            }

            bool is_bad() const{
                return _buf&&(_buf->state() & Iiostream_state::IO_BAD)!=0;
            }

            bool is_eof() const{
               return _buf&&(_buf->state() & Iiostream_state::IO_EOF)!=0;
            }

            // 清除标志位
            void clear(){
                if(_buf){
                    _buf->setState(Iiostream_state::IO_GOOD);
                }
            }

            // 刷新缓冲区
            void flush(){
                if(_buf && is_good()){
                    _buf -> flush();
                }
            }
    };
}

#endif // IiostreamBase_H