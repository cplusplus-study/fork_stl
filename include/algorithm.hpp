
#ifndef __FROK_STL_ALGORITHM__
#define __FROK_STL_ALGORITHM__
#include <iterator.hpp>
#include <utility.hpp>
#include <functional.hpp>

namespace xusd {
    //数值算法
    template<typename InputIterator,typename T>
    T accumulate(InputIterator first,InputIterator last,T init){
        for(;first!=last;++first){
            init = init + *first;
        }
        return init;
    }

    template<typename InputIterator,typename T,typename BinaryFunction>
    T accumulate(InputIterator first,InputIterator last,T init,BinaryFunction binary_op){
        for(;first!=last;++first){
            init = binary_op(init,*first);
        }
        return init;
    }

    template<typename InputIterator1,typename InputIterator2,typename T>
    T inner_product(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,T init){
        for(;first1 != last1;++first1,++first2){
            init = init + (*first1) * (*first2);
        }
        return init;
    }

    template<typename InputIterator1,typename InputIterator2,typename T,
        typename BinaryFunction1,typename BinaryFunction2>
    T inner_product(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,T init,
            BinaryFunction1 binary_op1,BinaryFunction2 binary_op2){
        for(;first1 != last1;++first1,++first2){
            init = binary_op1(init , binary_op2((*first1) , (*first2)));
        }
        return init;
    }

    //  y0 = x0 
    //  y1 = x0 + x1 
    //  y2 = x0 + x1 + x2 
    //  y3 = x0 + x1 + x2 + x3 
    //  y4 = x0 + x1 + x2 + x3 + x4 
    template<typename InputIterator,typename OutputIterator>
    OutputIterator partial_sum(InputIterator first,InputIterator last,OutputIterator result){
        if(first == last) return result;
        typedef typename iterator_traits<InputIterator>::value_type value_type;
        value_type sum = *first;
        *result=sum;
        while(++first != last){ 
            sum = sum + *first;
            *++result = sum;
        }
        return result;
    }

    template<typename InputIterator,typename OutputIterator,typename BinaryFunction>
    OutputIterator partial_sum(InputIterator first,InputIterator last,OutputIterator result,BinaryFunction binary_op){
        if(first == last) return result;
        typedef typename iterator_traits<InputIterator>::value_type value_type;
        value_type sum = *first;
        *result=sum;
        while(++first != last){ 
            sum = binary_op(sum,*first);
            *++result = sum;
        }
        return result;
    }

    //  y0 = x0 
    //  y1 = x1 - x0 
    //  y2 = x2 - x1 
    //  y3 = x3 - x2 
    //  y4 = x4 - x3 
    template<typename InputIterator,typename OutputIterator>
    OutputIterator adjacent_difference(InputIterator first,InputIterator last,OutputIterator result){
        if(first == last) return result;
        typedef typename iterator_traits<InputIterator>::value_type value_type;
        value_type prev = *first++;
        *result++ = prev;
        while(first != last){
            value_type value = *first++;
            *result++ = (value - prev );
            prev = value;
        }
        return result;

    }

    template<typename InputIterator,typename OutputIterator,typename BinaryFunction>
    OutputIterator adjacent_difference(InputIterator first,InputIterator last,OutputIterator result,BinaryFunction binary_op){
        if(first == last) return result;
        typedef typename iterator_traits<InputIterator>::value_type value_type;
        value_type prev = *first++;
        *result++ = prev;
        while(first != last){
            value_type value = *first++;
            *result++ = binary_op(value , prev );
            prev = value;
        }
        return result;

    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // 以下开始,都是不会改变容器内容的算法
    //

    template<typename T>
    inline const T& max(T const& a,T const& b){
        return a < b ? b : a;
    }

    template<typename T,typename Compare>
    inline const T& max(T const& a,T const& b, Compare comp){
        return comp(a, b) ? b : a;
    }

    template<typename T>
    inline const T& min(T const& a,T const& b){
        return b < a ? b : a;
    }

    template<typename T,typename Compare>
    inline const T& min(T const& a,T const& b, Compare comp){
        return comp(b, a) ? b : a;
    }


	template<typename InputIterator, typename UnaryFunction>
	UnaryFunction for_each(InputIterator first,InputIterator last,UnaryFunction f)
	{
		for(;first != last; ++first){
			f(*first);
		}		
		return f;
	}

	template<typename InputIterator,typename T>
	InputIterator find(InputIterator first, InputIterator last, T const& value){
		for(;(first != last) && (*first != value); ++first);
		
		return first;
	}

	template<typename InputIterator,typename Predicate>
	InputIterator find_if(InputIterator first, InputIterator last, Predicate pred){
		for(;(first != last) && (!perd(*first)); ++first);
		
		return first;
	}

    template<typename InputIterator1,typename InputIterator2>
    InputIterator1 find_first_of(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2){
        for(;first1!=last1;++first1){
            for(InputIterator2 i = first2; i!=last2; ++i){
                if(*i == *first1){ return first1; }
            }
        }

        return last1;
    }

    template<typename InputIterator1,typename InputIterator2,typename Predicate>
    InputIterator1 find_first_of(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,InputIterator2 last2,Predicate comp){
        for(;first1!=last1;++first1){
            for(InputIterator2 i = first2; i!=last2; ++i){
                if(comp(*i,*first1)){ return first1; }
            }
        }

        return last1;
    }

	template<typename ForwardIterator>
	ForwardIterator adjacent_find(ForwardIterator first,ForwardIterator last){
        if(first == last){ return last; }
        ForwardIterator next = first;
        while((++next) != last){
            if(*first == *next){ return first; }
            first = next;
        }
        return last;
	}
	
	template<typename ForwardIterator,typename Predicate>
	ForwardIterator adjacent_find(ForwardIterator first,ForwardIterator last,Predicate pred){
        if(first == last){ return last; }
        ForwardIterator next = first;
        while((++next) != last){
            if(pred(*first, *next)){ return first; }
            first = next;
        }
        return last;
	}


    template<typename ForwardIterator1,typename ForwardIterator2>
    ForwardIterator1 search(ForwardIterator1 first1,ForwardIterator1 last1, ForwardIterator2 first2,ForwardIterator2 last2){
        typedef typename iterator_traits<ForwardIterator1>::difference_type diff1;
        typedef typename iterator_traits<ForwardIterator2>::difference_type diff2;
        diff1 d1 = distance(first1,last1);
        diff2 d2 = distance(first2,last2);
        if(d1 < d2) return last1;
        ForwardIterator1 end = first1;
        advance(end,d1-d2+1);

        for(;first1 != end; ++first1){
            bool flag = true;
            ForwardIterator1 cur = first1;
            for(ForwardIterator2 i = first2; i!=last2;++i,++cur){
                if(*i != *cur){
                    flag = false;
                    break;
                }
            }

            if(flag) return first1;
        }
        return last1;
    }

    template<typename ForwardIterator1,typename ForwardIterator2,typename Predicate>
    ForwardIterator1 search(
            ForwardIterator1 first1,
            ForwardIterator1 last1, 
            ForwardIterator2 first2,
            ForwardIterator2 last2,
            Predicate pred)
    {
        typedef typename iterator_traits<ForwardIterator1>::difference_type diff1;
        typedef typename iterator_traits<ForwardIterator2>::difference_type diff2;
        diff1 d1 = distance(first1,last1);
        diff2 d2 = distance(first2,last2);
        if(d1 < d2) return last1;
        ForwardIterator1 end = first1;
        advance(end,d1-d2+1);

        for(;first1 != end; ++first1){
            bool flag = true;
            ForwardIterator1 cur = first1;
            for(ForwardIterator2 i = first2; i!=last2;++i){
                if(!pred(*i,*cur)){
                    flag = false;
                    break;
                }
            }

            if(flag) return first1;
        }
        return last1;
    }

    template<typename ForwardIterator1,typename ForwardIterator2>
    ForwardIterator1 /*find_end*/search_end(
            ForwardIterator1 first1,
            ForwardIterator1 last1, 
            ForwardIterator2 first2,
            ForwardIterator2 last2){
        typedef typename iterator_traits<ForwardIterator1>::difference_type diff1;
        typedef typename iterator_traits<ForwardIterator2>::difference_type diff2;
        diff1 d1 = distance(first1,last1);
        diff2 d2 = distance(first2,last2);
        if(d1<d2){ return last1; }
        ForwardIterator1 end = first1;
        advance(end,d1-d2+1);
        ForwardIterator1 ret = last1;
        for(;first1 != end;++first1){
            ForwardIterator1 cur = first1;
            bool flag = true;
            for(ForwardIterator2 i = first2; i!=last2;++i,++cur){
                if(*cur != *i){
                    flag = false;
                    break;
                }
            }
            if(flag) ret = first1;
        }
        
        return ret;
    }

    template<typename ForwardIterator1,typename ForwardIterator2>
    ForwardIterator1 find_end (
            ForwardIterator1 first1,
            ForwardIterator1 last1, 
            ForwardIterator2 first2,
            ForwardIterator2 last2){
        return search_end(first1,last1,first2,last2);
    }
    

    template<typename ForwardIterator1,typename ForwardIterator2,typename Predicate>
    ForwardIterator1 /*find_end*/search_end(
            ForwardIterator1 first1,
            ForwardIterator1 last1, 
            ForwardIterator2 first2,
            ForwardIterator2 last2,
            Predicate pred){
        typedef typename iterator_traits<ForwardIterator1>::difference_type diff1;
        typedef typename iterator_traits<ForwardIterator2>::difference_type diff2;
        diff1 d1 = distance(first1,last1);
        diff2 d2 = distance(first2,last2);
        if(d1<d2){ return last1; }
        ForwardIterator1 end = first1;
        advance(end,d1-d2+1);
        ForwardIterator1 ret = last1;
        for(;first1 != end;++first1){
            ForwardIterator1 cur = first1;
            bool flag = true;
            for(ForwardIterator2 i = first2; i!=last2;++i,++cur){
                if(!pred(*cur,*i)){
                    flag = false;
                    break;
                }
            }
            if(flag) ret = first1;
        }
        
        return ret;
    }

    template<typename ForwardIterator1,typename ForwardIterator2,typename Predicate>
    ForwardIterator1 find_end(
            ForwardIterator1 first1,
            ForwardIterator1 last1, 
            ForwardIterator2 first2,
            ForwardIterator2 last2,
            Predicate pred){
        return search_end(first1,last1,first2,last2,pred);
    }

    template<typename ForwardIterator,typename Integer,typename T>
    ForwardIterator search_n(ForwardIterator first,ForwardIterator last,Integer count,T const&value){
        if(count==0) return first;

        typedef typename iterator_traits<ForwardIterator>::difference_type diff;
        diff d = distance(first,last);
        if(d < count) return last;
        ForwardIterator end = first;
        advance(end,d-count+1);

        for(;first != end; ++first){
            bool flag = true;
            ForwardIterator i = first;
            for(Integer n = count; n>0;--n,++i){
                if(!(*i == value)){
                    flag = false;
                    break;
                }
            }

            if(flag) return first;
        }
        return last;
    }
    template<typename ForwardIterator,typename Integer,typename T,typename BinaryPredicate>
    ForwardIterator search_n(ForwardIterator first,ForwardIterator last,Integer count,T const&value,BinaryPredicate binary_op){
        if(count==0) return first;

        typedef typename iterator_traits<ForwardIterator>::difference_type diff;
        diff d = distance(first,last);
        if(d < count) return last;
        ForwardIterator end = first;
        advance(end,d-count+1);

        for(;first != end; ++first){
            bool flag = true;
            ForwardIterator i = first;
            for(Integer n = count; n>0;--n,++i){
                if(!binary_op(*i,value)){
                    flag = false;
                    break;
                }
            }

            if(flag) return first;
        }
        return last;
    }

	template<typename ForwardIterator,typename T,typename Size>
	void count(ForwardIterator first,ForwardIterator last,T const& value,Size& n){
        for(;first != last; ++first){ if(*first == value){ ++n; } }
	}

	template<typename ForwardIterator,typename T>
	typename iterator_traits<ForwardIterator>::difference_type 
    count(ForwardIterator first,ForwardIterator last,T const& value){
        typename iterator_traits<ForwardIterator>::difference_type  ret = 0;
        for(;first != last; ++first){ if(*first == value){ ++ret; } }
        return ret;
	}
	
	template<typename ForwardIterator,typename Predicate,typename Size>
	void count_if(ForwardIterator first,ForwardIterator last,Predicate pred,Size& n){
        for(;first != last; ++first){ if(perd(*first)){ ++n; } }
	}

	template<typename ForwardIterator,typename Predicate>
	typename iterator_traits<ForwardIterator>::difference_type 
	count_if(ForwardIterator first,ForwardIterator last,Predicate pred){
        typename iterator_traits<ForwardIterator>::difference_type  ret = 0;
        for(;first != last; ++first){ if(perd(*first)){ ++ret; } }
        return ret;
	}

    //template<typename InputIterator1,typename InputIterator2>
    //bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2){
    //    for(;first1 != last1; ++first1,++first2){
    //        if(*first1 != *first2) return false;
    //    }
    //    return true;
    //}
    //template<typename InputIterator1,typename InputIterator2,typename BinaryPredicate>
    //bool equal(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,BinaryPredicate pred){
    //    for(;first1 != last1; ++first1,++first2){
    //        if(!pred(*first1 , *first2)) return false;
    //    }
    //    return true;
    //}

	template<typename InputIterator1,typename InputIterator2>
	pair<InputIterator1,InputIterator2> mismatch(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2){
        for(;first1 != last1; ++first1,++first2){
            if(*first1 != *first2) break;
        }
        return pair<InputIterator1,InputIterator2>(first1,first2);
	}
	template<typename InputIterator1,typename InputIterator2,typename BinaryPredicate>
	pair<InputIterator1,InputIterator2> 
	mismatch(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,BinaryPredicate binPred){
        for(;first1 != last1; ++first1,++first2){
            if(!binPred(*first1 , *first2)) break;
        }
        return pair<InputIterator1,InputIterator2>(first1,first2);
	}


    template<typename ForwardIterator>
    ForwardIterator min_element(ForwardIterator first, ForwardIterator last){
        ForwardIterator cur = first;
        for(++first;first != last; ++first){
            if(*first < *cur){ cur = first; }
        }

        return cur;
    }

    template<typename ForwardIterator,typename BinaryPredicate>
    ForwardIterator min_element(ForwardIterator first, ForwardIterator last,BinaryPredicate pred){
        ForwardIterator cur = first;
        for(++first;first != last; ++first){
            if(pred(*first , *cur)){ cur = first; }
        }

        return cur;
    }

    template<typename ForwardIterator>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last){
        ForwardIterator cur = first;
        for(++first;first != last; ++first){
            if(*cur < *first){ cur = first; }
        }

        return cur;
    }

    template<typename ForwardIterator,typename BinaryPredicate>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last,BinaryPredicate pred){
        ForwardIterator cur = first;
        for(++first;first != last; ++first){
            if(pred(*cur,*first)){ cur = first; }
        }

        return cur;
    }

    template<typename InputIterator1,typename InputIterator2>
    inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
        for(;first1 != last1; ++first1,++first2){
            if(*first1 != *first2){
                return false;
            }
        }
        return true;
    }

    template<typename InputIterator1,typename InputIterator2,typename BinaryPredicate>
    inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryPredicate binpre){
        for(;first1 != last1; ++first1,++first2){
            if(!binpre(*first1, *first2)){
                return false;
            }
        }
        return true;
    }

    template<typename InputIterator1,typename InputIterator2>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2){
        for(;first1 != last1 && first2 != last2; ++first1,++first2){
            if(*first1 < *first2){ return true; }
            if(*first2 < *first1){ return false; }
        }
        return ((first1 == last1) && (first2 != last2));
    }

    template<typename InputIterator1,typename InputIterator2,typename Compare>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2,Compare comp)
    {
        for(;first1 != last1 && first2 != last2; ++first1,++first2){
            if(comp(*first1,*first2)){ return true; }
            if(comp(*first2,*first1)){ return false; }
        }
        return ((first1 == last1) && (first2 != last2));
    }



    //////////////////////////////////////////////////////////////////////////////////////////
    // 以下开始,都是可能会改变容器内容的算法


    
    template<typename InputIterator,typename OutputIterator>
    OutputIterator copy(InputIterator first,InputIterator last,OutputIterator result){
        for(; first != last; ++first,++result){
            *result = *first;
        }
        return result;
    }
    template<typename InputIterator,typename OutputIterator,typename Predicate>
    OutputIterator copy(InputIterator first,InputIterator last,OutputIterator result,Predicate pred){
        for(; first != last; ++first,++result){
            if(pred(*first)){
                *result = *first;
            }
        }
        return result;
    }

    template<typename BidirectionalIterator1,typename BidirectionalIterator2>
    BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,BidirectionalIterator1 last,BidirectionalIterator2 result){
        while(last != first){
            *--result = *--last;
        }
        return result;
    }

    template <class T> void swap(T& a, T& b){
        T c(a); a=b; b=c;
    }

    template<typename ForwardIterator>
    inline void iter_swap(ForwardIterator a,ForwardIterator b){
        swap(*a,*b);
    }


    template<typename ForwardIterator1,typename ForwardIterator2>
    ForwardIterator2 swap_ranges(ForwardIterator1 first1,ForwardIterator1 last1,ForwardIterator2 first2){
        for(;first1 != last1; ++first1,++first2){
            swap(*first1,*first2);
        }
        return first2;
    }


    template<typename InputIterator,typename OutputIterator,typename UnaryFunction>
    OutputIterator transform(InputIterator first,InputIterator last,OutputIterator result,UnaryFunction op){
        for(;first!=last;++first,++result){
            *result = op(*first);
        }
        return result;
    }

    template<typename InputIterator1,typename InputIterator2,typename OutputIterator,typename BinaryFunction>
    OutputIterator transform(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,OutputIterator result,BinaryFunction op){
        for(;first1!=last1;++first1,++first2,++result){
            *result = op(*first1,*first2);
        }
        return result;
    }

    template<typename ForwardIterator,typename T>
    void replace(ForwardIterator first,ForwardIterator last,T const& old_value, T const& new_value){
        for(;first!=last; ++first){
            if(*first == old_value){
                *first = new_value;
            }
        }
    }

    template<typename ForwardIterator,typename T,typename Predicate>
    void replace_if(ForwardIterator first,ForwardIterator last,Predicate pred, T const& new_value){
        for(;first!=last; ++first){
            if(pred(*first)){
                *first = new_value;
            }
        }
    }

    template<typename InputIterator,typename OutputIterator,typename T>
    OutputIterator replace_copy(InputIterator first,InputIterator last,OutputIterator result,T const& old_value, T const& new_value){
        for(;first!=last; ++first,++result){
            if(*first == old_value){
                *result = new_value;
            }else{
                *result = *first;
            }
        }
        return result;
    }

    template<typename InputIterator,typename OutputIterator,typename T,typename Predicate>
    OutputIterator replace_copy_if(InputIterator first,InputIterator last,OutputIterator result,Predicate pred, T const& new_value){
        for(;first!=last; ++first,++result){
            if(pred(*first)){
                *result = new_value;
            }else{
                *result = *first;
            }
        }
        return result;
    }

    template<typename ForwardIterator,typename T>
    void fill(ForwardIterator first,ForwardIterator last, T const& value){
        for(;first != last; ++first){
            *first = value;
        }
    }

    template<typename OutputIterator,typename Size,typename T>
    OutputIterator fill_n(OutputIterator first,Size n, T const& value){
        for(;n>0; --n,++first){
            *first = value;
        }
        return first;
    }



    template<typename ForwardIterator,typename Generator>
    void generate(ForwardIterator first, ForwardIterator last,Generator gen){
        for(;first!=last;++first){
            *first = gen();
        }
    }

    template<typename OutputIterator,typename Size,typename Generator>
    void generate_n(OutputIterator result, Size n, Generator gen){
        for(;n>0;--n,++result){
            *result = gen();
        }
        return result;
    }


    template<typename ForwardIterator,typename T>
    ForwardIterator remove(ForwardIterator first,ForwardIterator last,T const&value){
        ForwardIterator cur = first ;
        for(;first!=last;++first){
            if(*first != value){
                if(first != cur){
                    *cur = *first;
                }
                ++cur;
            }
        }
        return cur;
    }

    template<typename ForwardIterator,typename Predicate>
    ForwardIterator remove_if(ForwardIterator first,ForwardIterator last,Predicate pred){
        ForwardIterator cur = first ;
        for(;first!=last;++first){
            if(!pred(*first)){
                if(first != cur){
                    *cur = *first;
                }
                ++cur;
            }
        }

        return cur;

    }

    template<typename ForwardIterator,typename OutputIterator,typename T>
    ForwardIterator remove_copy(ForwardIterator first,ForwardIterator last,OutputIterator result,T const&value){
        for(;first!=last;++first){
            if(*first != value){
                *result++ = *first;
            }
        }
        return result;
    }

    template<typename ForwardIterator,typename OutputIterator,typename Predicate>
    ForwardIterator remove_copy_if(ForwardIterator first,ForwardIterator last,OutputIterator result,Predicate pred){
        for(;first!=last;++first){
            if(!pred(*first)){
                *result++ = *first;
            }
        }
        return result;
    }

    ////////////////////////////////////////////////////////////////////////
    // 以下都未完成
    template<typename ForwardIterator>
    ForwardIterator unique(ForwardIterator first,ForwardIterator last){
        if(first == last) return last;

        ForwardIterator next = first;
        while(++next != last){
            if((!(*first==*next)) && (++first != next)){
                *first = *next;
            }
        }
        return ++first;
    }

    template<typename ForwardIterator,typename BinaryPredicate>
    ForwardIterator unique(ForwardIterator first,ForwardIterator last,BinaryPredicate pred){
        if(first == last) return last;

        ForwardIterator next = first;
        while(++next != last){
            if((!pred(*first,*next)) && (++first != next)){
                *first = *next;
            }
        }
        return ++first;
    }

    template<typename ForwardIterator,typename OutputIterator>
    OutputIterator unique_copy(ForwardIterator first,ForwardIterator last,OutputIterator result){
        if(first == last) return result;
        ForwardIterator cur = first;
        *result++ = *first;
        while(++first != last){
            if(*cur!=*first){
                cur = first;//指向当前要比较的值
                *result++ = *first;
            }
        }
        return result;

    }

    template<typename ForwardIterator,typename OutputIterator,typename BinaryPredicate>
    OutputIterator unique_copy(ForwardIterator first,ForwardIterator last,OutputIterator result,BinaryPredicate pred){
        if(first == last) return result;
        ForwardIterator cur = first;
        *result++ = *first;
        while(++first != last){
            if(!pred(*cur,*first)){
                cur = first;//指向当前要比较的值
                *result++ = *first;
            }
        }
        return result;
    }


    template<typename BidircetionalIterator>
    void reverse(BidircetionalIterator first,BidircetionalIterator last){
        typedef typename iterator_traits<BidircetionalIterator>::difference_type diff;
        diff d = xusd::distance(first,last);
        BidircetionalIterator end = first;
        xusd::advance(end,d/2);
        xusd::swap_ranges(first,end,reverse_iterator<BidircetionalIterator>(last));
    }

    template<typename BidircetionalIterator,typename OutputIterator>
    OutputIterator reverse_copy(BidircetionalIterator first,BidircetionalIterator last,OutputIterator result){
        reverse_iterator<BidircetionalIterator> rbegin(last);
        reverse_iterator<BidircetionalIterator> rend(first);
        return copy(rbegin,rend,result);
    }


    //_rotate结构体是为rotate函数做特化用的
    template<typename T>
    struct _rotate;

    template<>
    struct _rotate<forward_iterator_tag>{
        template<typename ForwardIterator>
        static void do_rotate(ForwardIterator first,ForwardIterator middle,ForwardIterator last){
            if(first == middle || middle == last) return ;
            ForwardIterator i = middle;
            while(true){
                iter_swap(first++,i++);
                if(first == middle){//前面比较短
                    if(i == last){//同时交换完成
                        return ;
                    }else{
                        middle = i;
                        //相当于递归调用do_rotate(first,i,last);
                    }
                }else{//后面一段比较长
                    i = middle;
                    //相当于递归调用do_rotate(first,middle,last);
                }
            }

        }
    };

    template<>
    struct _rotate<bidirectional_iterator_tag>{
        template<typename BidirectionalIterator>
        static void do_rotate(BidirectionalIterator first,BidirectionalIterator middle,BidirectionalIterator last){
            reverse(first,middle);
            reverse(middle,last);
            reverse(first,last);
        }
    };

    template<>
    struct _rotate<random_access_iterator_tag>{
        template<typename RandomAccessIterator>
        static void do_rotate(RandomAccessIterator first,RandomAccessIterator middle,RandomAccessIterator last){
            //暂时用该方法,等想出更好的方法时替换之
            reverse(first,middle);
            reverse(middle,last);
            reverse(first,last);
        }
    };

    template<typename ForwardIterator>
    void rotate(ForwardIterator first,ForwardIterator middle,ForwardIterator last){
        _rotate<typename iterator_traits<ForwardIterator>::iterator_category>::do_rotate(first,middle,last);
    }
    template<typename ForwardIterator,typename OutputIterator>
    OutputIterator rotate(ForwardIterator first,ForwardIterator middle,ForwardIterator last,OutputIterator result){
        return copy(first,middle,copy(middle,last,result));
    }

    template<typename BidirectionalIterator>
    bool prev_premutation(BidirectionalIterator first,BidirectionalIterator last){
        //暂未实现
        return false;
    }
    template<typename BidirectionalIterator>
    bool next_premutation(BidirectionalIterator first,BidirectionalIterator last){
        //暂未实现
        return false;
    }


    template<typename BidirectionalIterator,typename Predicate>
    BidirectionalIterator partition(BidirectionalIterator first,BidirectionalIterator last, Predicate pred){
        while(first != last){
            while(pred(*first)) ++first;//停下来的时候,不满足pred(*first)
            if(first == last) break;//first和last同时指向不满足pred的元素说明完成了
            while(!pred(*last)) --last;//停下来的时候,满足pred(*first)
            iter_swap(first,last);//交换后first指向满足pred判断的元素,last指向不满足pred的元素
        }
        return last;
    }

    template<typename BidirectionalIterator,typename Predicate>
    BidirectionalIterator stable_partition(BidirectionalIterator first,BidirectionalIterator last, Predicate pred){
        //暂未实现
    }

    template<typename RandomAccessIterator>
    void random_shuffle(RandomAccessIterator first,RandomAccessIterator last){
        //暂未实现
    }

    template<typename RandomAccessIterator,typename RandomNumberGenerator>
    void random_shuffle(RandomAccessIterator first,RandomAccessIterator last, RandomNumberGenerator &rand){
        //暂未实现                                                            
    }

    template<typename InputIterator,typename RandomAccessIterator>
    void random_sample(InputIterator first,InputIterator last,RandomAccessIterator ofirst,RandomAccessIterator olast){
        //暂未实现
    }

    template<typename InputIterator,typename RandomAccessIterator,typename RandomNumberGenerator>
    void random_sample(InputIterator first,InputIterator last,RandomAccessIterator ofirst,RandomAccessIterator olast,RandomNumberGenerator &rand){
        //暂未实现
    }

    template<typename ForwardIterator, typename OutputIterator,typename Distance>
    OutputIterator random_sample_n(ForwardIterator first,ForwardIterator last,OutputIterator result,Distance n){
        //暂未实现
        return result;
    }

    template<typename ForwardIterator, typename OutputIterator,typename Distance,typename RandomNumberGenerator>
    OutputIterator random_sample_n(ForwardIterator first,ForwardIterator last,OutputIterator result,Distance n,RandomNumberGenerator &rand){
        //暂未实现
        return result;
    }



}



#endif

