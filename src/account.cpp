#include "account.h"

using poser = string_info<int>;
using pointer = int_info<Taccountinfo>;

void AccountSystem::add_user(char20 curname, char64 username, Taccountinfo info, bool fir){
    if(fir){
        int cou = accountMemory_.element_count;
        account_.insert(poser(username, cou));
        accountMemory_.push_back(info);
        std::cout << '0' << std::endl;
    }
    else{
        if(!stack_.have(poser(curname, 0))){
            //std::cout << "stack dont have username" << std::endl;
            std::cout << "-1" << std::endl;
            return;
        }
        if(account_.have(poser(username, 0))){
            //std::cout << "stack already have curname" << std::endl;
            std::cout << "-1" << std::endl;
            return;
        }
        poser now = stack_.only(poser(curname, 0));
        Taccountinfo value = accountMemory_.get(now.value);
        int level = get<3>(value);
        if(level <= get<3>(info)){
            std::cout << "-1" << std::endl;
            return;
        }
        int cou = accountMemory_.element_count;
        account_.insert(poser(username, cou));
        accountMemory_.push_back(info);
        std::cout << '0' << std::endl;
    }
    return;
}
void AccountSystem::login(char64 username, char30 password){
    //std::cout << "login, username = " << username << std::endl;
    //std::cout << "stack.number = " << stack_.number << std::endl;
    if(!account_.have(poser(username, 0))){
        //std::cout << "account dont have username" << std::endl;
        std::cout << "-1" << std::endl;
        return;
    }
    if(stack_.have(poser(username, 0))){
        //std::cout << "stack already had username" << std::endl;
        std::cout << "-1" << std::endl;
        return;
    }
    poser now = account_.only(poser(username, 0));
    //std::cout << "now.value = " << now.value << std::endl;
    Taccountinfo value = accountMemory_.get(now.value);
    if(get<0>(value) != password){
        //std::cout << "wrong password" << std::endl;
        //std::cout << get<0>(ptr.value) << "|||" << password << std::endl;
        std::cout << "-1" << std::endl;
        return;
    }
    stack_.insert(poser(username, now.value));
    std::cout << "0" << std::endl;
    return;
}

void AccountSystem::logout(char64 username){
    if(!stack_.have(poser(username, 0))){
        std::cout << "-1" << std::endl;
        return;
    }
    poser now = stack_.only(poser(username, 0));
    stack_.remove(now);
    std::cout << '0' << std::endl;
    return;
}

void AccountSystem::query_profile(const char64& curname,const char64& username){
    //std::cout << "query_profile, username = " << username << std::endl;
    //std::cout << "stack.number = " << stack_.number << std::endl;
    if(!stack_.have(poser(curname, 0))){
        //std::cout << "stack dont have curname" << std::endl;
        std::cout << "-1" << std::endl;
        return;
    }
    //std::cout << "?" << std::endl;
    if(!account_.have(poser(username, 0))){
        //std::cout << "account dont have username" << std::endl;
        std::cout << "-1" << std::endl;
        return;
    }
    poser cp = account_.only(poser(curname, 0)), up = account_.only(poser(username, 0));
    //std::cout << "cp.value = " << cp.value << ", up.value = " << up.value << std::endl;
    Taccountinfo mcp = accountMemory_.get(cp.value),
        mup = accountMemory_.get(up.value);
    if(cp.value != up.value && get<3>(mcp) <= get<3>(mup)){
        std::cout << "-1" << std::endl;
        return;
    }
    std::cout << username << ' ' << get<1>(mup) << ' ' << 
        get<2>(mup) << ' ' << get<3>(mup) << ' ' << std::endl;
    return;
}

void AccountSystem::modify_profile(char64 curname, char64 username, Taccountinfo info){
    if(!stack_.have(poser(curname, 0))){
        //std::cout << "stack dont have curname" << std::endl;
        std::cout << "-1" << std::endl;
        return;
    }
    if(!account_.have(poser(username, 0))){
        //std::cout << "account dont have username" << std::endl;
        std::cout << "-1" << std::endl;
        return;
    }
    poser cp = account_.only(poser(curname, 0)), up = account_.only(poser(username, 0));
    //std::cout << "cp.value = " << cp.value << ", up.value = " << up.value << std::endl;
    Taccountinfo mcp = accountMemory_.get(cp.value),
        mup = accountMemory_.get(up.value);
    if(cp.value != up.value && get<3>(mcp) <= get<3>(mup)){
        std::cout << "-1" << std::endl;
        return;
    }
    if(get<3>(mcp) <= get<3>(info)){
        std::cout << "-1" << std::endl;
        return;
    }
    if(auto v = get<0>(info); v != char30()){
        get<0>(mup) = get<0>(info);
    }
    if(auto v = get<1>(info); v != personName()){
        get<1>(mup) = get<1>(info);
    }
    if(auto v = get<2>(info); v != char30()){
        get<2>(mup) = get<2>(info);
    }
    if(auto v = get<3>(info); v != -1){
        get<3>(mup) = get<3>(info);
    }
    accountMemory_.set(up.value, mup);
    std::cout << username << ' ' << get<1>(mup) << ' ' << 
        get<2>(mup) << ' ' << get<3>(mup) << ' ' << std::endl;
    return;
}

void AccountSystem::clean(){
    account_.initialise(account_.file_name, true);
    stack_.initialise(stack_.file_name, true);
    accountMemory_.initialise(accountMemory_.file_name, true);
    return;
}

void AccountSystem::exit(){
    stack_.initialise(stack_.file_name, true);
    std::cout << "bye" << std::endl;
    return;
}