#ifndef SSTACK_H
#define SSTACK_H

#include "Vvector.h"
#include "Eexception.h"

namespace shed_std{
    template <typename T>
    class Sstack{
        private:
            Vvector<T> _data;
        public:
            /**
             * 在栈顶添加元素
             * @param value 值
             */
            void push(const T& value){
                _data.push_back(value);
            }

            /**
             * 栈是否为空
             * @return 栈是否为空
             */
            bool empty() const{
                return _data.empty();
            }

            /**
             * 返回栈的元素数量
             * @return 元素数量
             */
            int size() const{
                return _data.size();
            }
            
            /**
             * 弹出栈顶元素
             */
            void pop(){
                if(empty()){
                    throw Eexception("Eexception:Sstack is empty,could not pop!");
                }
                _data.pop_back();
            }

            /**
             * 返回栈顶元素
             * @param 栈顶元素
             */
            T& top(){
                if(empty()){
                    throw Eexception("Eexception:Sstack is empty, having no top!");
                }
                return _data.back();
            }

            /**
             * 返回栈顶元素，供常量使用
             * @param 栈顶元素
             */
            const T& top() const{
                if(empty()){
                    throw Eexception("Eexception:Sstack is empty, having no top!");
                }
                return _data.back();
            }

            /**
             * 将栈的大小置零
             */
            void clear(){
                _data.clear();
            }

            // 判断两个栈是否相同
            bool operator== (const Sstack& other) const{
                return _data == other._data;
            }
            
            // 判断两个栈是否不等
            bool operator!= (const Sstack& other) const{
                return _data!=other._data;
            }
    };
}

#endif // SSTACK_H
