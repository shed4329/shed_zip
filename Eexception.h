#ifndef EEXCEPTION_H
#define EEXCEPTION_H

namespace shed_std{
    class Eexception{
    private:
        const char* _msg;//错误信息
    public:
        explicit Eexception(const char* msg):_msg(msg){} 

        /**
         * 返回错误信息
         * @return 错误信息，类别为const char*
         */
        const char* what() const{
            return _msg;
        }
    };
}
#endif // EEXCEPTION_H

