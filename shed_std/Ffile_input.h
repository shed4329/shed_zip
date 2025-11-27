#ifndef FFILE_INPUT_H
#define FFILE_INPUT_H

#include "Iistream.h"
#include "IiostreamBufFileInput.h"

namespace shed_std {
    // 文件输入流类（可实例化，支持多文件）
    class Ffile_input : public Iistream {
    private:
        IiostreamBufFileInput _buf;  // 每个实例独立的缓冲区

    public:
        // 构造函数：绑定当前实例的缓冲区
        Ffile_input() : Iistream(&_buf) {}

        // 打开文件（返回是否成功）
        bool open(const char* filename) {
            return _buf.open(filename);
        }

        // 关闭文件
        void close() {
            _buf.close();
        }

        // 检查文件是否打开
        bool is_open() const {
            return _buf.state() == Iiostream_state::IO_GOOD;
        }

        // 读取单个字符
        int get() {
            return _buf.stream_bump_char();
        }

        // 预览下一个字符
        int peek() {
            return _buf.stream_get_char();
        }

        // 禁用拷贝（文件句柄不能直接拷贝）
        Ffile_input(const Ffile_input&) = delete;
        Ffile_input& operator=(const Ffile_input&) = delete;

        // 支持移动（可选，方便传递所有权）
        Ffile_input(Ffile_input&&) = default;
        Ffile_input& operator=(Ffile_input&&) = default;
    };
}

#endif // FFILE_INPUT_H