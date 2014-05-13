
#ifndef __FORK_STL__ITERATOR__
#define __FORK_STL__ITERATOR__
#include <cstddef>
#include <iostream>

namespace xusd{

    //自行编写iterator时候继承下面的类就可以了,避免遗漏了
    template<typename Category,typename T,typename Distance = std::ptrdiff_t,typename Pointer = T*,typename Reference = T&>
    struct iterator{
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type ;
        typedef Pointer pointer;
        typedef Reference reference;
    };


    //下面这个类还是蛮强的,主要是萃取各个iterator的内部5种类型,有两个偏特化版本,T* and T const*
    template<typename Iterator>
    struct iterator_traits{
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };

    //以下为iterator中的category五种类型
    struct output_iterator_tag{};
    struct input_iterator_tag{};
    struct forward_iterator_tag:public input_iterator_tag{};
    struct bidirectional_iterator_tag:public forward_iterator_tag{};
    struct random_access_iterator_tag:public bidirectional_iterator_tag{};

    //对普通指针进行特化
    template<typename T>
    struct iterator_traits<T*>{
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
    };

    //对普通const指针进行特化
    template<typename T>
    struct iterator_traits<T const*>{
        typedef random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef T const* pointer;
        typedef T const& reference;
    };


    //iterator类型的萃取,本来想用来作为函数偏特化用的,暂时没有用到
    struct iterator_category_traits{
    public:
        enum{
            is_input_iterator = 1,
            is_forward_iterator = 2,
            is_bidirectional_iterator = 3,
            is_random_access_iterator = 4,
            is_output_iterator = 5
        };
    };


    template<typename InputIterator>
    typename iterator_traits<InputIterator>::difference_type 
    distance_I(InputIterator first,InputIterator last,typename iterator_traits<InputIterator>::iterator_category input_iterator_tag){
        typename iterator_traits<InputIterator>::difference_type  x = 0;//这里只能为0了,没有想出更加好的办法
        for(;first != last;++first) ++x;
        return x;
    }

    template<typename RandomAccessIterator>
    typename iterator_traits<RandomAccessIterator>::difference_type 
    distance_I(RandomAccessIterator first,RandomAccessIterator last,random_access_iterator_tag){
        return (last - first);
    }

    template<typename InputIterator>
    typename iterator_traits<InputIterator>::difference_type 
    distance(InputIterator first,InputIterator last){
        typedef typename xusd::iterator_traits<InputIterator>::iterator_category category; 
        return xusd::distance_I<InputIterator>(first,last,category());
    }

    template<typename InputIterator,typename Distance>
    void advance_I(InputIterator& i,Distance n,typename iterator_traits<InputIterator>::iterator_category input_iterator_tag){
        while(n--) ++i;
    }

    template<typename InputIterator,typename Distance>
    void advance_I(InputIterator& i,Distance n,bidirectional_iterator_tag){
        if(n>=0){
            while(n--) ++i;
        }else{
            while(n++) --i;
        }
    }

    template<typename InputIterator,typename Distance>
    void advance_I(InputIterator& i,Distance n,random_access_iterator_tag){
        i = i+n;
    }

    template<typename InputIterator,typename Distance>
    void advance(InputIterator& i,Distance n){
        typedef typename iterator_traits<InputIterator>::iterator_category category; 
        advance_I(i,n,category());
    }



    //////////////////////////////////////////////////////
    //以下是Iterator adapter

    template<typename FrontInsertionSequence>
    class front_insert_iterator:public iterator<output_iterator_tag,void,void,void,void>
    {
    public:
        //typedef output_iterator_tag iterator_category;//这里可以去掉在父类里面已经定义了
        front_insert_iterator(FrontInsertionSequence &S):s(&S){ }
        front_insert_iterator(front_insert_iterator const&fi):s((fi.s)){}
        ~front_insert_iterator(){}
        front_insert_iterator& operator=(front_insert_iterator<FrontInsertionSequence> const& o){
            if(this == &o) return *this;
            this->s = o.s;
            return *this;
        }

        front_insert_iterator& operator*(){ return *this; }
        front_insert_iterator& operator++(){ return *this; }
        front_insert_iterator& operator++(int){ return *this; }
        front_insert_iterator& operator=(typename FrontInsertionSequence::value_type const& value){ 
            s->push_front(value);//重点在这里,替代了operator*=
            return *this;
        }

    private:
        FrontInsertionSequence *s;
    };

    template<typename FrontInsertionSequence>
    front_insert_iterator<FrontInsertionSequence>
    front_inserter(FrontInsertionSequence &S){
        return front_insert_iterator<FrontInsertionSequence>(S);
    }

    template<typename BackInsertionSequence>
    class back_insert_iterator:public iterator<output_iterator_tag,void,void,void,void>
    {
    public:
        //typedef output_iterator_tag iterator_category;//这里可以去掉在父类里面已经定义了
        back_insert_iterator(BackInsertionSequence &S):s(&S){ }
        back_insert_iterator(back_insert_iterator const&fi):s((fi.s)){}
        ~back_insert_iterator(){}
        back_insert_iterator& operator=(back_insert_iterator<BackInsertionSequence> const& o){
            this->s = o.s;
            return *this;
        }

        back_insert_iterator& operator*(){ return *this; }
        back_insert_iterator& operator++(){ return *this; }
        back_insert_iterator& operator++(int){ return *this; }
        back_insert_iterator& operator=(typename BackInsertionSequence::value_type const& value){ 
            s->push_back(value);//重点在这里,替代了operator*=
            return *this;
        }

    private:
        BackInsertionSequence *s;
    };


    template<typename BackInsertionSequence>
    back_insert_iterator<BackInsertionSequence>
    back_inserter(BackInsertionSequence &S){
        return back_insert_iterator<BackInsertionSequence>(S);
    }


    template<typename Container>
    class insert_iterator: public iterator<output_iterator_tag,void,void,void,void>{
    public:
        insert_iterator(Container &C,typename Container::iterator i):s(&C),iterator(i){ }
        insert_iterator(insert_iterator const& o):s(o.s),iterator(o.iterator){}
        ~insert_iterator(){}

        insert_iterator& operator*(){ return *this; }
        insert_iterator& operator++(){ return *this; }
        insert_iterator& operator++(int){ return *this; }
        insert_iterator& operator=(typename Container::value_type const&value){ 
            s->insert(iterator,value);
            ++iterator; //使得iterator一直指向构造时传入的指针指向的对象
            return *this;
        }
    private:
        Container* s;
        typename Container::iterator iterator;
    };

    template<typename Container,typename Iterator>
    insert_iterator<Container>
    inserter(Container& C,Iterator p){
        return insert_iterator<Container>(C,p);
    }


    template<typename Iterator>
    class reverse_iterator:public iterator<
                           typename iterator_traits<Iterator>::iterator_category,
                           typename iterator_traits<Iterator>::value_type,
                           typename iterator_traits<Iterator>::difference_type,
                           typename iterator_traits<Iterator>::pointer,
                           typename iterator_traits<Iterator>::reference
                           >
    {
    public:
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits<Iterator>::value_type value_type;
        typedef typename iterator_traits<Iterator>::difference_type difference_type;
        typedef typename iterator_traits<Iterator>::pointer pointer;
        typedef typename iterator_traits<Iterator>::reference reference;
        typedef Iterator iterator_type;
        typedef reverse_iterator<Iterator> self;
        
        explicit reverse_iterator(Iterator i):base_iterator(i){ }
        reverse_iterator():base_iterator(){}
        ~reverse_iterator(){}
        reverse_iterator(reverse_iterator<Iterator> const& o):base_iterator(o.base_iterator){}
        template<typename I>
        reverse_iterator(reverse_iterator<I>& o):base_iterator(o.base_iterator){}//主要是应用于像,iterator=>const_iterator的转换
        reverse_iterator& operator=(reverse_iterator<Iterator> const& o){ base_iterator = o.base_iterator; }
        Iterator base()const{
            return base_iterator;
        }
        reference operator*() const { 
            iterator_type tmp = base_iterator;
            return *(--tmp);
        } 

        self& operator++(){--base_iterator; return *(this); } 
        self& operator--(){++base_iterator; return *(this); } 

        self operator++(int){ 
            iterator_type tmp = base_iterator;
            --base_iterator;
            return self(tmp); 
        } 
        self operator--(int){ 
            iterator_type tmp = base_iterator;
            ++base_iterator;
            return self(tmp); 
        } 

        self operator+(difference_type n){ return self(base_iterator-n); }
        self operator-(difference_type n){ return self(base_iterator+n); }

        self& operator+=(difference_type n){
            base_iterator -= n;
            return *this;
        }
        self& operator-=(difference_type n){
            base_iterator += n;
            return *this;
        }

        reference operator[](difference_type n){
           return *(*this+n); 
        }

        pointer operator->(){
            return &(operator*());//暂时么有验证这里有木有问题
        }


    private:
        Iterator base_iterator;
    };

    template<typename Iterator>
    bool operator==(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r){
        return (l.base() == r.base());
    }

    template<typename Iterator>
    bool operator!=(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r){
        return (l.base() != r.base());
    }
    template<typename Iterator>
    bool operator<(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r){
        return (r.base() < l.base());
    }

    template<typename Iterator>
    bool operator>(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r){
        return (r<l);
    }
    template<typename Iterator>
    bool operator>=(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r){
        return !(l<r);
    }
    template<typename Iterator>
    bool operator<=(reverse_iterator<Iterator> const&l,reverse_iterator<Iterator> const&r){
        return !(r<l);
    }


    //以下是iostream_iterator
    template<typename T,typename Distance = std::ptrdiff_t>  
    class istream_iterator:public iterator<input_iterator_tag,T,Distance,const T*,const T& >
    {
    public:
        typedef input_iterator_tag iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef const T* pointer;
        typedef const T& reference;

        istream_iterator():stream(NULL),end_mark(false){ }
        istream_iterator(std::istream &s):stream(&s){read(); }

        reference operator*(){
            return value;
        }

        pointer operator->(){
            return &(operator*());
        }

        istream_iterator<T,Distance>& operator++(){
            this->read();
            return *this;
        }
        istream_iterator<T,Distance> operator++(int){
            istream_iterator<T,Distance> tmp(*this);
            read();
            return tmp;
        }

        bool operator==(istream_iterator<T,Distance> const& o){
            return ( (o.stream == this->stream && o.end_mark == this->end_mark) || (o.end_mark == false && this->end_mark == false));
        }

        bool operator!=(istream_iterator<T,Distance> const& o){
            return !(*this==o);
        }

    private:
        std::istream* stream;
        T value;
        bool end_mark;
        inline void read(){
            end_mark = ( (*stream) ? true : false );
            if(end_mark){ *stream >> value; }
            end_mark = ( (*stream) ? true : false );
        }

    };

    template<typename T>
    class ostream_iterator:public iterator<output_iterator_tag,void,void,void,void>
    {
    public:
        typedef input_iterator_tag iterator_category;
        ostream_iterator(std::ostream& s):stream(&s),string(NULL){}
        ostream_iterator(std::ostream& s,const char*c):stream(&s),string(c){}
        ostream_iterator<T>& operator=(T const& value){
            *stream<<value;
            if(string) 
                *stream<<string;
            return *this;
        }

        ostream_iterator<T>& operator*(){ return *this; }
        ostream_iterator<T>& operator++(){ return *this; }
        ostream_iterator<T>& operator++(int){ return *this; }//这里编译器会报后置operator++不能返回引用类型的Warning,可以忽略

    private:
        std::ostream* stream;
        const char*string;//分隔符
    };


}


#endif

