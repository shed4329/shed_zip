#ifndef CCONSOLE_INTPUT_H
#define CCONSOLE_INTPUT_H

#include "Iistream.h"
#include "IiostreamBufConsole.h"

namespace shed_std{
    // 全局控制台输入缓冲区
    static IiostreamBufConsoleInput _global_console_in_buf;
    // 全局控制台输入流
    Iistream Cconsole_input(&_global_console_in_buf);    
}
#endif // CCONSOLE_OUTPUT_H