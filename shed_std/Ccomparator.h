#ifndef CCOMPARATOR_H
#define CCOMPARATOR_H

#include "type_traits.h"

namespace shed_std{
    template <typename T>
    struct less{
        bool operator()(const T& a, const T& b) const{
            return a<b;
        }


        template <typename Other>
        bool operator==(const Other& other) const {
            return is_same<less<T>, Other>::value;
        }
    };
    

    template <typename T>
    struct greater{
        bool operator()(const T& a, const T& b) const{
            return a>b;
        }

        template <typename Other>
        bool operator==(const Other& other) const {
            return is_same<greater<T>, Other>::value;
        }
    };
    

}// namespace shed_std

#endif // CCOMPARATOR_H
