#ifndef IIOSTREAMBUFCONSOLE_H
#define IIOSTREAMBUFCONSOLE_H

#include "IiostreamBuf.h"

#ifdef _WIN32
// 可以去看MS的文档
typedef void*                   HANDLE;         // 句柄类型
typedef unsigned long           DWORD;          // 32位无符号整数，双字
#define STD_OUTPUT_HANDLE       ((DWORD)-11)    // 标准输出设备标识
#define STD_INTPUT_HANDLE       ((DWORD)-10)    // 标准输入设备标识
#define INVALID_HANDLE_VALUE    ((HANDLE)-1)    // 无效句柄标识

typedef int BOOL;           //Windows定义的BOOL
#define TRUE 1
#define FALSE 0
typedef DWORD* LPDWORD;     // LPDWORD 是 DWORD* 的别名
typedef void* LPVOID;       // LPVOID  是 VOID*  的别名
#define VOID void           // Microsoft在WinNT.h中的声明

// 对应函数API在Windows的kernel32.dll里
// 函数1:获取标准输出句柄
extern "C" HANDLE __stdcall GetStdHandle(DWORD nStdHandle);
// 函数2:向控制台写入字符(ANSI编码)
extern "C" BOOL __stdcall WriteConsoleA(
    HANDLE hConsoleOutput,           // 控制台句柄
    const VOID*  lpBuffer,          // 要输出的字符缓冲区
    DWORD nNumberOfCharsToWrite,    // 要输出的字符个数
    LPDWORD lpNumberOfCharsWritten, // 实际输出的字符数量，用于接受实际写入的字符数
    LPVOID lpReserved               // 保留参数，必须为nullptr
);
// 函数3:从控制台读入字符(ANSI编码)
extern "C" BOOL __stdcall ReadConsoleA(
    HANDLE hConsoleOutput,          // 控制台句柄
    VOID*  lpBuffer,                // 要输入的字符缓冲区
    DWORD nNumberOfCharsToRead,     // 要输出的字符个数
    LPDWORD lpNumberOfCharsRead,    // 实际输出的字符数量，用于接受实际写入的字符数
    LPVOID lpReserved               // 保留参数，必须为nullptr
);
#else
    #define STDOUT_FILENO 1 // stdout 的文件描述符
    #define STDIN_FILENO 0  // stdin 的文件描述符
    // 声明 write 系统调用：参数为文件描述符、缓冲区、长度，返回写入的字节数（int）
    extern "C" int write(int fd, const void* buf, unsigned int count);
    // 声明 read 系统调用
    extern "C" int read(int fd, void* buf, unsigned int count);  
#endif

namespace shed_std{
 class IiostreamBufConsoleInput : public IiostreamBuf {
    private:
        static const int INNER_BUF_SIZE = 1024;  // 输入缓冲区大小
        char _input_buf[INNER_BUF_SIZE];         // 仅用于输入的缓冲区
        int _input_pos = 0;                       // 输入读取位置（当前已读到的位置）
        int _input_len = 0;                       // 输入缓冲区中有效数据长度

        // 从控制台读取数据填充输入缓冲区
        void _fill_input_buf() {
            #ifdef _WIN32
                HANDLE hConsole = GetStdHandle(STD_INTPUT_HANDLE);  
                if (hConsole == INVALID_HANDLE_VALUE) {
                    setState(Iiostream_state::IO_BAD);
                    _input_len = 0;
                    return;
                }
                DWORD read = 0;
                if (!ReadConsoleA(hConsole, _input_buf, INNER_BUF_SIZE, &read, nullptr)) {
                    setState(Iiostream_state::IO_FAIL);
                    _input_len = 0;
                    return;
                }
                _input_len = static_cast<int>(read);
            #else
                int read_bytes = read(STDIN_FILENO, _input_buf, INNER_BUF_SIZE);
                if (read_bytes < 0) {
                    setState(Iiostream_state::IO_FAIL);
                    _input_len = 0;
                    return;
                }
                _input_len = read_bytes;
            #endif
                _input_pos = 0;  // 重置读取位置
        }

    public:
        ~IiostreamBufConsoleInput() override = default;  // 输入缓冲区无需刷新

        // 输入缓冲区不支持输出操作，调用则设置错误状态
        void flush() override {}
        void stream_put_char(char) override { setState(Iiostream_state::IO_BAD); }
        void stream_put_n_char(const char*, int) override { setState(Iiostream_state::IO_BAD); }

        // 输入接口：获取下一个字符并移动指针
        int stream_bump_char() override {
            if (_input_pos >= _input_len) {
                _fill_input_buf();  // 输入缓冲区空了，填充新数据
                if (_input_len == 0) {
                    setState(Iiostream_state::IO_EOF);
                    return -1;  // EOF
                }
            }
            return static_cast<unsigned char>(_input_buf[_input_pos++]);
        }

        // 输入接口：获取当前字符（不移动指针）
        int stream_get_char() override {
            if (_input_pos >= _input_len) {
                _fill_input_buf();  // 输入缓冲区空了，填充新数据
                if (_input_len == 0) {
                    setState(Iiostream_state::IO_EOF);
                    return -1;  // EOF
                }
            }
            return static_cast<unsigned char>(_input_buf[_input_pos]);
        }
    };

    class IiostreamBufConsoleOutput : public IiostreamBuf {
    private:
        static const int INNER_BUF_SIZE = 1024;  // 输出缓冲区大小
        char _output_buf[INNER_BUF_SIZE];        // 仅用于输出的缓冲区
        int _output_pos = 0;                      // 输出写入位置（当前已写到的位置）

        // 刷新输出缓冲区到控制台
        void _flush_output() {
            if (_output_pos <= 0) return;  // 缓冲区为空，无需刷新

            #ifdef _WIN32
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                if (hConsole == INVALID_HANDLE_VALUE) {
                    setState(Iiostream_state::IO_BAD);
                    return;
                }
                DWORD written = 0;
                if (!WriteConsoleA(hConsole, _output_buf, _output_pos, &written, nullptr)) {
                    setState(Iiostream_state::IO_FAIL);
                }
            #else
                int written = write(STDOUT_FILENO, _output_buf, _output_pos);
                if (written < 0) {
                    setState(Iiostream_state::IO_FAIL);
                }
            #endif
                _output_pos = 0;  // 重置写入位置
        }

    public:
        ~IiostreamBufConsoleOutput() override {
            _flush_output();  // 析构时确保缓冲区数据输出
        }

        // 刷新输出缓冲区
        void flush() override {
            _flush_output();
        }

        // 输出接口：写入单个字符（满了自动刷新）
        void stream_put_char(char c) override {
            if (_output_pos >= INNER_BUF_SIZE) {
                _flush_output();  // 缓冲区满了，先刷新
            }
            _output_buf[_output_pos++] = c;
        }

        // 输出接口：写入指定长度字符串（分批处理）
        void stream_put_n_char(const char* str, int n) override {
            if (!str || n <= 0) return;

            while (n > 0) {
                int remain = INNER_BUF_SIZE - _output_pos;  // 缓冲区剩余空间
                if (remain <= 0) {
                    _flush_output();  // 空间不足，先刷新
                    remain = INNER_BUF_SIZE;
                }
                // 复制最小剩余空间和剩余字符数
                int copy_len = (n < remain) ? n : remain;
                for (int i = 0; i < copy_len; ++i) {
                    _output_buf[_output_pos++] = str[i];
                }
                str += copy_len;
                n -= copy_len;
            }
        }

        // 输出缓冲区不支持输入操作，调用则设置错误状态
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

#endif // IIOSTREAMBUFCONSOLE_H