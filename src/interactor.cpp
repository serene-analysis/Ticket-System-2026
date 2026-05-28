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
            got += read_chinese();
        } else {
            got += static_cast<char>(ch);
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

char turn_char(std::string str){
    return str[0];
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

stationName turn_stationName(std::string str){
    stationName ret;
    int len = str.length();
    for(int i=0;i<len;i+=3){
        ret.ch[i / 3].ch[0] = str[i];
        ret.ch[i / 3].ch[1] = str[i + 1];
        ret.ch[i / 3].ch[2] = str[i + 2];
    }
    return ret;
}

stationNames turn_stationNames(std::string str){
    stationNames ret;
    std::string got;
    int len = str.length();
    int nc = 0;
    for(int i=0;i<len;i++){
        if(str[i] == '|'){
            ret.ch[nc++] = turn_stationName(got);
            got.clear();
        }
        else got += str[i];
    }
    ret.ch[nc++] = turn_stationName(got);
    return ret;
}

int100 turn_int100(std::string str){
    if(str.length() == 1 && str[0] == '_')return int100();
    int100 ret;
    std::string got;
    int len = str.length();
    int nc = 0;
    for(int i=0;i<len;i++){
        if(str[i] == '|'){
            ret.v[nc++] = turn_int(got);
            got.clear();
        }
        else got += str[i];
    }
    ret.v[nc++] = turn_int(got);
    return ret;
}

dailyTime turn_dailyTime(std::string str){
    int hr = (str[0] - 48) * 10 + str[1] - 48, mi = (str[3] - 48) * 10 + str[4] - 48;
    return dailyTime(hr * 60 + mi);
}

date turn_date(std::string str){
    return date((str[0] - 48) * 10 + str[1] - 48, (str[3] - 48) * 10 + str[4] - 48);
}

dateRange turn_dateRange(std::string str){
    date l = date((str[0] - 48) * 10 + str[1] - 48, (str[3] - 48) * 10 + str[4] - 48),
        r = date((str[6] - 48) * 10 + str[7] - 48, (str[9] - 48) * 10 + str[10] - 48);
    return dateRange(l, r);
}

mat mat_fill(int x){
    mat ret;
    for(int i=0;i<99;i++)for(int j=0;j<99;j++){
        ret.v[i].v[j] = x;
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

#include <cstdlib>

void operate(std::vector<std::string> got, AccountSystem &account, TrainSystem &train, OrderSystem &order){
    got.push_back(std::string());
    std::cout << got[0] << ' ';
    if(got[0] == "----[3470]"){
        exit(0);
        return;
    }
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
    else if(got[1] == "add_train"){
        char64 trainId = turn_char64(got[index(got, "-i")]);
        //std::cout << "add_train, index(\"-i\") = " << index(got, "-i") << std::endl;
        TtrainInfo info;
        int size = turn_int(got[index(got, "-n")]);
        get<0>(info) = size;
        get<1>(info) = turn_stationNames(got[index(got, "-s")]);
        int seatNum = turn_int(got[index(got, "-m")]);
        get<2>(info) = seatNum;
        get<3>(info) = turn_int100(got[index(got, "-p")]);
        get<4>(info) = turn_dailyTime(got[index(got, "-x")]);
        get<5>(info) = turn_int100(got[index(got, "-t")]);
        get<6>(info) = turn_int100(got[index(got, "-o")]);
        get<7>(info) = turn_dateRange(got[index(got, "-d")]);
        get<8>(info) = turn_char(got[index(got, "-y")]);
        train.add_train(trainId, info, mat_fill(seatNum));
    }
    else if(got[1] == "delete_train"){
        char64 trainId = turn_char64(got[index(got, "-i")]);
        train.delete_train(trainId);
    }
    else if(got[1] == "release_train"){
        char64 trainId = turn_char64(got[index(got, "-i")]);
        train.release_train(trainId);
    }
    else if(got[1] == "query_train"){
        char64 trainId = turn_char64(got[index(got, "-i")]);
        date startDate = turn_date(got[index(got, "-d")]);
        train.query_train(trainId, startDate);
    }
    else if(got[1] == "query_ticket"){
        date startdate = turn_date(got[index(got, "-d")]);
        stationName st = turn_stationName(got[index(got, "-s")]),
            en = turn_stationName(got[index(got, "-t")]);
        std::string type = got[index(got, "-p")];
        train.query_ticket(st, en, startdate, type);
    }
    else if(got[1] == "query_transfer"){
        date startdate = turn_date(got[index(got, "-d")]);
        stationName st = turn_stationName(got[index(got, "-s")]),
            en = turn_stationName(got[index(got, "-t")]);
        std::string type = got[index(got, "-p")];
        train.query_ticket(st, en, startdate, type);
    }
    else if(got[1] == "exit"){
        account.exit();
    }
    //else throw false;
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