
#ifndef __FORK_STL_VECTOR__
#define __FORK_STL_VECTOR__
#include <cstddef>
#include <climits>
#include <stdexcept> //lenght_error out_of_range exceptional
#include <iterator.hpp>
#include <memory.hpp>
#include <algorithm.hpp>
#include <functional.hpp>

namespace xusd{

    //TODO: vector<bool>
    template<typename T,typename Alloc = xusd::allocator<T> >
    class vector{
    public:
        //以下是c++标准里面的11个类型
        typedef T           value_type;
        typedef T&          reference;
        typedef T const&    const_reference;
        typedef T*          pointer;
        typedef T const*    const_pointer;
        typedef T*          iterator;
        typedef T const*    const_iterator;
        typedef xusd::reverse_iterator<iterator>         reverse_iterator;
        typedef xusd::reverse_iterator<const_iterator>   const_reverse_iterator;
        typedef std::size_t     size_type;
        typedef std::ptrdiff_t  difference_type;
        
        typedef Alloc allocator_type;

        iterator begin(){ return start; }
        const_iterator begin()const{ return start; }

        iterator end(){ return finish; }
        const_iterator end()const{ return finish; }

        reverse_iterator rbegin(){ return reverse_iterator(end()); }
        const_reverse_iterator rbegin()const{ return const_reverse_iterator(end()); }

        reverse_iterator rend(){ return reverse_iterator(begin()); }
        const_reverse_iterator rend()const{ return const_reverse_iterator(begin()); }


        size_type size()const{ return finish - start; }
        size_type max_size()const{ return UINT_MAX/sizeof(value_type); };

        size_type capacity()const{ return end_of_storage - start; } //总共容量

        bool empty(){ return start == finish; };
        
        reference operator[](size_type n){ return *(start + n); }
        const_reference operator[](size_type n)const{ return *(start + n); }

        reference at(size_type n){
            if(n>=size()){
                throw std::out_of_range("xusd: out_of_range");
            }else{
                return operator[](n);
            }

        }
        const_reference at(size_type n) const{
            if(n>=size()){
                throw std::out_of_range("xusd: out_of_range");
            }else{
                return operator[](n);
            }
        }

        //默认构造函数
        explicit vector(Alloc const&A = Alloc()):data_allocator(A),start(NULL),finish(NULL),end_of_storage(NULL){ }

        //我本来是想申请最接近n的满足2的x次方的空间2^x,暂不这么实现
        explicit vector(size_type n, const T& x = T(), Alloc const&A = Alloc())
            :data_allocator(A),start(NULL),finish(NULL),end_of_storage(NULL){
                this->start = data_allocator.allocate(n);
                this->finish = this->start;
                this->end_of_storage = this->start + n;

                this->finish = uninitialized_fill_n(start,n,x);//调用全局的xusd::uninitinalized_fill_n填充数据
            }
        //copy constructor
        vector(vector const&o):data_allocator(o.data_allocator){
            iterator new_start = data_allocator.allocate(o.size());
            iterator new_finish = xusd::uninitialized_copy(o.begin(),o.end(),new_start);
            this->start = new_start;
            this->finish = new_finish;
            this->end_of_storage = this->start + o.size();
        }

        //以某一个区间构造vector
        template<typename InputIterator>
        vector(InputIterator first, InputIterator last, Alloc const&A = Alloc())
        :data_allocator(A),start(NULL),finish(NULL),end_of_storage(NULL)
        { 
            size_type n = xusd::distance(first,last);
            if(n>0){
                this->start = data_allocator.allocate(n);
                this->finish = this->start;
                this->end_of_storage = this->start + n;
                this->finish = xusd::uninitialized_copy(first,last,start);
            }
        }

        //destructor
        ~vector(){
            //先析构所有的内部元素,然后释放空间
            xusd::destroy(this->start,this->finish);//这里应该替换为针对本类型里面的allocator的destroy
            data_allocator.deallocate(this->start,capacity());
        }

        //operator =
        vector<T,Alloc>& operator=(vector<T,Alloc>const& o){
            Alloc new_data_allocator = o.data_allocator;
            iterator new_start = data_allocator.allocate(o.size());
            iterator new_end_of_storage = this->start + o.size();
            iterator new_finish = xusd::uninitialized_copy(o.begin(),o.end(),new_start);

            xusd::destroy(this->start,this->finish);
            data_allocator.deallocate(this->start,capacity());

            this->start = new_start;
            this->finish = new_finish;
            this->end_of_storage = new_end_of_storage;

        }

        allocator_type get_allocator()const{
            return data_allocator;
        }

        void swap(vector<T,Alloc> &v){
            //这里是肯定会被修改的,调用每个成员的swap最好
            using xusd::swap;
            //swap(*this,v);
            swap(this->start,v.start);
            swap(this->finish,v.finish);
            swap(this->end_of_storage,v.end_of_storage);
        }

        void reserve(size_type n){
            if(n<=size()){
                //这里什么都不干
                //this->finish = this->start + n;
            }else if(n > max_size()){
                throw std::length_error("xusd: length_error"); 
            }else {
                iterator new_start = data_allocator.allocate(n);
                xusd::uninitialized_copy(start,finish,new_start);

                xusd::destroy(this->start,this->finish);
                data_allocator.deallocate(this->start,capacity());
                this->finish = new_start + size();
                this->start = new_start;
            }
        }

        reference front(){
            return *(begin());
        }
        const_reference front()const{
            return *(begin());
        }

        reference back(){
            return *(back() - 1);
        }

        const_reference back()const{
            return *(back() - 1);
        }

        void push_back(value_type const& v){
            if(finish != end_of_storage){
                xusd::construct(finish,v);
                ++finish;
            }else{
                insert(end(),v);
            }
        }

        void pop_back(){
            data_allocator.destroy(finish);
            --finish;
        }

        iterator erase(iterator first,iterator last){
            iterator i = std::copy(last,finish,first);//因暂时没有实现copy算法,就用std::copy代替
            xusd::destroy(i,finish);
            //finish = i;
            finish = finish - (last - first);
            return first;
        }

        iterator erase(iterator position){
            if(position +1 != end()){
                std::copy(position+1,finish,position);//因暂时没有实现copy算法,就用std::copy代替
            }
            --finish;
            xusd::destroy(finish);
            return position;
        }

        void clear(){
            xusd::destroy(start,finish);
            this->finish = start;
        }

        void insert(iterator position,value_type const& v){
            if(finish != end_of_storage){
                iterator tmp= ++finish;
                while(tmp != position){
                    *tmp = *(tmp-1);
                }
                *tmp = v;
            }else{
                const size_type old_size = size();
                const size_type len = (old_size == 0 ? 1 : 2*old_size);
                iterator new_start = data_allocator.allocate(len);
                iterator new_finish = new_start;
                try{
                    new_finish = xusd::uninitialized_copy(start,position,new_start);
                    xusd::construct(new_finish,v);
                    ++new_finish;
                    new_finish = xusd::uninitialized_copy(position,finish,new_finish);
                }catch(...){
                    destroy(new_start,new_finish);
                    data_allocator.deallocate(new_start,len);
                }

                destroy(start,finish);
                data_allocator.deallocate(start,old_size);
                start = new_start;
                finish = new_finish;
                end_of_storage = start + len;
            }
        }

        void insert(iterator position,size_type n, value_type const& x){
            if(n <= 0) return ;
            if(size()+n > capacity()){//容量不够时候
                const size_type old_size = size();
                const size_type len = size() + n;
                iterator new_start = data_allocator.allocate(len);
                iterator new_finish = new_start;
                try{
                    new_finish = xusd::uninitialized_copy(start,position,new_start);
                    new_finish = xusd::uninitialized_fill_n(new_finish,n,x);
                    new_finish = xusd::uninitialized_copy(position,finish,new_finish);
                }catch(...){
                    destroy(new_start,new_finish);
                    data_allocator.deallocate(new_start,len);
                }
                destroy(start,finish);
                data_allocator.deallocate(start,old_size);
                start = new_start;
                finish = new_finish;
                end_of_storage = start + len;
            }else{ //容量很充足
                //用copy_backword
                vector<T> tmp(position,finish);
                xusd::destroy(position,finish);
                iterator new_finish = xusd::uninitialized_fill_n(position,n,x);
                new_finish = xusd::uninitialized_copy(tmp.begin(),tmp.end(),new_finish);
                this->finish = new_finish;
            }

        }

        template<typename InputIterator>
        void insert(iterator position,InputIterator first,InputIterator last){
            size_type n = distance(first,last);
            if(size() + n > capacity()){//容量不够时候
                const size_type old_size = size();
                const size_type len = old_size + max(n,old_size);
                iterator new_start = data_allocator.allocate(len);
                iterator new_finish = new_start;
                try{
                    new_finish = xusd::uninitialized_copy(start,position,new_start);
                    new_finish = xusd::uninitialized_copy(first,last,new_finish);
                    new_finish = xusd::uninitialized_copy(position,finish,new_finish);
                }catch(...){
                    destroy(new_start,new_finish);
                    data_allocator.deallocate(new_start,len);
                }
                destroy(start,finish);
                data_allocator.deallocate(start,old_size);
                start = new_start;
                finish = new_finish;
                end_of_storage = start + len;
            }else{ //容量很充足
                //用copy_backword
                vector<T> tmp(position,finish);
                xusd::destroy(position,finish);
                iterator new_finish = xusd::uninitialized_copy(first,last,position);
                new_finish = xusd::uninitialized_copy(tmp.begin(),tmp.end(),new_finish);
                this->finish = new_finish;
            }

        }

        void resize(size_type n,const_reference x = value_type()){
            if(n<size()){
                erase(start + n,finish);
            }else{
                insert(end(),n-size,x);
            }
        }

        template<typename InputIterator>
        void assign(InputIterator first,InputIterator last){
            //注意这里不是异常安全的
            clear();
            //finish = xusd::uninitialized_copy(first,last,start);
            insert(end(),first,last);
        }

        void assign(size_type n,value_type const& x){
            clear();
            insert(end(),n,x);
        }

        


    private:

    private:
        Alloc data_allocator;
        iterator start;
        iterator finish;
        iterator end_of_storage;
    };



    template<typename T>
    bool operator==(const xusd::vector<T>& a,const xusd::vector<T>& b){
        if(a.size() != b.size()){ return false; }
        typename xusd::vector<T>::iterator i1 = a.begin();
        typename xusd::vector<T>::iterator i2 = b.begin();
        for(;i1 != a.end(); ++i1,++i2){
            if(*i1 != *i2) 
                return false;
        }
        return true;
    }

    template<typename T>
    bool operator!=(const xusd::vector<T>& a,const xusd::vector<T>& b){
        return !(a == b);
    }

    template<typename T>
    bool operator<(const xusd::vector<T>& a,const xusd::vector<T>& b){
        return lexicographical_compare(a.begin(),a.end(),b.begin(),b.end());
    }

    template<typename T>
    bool operator>(const xusd::vector<T>& a,const xusd::vector<T>& b){
        return lexicographical_compare(a.begin(),a.end(),b.begin(),b.end(),greater<T>());
    }

    template<typename T>
    bool operator<=(const xusd::vector<T>& a,const xusd::vector<T>& b){
        return !(a > b);
    }

    template<typename T>
    bool operator>=(const xusd::vector<T>& a,const xusd::vector<T>& b){
        return !(a < b);
    }

    //我这里实现这个是有原因的
    template<typename T>
    void swap(const xusd::vector<T>& a,const xusd::vector<T>& b){
        a.swap(b);
    }


}

#endif
