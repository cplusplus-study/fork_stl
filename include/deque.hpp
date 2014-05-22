// -*- C++ -*-

#ifndef __FORK_STL_DEQUE__
#define __FORK_STL_DEQUE__
#include <climits>
#include <memory.hpp>
#include <iterator.hpp>
#include <algorithm.hpp>

namespace xusd{

    template<typename T,typename Alloc = allocator<T> >
    class deque{

        struct _iterator:xusd::iterator<random_access_iterator_tag,T,std::ptrdiff_t,T*,T &>{ 
            typedef random_access_iterator_tag iterator_category;
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef T* pointer;
            typedef T& reference;
            typedef _iterator Self;

            _iterator(_iterator const& i):data(i.data),index(i.index){ }
            _iterator(deque<T> &d,typename deque<T>::size_type i):data(&d),index(i){ }
            _iterator& operator=(_iterator const&i){ 
                this->data = i.data;
                this->index = i.index; 
                return *this; 
            }
            reference operator*(){
                return *(data->start_of_storage + index);
            }
            pointer operator->(){
                return &(operator*());
            }

            Self& operator++(){ 
                index = (index+1) % data->capacity();
                return *this;  
            }

            Self operator++(int){ 
                Self temp(*this); 
                index = (index+1) % data->capacity();
                return temp;   
            }

            Self& operator--(){
                index = ((index -1)+data->capacity()) % data->capacity();
                return *this;
            }
            Self operator--(int){
                Self tmp(*this);
                index = ((index -1)+data->capacity()) % data->capacity();
                return tmp;
            }

            Self operator+(difference_type n){  
                Self tmp(*this);
                tmp.index = (tmp.index+n) % tmp.data->capacity();
                return tmp;
            }
            Self operator-(difference_type n){  
                Self tmp(*this);
                tmp.index = ((tmp.index - n)+tmp.data->capacity()) % tmp.data->capacity();
                return tmp;
            }
            Self& operator+=(difference_type n){  
                index = (index+n) % data->capacity();
                return *this;
            }
            Self& operator-=(difference_type n){  
                index = ((index - n)+data->capacity()) % data->capacity();
                return *this;
            }

            difference_type operator-(Self const&o){
                return (this->index - o.index + data->capacity())%data->capacity();
            }

            bool operator==(Self const& i){ return data == i.data && index == i.index; }
            bool operator!=(Self const& i){ return data == i.data && index != i.index; }
            bool operator< (Self const& i){ return data == i.data && true_index() < i.true_index(); }
            bool operator> (Self const& i){ return data == i.data && true_index() > i.true_index(); }
            bool operator<=(Self const& i){ return data == i.data && true_index() <= i.true_index(); }
            bool operator>=(Self const& i){ return data == i.data && true_index() >= i.true_index(); }

        private:
            typename deque<T>::size_type true_index(){
                if(index < data->_start ){
                    index + data->capacity() - data->_start;
                }else{
                    return index - data->_start;
                }
            }
            deque<T> *data;
            std::size_t index;
        };
        struct _const_iterator:xusd::iterator<random_access_iterator_tag,T,std::ptrdiff_t,T const*,T const&>{ 
            typedef random_access_iterator_tag iterator_category;
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef T const* pointer;
            typedef T const& reference;
            typedef _const_iterator Self;

            _const_iterator(_const_iterator const& i):data(i.data),index(i.index){}
            explicit _const_iterator(_iterator const& i):data(i.data),index(i.index){}//为了可以在iterator 和const_iterator转换
            _const_iterator(deque<T> &d,typename deque<T>::size_type i):data(&d),index(i){}
            _const_iterator& operator=(_const_iterator const&i){ this->data = i.data;this->index = i.index;  return *this;}
            _const_iterator& operator=(_iterator const&i){ this->data = i.data;this->index = i.index;  return *this;}
            //_const_iterator& operator=(_iterator const&i){ this->data = i.data;this->index = i.index;  return *this;}

            reference operator*(){
                return *(data->start_of_storage + index);
            }
            pointer operator->(){
                return &(operator*());
            }

            Self& operator++(){ 
                index = (index+1) % data->capacity();
                return *this;  
            }

            Self operator++(int){ 
                Self temp(*this); 
                index = (index+1) % data->capacity();
                return temp;   
            }

            Self& operator--(){
                index = ((index -1)+data->capacity()) % data->capacity();
            }
            Self operator--(int){
                Self tmp(*this);
                index = ((index -1)+data->capacity()) % data->capacity();
                return tmp;
            }

            Self operator+(difference_type n){  
                Self tmp(*this);
                tmp.index = (tmp.index+n) % (tmp.data->end_of_storage - tmp.data->start_of_storage);
                return tmp;
            }
            Self operator-(difference_type n){  
                Self tmp(*this);
                tmp.index = ((tmp.index - n)+tmp.data->capacity()) % tmp.data->capacity();
                return tmp;
            }

            Self& operator+=(difference_type n){  
                index = (index+n) % data->capacity();
                return *this;
            }
            Self& operator-=(difference_type n){  
                index = ((index - n)+data->capacity()) % data->capacity();
                return *this;
            }

            difference_type operator-(Self const&o){
                return (this->index - o.index + data->capacity())%data->capacity();
            }

            bool operator==(Self const& i){ return index == i.index; }
            bool operator!=(Self const& i){ return index != i.index; }
            bool operator<(Self const& i){ return true_index() < i.true_index(); }
            bool operator>(Self const& i){ return true_index() > i.true_index(); }
            bool operator<=(Self const& i){ return true_index() <= i.true_index(); }
            bool operator>=(Self const& i){ return true_index() >= i.true_index(); }

            private:
            difference_type true_index(){
                if(index < data->_start ){
                    index + data->capacity() - data->_start;
                }else{
                    return index - data->_start;
                }
            }
            deque<T> *data;
            std::size_t index;
        };
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef T const* const_pointer;
        typedef T& reference;
        typedef T const& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        typedef _iterator iterator;
        typedef _const_iterator const_iterator;

        typedef xusd::reverse_iterator<typename deque<T>::iterator> reverse_iterator;
        typedef xusd::reverse_iterator<const_iterator> const_reverse_iterator;

    public:

        typename deque<T>::iterator begin(){ return typename deque<T>::iterator(*this,_start); }
        typename deque<T>::iterator end(){ return typename deque<T>::iterator(*this,_end); }
        const_iterator begin()const{ return const_iterator(*this,_start); }
        const_iterator end()const{ return const_iterator(*this,_end); }

        reverse_iterator rbegin(){ return reverse_iterator(end()); }
        reverse_iterator rend(){ return reverse_iterator(begin()); }
        const_reverse_iterator rbegin()const{ return const_reverse_iterator(end()); }
        const_reverse_iterator rend()const{ return const_reverse_iterator(begin()); }

        reference operator[](size_type n){ return *(start_of_storage +(_start + n)%capacity()); }
        const_reference operator[](size_type n) const{ return *(start_of_storage +(_start + n)%capacity()); }

        bool empty()const{ 
            //这里要注意的就是:capacity() 一定要比size() 多1,不然就会出现 满数据的时候也是 _start == _end
            return _start == _end; 
        }

        size_type max_size()const{ return UINT_MAX/sizeof(value_type); }

        size_type size()const{
            if(capacity() == 0) return 0;
            if(_end >= _start){
                return _end - _start;
            }else{
                return _end - _start + capacity();
            }
        }

        size_type capacity()const{ return end_of_storage -  start_of_storage; }

        deque(Alloc alloc = Alloc()):allocator(alloc),start_of_storage(NULL),end_of_storage(NULL),_start(0),_end(0){ }
        deque(deque const&d):allocator(d.allocator),start_of_storage(NULL),end_of_storage(NULL),_start(0),_end(0){ 
            start_of_storage = allocator.allocate(d.size()+1);            
            end_of_storage = start_of_storage + d.size()+1;
            xusd::uninitialized_copy(d.begin(),d.end(),start_of_storage);
            _start = 0;
            _end = _start + d.size();
        }
        deque(size_type n,value_type const& x = value_type(),Alloc alloc = Alloc())
            :allocator(alloc),start_of_storage(NULL),end_of_storage(NULL),_start(0),_end(0){
                start_of_storage = allocator.allocate(n+1);            
                end_of_storage = start_of_storage + n + 1;
                xusd::uninitialized_fill_n(start_of_storage,n,x);
                _start = 0;
                _end = _start + n;
            }

        template<typename InputIterator>
        deque(InputIterator first,InputIterator last,Alloc alloc = Alloc())
        :allocator(alloc),start_of_storage(NULL),end_of_storage(NULL),_start(0),_end(0){
            typename iterator_traits<InputIterator>::difference_type n = distance(first,last);
            start_of_storage = allocator.allocate(n + 1);            
            end_of_storage = start_of_storage + n + 1;
            xusd::uninitialized_copy(first,last,start_of_storage);
            _start = 0;
            _end = _start + n;
        }

        ~deque(){
            xusd::destroy(begin(),end());
            allocator.deallocate(start_of_storage,capacity());
        }

        deque& operator=(deque const& d){
            start_of_storage = allocator.allocate(d.size()+1);            
            end_of_storage = start_of_storage + d.size()+1;
            xusd::uninitialized_copy(d.begin(),d.end(),start_of_storage);
            _start = 0;
            _end = _start + d.size();
            return *this;
        }

        Alloc get_allocator(){
            return allocator;
        }

        void swap(deque &d){
            using xusd::swap;
            swap(start_of_storage,start_of_storage);
            swap(end_of_storage,end_of_storage);
            swap(_start,_start);
            swap(_end,_end);
        }


        reference front(){ return *(start_of_storage + _start); }
        const_reference front()const{ return *(start_of_storage + _start); }

        reference back(){
            return *(start_of_storage + (((_end - 1) + capacity()) % capacity()));
        }
        const_reference back()const{
            return *(start_of_storage + (((_end - 1) + capacity()) % capacity()));
        }

        void push_front(const value_type & x){
            if(capacity() - size() > 1){
                size_type tmp = (_start - 1 + capacity())%capacity();
                xusd::construct(start_of_storage + tmp,x);
                _start = tmp;
            }else{
                insert(begin(),x);
            }
        }

        void pop_front(){
            //是否要抛出异常
            xusd::destroy(start_of_storage+_start,1);
            _start = (_start + 1)%capacity();
        }

        void push_back(value_type const& x){
            if(capacity() - size() > 1){
                size_type tmp = (_end + 1)%capacity();
                xusd::construct(start_of_storage + _end,x);
                _end = tmp;
            }else{//容量不足,用insert插入
                insert(end(),x);
            }
        }

        void pop_back(){
            //是否要抛出异常
            size_type tmp = (_end -1 + capacity()) % capacity();
            destroy(start_of_storage+tmp);
            _end = tmp;
        }

        typename deque<T>::iterator insert(typename deque<T>::iterator pos,value_type const&x){
            if(capacity() - size() > 1){//容量充足
                //移位
                if(pos == end()){ //若容器为空,则直接push_back(x)
                    push_back(x);
                }else{
                    xusd::construct(start_of_storage + _end/*相当于end(),但是end()没有析取操作*/,back());
                    typename deque<T>::iterator result = copy_backward(pos,end()-1,end());
                    *(--result) = x;
                    _end = (_end + 1)%capacity();//(++end()).index
                }
            }else{
                size_type new_size = capacity() == 0 ? 2: capacity()*2+1; //capacity() >= size() + 1 是一定要成立的
                pointer new_start = allocator.allocate(new_size);            
                pointer new_end  = new_start;
                try{
                    new_end = xusd::uninitialized_copy(begin(),pos,new_end);
                    xusd::construct(new_end,x);
                    ++new_end;
                    new_end = xusd::uninitialized_copy(pos,end(),new_end);

                    //释放原来的数据空间
                    xusd::destroy(begin(),end());
                    allocator.deallocate(start_of_storage,capacity());

                    start_of_storage = new_start;
                    end_of_storage = new_start + new_size;

                    _start = 0;
                    _end = distance(new_start,new_end);
                }catch(...){
                    xusd::destroy(new_start,new_end);
                    allocator.deallocate(new_start,new_size);
                    throw;
                }
            }

            return pos;
        }



        template<typename InputIterator>
        void insert(typename deque<T>::iterator pos,InputIterator first,InputIterator last){
            size_type n = distance(first,last);
            if(capacity() - size() > n){//容量充足
                /*difference_type*/size_type dis =  distance(pos,end());
                if(dis > n ){
                    //1.拷贝[end() - distance(first,last),end()到未初始化空间[end(),end()+distance(first,last))
                    //2.拷贝[pos,end() - distance(first,last))到[pos+distance(first,last),end())
                    //3.拷贝[first,last)到[pos,pos+distance(first,last))
                    uninitialized_copy(end()-n,end(),end());
                    copy_backward(pos,end()-n,end());
                    copy(first,last,pos);
                }else{//pos + n >= end()
                    //1.空间[pos,end())拷贝到未初始化空间[pos + distance(first,last), end + distance(first,last))
                    //2.拷贝[first,first + distance(pos,end()))到[pos,end)
                    //3.拷贝[first + distance(pos,end()),last)到[end(), pos+ distance(first,last))
                    iterator new_end = xusd::uninitialized_copy(pos,end(),pos+n);
                    xusd::uninitialized_copy(first+dis,last,copy(first,first+dis,pos));
                }
                _end = (_end+n)%capacity();
            }else{
                size_type new_size = capacity() + max(n,capacity()) + 1;
                pointer new_start = allocator.allocate(new_size);            
                pointer new_end = new_start;
                try{
                    //1.申请一块更大空间
                    //2.拷贝[begin(),pos)数据到[new_begin,new_begin + (pos - begin()) )
                    //3.拷贝[first,last)到[new_begin+distance(begin(),pos),new_begin+distance(begin(),pos)+distance(first,last) )  
                    //4.拷贝[pos,end)数据到空间[new_begin + (pos - begin()) +n,new_begin + (pos - begin()) +n + end() - pos);
                    new_end = xusd::uninitialized_copy(begin(),pos,new_end);
                    new_end = xusd::uninitialized_copy(first,last,new_end);
                    new_end = xusd::uninitialized_copy(pos,end(),new_end);

                    start_of_storage = new_start;
                    end_of_storage = new_start + new_size;

                    _start = 0;
                    _end = distance(new_start,new_end);
                }catch(...){
                    xusd::destroy(new_start,new_end);
                    allocator.deallocate(new_start,new_size);
                    throw;
                }

            }
        }

        void insert(typename deque<T>::iterator pos,size_type n,value_type const& x /*= value_type()*/){
            if(size() + n < capacity()){//容量充足
                difference_type move_size = distance(pos,end());
                if(move_size <= n){//要移动的元素个数小于n说明,要移动的元素要全部移到未初始化空间
                    //1.[pos,end())全部移动到未初始化空间[pos+n,end()+n)
                    //2.在剩下未初始化空间[end(),pos+n)填充x
                    //3.在已经初始化空间[pos,end())填充(n - (pos + n -end()))个x
                    uninitialized_copy(pos,end(),pos+n);
                    uninitialized_fill(end(),pos+n,x);
                    fill(pos,end(),x);
                }else{
                    //1.[end()-n,end)据移动到未初始化空间[end(),end()+n)
                    //2.[pos,end()-n)移动到后面[pos+n,end)
                    //3.填充n个x到空间[pos,pos+n)
                    uninitialized_copy(end()-n,end(),end());
                    copy_backward(pos,end()-n,end());
                    fill(pos,pos+n,x);
                }
                _end = (_end+n)%capacity();
            }else{
                size_type new_size = capacity() + max(n,capacity()) + 1;
                pointer first = allocator.allocate(new_size);            
                pointer last = first;
                try{
                    //1.申请一块更大空间
                    //2.拷贝[begin(),pos)数据到[new_begin,new_begin + (pos - begin()) )
                    //3.填充n个x到空间[new_begin + (pos - begin()),new_begin + (pos - begin()) +n)
                    //4.拷贝[pos,end)数据到空间[new_begin + (pos - begin()) +n,new_begin + (pos - begin()) +n + end() - pos);
                    last = xusd::uninitialized_copy(begin(),pos,first);
                    last = xusd::uninitialized_fill_n(last,n,x);
                    last = xusd::uninitialized_copy(pos,end(),last);

                    start_of_storage = first;
                    end_of_storage = first + new_size;

                    _start = 0;
                    _end = last - first;
                }catch(...){
                    xusd::destroy(first,last);
                    allocator.deallocate(first,new_size);
                    throw;
                }

            }
        }

        void clear(){
            xusd::destroy(begin(),end());
            _end = _start = 0;
        }


        iterator erase(iterator pos){
            copy(pos+1,end(),pos);
            destroy(end()-1,end());
            _end = (_end - 1 + capacity())%capacity();
        }

        iterator erase(iterator first,iterator last){
            if(first != last){
                //1.拷贝[last,end())覆盖到[first,first+(end()-last))
                //2.删除空间[first + end() - last,end())内数据
                copy(last,end(),first);
                destroy(end()-(last-first),end());
                _end = (_end - (last-first) + capacity())%capacity();
            }
        }

        void resize(size_type n,value_type const& x = value_type()){
            if(n>size()){
                insert(end(),n-size(),x);
            }
            if(n<size()){
                erase(end() - (size()-n),end());
            }
        }

        void assign(size_type n,const value_type & x /*= value_type()*/){
            clear();
            insert(begin(),n,x);
        }

        template<typename InputIterator>
        void assign(InputIterator first,InputIterator last){
            clear();
            insert(begin(),first,last);
        }

    private:
        Alloc allocator;
        pointer start_of_storage;
        pointer end_of_storage;
        size_type _start;//这个要改为指针形式
        size_type _end; //这个要改为指针形式
    };

    template<typename T>
    bool operator==(deque<T> const& x,deque<T> const&y){
        if(x.size() != y.size()) return false;
        typename deque<T>::iterator i = x.begin();
        typename deque<T>::iterator j = y.begin();
        for(;i!= x.end() && j!=y.end(); ++i,++j){
            if(*i != *j){
                return false;
            }
        }
        return true;
    }

    template<typename T>
    bool operator<(deque<T> const& x,deque<T> const&y){
        return lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
    }

}

#endif
