#ifndef EEXCEPTION_H
#define EEXCEPTION_H

namespace shed_std {
    // 基础异常类（仅保留核心接口，避免成员冗余）
    class Eexception {
    protected:
        const char* _msg = nullptr; // 初始化避免野指针
    public:
        explicit Eexception(const char* msg) : _msg(msg) {} 

        virtual const char* what() const {
            return _msg ? _msg : "unknown exception";
        }

        virtual ~Eexception() = default;

    };
    
    // 扩展异常类（核心：统一字符串处理 + 内存管理）
    class EexceptionExtended : public Eexception {
    protected:
        static constexpr int MAX_LEN = 65535;
        char* buffer = nullptr; // 仅子类管理格式化字符串，消除基类_msg隐藏问题

        // 安全计算字符串长度（通用工具）
        int string_len(const char* str) const {
            if (str == nullptr) return 0;
            int len = 0;
            while (len < MAX_LEN && *str != '\0') {
                str++;
                len++;
            }
            return len;
        }

        // 安全字符串拼接（通用工具）
        char* append(char* dest, const char* source) const {
            if (dest == nullptr || source == nullptr) return dest;
            int len = 0;
            while (*source != '\0' && len < MAX_LEN) {
                *dest = *source;
                dest++;
                source++;
                len++;
            }
            return dest;
        }

        // 通用数字转字符串（修复0/负数问题，统一复用）
        const char* int_to_str(int num, char* buf) const {
            if (buf == nullptr) return "0";
            char* ptr = buf;
            
            // 处理负数
            if (num < 0) {
                *ptr++ = '-';
                num = -num;
            }

            // 处理0（关键修复）
            if (num == 0) {
                *ptr++ = '0';
                *ptr = '\0';
                return buf;
            }

            // 逆序存储数字
            char temp[32] = {0};
            int i = 0;
            while (num > 0 && i < 31) {
                temp[i++] = '0' + (num % 10);
                num /= 10;
            }

            // 正序拷贝到buf
            for (int j = 0; j < i; j++) {
                ptr[j] = temp[i - 1 - j];
            }
            ptr[i] = '\0';
            return buf;
        }

        // 子类可重写异常类型标识
        virtual const char* get_type() const {
            return "Eexception: Extended! ";
        }

        // 基础格式化逻辑（子类可重写）
        virtual void format_string(const char* cause, const char* source) {
            const char* safe_cause = (cause == nullptr) ? "unknown cause" : cause;
            const char* safe_source = (source == nullptr) ? "unknown source" : source;
            
            // 计算总长度
            int len = string_len(get_type()) + string_len("happened for ") + 
                      string_len(safe_cause) + string_len(" at ") + string_len(safe_source) + 1;
            len = len > MAX_LEN ? MAX_LEN : len;

            // 释放旧buffer（防止内存泄漏）
            if (buffer != nullptr) {
                delete[] buffer;
                buffer = nullptr;
            }

            // 分配新内存并拼接
            buffer = new char[len + 1];
            char* ptr = buffer;
            ptr = append(ptr, get_type());
            ptr = append(ptr, "happened for ");
            ptr = append(ptr, safe_cause);
            ptr = append(ptr, " at ");
            ptr = append(ptr, safe_source);
            *ptr = '\0';
            _msg = buffer; // 赋值给基类_msg，统一通过what()返回
        }

    public:
        // 基础构造：自定义原因+位置
        EexceptionExtended(const char* cause, const char* source)
            : Eexception(nullptr) {
            format_string(cause, source);
        }

        // 虚析构（确保子类buffer被释放）
        virtual ~EexceptionExtended() {
            delete[] buffer;
            buffer = nullptr;
            _msg = nullptr;
        }

        // 深拷贝构造（解决double free）
        EexceptionExtended(const EexceptionExtended& other)
            : Eexception(nullptr) {
            int len = string_len(other._msg);
            buffer = new char[len + 1];
            for (int i = 0; i < len; i++) {
                buffer[i] = other._msg[i];
            }
            buffer[len] = '\0';
            _msg = buffer;
        }

        // 赋值运算符重载
        EexceptionExtended& operator=(const EexceptionExtended& other) {
            if (this != &other) {
                delete[] buffer; // 释放原有内存

                int len = string_len(other._msg);
                buffer = new char[len + 1];
                for (int i = 0; i < len; i++) {
                    buffer[i] = other._msg[i];
                }
                buffer[len] = '\0';
                _msg = buffer;
            }
            return *this;
        }

        // 重写what()，返回格式化后的信息
        const char* what() const override {
            return _msg ? _msg : "unknown extended exception";
        }
    };

    // 1. 越界异常类（最终版）
    class EexceptionOutOfBoundary : public EexceptionExtended {
    protected:
        const char* get_type() const override {
            return "Eexception: Out of Boundary! ";
        }

    public:
        // 构造1：默认信息
        EexceptionOutOfBoundary() 
            : EexceptionExtended("index out of boundary", "unknown location") {}

        // 构造2：自定义原因+位置
        EexceptionOutOfBoundary(const char* cause, const char* source)
            : EexceptionExtended(cause, source) {}

        // 构造3：带索引+边界值（精准定位）
        EexceptionOutOfBoundary(int index, int boundary, const char* source)
            : EexceptionExtended(nullptr, nullptr) {
            const char* prefix = get_type();
            const char* sep1 = "Index = ";
            const char* sep2 = ", Boundary = ";
            const char* sep3 = " at ";

            // 数字转字符串
            char index_str[32] = {0};
            char boundary_str[32] = {0};
            int_to_str(index, index_str);
            int_to_str(boundary, boundary_str);

            // 计算总长度
            int len = string_len(prefix) + string_len(sep1) + string_len(index_str) + 
                      string_len(sep2) + string_len(boundary_str) + string_len(sep3) + 
                      string_len(source) + 1;
            len = len > MAX_LEN ? MAX_LEN : len;

            // 释放父类默认buffer，重新分配
            delete[] buffer;
            buffer = new char[len + 1];

            // 拼接最终信息
            char* ptr = buffer;
            ptr = append(ptr, prefix);
            ptr = append(ptr, sep1);
            ptr = append(ptr, index_str);
            ptr = append(ptr, sep2);
            ptr = append(ptr, boundary_str);
            ptr = append(ptr, sep3);
            ptr = append(ptr, source ? source : "unknown location");
            *ptr = '\0';
            _msg = buffer;
        }

        // 复用父类的拷贝构造/赋值
        using EexceptionExtended::EexceptionExtended;
        using EexceptionExtended::operator=;
    };

    // 2. 空容器操作异常
    class EexceptionEmptyContainer : public EexceptionExtended {
    protected:
        const char* get_type() const override {
            return "Eexception: Empty Container! ";
        }
    public:
        // 构造1：默认信息
        EexceptionEmptyContainer() 
            : EexceptionExtended("operation on empty container", "unknown location") {}
        // 构造2：自定义操作+位置
        EexceptionEmptyContainer(const char* operation, const char* location)
            : EexceptionExtended(operation, location) {}
        // 复用父类拷贝/赋值
        using EexceptionExtended::EexceptionExtended;
        using EexceptionExtended::operator=;
    };

    // 3. 超出最大容量异常
    class EexceptionCapacityExceeded : public EexceptionExtended {
    protected:
        const char* get_type() const override {
            return "Eexception: Capacity Exceeded! ";
        }
    public:
        // 构造1：默认信息
        EexceptionCapacityExceeded()
            : EexceptionExtended("container reach max capacity limit", "unknown location") {}
        // 构造2：自定义原因+位置
        EexceptionCapacityExceeded(const char* cause, const char* location)
            : EexceptionExtended(cause, location) {}
        // 构造3：带当前容量+最大容量（精准提示）
        EexceptionCapacityExceeded(int current_cap, int max_cap, const char* location)
            : EexceptionExtended(nullptr, nullptr) {
            const char* prefix = get_type();
            const char* sep1 = "container capacity (";
            const char* sep2 = ") exceed max limit (";
            const char* sep3 = ") at ";

            // 数字转字符串（复用父类工具函数）
            char curr_str[32] = {0};
            char max_str[32] = {0};
            int_to_str(current_cap, curr_str);
            int_to_str(max_cap, max_str);

            // 计算总长度
            int len = string_len(prefix) + string_len(sep1) + string_len(curr_str) + 
                      string_len(sep2) + string_len(max_str) + string_len(sep3) + 
                      string_len(location) + 1;
            len = len > MAX_LEN ? MAX_LEN : len;

            // 重新分配buffer
            delete[] buffer;
            buffer = new char[len + 1];

            // 拼接信息
            char* ptr = buffer;
            ptr = append(ptr, prefix);
            ptr = append(ptr, sep1);
            ptr = append(ptr, curr_str);
            ptr = append(ptr, sep2);
            ptr = append(ptr, max_str);
            ptr = append(ptr, sep3);
            ptr = append(ptr, location ? location : "unknown");
            *ptr = '\0';
            _msg = buffer;
        }
        // 复用父类拷贝/赋值
        using EexceptionExtended::EexceptionExtended;
        using EexceptionExtended::operator=;
    };
    
    // 4. 无效参数异常
    class EexceptionInvalidArgument : public EexceptionExtended {
    protected:
        const char* get_type() const override {
            return "Eexception: Invalid Argument! ";
        }
    public:
        EexceptionInvalidArgument(const char* arg_desc, const char* location)
            : EexceptionExtended(arg_desc, location) {}
        // 复用父类拷贝/赋值
        using EexceptionExtended::EexceptionExtended;
        using EexceptionExtended::operator=;
    };

} // namespace shed_std

#endif // EEXCEPTION_H