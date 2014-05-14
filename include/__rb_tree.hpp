// -*- C++ -*-

#ifndef __FORK_STL_RB_TREE__
#define __FORK_STL_RB_TREE__
#include <cstddef>
#include "memory.hpp"
#include "iterator.hpp"

namespace xusd{

typedef bool ColorType; //true:black,false:red
#define _rb_tree_red false
#define _rb_tree_black true

struct _rb_tree_node_base {
    typedef _rb_tree_node_base Self;
    typedef Self* base_ptr;
    ColorType color;
    Self* parent;
    Self* left;
    Self* right;
    static Self* minimum(Self * ptr){
        Self* x = ptr;
        while(x->left != NULL){
            x = x->left;
        }
        return x;
    }

    static Self* maximum(Self * ptr){
        Self* x = ptr;
        while(x->right != NULL){
            x = x->right;
        }
        return x;
    }
    
};

struct _rb_tree_base_iterator{
    typedef _rb_tree_node_base::base_ptr base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;

    base_ptr node;

    _rb_tree_base_iterator():node(NULL){}
    _rb_tree_base_iterator(base_ptr x):node(x){}

    //后继
    void increment(){
        if(node->right != NULL){
            node = _rb_tree_node_base::minimum(node->right);    
        }else{
            base_ptr y = node->parent;
            while(/*y != NULL &&*/ node == y->right){//添加一个header节点后,就不用判断(y != NULL)了
                node = y;
                y = y->parent;
            }
            if(node->right != y){//当当前节点为root的时候就会出现该问题
                node = y;
            }
        }
    }

    //前驱
    void decrement(){
        if(node->left != NULL){
            node = _rb_tree_node_base::maximum(node->left);
        }else{
            base_ptr y = node->parent;
            while(node == y->left){
                node = y;
                y = y->parent;
            }

            if(node->left != y){
                node = y;
            }
        }
    }
};



template <typename T>
struct _rb_tree_node:public _rb_tree_node_base{
    typedef _rb_tree_node<T>* link_type;
    T value;
    //添加traits信息
};

template <typename Value,typename Ref,typename Ptr>
struct _rb_tree_iterator:public _rb_tree_base_iterator{
    using _rb_tree_base_iterator::node;
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;

    typedef _rb_tree_iterator<value_type,value_type&,value_type*> iterator;
    typedef _rb_tree_iterator<value_type,value_type const&,value_type const*> const_iterator;
    typedef _rb_tree_iterator<Value,Ref,Ptr> Self;
    typedef _rb_tree_node<Value>* link_type;

    _rb_tree_iterator(){}
    _rb_tree_iterator(link_type x):_rb_tree_base_iterator(x){ }
    _rb_tree_iterator(Self const&it):_rb_tree_base_iterator(it.node){ }
    Self& operator()(Self const&it){
        node = it.node;
    }

    reference operator*()const{
        return static_cast<link_type>(node)->value;
    }

    pointer operator->()const{
        return &(operator*());
    }

    Self& operator++(){
        increment();
        return *this;
    }

    Self operator++(int){
        Self tmp(*this);
        increment();
        return tmp;
    }
    Self& operator--(){
        decrement();
        return *this;
    }

    Self operator--(int){
        Self tmp(*this);
        decrement();
        return tmp;
    }

};

//_rb_tree_node_base* successor(_rb_tree_node_base* x){
//    if(node->right != NULL){
//        node = _rb_tree_node_base::minimum(node->right);    
//    }else{
//        base_ptr y = node->parent;
//        while(/*y != NULL &&*/ node == y->right){//添加一个header节点后,就不用判断(y != NULL)了
//            node = y;
//            y = y->parent;
//        }
//        if(node->right != y){//当当前节点为root的时候就会出现该问题
//            node = y;
//        }
//    }
//}
//
//_rb_tree_node_base* predecessor(_rb_tree_node_base *x){
//    _rb_tree_node_base *y = NULL;
//    if(x->left != NULL){
//        y = _rb_tree_node_base::maximum(node->left);
//    }else{
//        y = x->parent;
//        while(x == y->left){
//            x = y;
//            y = y->parent;
//        }
//
//        if(x->left != y){
//        }
//    }
//    return y;
//}
//
void left_rotate(_rb_tree_node_base* x,_rb_tree_node_base*& root){
    _rb_tree_node_base* y = x->right;
    
    //连接x和y->left
    x->right = y->left; // _1
    if(x->right != NULL){
        x->right->parent = x; //_2
    }

    //连接x->parent 和 y
    y->parent = x->parent; //_3
    if(root == x){
        root = y; // _4
    }else if(x->parent->left == x){
        x->parent->left = y; // _4
    }else{
        x->parent->right = y; // _4
    }

    //连接x和y
    y->left = x;  // _5
    x->parent = y; // _6
}

void right_rotate(_rb_tree_node_base* x,_rb_tree_node_base*& root){
    _rb_tree_node_base* y = x->left;
    //连接x和y->right
    x->left = y->right; // _1
    if(x->left != NULL){
        x->left->parent = x; // _2
    }

    //连接x->parent 和 y 
    y->parent = x->parent;  // _3
    if(root == x){
        root = y;  // _4
    }else if(x->parent->left == x){
        x->parent->left = y; // _4
    }else{
        x->parent->right = y; // _4
    }

    //连接x y
    y->right = x; // _5
    x->parent = y; // _6
}


//1. 每个节点是红色或者黑色的
//2. 根节点是黑色的
//3. 每个叶子节点(这里表示为NULL)为黑色
//4. 若一个节点为红色则其子几点为黑色或者NULL
//5. 对每个节点,从该节点到其子孙节点的所有路径上包含相同的黑节点数目
//以上5点假设最后为NULL的节点为叶子节点,color(NULL) == black
//调整红黑树
bool is_black(_rb_tree_node_base *x){
    if(x == NULL || x->color){
        return true;
    }
    return false;
}
void reblance(_rb_tree_node_base* x,_rb_tree_node_base *root){
    x->color = _rb_tree_red;//先把当前节点设置为红色
    //当前节点设置为红色,不违反条款1
    //若x是根节点就不需要做调整,直接root->color = black,x不是根节点则不违反条款2
    //每个叶子节点都为黑色,这个重来都不会被违反
    //当x节点的父节点为红色的时候,会违反条款4
    //因x是相当于新添加的节点,x子树节点都是经过本函数调整过的,所以不会违反条款5
    //最终得出结论:只有当前节点为root或者其父节点为红色的时候才会违反以上条款,需进行调整
    while(x != root && x->parent->color == _rb_tree_red){//只有当前节点其父节点为红色的时候才会违反以上条款,需进行调整     
        //x不为root的时候其父节点是一定存在的
        //x->parent->color == red 也就决定了x->parent 不为root,则x->parent->parent 是存在的,且x->parent->parent为黑色
        if(x->parent == x->parent->parent->left){
            _rb_tree_node_base *y = x->parent->parent->right;//一定要注意y为NULL的情况
            if(!(y == NULL || y->color == _rb_tree_black)){//若其叔叔节点为红色
                //调整父节点和叔叔节点为红色,祖父节点为黑色
                //黑高度不变. 父,叔节点由红色变为黑色,祖父节点由黑色变为红色
                x->parent->color = _rb_tree_black;//父变为黑色 
                y->color = _rb_tree_black;//叔变为黑色
                x->parent->parent->color = _rb_tree_red;//祖父变为红色
                x = x->parent->parent;//把要调整的节点往上移,最后到x == root节点
            }else{ //当y为黑色的时候(y可能为NULL)
                if(x == x->parent->right){//x为右孩子,内插入型,先转换为外插入型然后在操作
                    x = x->parent;
                    left_rotate(x,root);//旋转父节点,祖父节点的左子树所有路径黑高度不变,但是x和x->parent同为红色,转换为以下情况进行调整
                }
                //开始外插入型操作
                x->parent->color = _rb_tree_black;//改变父节点为黑色,使x和x->parent不同为红色
                x->parent->parent->color = _rb_tree_red;//改变祖父节点为红色,则祖父左子树所有路径黑高度不变
                //但是祖父右子树的黑高度减一了
                //这就会导致必然的以下途径发生
                right_rotate(x->parent->parent,root);//右旋转祖父节点,使得旋转后x->parent
            }
        }else{
            _rb_tree_node_base *y = x->parent->parent->left;//一定要注意y为NULL的情况
            if(!(y == NULL || y->color == _rb_tree_black)){//若其叔叔节点为红色
                x->parent->color = _rb_tree_black;
                y->color = _rb_tree_black; 
                x->parent->parent->color = _rb_tree_red;
                x = x->parent->parent;
            }else{
                if(x == x->parent->left){
                    x = x->parent;
                    right_rotate(x,root);
                }
                x->parent->color = _rb_tree_black;
                x->parent->parent->color = _rb_tree_red;
                left_rotate(x->parent->parent,root);
            }
        }

    }
    root->color = _rb_tree_black;//根节点一定为黑色,对所有子树黑高度影响一样
}


template <typename Key,typename Value,typename KeyOfValue, typename Compare,typename Alloc = allocator<_rb_tree_node<Value> > >
class rb_tree{
protected:
    typedef void* void_pointer;
    typedef _rb_tree_node_base::base_ptr base_ptr;
    typedef _rb_tree_node<Value> rb_tree_node;
    typedef Alloc rb_tree_node_allocator;
    typedef ColorType color_type;
    typedef rb_tree<Key,Value,KeyOfValue,Compare,Alloc> Self;
public:
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef value_type const* const_pointer;
    typedef value_type& reference; 
    typedef value_type const& const_reference; 
    typedef rb_tree_node* link_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef _rb_tree_iterator<value_type,reference,pointer> iterator;
private:
    rb_tree_node_allocator allocator;
protected:
    link_type get_node(){//获取没有初始化的内存空间
        return allocator.allocate(1);
    }
    void put_node(link_type p){
        allocator.deallocate(p);
    }

    link_type create_node(value_type const& x){
        link_type tmp = get_node();
        try{
        construct(&tmp->value,x);
        }catch(...){
            put_node(tmp);
            throw;
        }
        return tmp;
    }

    void destroy_node(link_type p){
        destroy(&p->value);
        put_node(p);
    }

    link_type clone_node(link_type x){
        link_type tmp = create_node(x->value);
        tmp->color = x->color;
        tmp->left = x->left;
        tmp->right = x->right;
        return tmp;
    }
protected:
    size_type node_count;
    link_type header;
    Compare key_compare;

    link_type& root()const{ return static_cast<link_type>(header->parent); };
    link_type& leftmost()const{ return static_cast<link_type>(header->left); };
    link_type& rightmost()const{ return static_cast<link_type>(header->right); };

    static link_type& left(link_type x){ return static_cast<link_type>(x->left);}
    static link_type& right(link_type x){ return static_cast<link_type>(x->right);}
    static link_type& parent(link_type x){ return static_cast<link_type>(x->parent);}
    //static color_type& parent(link_type x){ return (x->color);}
    static reference value(link_type x){ return x->value;}
    static Key const& key(link_type x){ return KeyOfValue()(value(x)); }

    static link_type minimum(link_type x){ return static_cast<link_type>(_rb_tree_node_base::minimum(x)); }

    static link_type maximum(link_type x){ return static_cast<link_type>(_rb_tree_node_base::maximum(x)); }

private:
    void init(){
        header = get_node();
        color(header) = _rb_tree_red;
        root() = NULL;
        leftmost() = header;
        rightmost() = header;
    }

    void erase(base_ptr x){
        while(x != NULL){
            erase(x->right);
            base_ptr y = x->left;
            destroy_node(static_cast<link_type>(x));
            x = y;
        }
    }

public:
    rb_tree(Compare const& comp = Compare()):node_count(0),key_compare(comp){
        init();
    }
    rb_tree(rb_tree const& tree){
        init();
    }

    ~rb_tree(){
        //clear();
        put_node(header);
    }

//    Self& operator=(Self const&);

public:
    Compare key_comp() const { return key_compare; };
    iterator begin(){
        return leftmost();
    }
    iterator end(){
        return rightmost();
    }
    //const_iterator begin() const{
    //    return leftmost();
    //}
    //const_iterator end() const{
    //    return rightmost();
    //}

    bool empty()const{ return node_count == 0; }

    size_type size() const{ return node_count; }

    size_type max_size() const{ return allocator.max_size(); }


public:
    void clear(){
        base_ptr x = root();
        erase(x);
        root() = NULL;
        leftmost() = header;
        rightmost() = header;
    }

    pair<iterator,bool> insert_unique(const value_type&x);
    iterator insert_equal(const value_type&x);

private:
};


}

#endif

