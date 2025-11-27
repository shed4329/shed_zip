#ifndef IIOMANIPULATOR_H
#define IIOMANIPULATOR_H

#include "Oostream.h"
#include "Ffunction.h"

namespace shed_std{
    // 由于Oostream中的函数直接调用不可见，需要通过友元访问不可见函数
    class Iiomanipulator{
        public:
            static Oostream& _manipulator_set_precision(Oostream& os,int precision){
                os._set_precision_manipulator(precision);
                return os;
            }
    };

    // 重载operator<< 支持传入os和函数双参数
    // eg. Cconsole_output<<set_precision(1),触发重载条件
    inline Oostream& operator<<(Oostream& os,const Ffunction<Oostream&,Oostream&>& manipulator_function){
        return manipulator_function(os);
    }

    /**
     * 设置流的小数点显示位数
     * @param p
     * @return 返回可调用对象
     */
    inline Ffunction<Oostream&,Oostream&> set_precision(int p){
        return Ffunction<Oostream&,Oostream&>(
            [p](Oostream& os) -> Oostream&{
                Iiomanipulator::_manipulator_set_precision(os,p);
                return os;
            }
        );
    }

}

#endif //IIOMANIPULATOR_H
    