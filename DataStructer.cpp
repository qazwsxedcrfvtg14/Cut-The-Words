#include<bits/stdc++.h>
#include<string>
using namespace std;
typedef struct Treap_Node* Treap_Ptr;
//typedef struct Trie_Node* Trie_Ptr;
struct Treap_Node{
    wchar_t v;
    void* data;
    unsigned int sz;
    Treap_Ptr l,r,fa;
    Treap_Node(wchar_t _v,void * _data){v=_v,sz=1,l=r=fa=NULL;data=_data;}
    inline void push(){fa=NULL;}
    inline void pull(){sz=(l?l->sz:0)+(r?r->sz:0);if(l)l->fa=this;if(r)r->fa=this;}
    Treap_Ptr top(){
        Treap_Ptr now=this;
        if(fa)now=fa->top();
        return now;
        }
    Treap_Ptr left(){
        if(l)return l->left();
        return this;
        }
    Treap_Ptr right(){
        if(r)return r->right();
        return this;
        }
    Treap_Ptr next(){
        if(r)return r->left();
        Treap_Ptr now=this;
        while(now->fa)
            if(now->fa->l==now)return now->fa;
            else now=now->fa;
        return NULL;
        }
    Treap_Ptr pre(){
        if(l)return l->right();
        Treap_Ptr now=this;
        while(now->fa)
            if(now->fa->r==now)return now->fa;
            else now=now->fa;
        return NULL;
        }
    };
Treap_Ptr merge(Treap_Ptr a,Treap_Ptr b){
    static unsigned int S=rand();
    if(!a)return b;if(!b)return a;
    if((S++)%(a->sz+b->sz)<a->sz){a->push(),a->r=merge(a->r,b),a->pull();return a;}
    else{b->push(),b->l=merge(a,b->l),b->pull();return b;}
    }
void split(Treap_Ptr now,wchar_t k,Treap_Ptr &a,Treap_Ptr &b){
    if(!now)a=b=NULL;
    else if(now->v<k)
        a=now,a->push(),split(now->r,k,a->r,b),a->pull();
    else
        b=now,b->push(),split(now->l,k,a,b->l),b->pull();
    }
struct Treap{
    Treap_Ptr root;
    Treap(){root=NULL;}
    void insert(wchar_t k,void* data=NULL){
        Treap_Ptr a=NULL,b=NULL;
        split(root,k,a,b);
        root=merge(merge(a,new Treap_Node(k,data)),b);
        }
    void erase(wchar_t k){
        Treap_Ptr a=NULL,b=NULL,c=NULL;
        split(root,k-1,a,b);
        split(b,k,b,c);
        delete b;
        root=merge(a,c);
        }
    Treap_Ptr kth(Treap_Ptr rot,unsigned int k){//0base
        if(k<rot->l->sz)return kth(rot->l,k);
        if(k==rot->l->sz)return rot;
        return kth(rot->r,k-rot->l->sz-1);
        }
    /*Treap_Ptr operator[](unsigned int k){
        if(k>=root->sz)return NULL;
        return kth(root,k);
        }*/
    Treap_Ptr operator[](wchar_t k){
        Treap_Ptr now=root,pri=NULL;
        while(now)
            if(k<=now->v)pri=now,now=now->l;
            else now=now->r;
        if(pri==NULL||pri->v!=k){
            insert(k);
            now=root,pri=NULL;
            while(now)
                if(k<=now->v)pri=now,now=now->l;
                else now=now->r;
            }
        return pri;
        }
    Treap_Ptr lower_bound(wchar_t k){
        Treap_Ptr now=root,pri=NULL;
        while(now)
            if(k<=now->v)pri=now,now=now->l;
            else now=now->r;
        return pri;
        }
    Treap_Ptr upper_bound(wchar_t k){
        Treap_Ptr now=root,pri=NULL;
        while(now)
            if(k<now->v)pri=now,now=now->r;
            else now=now->l;
        return pri;
        }
    };
template <typename T>
struct Trie_Node{
    bool real;
    T data;
    Treap kids;
    Trie_Node(){real=0;}
    Trie_Node(bool _r){real=_r;}
    Trie_Node(bool _r,T _data){real=_r;data=_data;}
    };

template <typename T>
struct Trie{
    Trie_Node<T> root;
    Trie_Node<T>& operator[](wstring s){
        Trie_Node<T>* now=&root;
        for(auto &x:s){
            Treap_Ptr tp=now->kids[x];
            if(!tp->data)tp->data=new Trie_Node<T>();
            now=(Trie_Node<T>*)tp->data;
            }
        now->real=1;
        return *now;
        }
    void erase(wstring s){
        Trie_Node<T>* now=&root;
        for(auto &x:s){
            Treap_Ptr tp=now->kids[x];
            if(!tp->data)return;
            now=(Trie_Node<T>*)tp->data;
            }
        now->real=0;
        now->data=T();
        }
    };

string w2s(wstring ws) {
	string s;
	for (auto &x : ws)s += (char)x;
	return s;
}
wstring s2w(string s) {
	wstring ws;
	for (auto x : s)ws += (wchar_t)x;
	return ws;
}
int main(){
    Trie<string> t;
    string s;
    string w;
    while(1){
        cin>>s>>w;
        if(s=="new"){
            cin>>s;
            t[s2w(w)].data=s;
            }
        else if(s=="find"){
            cout<<t[s2w(w)].data<<endl;
            }
        else if(s=="erase"){
            t.erase(s2w(w));
            }
        }
    }
