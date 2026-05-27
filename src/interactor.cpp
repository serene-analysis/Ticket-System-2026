#include "interactor.h"
#include <cassert>

std::vector<std::string> Interactor::readLine() {
    std::vector<std::string> ret;
    std::string got;
    while (true) {
        int first = getchar();
        if (first == EOF) {
            end_ = true;
            break;
        }
        unsigned char ch = static_cast<unsigned char>(first);
        if (ch == '\n') break;
        if (ch == '\r') {
            assert(getchar() == '\n');
            break;
        }
        if (ch == ' ') {
            if (!got.empty()) {
                ret.push_back(got);
                got.clear();
            }
            continue;
        }
        if (ch >= 0xE0 && ch <= 0xEF) {
            ungetc(ch, stdin);
            while (true) {
                int peek = getchar();
                if (peek == ' ' || peek == '\n' || peek == '\r' || peek == EOF) {
                    ungetc(peek, stdin);
                    break;
                }
                ungetc(peek, stdin);
                got += read_chinese();
            }
        }
        else {
            got += static_cast<char>(ch);
            while (true) {
                int peek = getchar();
                if (peek == ' ' || peek == '\n' || peek == '\r' || peek == EOF) {
                    ungetc(peek, stdin);
                    break;
                }
                got += static_cast<char>(peek);
            }
        }
    }
    if (!got.empty()) {
        ret.push_back(got);
    }
    return ret;
}

char20 turn_char20(std::string str){
    char20 ret;
    int len = str.length();
    for(int i=0;i<len;i++){
        ret.ch[i] = str[i];
    }
    return ret;
}

char30 turn_char30(std::string str){
    char30 ret;
    int len = str.length();
    for(int i=0;i<len;i++){
        ret.ch[i] = str[i];
    }
    return ret;
}

char64 turn_char64(std::string str){
    char64 ret;
    int len = str.length();
    for(int i=0;i<len;i++){
        ret.ch[i] = str[i];
    }
    return ret;
}

int turn_int(std::string str){
    if(str.length() == 0){
        return -1;
    }
    int ret = 0;
    int len = str.length();
    for(int i=0;i<len;i++){
        ret = ret * 10 + str[i] - 48;
    }
    return ret;
}

personName turn_personName(std::string str){
    personName ret;
    int len = str.length();
    for(int i=0;i<len;i+=3){
        ret.ch[i / 3].ch[0] = str[i];
        ret.ch[i / 3].ch[1] = str[i + 1];
        ret.ch[i / 3].ch[2] = str[i + 2];
    }
    return ret;
}

int index(std::vector<std::string> strs, std::string goal){
    int size = strs.size();
    for(int i=0;i<size-1;i++){
        if(strs[i] == goal){
            return i + 1;
        }
    }
    return strs.size() - 1;
}

/*#define g0 get<0>
#define g1 get<1>
#define g2 get<2>
#define g3 get<3>
#define g4 get<4>
#define g5 get<5>
#define g6 get<6>
#define g7 get<7>
#define g8 get<8>
#define g9 get<9>*/

void operate(std::vector<std::string> got, AccountSystem &account, TrainSystem &train, OrderSystem &order){
    got.push_back(std::string());
    std::cout << got[0] << ' ';
    if(got[1] == "add_user"){
        char64 username;
        char20 curname;
        Taccountinfo nv;
        bool fir = false;
        //std::cout << "number = " << account.accountMemory_.number << std::endl;
        username = turn_char64(got[index(got, "-u")]);
        get<0>(nv) = turn_char30(got[index(got, "-p")]);
        get<1>(nv) = turn_personName(got[index(got, "-n")]);
        get<2>(nv) = turn_char30(got[index(got, "-m")]);
        if(account.accountMemory_.number == 0){
            get<3>(nv) = 10;
            fir = true;
        }
        else{
            curname = turn_char20(got[index(got, "-c")]);
            get<3>(nv) = turn_int(got[index(got, "-g")]);
        }
        //std::cout << "fir = " << fir << std::endl;
        account.add_user(curname, username, nv, fir);
    }
    else if(got[1] == "login"){
        char64 username = turn_char64(got[index(got, "-u")]);
        char30 password = turn_char30(got[index(got, "-p")]);
        account.login(username, password);
    }
    else if(got[1] == "logout"){
        char64 username = turn_char64(got[index(got, "-u")]);
        account.logout(username);
    }
    else if(got[1] == "query_profile"){
        char64 curname = turn_char20(got[index(got, "-c")]);
        char64 username = turn_char64(got[index(got, "-u")]);
        account.query_profile(curname, username);
    }
    else if(got[1] == "modify_profile"){
        char64 curname = turn_char20(got[index(got, "-c")]);
        char64 username = turn_char64(got[index(got, "-u")]);
        Taccountinfo info(turn_char30(got[index(got, "-p")]),
            turn_personName(got[index(got, "-n")]),
            turn_char30(got[index(got, "-m")]),
            turn_int(got[index(got, "-g")]));
        account.modify_profile(curname, username, info);
    }
    else if(got[1] == "exit"){
        account.exit();
    }
    else throw false;
}

void Interactor::tian(AccountSystem &account, TrainSystem &train, OrderSystem &order){
    /*if(account.account_.number == 0){
        //std::cout << "create root" << std::endl;
        assert(account.signup(turn("root"), turn("sjtu"), turn("root"), true, log));
    }*/
    //std::cout << "ok!" << std::endl;
    
    while(!end_){
        auto got = readLine();
        if(end_){
            if(!got.empty()){
                operate(got, account, train, order);
            }
            return;
        }
        if(got.empty()){
            continue;
        }
        operate(got, account, train, order);
    }
    return;
}