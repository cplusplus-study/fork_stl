
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator.hpp>

int main(int argc,char* argv[]){
    int A[] = {1,2,3,4,5,6,7};
    std::cout<<"distance of between A and A+7 = "<<xusd::distance(A,A+7)<<std::endl;

    int *p = &A[0];
    xusd::advance(p,3);
    std::cout<<"*(p+3) = "<<*p<<std::endl;

    std::list<int> l;

    std::copy(A,A+7,xusd::front_inserter<std::list<int> >(l));
    std::copy(A,A+7,xusd::back_inserter<std::list<int> >(l));

    std::copy(l.begin(),l.end(),xusd::ostream_iterator<int>(std::cout," "));
    std::cout<<std::endl;

    xusd::reverse_iterator<int*> end(&A[0]);
    xusd::reverse_iterator<int*> begin(A+7);
    std::copy(begin,end,xusd::ostream_iterator<int>(std::cout," "));
    std::cout<<std::endl;

    std::cout<<"place input 10 numbers:"<<std::endl;

    xusd::istream_iterator<int> ibegin(std::cin);
    xusd::istream_iterator<int> iend;
    copy(ibegin,iend,l.begin());
    std::copy(l.begin(),l.end(),xusd::ostream_iterator<int>(std::cout," "));
    std::cout<<std::endl;

    return 0;
}

