#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

namespace shed_std{
    // 基础布尔类型
    template <bool V>
    struct bool_constant
    {
        static constexpr bool value = V;
    };

    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;

    // 条件启用工具
    // 主模板:条件为false时，无type成员
    template <bool Cond,typename T = void>
    struct enable_if{};

    // 偏特化:条件为true时，定义type为T
    template <typename T>
    struct enable_if<true,T>{
        using type = T;
    };

    // 简化别名
    template <bool Cond, typename T = void>
    using enable_if_type = typename enable_if<Cond,T>::type;

    // 1.判断是否为void类型
    template <typename T>
    struct  is_void : false_type{};

    // void偏特化
    template <>
    struct is_void<void> : true_type{};

    // 2.是否为指针类型(T*)
    template <typename T>
    struct is_pointer : false_type{};

    template <typename T>
    struct is_pointer<T*> : true_type{};

    // 3.判断是否为整数

    // 判断是否为整数类型（手动列举常见整数类型）
    template <typename T>
    struct is_integral : false_type {};

    // 特化所有整数类型
    template <> struct is_integral<bool> : true_type {};
    template <> struct is_integral<char> : true_type {};
    template <> struct is_integral<signed char> : true_type {};
    template <> struct is_integral<unsigned char> : true_type {};
    template <> struct is_integral<short> : true_type {};
    template <> struct is_integral<unsigned short> : true_type {};
    template <> struct is_integral<int> : true_type {};
    template <> struct is_integral<unsigned int> : true_type {};
    template <> struct is_integral<long> : true_type {};
    template <> struct is_integral<unsigned long> : true_type {};

    // 4.判断是否为浮点类型（手动列举常见浮点类型）

    // 主模板：默认为 false_type
    template <typename T>
    struct is_floating_point : false_type {};

    // 特化所有内置浮点类型
    template <> struct is_floating_point<float> : true_type {};
    template <> struct is_floating_point<double> : true_type {};
    template <> struct is_floating_point<long double> : true_type {};

    // 5.判断是否是相同类型
    template <typename T1,typename T2>
    struct is_same : false_type{};

    // 偏特化
    template <typename T>
    struct is_same<T,T>:true_type{};

    // 6.移除const
    // int -> int; const int -> int; const int* -> const int*( 可以理解为(const int)*); int* const -> int*;

    // 主模板
    template <typename T>
    struct remove_const{
        using type = T;
    };

    // const偏特化
    template <typename T>
    struct remove_const<const T>{
        using type = T;
    };

    // 7.逻辑或,使用递归支持可变参数
    // 递归终止条件 1：没有参数，结果为 false
    template <typename...>
    struct or_operation : false_type {};

    // 递归终止条件 2：只有一个参数 T1，结果就是 T1
    template <typename T1>
    struct or_operation<T1> : T1 {}; 

    // 递归定义：两个或更多参数
    template <typename T1, typename T2, typename... Rest>
    struct or_operation<T1, T2, Rest...>
        : bool_constant<T1::value || or_operation<T2, Rest...>::value> {};

    // 8.逻辑与，使用递归支持可变参数
    // 递归终止条件 1：没有参数，结果为 true (空集合的逻辑与为真)
    template <typename...>
    struct and_operation : true_type {};

    // 递归终止条件 2：只有一个参数 T1，结果就是 T1
    template <typename T1>
    struct and_operation<T1> : T1 {}; 

    // 递归定义：两个或更多参数
    template <typename T1, typename T2, typename... Rest>
    struct and_operation<T1, T2, Rest...>
        : bool_constant<T1::value && and_operation<T2, Rest...>::value> {};

    // 9.是否是算术类型(整数和浮点)
    template <typename T>
    struct is_arithmetic : or_operation<is_integral<T>,is_floating_point<T>>{};

    // 10.是否是引用
    template <typename T>
    struct is_reference : false_type{};

    // 左值偏特化
    template <typename T>
    struct is_reference<T&> : true_type{};

    // 右值引用偏特化
    template <typename T>
    struct is_reference<T&&> : true_type{};

    // 11.判断类型是否被 const 修饰
    template <typename T>
    struct is_const : false_type {};

    // 偏特化：捕获 const T 形式的类型
    template <typename T>
    struct is_const<const T> : true_type {};

    // 12.判断是否为数组
    template <typename T>
    struct is_array:false_type{};

    // T[N]
    template <typename T, unsigned long long N>
    struct is_array<T[N]> : true_type{};

    // T[]
    template <typename T>
    struct is_array<T[]> : true_type{};

    // 13.移除引用
    template <typename T>
    struct remove_reference{
        using type = T;
    };

    // 偏特化:左值引用
    template <typename T>
    struct remove_reference<T&>{
        using type = T;
    };

    // 偏特化:右值引用
    template <typename T>
    struct remove_reference<T&&>{
        using type = T;
    };

    // 14.移除指针
    template <typename T>
    struct remove_pointer{
        using type = T;
    };

    // 偏特化
    template <typename T>
    struct remove_pointer<T*>{
        using type = T;
    };

    // 15.辅助工具，获取右值引用
    template <typename T>
    T&& declare_value() noexcept;
    // 声明函数但不会实际调用，没有定义

    // 16.是否支持比较
    // 检查是否支持operator<
    template <typename T>
    struct _test_less_than{
        private:
            // SFINAE检查
            // 1. 尝试执行 declval<const U&>() < declval<const U&>()
            // 2. 逗号运算符确保表达式必须合法
            // 3. 最终返回类型是 int (用于与 test(0) 匹配)
            template <typename U>
            static true_type test(decltype(declare_value<const U&>() < declare_value<const U&>(),int()));

            // 匹配失败
            template <typename U>
            static false_type test(...);
        public:
            // 关于test<T>(0)
            // 如果支持operator<,那么第一个函数优先匹配（前面的比较是合法的，后面又有int())
            // 不支持匹配第二个
            static constexpr bool value = is_same<true_type,decltype(test<T>(0)) >::value;
    };

    template <typename T>
    struct is_less_than_comparable : bool_constant<_test_less_than<T>::value>{};

    // 检查是否支持 operator>
    template <typename T>
    struct _test_greater_than{
    private:
        template <typename U>
        static true_type test(decltype(declare_value<const U&>() > declare_value<const U&>(), int()));

        template <typename U>
        static false_type test(...);
    public:
        static constexpr bool value = is_same<true_type, decltype(test<T>(0)) >::value;
    };

    template <typename T>
    struct is_greater_than_comparable : bool_constant<_test_greater_than<T>::value>{};

    // 检查是否支持 operator>=
    template <typename T>
    struct _test_greater_equal{
    private:
        template <typename U>
        static true_type test(decltype(declare_value<const U&>() >= declare_value<const U&>(), int()));

        template <typename U>
        static false_type test(...);
    public:
        static constexpr bool value = is_same<true_type, decltype(test<T>(0)) >::value;
    };

    // 检查是否支持 operator<=
    template <typename T>
    struct _test_less_equal{
    private:
        template <typename U>
        static true_type test(decltype(declare_value<const U&>() <= declare_value<const U&>(), int()));

        template <typename U>
        static false_type test(...);
    public:
        static constexpr bool value = is_same<true_type, decltype(test<T>(0)) >::value;
    };

    template <typename T>
    struct is_less_equal_comparable : bool_constant<_test_less_equal<T>::value>{};

    template <typename T>
    struct is_greater_equal_comparable : bool_constant<_test_greater_equal<T>::value>{};

    // 检查是否支持 operator== (相等性比较)
    template <typename T>
    struct _test_equality{
    private:
        template <typename U>
        static true_type test(decltype(declare_value<const U&>() == declare_value<const U&>(), int()));

        template <typename U>
        static false_type test(...);
    public:
        static constexpr bool value = is_same<true_type, decltype(test<T>(0)) >::value;
    };

    template <typename T>
    struct is_equality_comparable : bool_constant<_test_equality<T>::value>{};

    // 检查是否支持 operator!= (不等性比较)
    template <typename T>
    struct _test_unequality{
    private:
        template <typename U>
        static true_type test(decltype(declare_value<const U&>() != declare_value<const U&>(), int()));

        template <typename U>
        static false_type test(...);
    public:
        static constexpr bool value = is_same<true_type, decltype(test<T>(0)) >::value;
    };

    template <typename T>
    struct is_unequality_comparable : bool_constant<_test_unequality<T>::value>{};

    // 复合特性：检查是否支持 == 和 !=
    template <typename T>
    struct is_equality_comparable_full
        : and_operation<
            is_equality_comparable<T>,  // 检查 operator==
            is_unequality_comparable<T> // 检查 operator!=
    > {};

    // 复合特性：检查是否支持 <, >, <=, >= 四个关系操作符
    template <typename T>
    struct is_totally_ordered
        : and_operation<
                is_less_than_comparable<T>,         // 检查 <
                is_greater_than_comparable<T>,       // 检查 >
                is_less_equal_comparable<T>,        // 检查 <=
                is_greater_equal_comparable<T>     // 检查 >=
    > {};

    // 17.逻辑非
    template <typename T>
    struct not_operation : bool_constant<!T::value> {};
}

#endif // TYPE_TRAITS_H