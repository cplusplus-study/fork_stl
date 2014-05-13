#include <cstdlib>
#include <vector>
#include <iterator>
#include <iostream>
#include <cassert>
#include <memory.hpp>

int main(int argc,char* argv[]){
    std::string *s =(std::string*) std::malloc(sizeof(std::string));
    xusd::construct(s,"test");

    std::cout<<"string(\"test\")  == \"test\" is "<<std::boolalpha<<bool(*s == "test")<<std::endl;

    xusd::destroy(s);
    std::free(s);

    int ia[] = {1, 2, 3, 4, 5};

    std::vector<int, xusd::allocator<int> > iv(ia,ia+5);
    std::copy(iv.begin(),iv.end(),std::ostream_iterator<int>(std::cout," "));
    std::cout<<std::endl;


    return 0;
}

