#ifndef IIOSTREAMBUFFILEINPUT_H
#define IIOSTREAMBUFFILEINPUT_H

#include "IiostreamBuf.h"

#ifdef _WIN32
// 可以去看MS的文档
typedef void*                   HANDLE;         // 句柄类型
typedef unsigned long           DWORD;          // 32位无符号整数，双字
typedef int                     BOOL;           // 布尔类型
#define TRUE 1
#define FALSE 0
typedef DWORD* LPDWORD;                         // LPDWORD 是 DWORD* 的别名
typedef void* LPVOID;                           // LPVOID  是 VOID*  的别名
#define VOID void                               // Microsoft在WinNT.h中的声明

// Windows 文件操作API声明
#define INVALID_HANDLE_VALUE    ((HANDLE)-1)    // 无效句柄标识
#define OPEN_EXISTING           3               // 创建文件模式，仅打开已存在的文件
#define GENERIC_READ            0x80000000      // 读权限

// 对应函数API在Windows的kernel32.dll里

extern "C" HANDLE __stdcall CreateFileA(
    const char* lpFileName,             // 要打开的文件或设备名称
    DWORD dwDesiredAccess,             // 对文件或设备的访问权限,，可以汇总为读取，写入或0，只是两者均为提供
    DWORD dwShareMode,                  // 请求的文件或设备的共享模式，可以读取，写入，删除，所有这些或无
    LPVOID lpSecurityAttributes,         // 该参数可以为NULL
    DWORD dwCreationDisposition,        // 对存在或不存在的
    DWORD dwFlagsAndAttributes,         // 文件或设备属性和标志
    HANDLE hTemplateFile                // 具有 GENERIC_READ 访问权限的模板文件的有效句柄。
);
extern "C" BOOL __stdcall ReadFile(
    HANDLE  hFile,                      // 设备的句柄
    LPVOID  lpBuffer,                   // 指向接收从文件或设备读取的数据的缓冲区的
    DWORD   nNumberOfBytesToRead,       // 要读取的最大字节数。
    LPDWORD lpNumberOfBytesRead,        // 指向使用同步 hFile 参数时接收读取的字节数的变量的指针。在Windows 7上，此参数不能NULL。
    LPVOID  lpOverlapped                // 如果使用 FILE_FLAG_OVERLAPPED打开 hFile 参数，则需要指向 OVERLAPPED 结构的指针，否则它可以 NULL。
);
extern "C" BOOL __stdcall CloseHandle(HANDLE hObject);

#else
    #define O_RDONLY        0                                               // 只读模式
    extern "C" int open(const char* pathname, int flags);                   // 打开文件
    extern "C" int read(int fd,void* buf,unsigned int count);  // 读文件
    extern "C" int close(int fd);                                           // 关闭文件
#endif

namespace shed_std{
    class IiostreamBufFileInput : public IiostreamBuf{
        private:
            static constexpr int BUF_SIZE = 1024;
            char _buf[BUF_SIZE];                    // 读缓冲区
            int _pos = 0;                           // 当前读取的位置
            int _len = 0;                           // 缓冲区有效长度
            
            #ifdef _WIN32
                HANDLE _hFile = INVALID_HANDLE_VALUE; // Windows文件句柄
            #else
                int _fd = -1;                         // Linux文件描述符
            #endif

            // 从文件填充缓冲区
            void fill_buf(){
                _pos = 0;
                _len = 0;
            
                #ifdef _WIN32
                    //Windows实现
                    if(_hFile == INVALID_HANDLE_VALUE){
                        setState(Iiostream_state::IO_BAD);
                        return;
                    }
                    DWORD bytes_read = 0;
                    BOOL success = ReadFile(
                        _hFile,
                        _buf,
                        BUF_SIZE,
                        &bytes_read,
                        nullptr
                    );
                    if(!success){
                        setState(Iiostream_state::IO_FAIL);
                        return;
                    }
                    _len = (int)bytes_read;
                    if(_len == 0){
                        // 读取到文件末尾
                        setState(Iiostream_state::IO_EOF);
                    }
                 #else  
                    // Linux实现
                    if(_fd == -1){
                        setState(Iiostream_state::IO_BAD);
                        return;
                    }
                    int bytes_read = read(_fd,_buf,BUF_SIZE);
                    if(bytes_read < 0){
                        // 读取错误
                        setState(Iiostream_state::IO_FAIL);
                        return;
                    }
                    _len = bytes_read;
                    if(_len == 0){
                        // 读取到文件末尾
                        setState(Iiostream_state::IO_EOF);
                    }
                #endif

            }
    
        public:
            // 以只读模式打开文件
            bool open(const char* filename){
                // 先关闭已经打开的文件
                close();
                #ifdef _WIN32
                    // Windows调用CreateFileA打开文件
                    _hFile = CreateFileA(
                        filename,
                        GENERIC_READ,       // 只读权限
                        0,                  // 不共享
                        nullptr,            // 默认安全属性
                        OPEN_EXISTING,      // 仅打开已存在文件
                        0,                  // 默认属性
                        nullptr
                    );
                    if(_hFile == INVALID_HANDLE_VALUE){
                        setState(Iiostream_state::IO_BAD);
                        return false;
                    }
                #else
                    // Linux 调用open打开文件 
                    _fd = ::open(filename,O_RDONLY);//指定为系统的namesapce
                    if(_fd == -1){
                        setState(Iiostream_state::IO_BAD);
                        return false;
                    }
                #endif
                    setState(Iiostream_state::IO_GOOD);
                    return true;
            }
            
            // 关闭文件
            void close(){
                #ifdef _WIN32
                    if(_hFile != INVALID_HANDLE_VALUE){
                        CloseHandle(_hFile);
                        _hFile = INVALID_HANDLE_VALUE;
                    }
                #else
                    if(_fd != -1){
                        ::close(_fd); // 显式指定为Linux系统的方法
                        _fd = -1;
                    }
                #endif  
                    _pos = 0;
                    _len = 0;
                    setState(Iiostream_state::IO_BAD);
            }

            // 析构函数，确保正确关闭文件
            ~IiostreamBufFileInput() override{
                close();
            }
            
            // 输入接口:获取下一个字符并移动指针
            int stream_bump_char() override{
                if(_pos >= _len){
                    fill_buf();
                    if(_len == 0){
                        return -1;
                    }
                }
                return (char)_buf[_pos++];
            }

            // 输入接口:获取当前字符，不移动指针
            int stream_get_char() override {
                if(_pos >= _len){
                    fill_buf();
                    if(_len == 0){
                        return -1;
                    }
                }
                return (char)_buf[_pos];
            }

            // 禁用输出相关接口
            void stream_put_char(char ) override{
                setState(Iiostream_state::IO_BAD);
            }

            void stream_put_n_char(const char* , int) override{
                setState(Iiostream_state::IO_BAD);
            }

            // 读文件时无需刷新
            void flush() override{}

    };
}


#endif // IIOSTREAMBUFFILEINPUT_H