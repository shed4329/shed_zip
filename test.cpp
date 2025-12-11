#include "shed_std/iostream.h"
#include "shed_std/Sstring.h"

int main(){
    shed_std::Sstring str;
    shed_std::Cconsole_input >> str;
    shed_std::Cconsole_output << str;
    for(int i = 0;i<str.length();i++){
        shed_std::Cconsole_output << i << "= "<< str[i] << " " << shed_std::end_line;
    }
}