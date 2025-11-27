#ifndef FFUNCTION_H
#define FFUNCTION_H

namespace shed_std{
    //基类:定义函数调用接口（纯虚函数），Ret = Return,Arg = Argument
    template<typename Ret,typename Arg>
    class FfunctionBase{
        public:
            virtual ~FfunctionBase(){}   // 虚析构
            virtual Ret operator()(Arg arg) const = 0; // 函数调用运算符(接口)
    };

    //派生类:适配可调用对象
    template<typename Ret,typename Arg,typename T>
    class FfunctionImpl:public FfunctionBase<Ret,Arg>{
        private:
            T f_callable;//可调用函数对象(函数指针,lambda等)
        public:
            // 构造函数:接受可存储对象并调用
            explicit FfunctionImpl(const T& callable):f_callable(callable){}
            // 实现调用接口
            Ret operator()(Arg arg) const override{
                return f_callable(arg); // 转发调用
            }
    };

    //派生类特化,因为C++不能直接存储bool(int)这种函数变量，所以转换为bool(*)(int)这样的函数指针(退化转换)
    template<typename Ret,typename Arg>
    class FfunctionImpl<Ret,Arg,Ret(Arg)>:public FfunctionBase<Ret,Arg>{
        private:
            Ret(*f_ptr)(Arg);//函数指针
        public:
            // 接收函数类型，并自动转为函数指针
            explicit FfunctionImpl(Ret(*func_ptr)(Arg)):f_ptr(func_ptr){}
            Ret operator()(Arg arg) const override{
                return f_ptr(arg);//调用函数指针
            }
    };

    // 包装类：对外统一接口
    template<typename Ret, typename Arg>
    class Ffunction {
    private:
        FfunctionBase<Ret, Arg>* f_ptr;  // 基类指针（多态核心）
    public:
        // 构造函数：接收任意可调用对象，创建对应派生类实例
        template<typename T>
        Ffunction(const T& callable) : f_ptr(new FfunctionImpl<Ret, Arg, T>(callable)) {}

        // 析构函数：释放派生类对象（合法，因为f_ptr是基类指针）
        ~Ffunction() {
            delete f_ptr;
        }

        // 禁止拷贝（简化实现）
        Ffunction(const Ffunction&) = delete;
        Ffunction& operator=(const Ffunction&) = delete;

        // 函数调用：转发到基类接口
        Ret operator()(Arg arg) const {
            return f_ptr->operator()(arg);
        }
    };

}

#endif //FFUNCTION_H