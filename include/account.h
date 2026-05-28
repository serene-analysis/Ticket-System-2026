#pragma once
#include "bpt.hpp"
#include "utils.h"

using Taccount = string_info<int>;
using Taccountinfo = tuple<char30, personName, char30, int>;
using TaccountMemory = int_info<Taccountinfo>;
//password, name, mailAddr, privilege
struct AccountSystem{
    BPT<Taccount> account_, stack_;
    BPT<TaccountMemory> accountMemory_;
    AccountSystem(){
        account_.initialise("account.data");
        stack_.initialise("stack.data");
        accountMemory_.initialise("accountMemory.data");
        return;
    }
    ~AccountSystem(){
        
        return;
    }
    void add_user(char20, char64, Taccountinfo, bool);
    void login(char64, char30);
    void logout(char64);
    void query_profile(char64, char64);
    void modify_profile(char64, char64, Taccountinfo);
    void clean();
    void exit();
};