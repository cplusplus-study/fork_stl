// -*- C++ -*-

#ifndef __FORK_STL__FUNCTIONAL__
#define __FORK_STL__FUNCTIONAL__

namespace xusd{
    //一元函数
    template<typename Arg,typename Result>
    struct unary_function{
        typedef Arg argument_type;
        typedef Result result_type;
    };

    //二元函数
    template<typename Arg1,typename Arg2,typename Result>
    struct binary_function{
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };

    // if( x == y) return true else return false,default use operator==  
    template<typename T>
    struct equal_to:public binary_function<T,T,bool>{
        bool operator()(T const&a,T const& b)const{
            return a == b;
        }
    };

    // if( x != y) return true else return false,default use operator!=  
    template<typename T>
    struct not_equal_to:public binary_function<T,T,bool>{
        bool operator()(T const&a,T const& b)const{
            return a != b;
        }
    };

    // if( x < y) return true else return false,default use operator<  
    template<typename T>
    struct less:public binary_function<T,T,bool>{
        bool operator()(T const&a,T const& b)const{
            return a < b;
        }
    };

    // if( x > y) return true else return false,default use operator>  
    template<typename T>
    struct greater:public binary_function<T,T,bool>{
        bool operator()(T const&a,T const& b)const{
            return a > b;
        }
    };

    // if( x <= y) return true else return false,default use operator<=  
    template<typename T>
    struct less_equal:public binary_function<T,T,bool>{
        bool operator()(T const&a,T const& b)const{
            return a <= b;
        }
    };

    // if( x >= y) return true else return false,default use operator>=  
    template<typename T>
    struct greater_equal:public binary_function<T,T,bool>{
        bool operator()(T const&a,T const& b) const{
            return a >= b;
        }
    };

    template<typename T>
    struct plus:public binary_function<T,T,T>{
        T operator()(T const&x,T const&y) const{
            return x + y;
        }
    };

    template<typename T>
    struct minus:public binary_function<T,T,T>{
        T operator()(T const&x,T const& y) const{
            return x - y;
        }
    };

    template<typename T>
    struct multiplies:public binary_function<T,T,T>{
        T operator()(T const& x,T const& y)const{
            return x * y;
        }
    };

    template<typename T>
    struct devides:public binary_function<T,T,T>{
        T operator()(T const& x,T const& y)const {
            return x / y;
        }
    };

    template<typename T>
    struct modulus:public binary_function<T,T,T>{
        T operator()(T const& x,T const& y) const{
            return x % y;
        }
    };

    template<typename T>
    struct negate:public unary_function<T,T>{
        T operator()(T const&x)const{
            return -x;
        }
    };

    template<typename T>
    struct logical_and:public binary_function<T,T,bool>{
        bool operator()(T const&x ,T const& y)const{
            return x && y;
        }
    };

    template<typename T>
    struct logical_or:public binary_function<T,T,bool>{
        bool operator()(T const&x ,T const& y)const{
            return x || y;
        }
    };

    template<typename T>
    struct logical_not:public unary_function<T,bool>{
        bool operator()(T const&x)const{
            return !x;
        }
    };

    template<typename T>
    struct identity:public unary_function<T,T>{
        T const& operator()(T const&x)const{
            return x;
        }
    };

    template<typename Arg1,typename Arg2>
    struct project1st:public binary_function<Arg1,Arg2,Arg1>{
        Arg1 operator()(Arg1 const&x, Arg2 const& y){
            return x;
        }
    };

    template<typename Arg1,typename Arg2>
    struct project2st:public binary_function<Arg1,Arg2,Arg2>{
        Arg2 operator()(Arg1 const&x, Arg2 const& y){
            return y;
        }
    };



    template<typename X,typename R>
    struct mem_fun_t:public unary_function<X*,R>{
        explicit mem_fun_t(R (X::*f)()):fun(f){ }
        R operator()(X*x){
            return (x->*fun)();
        }
    private:
        R (X::*fun)();
    };

    template<typename X,typename R>
    mem_fun_t<X,R> mem_fun(R (X::*f)()){
        return mem_fun_t<X,R>(f);
    }

    template<typename X,typename R>
    struct mem_fun_ref_t:public unary_function<X&,R>{
        explicit mem_fun_ref_t(R (X::*f)()):fun(f){ }
        R operator()(X&x){
            return (x.*fun)();
        }
    private:
        R (X::*fun)();
    };

    template<typename X,typename R>
    mem_fun_ref_t<X,R> mem_fun_ref(R (X::*f)()){
        return mem_fun_ref_t<X,R>(f);
    }


    template<typename X,typename R,typename A>
    struct mem_fun1_t:public binary_function<X*,A,R>{
        explicit mem_fun1_t(R (X::*f)(A)):fun(f){ }
        R operator()(X*x,A arg){
            return (x->*fun)(arg);
        }
    private:
        R (X::*fun)(A);
    };

    template<typename X,typename R,typename A>
    mem_fun1_t<X,R,A> mem_fun(R (X::*f)(A)){
        return mem_fun1_t<X,R,A>(f); 
    }

    template<typename X,typename R,typename A>
    struct mem_fun1_ref_t:public binary_function<X*,A,R>{
        explicit mem_fun1_ref_t(R (X::*f)(A)):fun(f){ }
        R operator()(X&x,A arg){
            return (x.*fun)(arg);
        }
    private:
        R (X::*fun)(A);
    };

    template<typename X,typename R,typename A>
    mem_fun1_ref_t<X,R,A> mem_fun_ref(R (X::*f)(A)){
        return mem_fun1_ref_t<X,R,A>(f);
    }




    template<typename X,typename R>
    struct const_mem_fun_t:public unary_function<X*,R>{
        explicit const_mem_fun_t(R (X::*f)() const):fun(f){ }
        R operator()(X const*x){
            return (x->*fun)();
        }
    private:
        R (X::*fun)()const;
    };

    template<typename X,typename R>
    const_mem_fun_t<X,R> mem_fun(R (X::*f)()const){
        return const_mem_fun_t<X,R>(f);
    }

    template<typename X,typename R>
    struct const_mem_fun_ref_t:public unary_function<X&,R>{
        explicit const_mem_fun_ref_t(R (X::*f)()const):fun(f){ }
        R operator()(X const&x){
            return (x.*fun)();
        }
    private:
        R (X::*fun)()const;
    };

    template<typename X,typename R>
    const_mem_fun_ref_t<X,R> mem_fun_ref(R (X::*f)()const){
        return const_mem_fun_ref_t<X,R>(f);
    }


    template<typename X,typename R,typename A>
    struct const_mem_fun1_t:public binary_function<X*,A,R>{
        explicit const_mem_fun1_t(R (X::*f)(A)const):fun(f){ }
        R operator()(X const*x,A arg){
            return (x->*fun)(arg);
        }
    private:
        R (X::*fun)(A)const;
    };

    template<typename X,typename R,typename A>
    const_mem_fun1_t<X,R,A> mem_fun(R (X::*f)(A)const){
        return const_mem_fun1_t<X,R,A>(f); 
    }

    template<typename X,typename R,typename A>
    struct const_mem_fun1_ref_t:public binary_function<X*,A,R>{
        explicit const_mem_fun1_ref_t(R (X::*f)(A)const):fun(f){ }
        R operator()(X const&x,A arg){
            return (x.*fun)(arg);
        }
    private:
        R (X::*fun)(A)const;
    };

    template<typename X,typename R,typename A>
    const_mem_fun1_ref_t<X,R,A> mem_fun_ref(R (X::*f)(A)const){
        return const_mem_fun1_ref_t<X,R,A>(f);
    }



    template<typename BinaryFun>
    struct binder1st:public unary_function<typename BinaryFun::second_argument_type,typename BinaryFun::result_type>{
        binder1st(BinaryFun f,typename BinaryFun::first_argument_type x):fun(f),arg1(x){ }
        typename BinaryFun::result_type operator()(typename BinaryFun::second_argument_type arg2){
            return fun(arg1,arg2);
        }

    private:
        BinaryFun fun;
        typename BinaryFun::first_argument_type arg1;
    };

    template<typename BinaryFun>
    binder1st<BinaryFun> bind1st(BinaryFun fun,typename BinaryFun::first_argument_type x){
        return binder1st<BinaryFun>(fun,x);
    }

    template<typename BinaryFun>
    struct binder2st:public unary_function<typename BinaryFun::first_argument_type,typename BinaryFun::result_type>{
        binder2st(BinaryFun f,typename BinaryFun::second_argument_type x):fun(f),arg2(x){}
        typename BinaryFun::result_type operator()(typename BinaryFun::first_argument_type arg1){
            return fun(arg1,arg2);
        }

    private:
        BinaryFun fun;
        typename BinaryFun::second_argument_type arg2;
    };

    template<typename BinaryFun>
    binder2st<BinaryFun> bind2nd(BinaryFun fun,typename BinaryFun::second_argument_type x){
        return binder2st<BinaryFun>(fun,x);
    }

    template<typename Arg,typename Result>
    struct pointer_to_unary_function:public unary_function<Arg,Result>{
        pointer_to_unary_function(Result (*f)(Arg)):fun(f){ }
        Result operator()(Arg x){
            return (*fun)(x);
        }

    private:
        Result (*fun)(Arg);
    };


    template<typename Arg,typename Result>
    pointer_to_unary_function<Arg,Result> ptr_fun(Result (*f)(Arg)){
        return pointer_to_unary_function<Arg,Result>(f);
    }

    template<typename Arg1,typename Arg2,typename Result>
    struct pointer_to_binary_function:public binary_function<Arg1,Arg2,Result>{
        pointer_to_binary_function(Result (*f)(Arg1,Arg2)):fun(f){ }
        Result operator()(Arg1 x,Arg2 y){
            return (*fun)(x,y);
        }

    private:
        Result (*fun)(Arg1,Arg2);
    };

    template<typename Arg1,typename Arg2,typename Result>
    pointer_to_binary_function<Arg1,Arg2,Result> ptr_fun(Result (*f)(Arg1,Arg2)){
        return pointer_to_binary_function<Arg1,Arg2,Result>(f);
    }


    template<typename Predicate>
    struct unary_negate:unary_function<typename Predicate::argument_type,bool>{
        unary_negate(Predicate const&p):predicate(p){ }
        bool operator()(typename Predicate::argument_type x){
            return !predicate(x);
        }

    private:
        Predicate predicate;
    };
    template<typename Predicate>
    unary_negate<Predicate> not1(Predicate const&p){
        return unary_negate<Predicate>(p);
    }

    template<typename Predicate>
    struct binary_negate:binary_function<typename Predicate::first_argument_type,typename Predicate::second_argument_type,bool>{
        binary_negate(Predicate const&p):predicate(p){ }
        bool operator()(typename Predicate::first_argument_type x,typename Predicate::second_argument_type y){
            return !predicate(x,y);
        }

    private:
        Predicate predicate;
    };

    template<typename Predicate>
    binary_negate<Predicate> not2(Predicate const&p){
        return binary_negate<Predicate>(p);
    }


    template<typename Function1,typename Function2>
    struct unary_compose:public unary_function<typename Function2::argument_type,typename Function1::result_type>{
        unary_compose(Function1 const&f1,Function2 const&f2):f(f1),g(f2){}
        typename Function1::result_type operator()(typename Function2::argument_type x){
            return f(g(x));
        }
    private:
        Function1 f;
        Function2 g;
    };

    template<typename Function1,typename Function2>
    unary_compose<Function1,Function2> compose1(Function1 const&f,Function2 const&g){
        return unary_compose<Function1,Function2>(f,g);
    }

    template<typename BinaryFunction,typename UnaryFunction1,typename UnaryFunction2>
    struct binary_compose:public unary_function<typename UnaryFunction1::argument_type,typename BinaryFunction::result_type>{
        binary_compose(BinaryFunction const&f,UnaryFunction1 const&g1,UnaryFunction2 const&g2):__f(f),__g1(g1),__g2(g2){ }
        typename BinaryFunction::result_type operator()(typename UnaryFunction1::argument_type x){
            return __f(__g1(x),__g2(x));
        }
    private:
        BinaryFunction __f;
        UnaryFunction1 __g1;
        UnaryFunction2 __g2;
    };
    template<typename BinaryFunction,typename UnaryFunction1,typename UnaryFunction2>
    binary_compose<BinaryFunction,UnaryFunction1,UnaryFunction2> 
    compose2(BinaryFunction const&f, UnaryFunction1 const&g1, UnaryFunction2 const&g2){
        return binary_compose<BinaryFunction,UnaryFunction1,UnaryFunction2>(f,g1,g2); 
    }

}

#endif

