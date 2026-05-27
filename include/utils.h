#pragma once

#include <iostream>
#include <cassert>
#include <cstdio>
#include <string>

template <typename... Args>
struct tuple;

template <size_t N, typename T>
struct tuple_element;

template <size_t N, typename First, typename... Rest>
struct tuple_element<N, tuple<First, Rest...>> 
    : tuple_element<N - 1, tuple<Rest...>> {};

template <typename First, typename... Rest>
struct tuple_element<0, tuple<First, Rest...>> {
    using type = First;
};

template <size_t N, typename T>
using tuple_element_t = typename tuple_element<N, T>::type;

template <size_t N, typename First, typename... Rest>
constexpr tuple_element_t<N, tuple<First, Rest...>>& get(tuple<First, Rest...>& t) {
    static_assert(N < sizeof...(Rest) + 1, "tuple 索引越界！");
    if constexpr (N == 0) {
        return t.head;
    } else {
        return get<N - 1>(t.tail);
    }
}

template <size_t N, typename First, typename... Rest>
constexpr const tuple_element_t<N, tuple<First, Rest...>>& get(const tuple<First, Rest...>& t) {
    static_assert(N < sizeof...(Rest) + 1, "tuple 索引越界！");
    if constexpr (N == 0) {
        return t.head;
    } else {
        return get<N - 1>(t.tail);
    }
}

template <typename First, typename... Rest>
struct tuple<First, Rest...> {
    First head;
    tuple<Rest...> tail;
    constexpr tuple() : head(), tail() {}
    constexpr tuple(First f, Rest... rest) : head(f), tail(rest...) {}

    template <size_t N>
    constexpr First& operator()() noexcept {
        return get<N>(*this);
    }

    template <size_t N>
    constexpr const First& operator()() const noexcept {
        return get<N>(*this);
    }

    template<typename... Ts>
    friend constexpr bool operator>(const tuple<Ts...>&, const tuple<Ts...>&);
};

template <>
struct tuple<> {};

constexpr bool operator>(const tuple<>&, const tuple<>&) {
    return false;
}

template <typename First, typename... Rest>
constexpr bool operator>(const tuple<First, Rest...>& a, const tuple<First, Rest...>& b) {
    if (a.head > b.head) return true;
    if (a.head < b.head) return false;
    return a.tail > b.tail;
}

template<typename... Ts>
constexpr bool operator<(const tuple<Ts...>& a, const tuple<Ts...>& b) {
    return b > a;
}

template<typename... Ts>
constexpr bool operator>=(const tuple<Ts...>& a, const tuple<Ts...>& b) {
    return !(a < b);
}

template<typename... Ts>
constexpr bool operator<=(const tuple<Ts...>& a, const tuple<Ts...>& b) {
    return !(a > b);
}

template<typename... Ts>
constexpr bool operator==(const tuple<>&, const tuple<>&) { return true; }
template<typename First, typename... Rest>
constexpr bool operator==(const tuple<First, Rest...>& a, const tuple<First, Rest...>& b) {
    return (a.head == b.head) && (a.tail == b.tail);
}

template<typename... Ts>
constexpr bool operator!=(const tuple<Ts...>& a, const tuple<Ts...>& b) {
    return !(a == b);
}

struct chineseCharacter{
    char ch[3];
    chineseCharacter(){
        for(int i=0;i<=2;i++)ch[i]=0;
        return;
    }
    friend bool operator>(chineseCharacter now, chineseCharacter oth){
        for(int i=0;i<3;i++){
            if(now.ch[i] > oth.ch[i]){
                return true;
            }
            if(now.ch[i] < oth.ch[i]){
                return false;
            }
        }
        return false;
    }
    friend bool operator<(chineseCharacter now, chineseCharacter oth){
        for(int i=0;i<3;i++){
            if(now.ch[i] < oth.ch[i]){
                return true;
            }
            if(now.ch[i] > oth.ch[i]){
                return false;
            }
        }
        return false;
    }
    friend bool operator!=(chineseCharacter now, chineseCharacter oth){
        for(int i=0;i<3;i++){
            if(now.ch[i] != oth.ch[i]){
                return true;
            }
        }
        return false;
    }
    friend std::ostream& operator<<(std::ostream& out, chineseCharacter now){
        out.write(now.ch, 3);
        return out;
    }
};

inline void print_chinese(std::ostream& out, const chineseCharacter& cn) {
    out.write(cn.ch, 3);
    return;
}

struct personName{
    chineseCharacter ch[5];
    friend bool operator<(personName now, personName oth){
        for(int i=0;i<5;i++){
            if(now.ch[i] < oth.ch[i]){
                return true;
            }
            if(now.ch[i] > oth.ch[i]){
                return false;
            }
        }
        return false;
    }
    friend bool operator>(personName now, personName oth){
        for(int i=0;i<5;i++){
            if(now.ch[i] > oth.ch[i]){
                return true;
            }
            if(now.ch[i] < oth.ch[i]){
                return false;
            }
        }
        return false;
    }
    friend bool operator!=(personName now, personName oth){
        for(int i=0;i<5;i++){
            if(now.ch[i] != oth.ch[i]){
                return true;
            }
        }
        return false;
    }
    friend bool operator==(personName now, personName oth){
        for(int i=0;i<5;i++){
            if(now.ch[i] != oth.ch[i]){
                return false;
            }
        }
        return true;
    }
    friend std::ostream& operator<<(std::ostream& out, personName now){
        for(int i=0;i<5;i++){
            if(now.ch[i] != chineseCharacter()){
                out << now.ch[i];
            }
            else break;
        }
        return out;
    }
};

struct stationName{
    chineseCharacter ch[10];
    friend bool operator<(stationName now, stationName oth){
        for(int i=0;i<10;i++){
            if(now.ch[i] < oth.ch[i]){
                return true;
            }
            if(now.ch[i] > oth.ch[i]){
                return false;
            }
        }
        return false;
    }
    friend bool operator>(stationName now, stationName oth){
        for(int i=0;i<10;i++){
            if(now.ch[i] > oth.ch[i]){
                return true;
            }
            if(now.ch[i] < oth.ch[i]){
                return false;
            }
        }
        return false;
    }
    friend bool operator!=(stationName now, stationName oth){
        for(int i=0;i<10;i++){
            if(now.ch[i] != oth.ch[i]){
                return true;
            }
        }
        return false;
    }
    friend bool operator==(stationName now, stationName oth){
        for(int i=0;i<10;i++){
            if(now.ch[i] != oth.ch[i]){
                return false;
            }
        }
        return true;
    }
    friend std::ostream& operator<<(std::ostream& out, stationName now){
        for(int i=0;i<10;i++){
            if(now.ch[i] != chineseCharacter()){
                out << now.ch[i];
            }
            else break;
        }
        return out;
    }
};

struct stationNames{
    stationName name[100];
};

struct char64{
    char ch[65];
    char64(){
        for(int i=0;i<65;i++)ch[i]=0;
        return;
    }
    friend bool operator<(char64 now, char64 oth){
        for(int i=0;i<65;i++){
            if(now.ch[i] < oth.ch[i]){
                return true;
            }
            if(now.ch[i] > oth.ch[i]){
                return false;
            }
        }
        return false;
    }
    friend bool operator>(char64 now, char64 oth){
        for(int i=0;i<65;i++){
            if(now.ch[i] > oth.ch[i]){
                return true;
            }
            if(now.ch[i] < oth.ch[i]){
                return false;
            }
        }
        return false;
    }
    friend bool operator==(char64 now, char64 oth){
        for(int i=0;i<65;i++){
            if(now.ch[i] != oth.ch[i]){
                return false;
            }
        }
        return true;
    }
    friend bool operator>=(char64 now, char64 oth){
        return now > oth || now == oth;
    }
    friend bool operator<=(char64 now, char64 oth){
        return now < oth || now == oth;
    }
    friend bool operator!=(char64 now, char64 oth){
        return !(now == oth);
    }
    friend std::ostream& operator<<(std::ostream& out, char64 now){
        out << now.ch;
        return out;
    }
};

struct char20{
    char ch[21];
    char20(){
        for(int i=0;i<=20;i++)ch[i]=0;
        return;
    }
    operator char64(){
        char64 ret;
        for(int i=0;i<=20;i++)ret.ch[i]=ch[i];
        return ret;
    }
};

struct char30{
    char ch[31];
    char30(){
        for(int i=0;i<=30;i++)ch[i]=0;
        return;
    }
    operator char64(){
        char64 ret;
        for(int i=0;i<=30;i++)ret.ch[i]=ch[i];
        return ret;
    }
    friend bool operator<(char30 now, char30 oth){
        for(int i=0;i<31;i++){
            if(now.ch[i] < oth.ch[i]){
                return true;
            }
            if(now.ch[i] > oth.ch[i]){
                return false;
            }
        }
        return false;
    }
    friend bool operator>(char30 now, char30 oth){
        for(int i=0;i<31;i++){
            if(now.ch[i] > oth.ch[i]){
                return true;
            }
            if(now.ch[i] < oth.ch[i]){
                return false;
            }
        }
        return false;
    }
    friend bool operator==(char30 now, char30 oth){
        for(int i=0;i<31;i++){
            if(now.ch[i] != oth.ch[i]){
                return false;
            }
        }
        return true;
    }
    friend bool operator>=(char30 now, char30 oth){
        return now > oth || now == oth;
    }
    friend bool operator<=(char30 now, char30 oth){
        return now < oth || now == oth;
    }
    friend bool operator!=(char30 now, char30 oth){
        return !(now == oth);
    }
    friend std::ostream& operator<<(std::ostream& out, char30 now){
        out << now.ch;
        return out;
    }
};


struct int100{
    int v[100];
    int100(){
        for(int i=0;i<=99;i++)v[i]=0;
        return;
    }
};

struct mat{
    int v[100][100];
    mat(){
        for(int i=0;i<=99;i++)for(int j=0;j<=99;j++)v[i][j]=0;
        return;
    }
};

template<typename tp>
struct string_info{
    char64 key;
    tp value;
    string_info(){
        
        return;
    }
    string_info(char64 gkey, tp gvalue){
        key = gkey;
        value = gvalue;
        return;
    }
    friend bool operator==(string_info now, string_info oth){
        if(!similar(now, oth)){
            return false;
        }
        return now.value == oth.value;
    }
    friend bool operator!=(string_info now, string_info oth){
        return !(now == oth);
    }
    friend bool operator>=(string_info now, string_info oth){
        if(now.key != oth.key){
            return now.key > oth.key;
        }
        return now.value >= oth.value;
    }
    friend bool operator>(string_info now, string_info oth){
        if(now.key != oth.key){
            return now.key > oth.key;
        }
        return now.value > oth.value;
    }
};

template<typename tp>
struct stationName_info{
    stationName key;
    tp value;
    stationName_info(){
        
        return;
    }
    stationName_info(stationName gkey, tp gvalue){
        key = gkey;
        value = gvalue;
        return;
    }
    friend bool operator==(stationName_info now, stationName_info oth){
        if(!similar(now, oth)){
            return false;
        }
        return now.value == oth.value;
    }
    friend bool operator!=(stationName_info now, stationName_info oth){
        return !(now == oth);
    }
    friend bool operator>=(stationName_info now, stationName_info oth){
        if(now.key != oth.key){
            return now.key > oth.key;
        }
        return now.value >= oth.value;
    }
    friend bool operator>(stationName_info now, stationName_info oth){
        if(now.key != oth.key){
            return now.key > oth.key;
        }
        return now.value > oth.value;
    }
};


template<typename tp>
struct int_info{
    int key;
    tp value;
    int_info(){
        key = 0;
        return;
    }
    int_info(int gkey, tp gvalue){
        key = gkey;
        value = gvalue;
        return;
    }
    friend bool operator==(int_info now, int_info oth){
        if(!similar(now, oth)){
            return false;
        }
        return now.value == oth.value;
    }
    friend bool operator!=(int_info now, int_info oth){
        return !(now == oth);
    }
    friend bool operator>=(int_info now, int_info oth){
        if(now.key != oth.key){
            return now.key > oth.key;
        }
        return now.value >= oth.value;
    }
    friend bool operator>(int_info now, int_info oth){
        if(now.key != oth.key){
            return now.key > oth.key;
        }
        return now.value > oth.value;
    }
};

inline std::string read_chinese(){
    chineseCharacter cn{};
    cn.ch[0] = getchar();
    cn.ch[1] = getchar();
    cn.ch[2] = getchar();
    return std::string{cn.ch, 3};
}