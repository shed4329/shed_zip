#ifndef PAIR_H
#define PAIR_H

#include "type_traits.h"

namespace shed_std{
    // 当T2可比较时会提供operator< ,> , <= ,>=
    template<typename T1,typename T2>
    struct pair
    {   
        // 成员变量
        T1 first;
        T2 second;

        // 无参构造
        pair():first(),second(){}
        
        // 含参构造
        pair(const T1& val1, const T2& val2):first(val1),second(val2){}

        // 拷贝构造
        pair(const pair<T1,T2>& other):first(other.first),second(other.second){}

        // 等号赋值
        pair& operator=(const pair& other){
            // 检查是否和自己相同
            if(this != &other){
                this->first = other.first;
                this->second = other.second;
            }
            return *this;
        }

        // 是否相等
        bool operator==(const pair& other) const{
            return first == other.first && second == other.second;
        }

        // 是否不等
        bool operator!=(const pair& other) const{
            return !(*this == other);
        }

        // 小于操作符 - 仅在T2可比较时提供
        template<typename U1 = T1, typename U2 = T2>
        enable_if_type<is_totally_ordered<U2>::value, bool> 
        operator<(const pair& other) const {
            return second < other.second;
        }

        // 大于操作符 - 仅在T2可比较时提供
        template<typename U1 = T1, typename U2 = T2>
        enable_if_type<is_totally_ordered<U2>::value, bool> 
        operator>(const pair& other) const {
            return other < *this;
        }

        // 小于等于操作符 - 仅在T2可比较时提供
        template<typename U1 = T1, typename U2 = T2>
        enable_if_type<is_totally_ordered<U2>::value, bool> 
        operator<=(const pair& other) const {
            return !(other < *this);
        }

        // 大于等于操作符 - 仅在T2可比较时提供
        template<typename U1 = T1, typename U2 = T2>
        enable_if_type<is_totally_ordered<U2>::value, bool> 
        operator>=(const pair& other) const {
            return !(*this < other);
        }
    };

    /**
     * 返回两个对象构成的pair
     * @param t1 第一个对象
     * @param t2 第二个对象
     * @return 由两个对象有序构成的pair
     */
    template<typename T1,typename T2>
    pair<T1,T2> make_pair(T1 t1,T2 t2){
        return pair<T1,T2>(t1,t2);
    }
}

#endif //PAIR_H