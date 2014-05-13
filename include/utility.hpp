
#ifndef __FROK_STL_UTILITY__
#define __FROK_STL_UTILITY__


namespace xusd{

    template<typename T1, typename T2>
    class pair{
    public:
        typedef T1 first_type; 
        typedef T2 second_type;
        pair(T1 const&x, T2 const&y):first(x),second(y){ }
        pair(const pair<T1,T2>& p):first(p.first),second(p.second){}
        template<typename U1,typename U2>
        pair(const pair<U1,U2>& p):first(p.first),second(p.second){}
        pair<T1,T2>& operator=(pair<T1,T2>&p){
            if(&p == this) return *this;
            first = p.first;
            second = p.second;
            return *this;
        }

        bool operator==(pair<T1,T2> const&p)const{
            if ((first == p.first) && (second == p.second)){
                return true;
            }else{
                return false;
            }
        }

        bool operator<(pair<T1,T2> const&p)const{
            if ((first < p.first) || ((second == p.second) && (second < p.second))){
                return true;
            }else{
                return false;
            }
        }


    public:
        first_type first;
        second_type second;
    };

    template<typename T1, typename T2>
    pair<T1,T2> make_pair(T1 const&x, T2 const&y){
        return xusd::pair<T1,T2>(x,y);
    }

}

#endif

