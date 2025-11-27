#ifndef CCONSOLE_OUTPUT_H
#define CCONSOLE_OUTPUT_H

#include "Oostream.h"
#include "IiostreamBufConsole.h"

// TODO：存在问题,多个包含的cpp编译的时候会重复定义
namespace shed_std{
    // 1.全局控制台缓冲区
    static IiostreamBufConsoleOutput _global_console_buf;
    // 2.全局控制输出流变量
    Oostream Cconsole_output(&_global_console_buf);    
}

#endif // CCONSOLE_OUTPUT_H