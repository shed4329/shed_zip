#ifndef ZIP_CONFIG_H
#define ZIP_CONFIG_H

#include "shed_std/Vvector.h"
#include "shed_std/Sstring.h"

namespace shed_zip{
    // 基础类别定义
    using unit8_t   = unsigned char;
    using uint16_t  = unsigned short;
    using uint32_t  = unsigned int;
    using int32_t   = int;

    // 压缩配置类
    struct ZipConfig{
        // 压缩等级:0 = 不压缩，1 = Fastest , 9 =Best，实际上会影响LZ77的搜索深度
        int level;
        // 滑动视窗大小 (deflate 标准最大32KB)
        int window_size;
        // 是否使用store模式，强制不压缩
        bool force_store;

        // 窗口的最大长度
        static constexpr int MAX_WINDOW_SIZE = 32*1024;
        // 窗口的最小长度
        static constexpr int MIN_WINDOW_SIZE = 32;
        // 窗口的默认长度
        static constexpr int DEFAULT_WINDWOS_SZIE = 8192;

        // 最大级别
        static constexpr int MAX_LEVEL = 9;
        // 最小级别
        static constexpr int MIN_LEVEL = 0;
        // 默认级别
        static constexpr int DEFAULT_LEVEL = 5;
        
        // 构造函数
        // 全参构造
       ZipConfig(int _level,int _window_size,bool _force_store){
            if(_level < MIN_LEVEL){
                level = MIN_LEVEL;
            }else if(_level > MAX_LEVEL){
                level = MAX_LEVEL;
            }else{
                level = _level;
            }

            if(_window_size > MAX_WINDOW_SIZE){
                window_size = MAX_WINDOW_SIZE;
            }else if(_window_size < MIN_WINDOW_SIZE){
                window_size = MIN_WINDOW_SIZE;
            }else{
                window_size = _window_size;
            }

            force_store = _force_store;
        };

       ZipConfig(int _level,int _window_size):ZipConfig(_level,_window_size,false){};
       ZipConfig(int _level):ZipConfig(_level,DEFAULT_WINDWOS_SZIE){};
       ZipConfig():ZipConfig(DEFAULT_LEVEL,DEFAULT_LEVEL){};
    };
}

#endif  // ZIP_CONFIG_H