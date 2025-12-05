#ifndef IIOSTREAMBUF_H
#define IIOSTREAMBUF_H

namespace shed_std{
    // 流状态枚举
    enum Iiostream_state{
        IO_GOOD = 0, // 正常状态
        IO_FAIL = 1, // 操作失败
        IO_BAD = 2, // 严重错误
        IO_EOF = 4 // 到达文件尾
    };

    // 缓冲区基类:纯抽象类，定义输入输出缓冲区的通用接口
    class IiostreamBuf { 
        protected:
            // 缓冲区状态，位图
            Iiostream_state _state = Iiostream_state::IO_GOOD;
            // 保护构造函数：只允许基类实例化
            IiostreamBuf() = default;
        public:
            // 析构函数
            virtual ~IiostreamBuf() = default;
            // 输出流操作:单个字符写入到缓冲区
            virtual void stream_put_char(char c) = 0;
            // 输出流操作:写入n个字符到缓冲区（n个字符以str开始)
            virtual void stream_put_n_char(const char* str,int n) = 0;
            // 输出流操作:刷新缓冲区，并将数据写入目标设备
            virtual void flush() = 0;
            // 输入操作:读取下一个字符（并移动位置），返回ASCII值，EOF返回-1
            virtual int stream_bump_char() = 0;
            // 输入操作:查看当前字符（不移动读取位置），返回字符ASCII值，EOF返回-1
            virtual int stream_get_char() = 0;
            // 获取缓冲区状态
            Iiostream_state state() const{return _state;}
             // 状态设置函数：供派生类使用
            void setState(Iiostream_state state){
                if(state == Iiostream_state::IO_GOOD){
                    _state = Iiostream_state::IO_GOOD;
                }else{
                    _state = static_cast<Iiostream_state>(_state | state);
                }
            }
    };
}

#endif // IIOSTREAMBUF_H