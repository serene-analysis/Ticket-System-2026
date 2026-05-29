#pragma once
#include "utils.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::ios;

//#define DEBUG
//#define merge_DEBUG
//#define overall_DEBUG

template<typename Key, typename Value, int MAX_CAP = 256>
class LRUCache {
private:
    static const int HASH_SIZE = 1024;

    struct Node {
        Key key;
        Value value;
        int prev;
        int next;
        int next_hash;
    };

    Node nodes[MAX_CAP];
    int hash_head[HASH_SIZE];
    int lru_head, lru_tail;
    int free_head;
    int size;

    static int hash(Key k) {
        return k % HASH_SIZE;
    }

    void remove_from_lru(int idx) {
        int p = nodes[idx].prev;
        int n = nodes[idx].next;
        if (p != -1) nodes[p].next = n;
        else lru_head = n;
        if (n != -1) nodes[n].prev = p;
        else lru_tail = p;
    }

    void add_to_lru_head(int idx) {
        nodes[idx].prev = -1;
        nodes[idx].next = lru_head;
        if (lru_head != -1) nodes[lru_head].prev = idx;
        lru_head = idx;
        if (lru_tail == -1) lru_tail = idx;
    }

    void remove_from_hash(int idx) {
        int h = hash(nodes[idx].key);
        int prev = -1;
        int cur = hash_head[h];
        while (cur != -1) {
            if (cur == idx) {
                if (prev == -1) hash_head[h] = nodes[cur].next_hash;
                else nodes[prev].next_hash = nodes[cur].next_hash;
                break;
            }
            prev = cur;
            cur = nodes[cur].next_hash;
        }
    }

    void add_to_hash(int idx) {
        int h = hash(nodes[idx].key);
        nodes[idx].next_hash = hash_head[h];
        hash_head[h] = idx;
    }

    int allocate_node() {
        if (free_head == -1) {
            return -1;
        }
        int idx = free_head;
        free_head = nodes[idx].next;
        return idx;
    }

    void free_node(int idx) {
        nodes[idx].next = free_head;
        free_head = idx;
    }

public:
    LRUCache() {
        for (int i = 0; i < HASH_SIZE; ++i) hash_head[i] = -1;
        lru_head = lru_tail = -1;
        size = 0;
        free_head = 0;
        for (int i = 0; i < MAX_CAP - 1; ++i) {
            nodes[i].next = i + 1;
        }
        nodes[MAX_CAP - 1].next = -1;
    }

    bool get(Key key, Value& value) {
        int h = hash(key);
        int cur = hash_head[h];
        while (cur != -1) {
            if (nodes[cur].key == key) {
                remove_from_lru(cur);
                add_to_lru_head(cur);
                value = nodes[cur].value;
                return true;
            }
            cur = nodes[cur].next_hash;
        }
        return false;
    }

    void put(Key key, const Value& value) {
        int h = hash(key);
        int cur = hash_head[h];
        while (cur != -1) {
            if (nodes[cur].key == key) {
                nodes[cur].value = value;
                remove_from_lru(cur);
                add_to_lru_head(cur);
                return;
            }
            cur = nodes[cur].next_hash;
        }

        if (size >= MAX_CAP) {
            int old = lru_tail;
            if (old != -1) {
                remove_from_lru(old);
                remove_from_hash(old);
                free_node(old);
                --size;
            }
        }
        int idx = allocate_node();
        if (idx == -1) return;
        nodes[idx].key = key;
        nodes[idx].value = value;
        nodes[idx].prev = nodes[idx].next = -1;
        nodes[idx].next_hash = -1;
        add_to_hash(idx);
        add_to_lru_head(idx);
        ++size;
    }

    void remove(Key key) {
        int h = hash(key);
        int cur = hash_head[h];
        while (cur != -1) {
            if (nodes[cur].key == key) {
                remove_from_lru(cur);
                remove_from_hash(cur);
                free_node(cur);
                --size;
                return;
            }
            cur = nodes[cur].next_hash;
        }
    }

    void clear() {
        for (int i = 0; i < HASH_SIZE; ++i) hash_head[i] = -1;
        lru_head = lru_tail = -1;
        size = 0;
        free_head = 0;
        for (int i = 0; i < MAX_CAP - 1; ++i) {
            nodes[i].next = i + 1;
        }
        nodes[MAX_CAP - 1].next = -1;
    }

    bool exists(Key key) {
        int h = hash(key);
        int cur = hash_head[h];
        while (cur != -1) {
            if (nodes[cur].key == key) return true;
            cur = nodes[cur].next_hash;
        }
        return false;
    }
};

template<class T>
bool similar(T now, T oth){
    return now.key == oth.key;
}

template<class T>
bool smaller(T now, T oth){
    return now.key < oth.key;
}

constexpr static int Ktype = 0, Ksize = 1, Kprev = 2, Knext = 3, Kfa = 4, Kleaf = 0, Kin = 1;

constexpr int max(int x, int y){
    return x > y ? x : y;
}

template<class T, int info_len, int M, int L>
class MemoryRiver {
private:
    /* your code here */
    std::string file_name;
    constexpr static int sizeofT = sizeof(T);
    constexpr static int sizeofRiver = sizeof(int) * info_len + sizeofT * max(M + 2, L + 1) + sizeof(int) * (M + 2);
    char pool[sizeofRiver];

public:
    struct block{
        int type, size, prev, next, fa;
        T val[(M > L ? M : L) + 2];
        int son[M + 2];
        void out(){
            std::cerr << "type = " << (type == Kleaf ? "leaf" : "in") << ", size = " << size << 
                ", prev = " << prev << ", next = " << next << ", fa = " << fa << std::endl;
            for(int i=0;i<size;i++){
                std::cerr << "val[" << i << "] = {" << val[i].key << ", " << val[i].value << "}" << std::endl;
            }
            for(int i=0;i<=size;i++){
                std::cerr << "son[" << i << "] = " << son[i] << std::endl;
            }
            return;
        }
        bool full(){
            if(type == Kleaf){
                return size == L;
            }
            else{
                return size == M - 1;
            }
        }
        bool limited(){
            if(type == Kleaf){
                return size <= (L + 1) / 2;
            }
            else{
                return size + 1 <= (M + 1) / 2;
            }
        }
        block(){
            return;
        }
        block(int gtype, int gsize, int gprev, int gnext, int gfa){
            type = gtype, size = gsize, prev = gprev, next = gnext, fa = gfa;
            return;
        }
        int get_info(int n){
            switch(n){
                case 0:return type;
                case 1:return size;
                case 2:return prev;
                case 3:return next;
                case 4:return fa;
            }
            throw -1.1;
            return 0;
        }
        void write_info(int tmp, int n){
            switch(n){
                case 0:type = tmp; return;
                case 1:size = tmp; return;
                case 2:prev = tmp; return;
                case 3:next = tmp; return;
                case 4:fa = tmp; return;
            }
            throw -1.1;
            return;
        }
    };
    LRUCache<int, block> block_cache;

    fstream file;

    MemoryRiver() = default;

    MemoryRiver(const std::string file_name) : file_name(file_name) {}

    //初始化偏移量为 delta 的块

    void initialise(std::string FN = "", int x = 0) {
        //std::cerr << "sizeofRiver = " << sizeofRiver << std::endl;
        int delta = x * sizeofRiver;
        //std::cout << "MemoryRiver : initialise " << FN << ", delta = " << delta << std::endl;
        if (FN != "") file_name = FN;
        /*file.seekp(delta, ios::beg);
        int tmp = 0;
        for (int i=0;i<info_len;i++){
            file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
        }
        T emp = T();
        for(int i=0;i<=max(M + 1, L);i++){
            file.write(reinterpret_cast<char*>(&emp), sizeofT);
        }
        for(int i=0;i <= M + 1;i++){
            file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
        }*/
        char* ptr = pool;
        *reinterpret_cast<int*>(ptr) = 0, ptr += sizeof(int);
        *reinterpret_cast<int*>(ptr) = 0, ptr += sizeof(int);
        *reinterpret_cast<int*>(ptr) = 0, ptr += sizeof(int);
        *reinterpret_cast<int*>(ptr) = 0, ptr += sizeof(int);
        *reinterpret_cast<int*>(ptr) = 0, ptr += sizeof(int);
        for(int i = 0; i <= max(M + 1, L); ++i) {
            *reinterpret_cast<T*>(ptr) = T(), ptr += sizeof(T);
        }
        for(int i = 0; i <= M + 1; ++i) {
            *reinterpret_cast<int*>(ptr) = 0, ptr += sizeof(int);
        }
        file.seekp(delta, ios::beg);
        file.write(pool, sizeofRiver);
        return;
    }

    /*std::string get_type(int type){
        if(type == Ktype){
            return "type";
        }
        if(type == Ksize){
            return "size";
        }
        if(type == Kprev){
            return "prev";
        }
        if(type == Knext){
            return "next";
        }
        if(type == Kfa){
            return "fa";
        }
        return "";
    }*/

    block get_block(int x){
        //std::cout << "get_block : " << x << std::endl;
        int delta = x * sizeofRiver;
#ifdef DEBUG
//std::cerr << "get_block : " << x << ", delta = " << delta << std::endl;
#endif
        block ret;
        if(block_cache.get(x, ret)){
            return ret;
        }
        file.seekg(delta, ios::beg);
        file.read(reinterpret_cast<char*>(pool), sizeofRiver);
        char* ptr = pool;
        ret.type = *reinterpret_cast<int*>(ptr), ptr += sizeof(int);
        ret.size = *reinterpret_cast<int*>(ptr), ptr += sizeof(int);
        ret.prev = *reinterpret_cast<int*>(ptr), ptr += sizeof(int);
        ret.next = *reinterpret_cast<int*>(ptr), ptr += sizeof(int);
        ret.fa = *reinterpret_cast<int*>(ptr), ptr += sizeof(int);
        for(int i=0;i<ret.size;i++){
            ret.val[i] = *reinterpret_cast<T*>(ptr), ptr += sizeof(T);
        }
        if(ret.type == Kin){
            for(int i=0;i<=ret.size;i++){
                ret.son[i] = *reinterpret_cast<int*>(ptr), ptr += sizeof(int);
            }
        }
        //std::cout << "get_block : " << x << "end" << std::endl;
        /*int tmp = 0;
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int)), ret.type = tmp;
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int)), ret.size = tmp;
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int)), ret.prev = tmp;
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int)), ret.next = tmp;
        file.read(reinterpret_cast<char*>(&tmp), sizeof(int)), ret.fa = tmp;
        for(int i=0;i<ret.size;i++){
            T ttmp;
            file.read(reinterpret_cast<char*>(&ttmp), sizeofT);
            ret.val[i] = ttmp;
        }
        if(ret.type == Kin){
            for(int i=0;i<=ret.size;i++){
                file.read(reinterpret_cast<char*>(&tmp), sizeof(int));
                ret.son[i] = tmp;
            }
        }*/
        block_cache.put(x, ret);
        return ret;
    }

    void write_block(int x, block v){
        int delta = x * sizeofRiver;
#ifdef DEBUG
    std::cerr << "write_block : " << x << ", delta = " << delta << std::endl;
    v.out();
#endif
        char* ptr = pool;
        *reinterpret_cast<int*>(ptr) = v.type, ptr += sizeof(int);
        *reinterpret_cast<int*>(ptr) = v.size, ptr += sizeof(int);
        *reinterpret_cast<int*>(ptr) = v.prev, ptr += sizeof(int);
        *reinterpret_cast<int*>(ptr) = v.next, ptr += sizeof(int);
        *reinterpret_cast<int*>(ptr) = v.fa, ptr += sizeof(int);
        for(int i = 0; i < v.size; ++i) {
            *reinterpret_cast<T*>(ptr) = v.val[i], ptr += sizeof(T);
        }
        if(v.type == Kin) {
            for(int i = 0; i <= v.size; ++i) {
                *reinterpret_cast<int*>(ptr) = v.son[i], ptr += sizeof(int);
            }
        }
        file.seekp(delta, ios::beg);
        file.write(pool, sizeofRiver);
        block_cache.put(x, v);
        return;
    }

    //读出第n个int的值赋给tmp，1_base
    int get_info(int n, int x) {
        int delta = x * sizeofRiver;
        int tmp = 0;
        if (n > info_len){
            //exit(0);
            throw std::out_of_range("get_info");
        }
        //file.seekg(delta + sizeof(int)*n, ios::beg);
        //file.read(reinterpret_cast<char*>(&tmp), sizeof(int));
        tmp = get_block(x).get_info(n);
#ifdef DEBUG
//std::cout << "get_info, type = " << ((n == 1) ? "next" : "size") << ", delta = " << delta << ", ret = " << tmp << std::endl;
#endif
        return tmp;
        /* your code here */
    }

    int get_son(int n, int x){
        int delta = x * sizeofRiver;
        int nsize = get_info(Ksize, x);
        if(n > nsize){
            //exit(0);
            throw std::out_of_range("write_son");
        }
        //file.seekg(delta + sizeof(int)*info_len + sizeofT*nsize + sizeof(int)*n, ios::beg);
        //int tmp = 0;
        //file.read(reinterpret_cast<char*>(&tmp), sizeof(int));
        //return tmp;
        return get_block(x).son[n];
    }

    T get_value(int n, int x){
        int delta = x * sizeofRiver;
        int nsize = get_info(Ksize, x);
        if(n >= nsize){
            //exit(0);
            throw std::out_of_range("get_value");
        }
        //file.seekg(delta + sizeof(int)*info_len + sizeofT*n, ios::beg);
        //T tmp = T();
        //file.read(reinterpret_cast<char*>(&tmp), sizeofT);
        //return tmp;
        return get_block(x).val[n];
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n, int x) {
        int delta = x * sizeofRiver;
//#ifdef DEBUG
//std::cerr << "write_info, type = " << get_type(n) << ", x = " << x << ", delta = " << delta << ", tmp = " << tmp << std::endl;
//#endif
        if (n > info_len){
            //exit(0);
            throw std::out_of_range("write_info");
        }
        //file.seekp(delta + sizeof(int)*n, ios::beg);
        //file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
        block now = get_block(x);
        now.write_info(tmp, n);
        write_block(x, now);
        return;
        /* your code here */
    }

    void write_value(T tmp, int n, int x){
        int delta = x * sizeofRiver;
        int nsize = get_info(Ksize, x);
        if(n >= nsize){
            //exit(0);
            throw std::out_of_range("write_value");
        }
        //file.seekp(delta + sizeof(int)*info_len + sizeofT*n, ios::beg);
        //file.write(reinterpret_cast<char*>(&tmp), sizeofT);
        block now = get_block(x);
        now.val[n] = tmp;
        write_block(x, now);
        return;
    }

    void write_son(int tmp, int n, int x){
        int delta = x * sizeofRiver;
        int nsize = get_info(Ksize, x);
        if(n > nsize){
            //exit(0);
            throw std::out_of_range("write_son");
        }
        //file.seekp(delta + sizeof(int)*info_len + sizeofT*nsize + sizeof(int)*n, ios::beg);
        //file.write(reinterpret_cast<char*>(&tmp), sizeof(int));
        block now = get_block(x);
        now.son[n] = tmp;
        write_block(x, now);
        return;
    }

    //是否满

    bool full(int x){
#ifdef DEBUG
//std::cout << "checkfull : " << delta << std::endl;
//int msize = get_info(Ksize, delta);
//std::cout << "msize = " << msize << ", block - 1 = " << block - 1 << std::endl;
#endif
        if(get_info(Ktype, x) == Kleaf){
            return get_info(Ksize, x) == L;
        }
        else{
            return get_info(Ksize, x) == M - 1;
        }
    }

    //是否空

    bool limited(int x){
        if(get_info(Ktype, x) == Kleaf){
            return get_info(Ksize, x) <= (L + 1) / 2;
        }
        else{
            return get_info(Ksize, x) + 1 <= (M + 1) / 2;
        }
    }

    bool insert(const T &t, int son, int x) {
        block now = get_block(x);
#ifdef DEBUG
std::cerr << "block : insert, t = {" << t.key << "," << t.value << "}, son = " << son << ", x = " << x << std::endl;
now.out();
#endif
        bool upd = false;
        if(now.size == 0){
            now.size = 1;
            now.val[0] = t, now.son[1] = son;
            upd = true;
        }
        else{
            int pos = -1;
            if(now.val[0] >= t){
                pos = 0;
                upd = true;
            }
            for(int i=1;i<now.size;i++){
                if(now.val[i] >= t && t >= now.val[i - 1]){
                    pos = i;
                    break;
                }
            }
            if(t >= now.val[now.size - 1]){
                pos = now.size;
            }
            if(pos == -1){
                exit(0);
                throw 1.14;
            }
            for(int i=now.size - 1;i>=pos;i--){
                now.val[i + 1] = now.val[i];
            }
            for(int i=now.size;i>=pos + 1;i--){
                now.son[i + 1] = now.son[i];
            }
            now.val[pos] = t, now.son[pos + 1] = son;
            now.size++;
        }
        write_block(x, now);
        return upd;
    }

    void remove_first(int x) {
        block now = get_block(x);
        if(now.size == 0){
            return;
        }
        for(int i=0;i<now.size - 1;i++){
            now.val[i] = now.val[i + 1];
        }
        for(int i=0;i<now.size;i++){
            now.son[i] = now.son[i + 1];
        }
        now.size--;
        write_block(x, now);
        return;
    }

    bool remove(const T &value, int x) {
        block now = get_block(x);
        if(now.size == 0){
            return false;
        }
        int pos = -1;
        for(int i=0;i<now.size;i++){
            if(now.val[i] == value){
                pos = i;
                break;
            }
        }
        if(pos == -1){
            return false;
        }
        for(int i=pos;i<now.size - 1;i++){
            now.val[i] = now.val[i + 1];
        }
        for(int i=pos + 1;i<now.size;i++){
            now.son[i] = now.son[i + 1];
        }
        now.size--;
        write_block(x, now);
        return true;
    }
    
    bool overall(T &value, int x) {
        block now = get_block(x);
        bool have = false;
        for(int i=0;i<now.size;i++){
            if(similar(now.val[i], value)){
                std::cout << now.val[i].value << ' ';
                have = true;
            }
        }
        return have;
    }
};


template<class T, int info_len = 5, int M = 40, int L = 40> // require T ==/<=/>=/</>/!= T
class BPT {
private:
    MemoryRiver<T, info_len, M, L> blocks;
    
public:

    std::string file_name;
    using block = typename MemoryRiver<T, info_len, M, L>::block;
    BPT() = default;
    int rt, size, number;

    BPT(const std::string file_name) : file_name(file_name) {}

    ~BPT(){
        blocks.file.seekp(0, ios::beg);
        blocks.file.write(reinterpret_cast<char*>(&rt), sizeof(int));
        blocks.file.write(reinterpret_cast<char*>(&size), sizeof(int));
        blocks.file.write(reinterpret_cast<char*>(&number), sizeof(int));
        blocks.file.close();
        return;
    }

    void initialise(std::string FN = "", bool forced = false) {
        bool online = false;
#ifdef ONLINE_JUDGE
online = true;
#endif
        blocks.block_cache.clear();
        file_name = FN;
        if(!blocks.file.is_open()){
            blocks.file.open(file_name, ios::in | ios::out | ios::binary);
        }
        if(!blocks.file.is_open() || forced/* || !online*/){
            if(blocks.file.is_open()){
                blocks.file.close();
            }
            blocks.file.open(file_name, ios::out | ios::binary | ios::trunc);
            blocks.file.close();
            blocks.file.open(file_name, ios::in | ios::out | ios::binary);
            blocks.initialise(file_name, 1);
            blocks.initialise(file_name, 2);
            blocks.write_info(Kin, Ktype, 1), blocks.write_info(Kleaf, Ktype, 2);
            blocks.write_info(0, Ksize, 1), blocks.write_son(2, 0, 1);
            blocks.write_info(1, Kfa, 2);
            rt = 1, size = 2, number = 0;
        }
        else{
            blocks.file.seekg(0, ios::beg);
            blocks.file.read(reinterpret_cast<char*>(&rt), sizeof(int));
            blocks.file.read(reinterpret_cast<char*>(&size), sizeof(int));
            blocks.file.read(reinterpret_cast<char*>(&number), sizeof(int));
        }
        //std::cout << "sizeofT = " << sizeof(T) << std::endl;
        return;
    }

    T get_min(int x){
if(x){
    //std::cerr << "get_min : " << x << std::endl;
    //std::cerr << "leftmost son = " << blocks.get_son(0, x) << std::endl;
}
        if(blocks.get_info(Ktype, x) == Kleaf){
            if(blocks.get_info(Ksize, x) == 0){
                return T();
                //exit(0);
            }
            return blocks.get_value(0, x);
        }
        else{
            return get_min(blocks.get_son(0, x));
        }
    }

    void update(int x){
#ifdef DEBUG
std::cout << "update : " << x << std::endl;
#endif
        T nv = get_min(x);
        int np = x;
        while(np != rt){
            int fa = blocks.get_info(Kfa, np);
            block now = blocks.get_block(fa);
            int fsize = now.size;
#ifdef DEBUG
std::cout << "np = " << np << ", fa = " << fa << std::endl;
#endif
            if(now.son[0] == np){
                np = fa;
            }
            else{
                for(int i=1;i<=fsize;i++){
                    if(now.son[i] == np){
                        blocks.write_value(nv, i-1, fa);
                        return;
                    }
                    if(i == fsize){
                        //exit(0);
                        throw false;
                    }
                }
            }
        }
        return;
    }

    void split(int x, int fa){
        block now = blocks.get_block(x);
#ifdef DEBUG
std::cerr << "split : x = " << x << ", fa = " << fa << std::endl;
now.out();
#endif
        if(x == rt){
            blocks.initialise(file_name, ++size);
            blocks.write_info(Kin, Ktype, size);
            blocks.write_info(size, Kfa, x);
            blocks.write_son(x, 0, size);
            rt = size;
            fa = rt;
            now.fa = size;
        }
        block oth;
        if(now.type == Kleaf){
            oth = block(Kleaf, now.size - (now.size + 1) / 2, x, now.next, fa);
            for(int i=0;i<oth.size;i++){
                oth.val[i] = now.val[i + (now.size + 1) / 2];
            }
            now.size = (now.size + 1) / 2;
            blocks.write_block(x, now);
        }
        else{//now.size = M
            oth = block(Kin, (now.size + 1 - (now.size + 1) / 2) - 1, x, now.next, fa);
            for(int i=(now.size + 1) / 2;i<now.size;i++){
                oth.val[i - (now.size + 1) / 2] = now.val[i];
            }
            for(int i=(now.size + 1) / 2;i<=now.size;i++){
                oth.son[i - (now.size + 1) / 2] = now.son[i];
            }
            now.size = (now.size + 1) / 2 - 1;
            blocks.write_block(x, now);
        }
//#ifdef DEBUG
//std::cerr << "now and oth:" << std::endl;
//blocks.get_block(x).out();
//oth.out();
//#endif
        blocks.initialise(file_name, ++size);
        if(oth.type == Kin){
//#ifdef DEBUG
//std::cerr << "renew father in split" << std::endl;
//#endif
            for(int i=0;i<=oth.size;i++){
                blocks.write_info(size, Kfa, oth.son[i]);
//#ifdef DEBUG
//std::cerr << "oth.son[i] = " << oth.son[i] << std::endl;
//#endif
            }
        }
        blocks.write_block(size, oth);
        blocks.write_info(size, Knext, x);
        if(now.next){
            blocks.write_info(size, Kprev, now.next);
        }
//std::cerr << "ok?" << std::endl;
        bool full = blocks.full(fa);
#ifdef DEBUG
std::cout << "M = " << M << ", fa.size = " << blocks.get_info(Ksize, fa) << std::endl;
#endif
        blocks.insert(get_min(size), size, fa);
        if(full){
            split(fa, blocks.get_info(Kfa, fa));
        }
    }

    void insert(const T &value) {
        number++;
        int x = rt;
        while(true){
            block now = blocks.get_block(x);
#ifdef DEBUG
std::cerr << "insert, x = " << x << std::endl;
now.out();
#endif
            if(now.type == Kleaf){
                bool full = now.full();
                if(blocks.insert(value, -1, x)){
                    update(x);
                }
                if(full){
                    split(x, now.fa);
                }
                return;
            }
            else{
                bool dw = false;
                for(int i=0;i<now.size;i++){
                    if(!(value >= now.val[i])){
                        x = now.son[i];
                        dw = true;
                        break;
                    }
                }
                if(!dw){
                    x = now.son[now.size];
                }
            }
        }
    }

    void merge(int x, int fa/*, int timestamp = 0*/){
        //std::cout << "timestamp : " << timestamp << std::endl;
        block now = blocks.get_block(x);
        block pre, nxt;
        if(now.prev){
            //std::cout << "prev = " << now.prev << std::endl;
            pre = blocks.get_block(now.prev);
        }
        if(now.next){
            //std::cout << "next = " << now.next << std::endl;
            nxt = blocks.get_block(now.next);
        }
#ifdef DEBUG
std::cerr << "merge : x = " << x << ", fa = " << fa << std::endl;
now.out();
#endif
        if(now.next && nxt.fa == fa){
            //std::cout << "??" << std::endl;
            block oth = nxt;
#ifdef DEBUG
std::cerr << "with next" << std::endl;
#endif
            if(!oth.limited()){
#ifdef DEBUG
std::cerr << "not limited" << std::endl;
#endif
                now.size++;
                now.val[now.size - 1] = get_min(now.next);
                now.son[now.size] = oth.son[0];
                if(now.type == Kin){
                    blocks.write_info(x, Kfa, now.son[now.size]);
                }
                blocks.remove_first(now.next);
                blocks.write_block(x, now);
                update(now.next);
                return;
            }
            else{
#ifdef DEBUG
std::cerr << "limited" << std::endl;
#endif
                now.size++;
                T mem = get_min(now.next);
#ifdef DEBUG
std::cerr << "next : " << std::endl;
std::cerr << "mem.key = " << mem.key << ", mem.value = " << mem.value << std::endl;
blocks.get_block(now.next).out();
#endif
                now.val[now.size - 1] = mem;
                now.son[now.size] = oth.son[0];
                if(now.type == Kin){
                    blocks.write_info(x, Kfa, now.son[now.size]);
                }
                int osize = oth.size;
                for(int i=1;i<=osize-(now.type == Kleaf);i++){
                    now.size++;
                    now.val[now.size - 1] = oth.val[i - (now.type == Kin)];
                    now.son[now.size] = oth.son[i];
                    if(now.type == Kin){
                        blocks.write_info(x, Kfa, now.son[now.size]);
                    }
                }
                bool lim = blocks.limited(fa);
#ifdef DEBUG
std::cerr << "before delete, fa.size = " << blocks.get_info(Ksize, fa) << std::endl;
#endif
                blocks.remove(mem, fa);
                int nnext = oth.next;
                if(nnext){
                    now.next = nnext, blocks.write_info(x, Kprev, nnext);
                }
                else{
                    now.next = 0;
                }
                blocks.write_block(x, now);
                if(fa == rt){
                    if(blocks.get_info(Ksize, fa) == 0){
                        blocks.write_info(0, Kfa, x);
                        rt = x;
                    }
                    return;
                }
                if(lim){
                    merge(fa, blocks.get_info(Kfa, fa));
                }
            }
        }
        else if(now.prev && pre.fa == fa){
            //std::cout << "???" << std::endl;
            block oth = pre;
#ifdef merge_DEBUG
std::cerr << "with prev" << std::endl;
#endif
            if(!oth.limited()){
#ifdef merge_DEBUG
std::cerr << "not limited" << std::endl;
blocks.get_block(now.prev).out();
#endif
                now.size++;
                now.son[now.size] = now.son[now.size - 1];
                for(int i=now.size-1;i;i--){
                    now.val[i] = now.val[i - 1], now.son[i] = now.son[i - 1];
                }
                int psize = oth.size;
                if(psize == 0){
                    //exit(0);
                }
                now.val[0] = (now.type == Kin ? get_min(x) : oth.val[psize - 1]);
                now.son[0] = oth.son[psize];
                if(now.type == Kin){
                    blocks.write_info(x, Kfa, now.son[0]);
                }
                blocks.write_block(x, now);
                update(x);
                blocks.remove(blocks.get_value(psize - 1, now.prev), now.prev);
                return;
            }
            else{
#ifdef merge_DEBUG
std::cerr << "limited" << std::endl;
#endif
                block oth = pre;
                x = now.prev, std::swap(now, oth);
                
#ifdef merge_DEBUG
std::cerr << "limited" << std::endl;
#endif
                now.size++;
                T mem = get_min(now.next);
#ifdef DEBUG
std::cerr << "next : " << std::endl;
std::cerr << "mem.key = " << mem.key << ", mem.value = " << mem.value << std::endl;
blocks.get_block(now.next).out();
#endif
                now.val[now.size - 1] = mem;
                now.son[now.size] = oth.son[0];
                if(now.type == Kin){
                    blocks.write_info(x, Kfa, now.son[now.size]);
                }
                int osize = oth.size;
                for(int i=1;i<=osize-(now.type == Kleaf);i++){
                    now.size++;
                    now.val[now.size - 1] = oth.val[i - (now.type == Kin)];
                    now.son[now.size] = oth.son[i];
                    if(now.type == Kin){
                        blocks.write_info(x, Kfa, now.son[now.size]);
                    }
                }
                bool lim = blocks.limited(fa);
#ifdef DEBUG
std::cerr << "before delete, fa.size = " << blocks.get_info(Ksize, fa) << std::endl;
#endif
                blocks.remove(mem, fa);
                int nnext = oth.next;
                if(nnext){
                    now.next = nnext, blocks.write_info(x, Kprev, nnext);
                }
                else{
                    now.next = 0;
                }
                blocks.write_block(x, now);
                if(fa == rt){
                    if(blocks.get_info(Ksize, fa) == 0){
                        blocks.write_info(0, Kfa, x);
                        rt = x;
                    }
                    return;
                }
                if(lim){
                    merge(fa, blocks.get_info(Kfa, fa));
                }
            }
        }
        else{
            //std::cout << "direct return" << std::endl;
            return;
        }
        return;
    }

    void remove(const T &value/*, int timestamp = 0*/) {
        //std::cout << "remove, size = " << size << ", number = " << number << std::endl;
        int x = rt;
        while(true){
            block now = blocks.get_block(x);
//std::cerr << "remove, x = " << x << std::endl;
            if(now.type == Kleaf){
                bool lim = now.limited();
                T mem = (now.size == 0 ? T() : now.val[0]);
                //std::cout << "block size = " << now.size << std::endl;
                if(!blocks.remove(value, x)){
                    return;
                }
                //std::cout << "removed" << std::endl;
                number--;
                update(x);
                if(lim){
                    //std::cout << "merge" << std::endl;
                    merge(x, now.fa/*, timestamp*/);
                }
#ifdef DEBUG
std::cerr << "???" << std::endl;
#endif
                return;
            }
            else{
                bool dw = false;
                for(int i=0;i<now.size;i++){
                    if(!(value >= now.val[i])){
                        x = now.son[i];
                        dw = true;
                        break;
                    }
                }
                if(!dw){
                    x = now.son[now.size];
                }
            }
        }
    }

    void find(const T &value) {
#ifdef DEBUG
std::cout << "\n\n\nfind begin\n\n\n" << std::endl;
std::cout << "value = {" << value.key << "," << value.value << "}" << std::endl;
#endif
        int x = rt;
        while(true){
            block now = blocks.get_block(x);
            if(now.type == Kleaf){
                break;
            }
            else{
                //x = now.son[0];
                //continue;
                bool dw = false;
                for(int i=0;i<now.size;i++){
                    if(!(value >= now.val[i])){
#ifdef DEBUG
std::cout << "dw, now.val = " << now.val[i].key << ", now.son = " << now.son[i] << std::endl;
#endif
                        x = now.son[i];
                        dw = true;
                        break;
                    }
                }
                if(!dw){
#ifdef DEBUG
if(now.size)std::cout << "dw, max value = " << now.val[now.size - 1].key << ", now.son = " << now.son[now.size] << std::endl;
#endif
                    x = now.son[now.size];
                }
            }
        }
        bool found = false;
#ifdef DEBUG
std::cout << "find start" << std::endl;
#endif
        while(x){
#ifdef DEBUG
std::cout << "find, x = " << x << std::endl;
std::cout << "key = " << blocks.get_value(0, x).key << std::endl;
std::cout << "value = " << blocks.get_value(0, x).value << std::endl;
#endif
            block now = blocks.get_block(x);
            int size = now.size;
            if(size && smaller(now.val[size - 1], value)){
                x = now.next;
#ifdef DEBUG
std::cout << "find, skipped, nxt = " << x << std::endl;
#endif
            }
            else if(size && smaller(value, now.val[0])){
                break;
            }
            else{
                for(int i=0;i<size;i++){
                    if(similar(now.val[i], value)){
                        std::cout << now.val[i].value << ' ';
                        found = true;
                    }
                }
                x = now.next;
#ifdef DEBUG
std::cout << "find, visited, nxt = " << x << std::endl;
#endif
            }
        }
        if(!found){
            std::cout << "null";
        }
        std::cout << std::endl;
        return;
    }

    std::vector<T> all() {
        int x = rt;
        while(true){
            block now = blocks.get_block(x);
            if(now.type == Kleaf){
                break;
            }
            else{
                x = now.son[0];
            }
        }
        std::vector<T> ret;
        while(x){
            block now = blocks.get_block(x);
            int size = now.size;
            for(int i=0;i<size;i++){
                ret.push_back(now.value[i].value);
            }
            x = now.next;
        }
        return ret;
    }

    std::vector<T> all_similar(const T &value) {
#ifdef DEBUG
std::cout << "\n\n\nfind begin\n\n\n" << std::endl;
std::cout << "value = {" << value.key << "," << value.value << "}" << std::endl;
#endif
        int x = rt;
        while(true){
            block now = blocks.get_block(x);
            if(now.type == Kleaf){
                break;
            }
            else{
                //x = now.son[0];
                //continue;
                bool dw = false;
                for(int i=0;i<now.size;i++){
                    if(!(value >= now.val[i])){
#ifdef DEBUG
std::cout << "dw, now.val = " << now.val[i].key << ", now.son = " << now.son[i] << std::endl;
#endif
                        x = now.son[i];
                        dw = true;
                        break;
                    }
                }
                if(!dw){
#ifdef DEBUG
if(now.size)std::cout << "dw, max value = " << now.val[now.size - 1].key << ", now.son = " << now.son[now.size] << std::endl;
#endif
                    x = now.son[now.size];
                }
            }
        }
        bool found = false;
#ifdef DEBUG
std::cout << "find start" << std::endl;
#endif
        std::vector<T> ret;
        while(x){
#ifdef DEBUG
std::cout << "find, x = " << x << std::endl;
std::cout << "key = " << blocks.get_value(0, x).key << std::endl;
std::cout << "value = " << blocks.get_value(0, x).value << std::endl;
#endif
            block now = blocks.get_block(x);
            int size = now.size;
            if(size && smaller(now.val[size - 1], value)){
                x = now.next;
#ifdef DEBUG
std::cout << "find, skipped, nxt = " << x << std::endl;
#endif
            }
            else if(size && smaller(value, now.val[0])){
                break;
            }
            else{
                for(int i=0;i<size;i++){
                    if(similar(now.val[i], value)){
                        ret.push_back(now.val[i]);
                        found = true;
                    }
                }
                x = now.next;
#ifdef DEBUG
std::cout << "find, visited, nxt = " << x << std::endl;
#endif
            }
        }
        return ret;
    }

    T only(const T &value) {
#ifdef DEBUG
std::cout << "\n\n\nfind begin\n\n\n" << std::endl;
std::cout << "value = {" << value.key << "," << value.value << "}" << std::endl;
#endif
        int x = rt;
        //std::cout << "only:rt = " << rt << ", number = " << number << std::endl;
        while(true){
            block now = blocks.get_block(x);
            if(now.type == Kleaf){
                break;
            }
            else{
                //x = now.son[0];
                //continue;
                bool dw = false;
                for(int i=0;i<now.size;i++){
                    if(!(value >= now.val[i])){
#ifdef DEBUG
std::cout << "dw, now.val = " << now.val[i].key << ", now.son = " << now.son[i] << std::endl;
#endif
                        x = now.son[i];
                        dw = true;
                        break;
                    }
                }
                if(!dw){
#ifdef DEBUG
if(now.size)std::cout << "dw, max value = " << now.val[now.size - 1].key << ", now.son = " << now.son[now.size] << std::endl;
#endif
                    x = now.son[now.size];
                }
            }
        }
#ifdef DEBUG
std::cout << "find start" << std::endl;
#endif
        while(x){
#ifdef DEBUG
std::cout << "find, x = " << x << std::endl;
std::cout << "key = " << blocks.get_value(0, x).key << std::endl;
std::cout << "value = " << blocks.get_value(0, x).value << std::endl;
#endif
            block now = blocks.get_block(x);
            int size = now.size;
            if(size && smaller(now.val[size - 1], value)){
                x = now.next;
#ifdef DEBUG
std::cout << "find, skipped, nxt = " << x << std::endl;
#endif
            }
            else if(size && smaller(value, now.val[0])){
                break;
            }
            else{
                for(int i=0;i<size;i++){
                    if(similar(now.val[i],value)){
                        return now.val[i];
                    }
                }
                x = now.next;
#ifdef DEBUG
std::cout << "find, visited, nxt = " << x << std::endl;
#endif
            }
        }
        assert(false);
        std::cout << "???" << std::endl;
        x = rt;
        while(true){
            std::cout << "x = " << x << std::endl;
            block now = blocks.get_block(x);
            if(now.type == Kleaf){
                break;
            }
            else{
                bool dw = false;
                for(int i=0;i<now.size;i++){
                    if(!(value >= now.val[i])){
                        x = now.son[i];
                        dw = true;
                        break;
                    }
                }
                if(!dw){
                    x = now.son[now.size];
                }
            }
        }
        std::cout << "final, x = " << x << std::endl;
        while(x){
std::cout << "find, x = " << x << std::endl;
std::cout << "key = " << blocks.get_value(0, x).key << std::endl;
            block now = blocks.get_block(x);
            int size = now.size;
            if(size && smaller(now.val[size - 1], value)){
                x = now.next;
std::cout << "find, skipped, nxt = " << x << std::endl;
            }
            else if(size && smaller(value, now.val[0])){
                break;
            }
            else{
                for(int i=0;i<size;i++){
                    if(similar(now.val[i],value)){
                        return now.val[i];
                    }
                }
                x = now.next;
std::cout << "find, visited, nxt = " << x << std::endl;
            }
        }
        assert(false);
        return T();
    }

    bool have(const T &value) {
#ifdef DEBUG
std::cout << "\n\n\nfind begin\n\n\n" << std::endl;
std::cout << "value = {" << value.key << "," << value.value << "}" << std::endl;
#endif
        //std::cout << "have, rt = " << rt << ", size = " << size << ", number = " << number << std::endl;
        int x = rt;
        while(true){
            block now = blocks.get_block(x);
            if(now.type == Kleaf){
                break;
            }
            else{
                //x = now.son[0];
                //continue;
                bool dw = false;
                for(int i=0;i<now.size;i++){
                    if(!(value >= now.val[i])){
#ifdef DEBUG
std::cout << "dw, now.val = " << now.val[i].key << ", now.son = " << now.son[i] << std::endl;
#endif
                        x = now.son[i];
                        dw = true;
                        break;
                    }
                }
                if(!dw){
#ifdef DEBUG
if(now.size)std::cout << "dw, max value = " << now.val[now.size - 1].key << ", now.son = " << now.son[now.size] << std::endl;
#endif
                    x = now.son[now.size];
                }
            }
        }
#ifdef DEBUG
std::cout << "find start" << std::endl;
#endif
        while(x){
#ifdef DEBUG
std::cout << "find, x = " << x << std::endl;
std::cout << "key = " << blocks.get_value(0, x).key << std::endl;
std::cout << "value = " << blocks.get_value(0, x).value << std::endl;
#endif
            block now = blocks.get_block(x);
            int size = now.size;
            //std::cout << "have, now_x = " << x << ", size = " << size << std::endl;
            if(size && smaller(now.val[size - 1], value)){
                x = now.next;
#ifdef DEBUG
std::cout << "find, skipped, nxt = " << x << std::endl;
#endif
            }
            else if(size && smaller(value, now.val[0])){
                break;
            }
            else{
                for(int i=0;i<size;i++){
                    if(similar(now.val[i],value)){
                        return true;
                    }
                }
                x = now.next;
#ifdef DEBUG
std::cout << "find, visited, nxt = " << x << std::endl;
#endif
            }
        }
        return false;
    }
    /*void overall(int x){
        block now = blocks.get_block(x);
#ifdef overall_DEBUG
std::cout << "overall, x = " << x << std::endl;
now.out();
#endif
        if(now.type == Kin){
            for(int i=0;i<=now.size;i++){
#ifdef overall_DEBUG
std::cout << "x = " << x << ", son = " << now.son[i] << std::endl;
#endif
                if(blocks.get_info(Kfa, now.son[i]) != x){
                    //exit(0);
                    throw false;
                }
                else if(i != now.size && i && !(now.val[i] > now.val[i-1])){
                    throw false;
                }
                else if(i != now.size && get_min(now.son[i+1]) != now.val[i]){
                    throw false;
                }
                else{
                    overall(now.son[i]);
                }
            }
        }
        else{
#ifdef overall_DEBUG
std::cout << "overall, leaf, x = " << x << std::endl;
#endif
        }
        return;
    }*/

};

template<typename tp>
class ARRAY {
public:
    std::string file_name;
    fstream file;
    LRUCache<int, tp, 4> cache;
    int element_count;
    static constexpr size_t META_SIZE = sizeof(int);
    static constexpr size_t BUF_SIZE = META_SIZE + sizeof(tp);
    char buffer[BUF_SIZE];

    void read_meta() {
        file.seekg(0, ios::beg);
        file.read(buffer, META_SIZE);
        element_count = *reinterpret_cast<int*>(buffer);
    }

    void write_meta() {
        file.seekp(0, ios::beg);
        *reinterpret_cast<int*>(buffer) = element_count;
        file.write(buffer, META_SIZE);
        file.flush();
    }

    size_t get_offset(int idx) const {
        return META_SIZE + idx * sizeof(tp);
    }

    ARRAY() = default;

    explicit ARRAY(const std::string& name) : file_name(name) {}

    ~ARRAY() {
        if (file.is_open()) {
            write_meta();
            file.close();
        }
    }

    void initialise(std::string name = "", bool forced = false) {
        if (!name.empty()) {
            file_name = name;
        }
        if (forced) {
            if (file.is_open()) {
                file.close();
            }
            file.open(file_name, ios::out | ios::binary | ios::trunc);
            element_count = 0;
            write_meta();
            file.close();
            file.open(file_name, ios::in | ios::out | ios::binary);
            cache.clear();
            return;
        }
        file.open(file_name, ios::in | ios::out | ios::binary);
        if (!file.is_open()) {
            file.open(file_name, ios::out | ios::binary);
            element_count = 0;
            write_meta();
            file.close();
            file.open(file_name, ios::in | ios::out | ios::binary);
        } else {
            read_meta();
        }
        cache.clear();
        return;
    }

    int size() const {
        return element_count;
    }

    void push_back(const tp& val) {
        size_t offset = get_offset(element_count);
        file.seekp(offset, ios::beg);
        file.write(reinterpret_cast<const char*>(&val), sizeof(tp));
        file.flush();
        cache.put(element_count, val);
        element_count++;
        write_meta();
    }

    void set(int idx, const tp& val) {
        size_t offset = get_offset(idx);
        file.seekp(offset, ios::beg);
        file.write(reinterpret_cast<const char*>(&val), sizeof(tp));
        file.flush();
        cache.put(idx, val);
    }

    tp get(int idx) {
        tp val;
        if (cache.get(idx, val)) {
            return val;
        }
        size_t offset = get_offset(idx);
        file.seekg(offset, ios::beg);
        file.read(reinterpret_cast<char*>(&val), sizeof(tp));
        cache.put(idx, val);
        return val;
    }
    void close() {
        write_meta();
        file.close();
    }
};