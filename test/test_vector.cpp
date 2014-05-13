#include <vector.hpp>
#include <iterator.hpp>
#include <iostream>
#include <algorithm>
#include <vector>

class A2{
public:
    A2(){ std::cout<<"A constructor"<<std::endl; }
    ~A2(){ std::cout<<"A destructor"<<std::endl;}

};

int main(int argc ,char* argv[]){

    xusd::vector<std::size_t> xxx((std::size_t)10,(std::size_t)10);
    xusd::vector<int> temp;
	std::cout<<"temp.size() = "<<temp.size()<<"  temp.capacity = " <<temp.capacity()<<" is empty ? "<<std::boolalpha<<temp.empty()<<std::endl;
	
	temp.push_back(1);
	std::cout<<"temp.size() = "<<temp.size()<<"  temp.capacity = " <<temp.capacity()<<" is empty ? "<<std::boolalpha<<temp.empty()<<std::endl;
	temp.push_back(2);
	std::cout<<"temp.size() = "<<temp.size()<<"  temp.capacity = " <<temp.capacity()<<" is empty ? "<<std::boolalpha<<temp.empty()<<std::endl;
	temp.push_back(3);
	std::cout<<"temp.size() = "<<temp.size()<<"  temp.capacity = " <<temp.capacity()<<" is empty ? "<<std::boolalpha<<temp.empty()<<std::endl;
	temp.push_back(4);
	std::cout<<"temp.size() = "<<temp.size()<<"  temp.capacity = " <<temp.capacity()<<" is empty ? "<<std::boolalpha<<temp.empty()<<std::endl;
	temp.push_back(5);
	std::cout<<"temp.size() = "<<temp.size()<<"  temp.capacity = " <<temp.capacity()<<" is empty ? "<<std::boolalpha<<temp.empty()<<std::endl;
	temp.push_back(6);
	std::cout<<"temp.size() = "<<temp.size()<<"  temp.capacity = " <<temp.capacity()<<" is empty ? "<<std::boolalpha<<temp.empty()<<std::endl;

	xusd::vector<int>::iterator i = temp.begin();
	xusd::advance(i,3);
	temp.erase(i);
	std::cout<<"temp.size() = "<<temp.size()<<"  temp.capacity = " <<temp.capacity()<<std::endl;
    std::copy(temp.begin(),temp.end(),xusd::ostream_iterator<int>(std::cout," "));
    std::cout<<std::endl;


    int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    xusd::vector<int> a(A,A+10);


    std::copy(a.begin(),a.end(),xusd::ostream_iterator<int>(std::cout," "));
    std::cout<<std::endl;

    xusd::vector<int>b = a;
    std::copy(b.begin(),b.end(),xusd::ostream_iterator<int>(std::cout," "));
    std::cout<<std::endl;

    xusd::vector<int> c;
    for(int i = 0; i< 100; ++i){
        c.push_back(i);
    }

    c.erase(c.begin() + 30, c.begin() + 70);

    std::copy(c.rbegin(),c.rend(),xusd::ostream_iterator<int>(std::cout," "));
    std::cout<<std::endl;

    //xusd::vector<A2> d;
    //A2 a2;
    //for(int i = 0; i< 100; ++i){
    //    d.push_back(a2);
    //}

    xusd::vector<int> d(A,A+10);
    d.insert(d.end(),A,A+7);
    std::copy(d.begin(),d.end(),xusd::ostream_iterator<int>(std::cout," "));
    std::cout<<std::endl;
    
    return 0;
}
