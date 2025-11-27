#ifndef IIOSTREAMBUFFILEOUTPUT_H
#define IIOSTREAMBUFFILEOUTPUT_H

#include "IiostreamBuf.h"

#ifdef _WIN32
typedef void*                   HANDLE;
typedef unsigned long           DWORD;
typedef int                     BOOL;
typedef long                    LONG;
typedef LONG*                   PLONG;
#define TRUE                    1
#define FALSE                   0
typedef DWORD*                  LPDWORD;
typedef void*                   LPVOID;
#define VOID                    void
// Windows 文件写入相关的API和宏
#define INVALID_HANDLE_VALUE    ((HANDLE)-1)  // 错误1修正：原(HANDLE - 1)语法错误，应为(HANDLE)-1
#define CREATE_ALWAYS           2               // 总是创建新文件
#define OPEN_EXISTING           3               // 打开现有文件（用于追加）
#define GENERIC_WRITE           0X40000000      // 写权限
#define FILE_ATTRIBUTE_NORMAL   0X80            // 正常文件属性
// Windows API 声明（从kernel32.dll导入)
extern "C" HANDLE __stdcall CreateFileA(
    const char* lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPVOID lpSecurityAttributes,
    DWORD  dwCreationDisposition,
    DWORD  dwFlagAndAttibutes,
    HANDLE hTemplateFile
);
// 错误2修正：WriteFile返回值是BOOL，原声明为HANDLE，与实际API不一致
extern "C" BOOL __stdcall WriteFile(
    HANDLE hFile,                       // 文件或IO设备的句柄
    const VOID* lpBuffer,               // 要写入文件和设备数据
    DWORD nNumberOfBytesToWrite,
    LPDWORD lpNumberOfBytesWritten,
    LPVOID lpOverlapped
);
extern "C" BOOL __stdcall CloseHandle(HANDLE hObject);
extern "C" DWORD __stdcall SetFilePointer(
    HANDLE  hFile,
    LONG lDistanceToMove,
    PLONG lDistanceToMoveHigh,
    DWORD  dwMoveMethod
);
#define FILE_END 2  // 文件末尾
#else
// Linux相关的宏和系统调用声明
#define O_WRONLY        1    // 只写模式
#define O_CREAT         64   // 若文件不存在则创建
#define O_TRUNC         512  // 截断模式（覆盖模式）
#define O_APPEND        2048 // 追加模式
#define S_IRUSR         0400 // 所有者读权限
#define S_IWUSR         0200 // 所有者写权限

// Linux 补充mode_t类型声明（避免隐式声明警告）
typedef unsigned int mode_t;
extern "C" int open(const char* pathname,int flags, mode_t mode);   // mode用于创建文件时指定权限
extern "C" int write(int fd, const void* buf, unsigned int count);
extern "C" int close(int fd);

#endif

namespace shed_std{
    class IiostreamBufFileOutput : public IiostreamBuf{
        private:
            static constexpr int BUF_SIZE = 1024;
            char _buf[BUF_SIZE];    // 写入缓冲区
            int _pos = 0;           // 当前写入位置

            // 平台相关句柄
            #ifdef _WIN32
                HANDLE _hFile = INVALID_HANDLE_VALUE;
            #else   
                int _fd = -1;
            #endif

            // 刷新缓冲区到文件（核心函数）
            void flush_buf(){
                if (_pos == 0) return; // 缓冲区为空，无需刷新

                #ifdef _WIN32
                    // Windows 实现:调用WriteFile
                    if(_hFile == INVALID_HANDLE_VALUE){
                        setState(Iiostream_state::IO_BAD);
                        return;
                    }
                    DWORD bytes_written = 0;
                    BOOL success = WriteFile(
                        _hFile,
                        _buf,
                        static_cast<DWORD>(_pos), // 要写入的字节数
                        &bytes_written,
                        nullptr
                    );
                    if(!success|| bytes_written != static_cast<DWORD>(_pos)){
                        setState(Iiostream_state::IO_FAIL);
                    }
                #else   
                    // Linux实现
                    if(_fd == -1){
                        setState(Iiostream_state::IO_BAD);
                        return;
                    }
                    int bytes_written = ::write(_fd,_buf,static_cast<unsigned int>(_pos)); // 调用系统函数
                    if( bytes_written <0 || static_cast<unsigned int>(bytes_written) != static_cast<unsigned int>(_pos)){
                        setState(Iiostream_state::IO_FAIL);
                    }
                #endif
                    _pos = 0; // 重置缓冲区位置
            }
        public:
            // 打开文件
            bool open(const char* path, bool append = false){
                // 先关闭已打开的文件
                close();
                #ifdef _WIN32
                    // Windows：打开文件(根据模式选择创建方式)
                    DWORD creation_mode = append ? OPEN_EXISTING : CREATE_ALWAYS;
                    _hFile = CreateFileA(
                        path,
                        GENERIC_WRITE,          // 写权限
                        0,                      // 不共享
                        nullptr,                // 默认安全属性
                        creation_mode,          // 创建/打开模式
                        FILE_ATTRIBUTE_NORMAL,  // 正常文件模式
                        nullptr
                    );  

                    // 错误3修正：原逻辑颠倒，追加模式应移动指针而非返回false
                    if(append && _hFile != INVALID_HANDLE_VALUE){
                        SetFilePointer(_hFile, 0, nullptr, FILE_END); // 移动到文件末尾
                    }
                    // 检查文件是否打开成功（补充遗漏的判断）
                    if(_hFile == INVALID_HANDLE_VALUE){
                        setState(Iiostream_state::IO_BAD);
                        return false;
                    }
                #else
                    int flags = O_WRONLY | O_CREAT; // 只写 + 不存在则创建
                    if(append){
                        flags |= O_APPEND;  // 追加模式
                    }else{
                        flags |= O_TRUNC;   // 覆盖模式
                    }
                    // 文件权限:所有者读写，其他无权限
                    _fd = ::open(path,flags,S_IRUSR | S_IWUSR); // 使用系统的命名空间里的函数，而不是自己的函数，防止循环调用
                    if(_fd == -1){
                        setState(Iiostream_state::IO_BAD);
                        return false;
                    }
                #endif
                setState(Iiostream_state::IO_GOOD);
                return true;
            }

            // 关闭文件(自动刷新缓冲区)
            void close(){
                flush_buf();

                #ifdef _WIN32
                    if(_hFile != INVALID_HANDLE_VALUE){
                        CloseHandle(_hFile);
                        _hFile = INVALID_HANDLE_VALUE;
                    }
                #else
                    if(_fd != -1){ // 修正：原_fd!=-1缺少空格，语法不规范（不影响编译但可读性差）
                        ::close(_fd);
                        _fd = -1;
                    }
                #endif
                    _pos = 0;
                    setState(Iiostream_state::IO_GOOD);
            }

            ~IiostreamBufFileOutput() override{
                close(); // 析构时确保关闭并刷新
            }

            // 写入单个字符
            void stream_put_char(char c) override{
                // 检查文件是否打开
                #ifdef _WIN32
                    if(_hFile == INVALID_HANDLE_VALUE){
                #else
                    if(_fd == -1){
                #endif
                        setState(Iiostream_state::IO_BAD);
                        return; // 补充return：避免文件未打开时仍执行后续写入逻辑
                    }
                
                // 缓冲区满则刷新
                if(_pos >= BUF_SIZE){
                    flush_buf();
                }

                _buf[_pos++] = c;
            }

            // 写入指定长度的字符串
            void stream_put_n_char(const char* str,int n) override{
                if(!str || n <= 0){
                    setState(Iiostream_state::IO_BAD);
                    return;
                }

                // 检查文件是否打开
                #ifdef _WIN32
                    if(_hFile == INVALID_HANDLE_VALUE){
                #else
                    if(_fd == -1){
                #endif
                        setState(Iiostream_state::IO_BAD);
                        return;
                    }

                    // 分批写入，避免缓冲区溢出
                    while(n > 0){
                        int remain = BUF_SIZE - _pos;// 缓冲区剩余空间
                        if(remain <= 0){
                            flush_buf();
                            remain = BUF_SIZE;
                        }

                        // 本次写入长度,取剩余空间和剩余字符的较小值
                        int write_len = (n < remain) ? n : remain;
                        // 复制到缓冲区
                        for(int i = 0;i<write_len;++i){
                            _buf[_pos++] = str[i];
                        }

                        str += write_len;   // 移动字符串指针
                        n -= write_len;     // 剩余字符串递减
                    }
            }

            // 手动刷新
            void flush() override{
                flush_buf();
            }

            // 禁用输入相关接口（写文件不支持读取）
            int stream_bump_char() override {
                setState(Iiostream_state::IO_BAD);
                return -1;
            }
            int stream_get_char() override {
                setState(Iiostream_state::IO_BAD);
                return -1;
            }
    };
}

#endif // IIOSTREAMBUFFFILEOUTPUT_H