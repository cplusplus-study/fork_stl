// -*- C++ -*-

#ifndef __FROK_STL_LIST__
#define __FROK_STL_LIST__

#include <climits>
#include <memory.hpp>
#include <iterator.hpp>
#include <algorithm.hpp>

#ifdef MY_TEST
#include <iostream>
#endif

namespace xusd{
    template<typename T>
    struct list_node{
        list_node<T>* prev;
        list_node<T>* next;
        T data;
    };

    //iterator 可转换为 const_iterator
    template<typename T,typename Distance = std::ptrdiff_t,typename Pointer = T*,typename Reference = T&>
    struct list_iterator:iterator<bidirectional_iterator_tag,T,Distance,Pointer,Reference>{

        typedef typename iterator_traits<iterator<bidirectional_iterator_tag,T> >::iterator_category iterator_category;
        typedef typename iterator_traits<iterator<bidirectional_iterator_tag,T> >::value_type value_type;
        typedef typename iterator_traits<iterator<bidirectional_iterator_tag,T> >::difference_type difference_type;
        typedef typename iterator_traits<iterator<bidirectional_iterator_tag,T> >::pointer pointer;
        typedef typename iterator_traits<iterator<bidirectional_iterator_tag,T> >::reference reference;

        typedef list_iterator<T,Distance,Pointer,Reference> Self;

        list_iterator(list_node<T>* p = NULL):ptr(p){ }
        list_iterator(Self const&o):ptr(o.ptr){}
        template<typename T1,typename Distance1 /*= std::ptrdiff_t*/,typename Pointer1 /*= T1**/,typename Reference1 /*= T1&*/>
        Self& operator=(list_iterator<T1,Distance1,Pointer1,Reference1> const&o){ 
            ptr = o.ptr; 
        }

        reference operator*(){ return (*ptr).data/*ptr->data*/; }
        pointer operator->(){ return &(operator*());}

        Self& operator++(){ ptr = ptr->next; return *this; }
        Self operator++(int){ Self temp(*this); ptr = ptr->next; return temp; }

        Self& operator--(){ ptr = ptr->prev; return *this; }
        Self operator--(int){ Self temp(*this); ptr = ptr->prev; return temp; }

        bool operator==(Self const& o)const{ return o.ptr == this->ptr; }
        bool operator!=(Self const& o)const{ return !(*this == o); }

    //private:
        list_node<T>* ptr;
    };

    //iterator 可转换为 const_iterator
    template<typename T,typename Distance = std::ptrdiff_t,typename Pointer = T const*,typename Reference = T const&>
    struct list_const_iterator:iterator<bidirectional_iterator_tag,T,Distance,Pointer,Reference>{

        typedef typename iterator_traits<iterator<bidirectional_iterator_tag,T> >::iterator_category iterator_category;
        typedef typename iterator_traits<iterator<bidirectional_iterator_tag,T> >::value_type value_type;
        typedef typename iterator_traits<iterator<bidirectional_iterator_tag,T> >::difference_type difference_type;
        typedef typename iterator_traits<iterator<bidirectional_iterator_tag,T> >::pointer pointer;
        typedef typename iterator_traits<iterator<bidirectional_iterator_tag,T> >::reference reference;

        typedef list_const_iterator<T,Distance,Pointer,Reference> Self;

        list_const_iterator(list_node<T>* p = NULL):ptr(p){ }
        list_const_iterator(list_const_iterator const&o):ptr(o.ptr){}
        Self& operator=(Self const&o){ ptr = o.ptr; }
        Self& operator=(list_iterator<T,difference_type,T*,T&> const&o){ ptr = o.ptr; }


        reference operator*(){ return (*ptr).data/*ptr->data*/; }
        pointer operator->(){ return &(operator*());}

        Self& operator++(){ ptr = ptr->next; return *this; }
        Self operator++(int){ Self temp(*this); ptr = ptr->next; return temp; }

        Self& operator--(){ ptr = ptr->prev; return *this; }
        Self operator--(int){ Self temp(*this); ptr = ptr->prev; return temp; }

        bool operator==(Self const& o)const{ return o.ptr == this->ptr; }
        bool operator!=(Self const& o)const{ return !(*this == o); }

    //private:
        list_node<T>* ptr;
    };

    template<typename T,typename Alloc = allocator<list_node<T> > >
    class list{
    public:
        typedef T value_type;
        typedef T*pointer;
        typedef T const* const_pointer;
        typedef T& reference;
        typedef T const& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef list_iterator<T> iterator;
        typedef list_iterator<T,std::ptrdiff_t,T const*,T const&> const_iterator;
        typedef xusd::reverse_iterator<iterator>         reverse_iterator;
        typedef xusd::reverse_iterator<const_iterator>   const_reverse_iterator;

        typedef list<T,Alloc> Self;
        typedef list_node<T>* link_type;

    public:
        iterator begin(){ return iterator(node->next); }
        const_iterator begin()const{ return const_iterator(node->next); }
        iterator end(){ return node; }
        const_iterator end()const{ return node; }
        reverse_iterator rbegin(){return reverse_iterator(end()); }
        reverse_iterator rend(){return reverse_iterator(end());}
        const_reverse_iterator rbegin()const{ return const_reverse_iterator(end());}
        const_reverse_iterator rend()const{ return const_reverse_iterator(end()); }
        size_type size() const{
            size_type n = 0;
            link_type p = node;
            while(p->next != node) {
                ++n;
                p = p->next;
            }
            return n;
        };

        size_type max_size() const{
            return UINT_MAX/sizeof(list_node<T>);
        }

        bool empty(){
            return ((node->next == node) && (node->prev == node));
        }
        
        explicit list(Alloc const&alloc = Alloc()):allocator(alloc){ 
            empty_initialize();
        }

        explicit list(size_type n,value_type const&value = value_type(),Alloc const&alloc = Alloc()):allocator(alloc){
            empty_initialize();
            insert(end(),n,value);
        }

        list(list<T> const& l):allocator(l.allocator){
            empty_initialize();
            insert(end(),l.begin(),l.end());
        }


        //这里template决议的时候会不会和list(size_type,value_type const&value)混淆?
        template<typename InputIterator>
        list(InputIterator first,InputIterator last,const Alloc &alloc = Alloc()):allocator(alloc){
            empty_initialize();
            insert(end(),first,last);
        }

        Self& operator=(Self const&l){
            if(!(&l == this)){
                allocator = l.allocator;
                empty_initialize();
                insert(end(),l.begin(),l.end());
            }
            return *this;
        }

        ~list(){
            clear();
        }

        Alloc get_allocator()const{
            return allocator;
        }

        void swap(Self& l){
            //为什么要用using,在寻找的时候会不会出现多个swap的歧义呢
            using xusd::swap;
            swap(node,l.node);
        }

        reference front(){
            return *(begin());
        }

        const_reference front() const{
            return *(begin());
        }

        reference back(){
            return node->prev->data;
        }

        const_reference back()const{
            return node->prev->data;
        }

        void push_front(const value_type& x){
            insert(begin(),x);
        }

        void push_back(const value_type& x){
            insert(end(),x);
        }

        void pop_front(){
            erase(begin());
        }
        void pop_back(){
            iterator tmp = end();
            erase(--tmp);
        }

        iterator erase(iterator i){

            link_type p1 = i.ptr->prev;
            link_type p2 = i.ptr->next;
            delete_node(i.ptr);
            p1->next = p2;
            p2->prev = p1;

            return iterator(p2);
        }

        iterator erase(iterator first,iterator last){
            link_type p1 = first.ptr->prev;
            link_type p2 = last.ptr;

            link_type p = first.ptr;
            while(p !=p2){
                p = p->next;
                delete_node(p->prev);
            }

            p1->next = p2;
            p2->prev = p1;

            return tmp(p2);
        }

        void resize(size_type n,value_type const& x){
            size_type this_size = size();
            if(n>this_size){
                insert(end(),n - this_size,x);
            }else{
                iterator i = begin();
                advance(i,n);
                erase(i,end());
            }
            
        }

        template<typename InputIterator>
        void assign(InputIterator first,InputIterator last){
            clear();
            insert(end(),first,last);
        }

        void assign(size_type n,value_type const& x){
            clear();
            insert(end(),n,x);
        }

        template<typename InputIterator>
        void insert(iterator pos,InputIterator first,InputIterator last){
            for(;first!=last;++first){
                link_type p = create_node(*first);
                pos.ptr->prev->next = p;
                p->prev = pos.ptr->prev;
                p->next = pos.ptr;
                pos.ptr->prev = p;
            }
        }

        iterator insert(iterator pos,T const&value){
            link_type p = create_node(value);
            pos.ptr->prev->next = p;
            p->prev = pos.ptr->prev;
            p->next = pos.ptr;
            pos.ptr->prev = p;
            return p;
        }

        void insert(iterator pos,size_type n,T const&value){
            while((n--)>0){
                link_type p = create_node(value);
                pos.ptr->prev->next = p;
                p->prev = pos.ptr->prev;
                p->next = pos.ptr;
                pos.ptr->prev = p;
            }
        }

        void clear(){
            link_type p = node->next;
            while(p != node){
                p = p->next;
                delete_node(p->prev);
            }
            node->prev = node;
            node->next = node;
        }


        void splice(iterator pos,list& x){
            if(x.empty()){
                return ;
            }
            link_type xp1 = x.node->next;
            link_type xp2 = x.node->prev;

            link_type p = pos.ptr;
            p->prev->next = xp1;
            xp1->prev = p->prev;

            xp2->next = p;
            p->prev = xp2;

            x.node->prev = x.node;
            x.node->next = x.node;//相当于x.clear
        }

        void splice(iterator pos,list& x,iterator i){
            iterator j = i;
            ++j;
            if(pos == i || pos ==j){//把i插入到自己前面或者后面一个的前面
                return ;
            }

            link_type p1 = i.ptr; 
            link_type p2 = pos.ptr;

            //断开i
            p1->prev->next = p1->next;
            p1->next->prev = p1->prev;

            //插入i到pos前面
            p2->prev->next = p1;
            p1->prev = p2->prev;
            p1->next = p2;
            p2->prev = p1;
            return ;
        }

        void splice(iterator pos,list& x,iterator first,iterator last){
            //下面的这个判断应该不比进行
            for(iterator i = first; i != last;++i){
                if(pos == i){ return ; }
            }

            link_type p1 = last.ptr->prev;
            last.ptr->prev->next = pos.ptr;

            //去掉first,last后连接两端
            first.ptr->prev->next = last.ptr;
            last.ptr->prev = first.ptr->prev;

            first.ptr->prev = pos.ptr->prev;
            pos.ptr->prev->next = first.ptr;
            pos.ptr->prev = p1;
            
            
            return ;
        }

        void remove(value_type const&x){
            iterator first = begin();
            iterator last = end();

            while(first != last){
                iterator next = first;
                ++next;
                if(*first == x) erase(first);
                first = next;
            }
        }
        
        void unique(){
            iterator first = begin();
            iterator last = end();

            if(first == last) return ;

            iterator next = first;
            while(++next != last){
                if(*next == *first){
                    erase(first);
                }
                first = next;
            }
        }

        template<typename BinaryPredicate>
        void unique(BinaryPredicate pred){
            iterator first = begin();
            iterator last = end();

            if(first == last) return ;

            iterator next = first;
            while(++next != last){
                if(pred(*next ,*first)){
                    erase(first);
                }
                first = next;
            }
        }


        void reverse(){
            link_type p = node;
            do{
                link_type next = p->next;
                p->next = p->prev;
                p->prev = next;
                p = next;
            }while(p!=node);
        }


        void merge(list& x){
            iterator first1 = begin();
            iterator last1 = end();

            iterator first2 = x.begin();
            iterator last2 = x.end();

            while((first1 != last1) && (first2 != last2)){
                if(*first2 < *first1){
                    splice(first1,x,first2++);
                }else{
                    ++first1;
                }
            }

            if(first2 != last2){
                splice(first1,x,first2,last2);
            }

        }

        template<typename StrictWeakOrdering>
        void merge(list& x,StrictWeakOrdering Comp){
            iterator first1 = begin();
            iterator last1 = end();

            iterator first2 = x.begin();
            iterator last2 = x.end();

            while((first1 != last1) && (first2 != last2)){
                if(Comp(*first2, *first1)){
                    splice(first1,x,first2++);
                }else{
                    ++first1;
                }
            }

            if(first2 != last2){
                splice(first1,x,first2,last2);
            }

        }

        //quick sort?
        void sort(){
            if(node->next == node || node->next->next == node) return ;

            Self carry;
            Self counter[64];

            int fill = 0;

            while(!empty()){
                carry.splice(carry.begin(),*this,begin());
                int i = 0;
                while(i<fill && !counter[i].empty()){
                    counter[i].merge(carry);
                    carry.swap(counter[i++]);
                }
                carry.swap(counter[i]);
                if(i==fill){
                    ++fill;
                }

            }

            for(int i = 1; i< fill; ++i){
                counter[i].merge(counter[i-1]);
            }

            swap(counter[fill-1]);

        }

        template<typename StrictWeakOrdering>
        void sort(StrictWeakOrdering Comp){
            if(node->next == node || node->next->next == node) return ;

            Self carry;
            Self counter[64];

            int fill = 0;

            while(!empty()){
                carry.splice(carry.begin(),*this,begin());
                int i = 0;
                while(i<fill && !counter[i].empty()){
                    counter[i].merge(carry,Comp);
                    carry.swap(counter[i++]);
                }
                carry.swap(counter[i]);
                if(i==fill){
                    ++fill;
                }

            }

            for(int i = 1; i< fill; ++i){
                counter[i].merge(counter[i-1],Comp);
            }

            swap(counter[fill-1]);

        }





    private:

        void swap_node(link_type i,link_type j){
            if(i == j) return ;
            if(i->next == j){
                link_type ip = i->prev;
                link_type jn = j->next;

                ip->next = j;
                jn->prev = i;

                i->prev = j;
                i->next = jn;
                j->prev = ip;
                j->next = i;

            }else if(j->next == i){
                swap_node(j,i);
            }else{
                link_type ip = i->prev;
                link_type in = i->next;
                link_type jp = j->prev;
                link_type jn = j->next;

                ip->next = j;
                in->prev = j;

                jp->next = i;
                jn->next = i;

                i->prev = jp;
                i->next = jn;

                j->prev = ip;
                j->next = in;

            }

        }
        void _delete(iterator i){
            if(i.ptr != node){
                link_type p = i.ptr->prev;
                p->next = i.ptr->next;
                p->next->prev = p;
                delete_node(i.ptr);
            }
        }
        link_type get_node(){ 
            return allocator.allocate(1); 
        }
        void put_node(link_type p){ 
            allocator.deallocate(p,1); 
        }
        link_type create_node(T const&x){ 
            link_type p = get_node(); 
            xusd::construct(&p->data,x); 
            return p; 
        }
        void delete_node(link_type p){ 
            xusd::destroy(&(p->data)); 
            put_node(p);
        }

        void empty_initialize(){ node = get_node(); node->next = node; node->prev = node; }


    private:
        list_node<T>* node;
        Alloc allocator;
    };

    template<typename T>
    void swap(list<T> &a,list<T> &b){
        a.swap(b);
    }

    template<typename T>
    bool operator==(list<T> const&a,list<T> const&b){
       typename list<T>::iterator i = a.begin();
       typename list<T>::iterator j = b.begin();
       for(; i!=a.end() && j!=b.end();++i,++j){
           if(*i != *j){
               return false;
           }
       }

       if(i!=a.end() || j!=b.end()){
           return false;
       }

       return true;
    }

    template<typename T>
    bool operator!=(list<T> const&a,list<T> const&b){
        return !(a == b);
    }

    template<typename T>
    bool operator<(list<T> const&a,list<T> const&b){
        return lexicographical_compare(a.first,a.last,b.first,b.last);
    }



}

#endif


