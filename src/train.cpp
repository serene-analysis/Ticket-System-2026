#include "train.h"

using poser = string_info<int>;
using sposer = stationName_info<char64>;
using pointer = int_info<TtrainInfo>;
using tpointer = int_info<mat>;

void TrainSystem::add_train(char64 trainId, TtrainInfo info, mat tickets){
    if(hidden_.have(poser(trainId, 0)) || released_.have(poser(trainId, 0))){
        std::cout << "-1" << std::endl;
        return;
    }
    int cou = trainMemory_.element_count;
    /*if(cou == 1){
        std::cerr << "cou = " << cou << std::endl;
        std::cerr << "trainId = " << trainId << std::endl;
        std::cerr << "startTime = " << get<4>(info) << std::endl;
    }*/
    hidden_.insert(poser(trainId, cou));
    trainMemory_.push_back(info);
    ticketMemory_.push_back(tickets);
    std::cout << '0' << std::endl;
    return;
}

void TrainSystem::delete_train(char64 trainId){
    if(!hidden_.have(poser(trainId, 0)) || released_.have(poser(trainId, 0))){
        std::cout << "-1" << std::endl;
        return;
    }
    hidden_.remove(hidden_.only(poser(trainId, 0)));
    std::cout << '0' << std::endl;
    return;
}

void TrainSystem::release_train(char64 trainId){
    if(!hidden_.have(poser(trainId, 0)) || released_.have(poser(trainId, 0))){
        //if(!hidden_.have(poser(trainId, 0)))std::cout << "not even hidden";
        std::cout << "-1" << std::endl;
        return;
    }
    poser pos = hidden_.only(poser(trainId, 0));
    //std::cout << "pos.value = " << pos.value << std::endl;
    hidden_.remove(pos), released_.insert(pos);
    TtrainInfo info = trainMemory_.get(pos.value);
    int num = get<0>(info);
    stationNames stations = get<1>(info);
    for(int i=0;i<num;i++){
        stationName now = stations.ch[i];
        sposer spos = sposer(now, trainId);
        stationer_.insert(spos);
    }
    std::cout << '0' << std::endl;
    return;
}

void TrainSystem::query_train(char64 trainId, date startDate){
    if(startDate < date(2, 1)){
        std::cout << "-1" << std::endl;
        return;
    }
    if(!hidden_.have(poser(trainId, 0)) && !released_.have(poser(trainId, 0))){
        std::cout << "-1" << std::endl;
        return;
    }
    poser pos;
    if(hidden_.have(poser(trainId, 0))){
        pos = hidden_.only(poser(trainId, 0));
    }
    else{
        pos = released_.only(poser(trainId, 0));
    }
    TtrainInfo info = trainMemory_.get(pos.value);
    dateRange saleDate = get<7>(info);
    if(!in(saleDate, startDate)){
        std::cout << "-1" << std::endl;
        return;
    }
    int idx = startDate - saleDate.l;
    //std::cout << startDate << "|||" << saleDate.l << std::endl;
    //std::cout << "idx = " << idx << std::endl;
    //std::cout << "trainId = " << trainId << ", poser.value = " << pos.value << std::endl;
    int num = get<0>(info);
    stationNames stations = get<1>(info);
    int100 prices = get<3>(info);
    dailyTime startTime = get<4>(info);
    int100 travelTimes = get<5>(info);
    int100 stopOverTimes = get<6>(info);
    char type = get<8>(info);
    mat tickets = ticketMemory_.get(pos.value);
    std::cout << trainId << ' ' << type << std::endl;
    Time now(startDate, startTime);
    int accumulatedPrice = 0;
    std::cout << stations.ch[0] << " xx-xx xx:xx -> "
        << now << ' ' << accumulatedPrice << ' ' << tickets.v[idx].v[0] << std::endl;
    for(int i=1;i<num-1;i++){
        accumulatedPrice += prices.v[i - 1];
        now += travelTimes.v[i - 1];
        std::cout << stations.ch[i] << ' ';
        std::cout << now << " -> ";
        now += stopOverTimes.v[i - 1];
        std::cout << now << ' ' << accumulatedPrice << ' ' << tickets.v[idx].v[i] << std::endl;
    }
    accumulatedPrice += prices.v[num - 2];
    now += travelTimes.v[num - 2];
    std::cout << stations.ch[num - 1] << ' ' << now << " -> xx-xx xx:xx ";
    std::cout << accumulatedPrice << " x" << std::endl;
    return;
}

int index(TtrainInfo &info, stationName station){
    int size = get<0>(info);
    stationNames &stations = get<1>(info);
    for(int i=0;i<size;i++){
        if(stations.ch[i] == station){
            return i;
        }
    }
    return -1;
}

int pure_distance(TtrainInfo &info, int st, int en){
    int100 &travelTimes = get<5>(info), &stopTimes = get<6>(info);
    int ret = 0;
    for(int i=st;i<en;i++){
        ret += travelTimes.v[i];
        if(i != en - 1){
            ret += stopTimes.v[i];
        }
    }
    return ret;
}

int cost(TtrainInfo &info, int st, int en){
    int100 &prices = get<3>(info);
    int ret = 0;
    for(int i=st;i<en;i++){
        ret += prices.v[i];
    }
    return ret;
}

int max_tickets(mat tickets, int st, int en, int idx){
    //std::cout << "max_tickets : st = " << st << ", en = " << en << ", idx = " << idx << std::endl;
    int ret = int_max;
    for(int i=st;i<en;i++){
        ret = min(ret, tickets.v[idx].v[i]);
    }
    return ret;
}

int leaving_gone_days(TtrainInfo &info, int x){
    int100 &travelTimes = get<5>(info), &stopTimes = get<6>(info);
    if(x == get<0>(info) - 1){
        std::cout << "leaving_gone_days throw" << std::endl;
        throw false;
    }
    dailyTime &start = get<4>(info), now = start;
    for(int i=0;i<x;i++){
        now += travelTimes.v[i];
        now += stopTimes.v[i];
    }
    //dailyTime mem = now;
    //mem.reduce();
    //std::cout << "start = " << start << ", arr = " << mem << ", days = " << now.days() << std::endl;
    return now.days();
}

dailyTime leaving_time(TtrainInfo &info, int x){
    int100 &travelTimes = get<5>(info), &stopTimes = get<6>(info);
    if(x == get<0>(info) - 1){
        std::cout << "leaving_time throw" << std::endl;
        throw false;
    }
    dailyTime &start = get<4>(info), now = start;
    //std::cout << "startTime = " << now << std::endl;
    for(int i=0;i<x;i++){
        now += travelTimes.v[i];
        now += stopTimes.v[i];
    }
    now.reduce();
    return now;
}

int leaving_time_since_begin(TtrainInfo &info, int x){
    int100 &travelTimes = get<5>(info), &stopTimes = get<6>(info);
    if(x == get<0>(info) - 1){
        std::cout << "leaving_time_since_begin throw" << std::endl;
        throw false;
    }
    int ret = 0;
    //std::cout << "startTime = " << now << std::endl;
    for(int i=0;i<x;i++){
        ret += travelTimes.v[i];
        ret += stopTimes.v[i];
    }
    return ret;
}

void TrainSystem::query_ticket(stationName st, stationName en, date curdate, std::string type){
    if(curdate < date(2, 1)){
        std::cout << "0" << std::endl;
        return;
    }
    std::vector<sposer> all = stationer_.all_similar(sposer(st, char64()));
    int size = all.size();
    using Tproposal = tuple<int, char64, std::string>;
    std::vector<Tproposal> ans;
    for(int i=0;i<size;i++){
        poser pos = released_.only(poser(all[i].value, 0));
        TtrainInfo info = trainMemory_.get(pos.value);
        mat tickets = ticketMemory_.get(pos.value);
        int spos = index(info, st), epos = index(info, en);
        if(epos == -1 || spos > epos){
            continue;
        }
        date startDate = curdate - leaving_gone_days(info, spos);
        dateRange saleDate = get<7>(info);
        if(!in(saleDate, startDate)){
            continue;
        }
        int idx = startDate - saleDate.l;
        std::ostringstream oss;
        int needed_time = pure_distance(info, spos, epos), needed_cost = cost(info, spos, epos);
        Time start(curdate, leaving_time(info, spos)), end = start + needed_time;
        oss << all[i].value << ' ' << st << ' ' << start << " -> "
            << en << ' ' << end << ' ' << needed_cost << ' ' << max_tickets(tickets, spos, epos, idx);
        //std::cout << oss.str() << "||| needed_time = " << needed_time << std::endl;
        if(type == "cost"){
            ans.push_back(Tproposal(needed_cost, all[i].value, oss.str()));
        }
        else{
            ans.push_back(Tproposal(needed_time, all[i].value, oss.str()));
        }
    }
    sort(ans);
    int nsize = ans.size();
    std::cout << nsize << std::endl;
    for(int i=0;i<nsize;i++){
        std::cout << get<2>(ans[i]) << std::endl;
    }
    return;
}

//template<typename tp, typename oth>
int find_first(std::vector<tuple<stationName, Time, int, int, char64, std::string>> got, stationName gave){
    int ef_lef = 0, ef_rig = got.size() - 1, ef_ans = -1;
    while(ef_lef <= ef_rig){
        int ef_mid = (ef_lef + ef_rig) >> 1;
        if(get<0>(got[ef_mid]) >= gave){
            ef_ans = ef_mid;
            ef_rig = ef_mid - 1;
        }
        else ef_lef = ef_mid + 1;
    }
    return ef_ans;
}

void TrainSystem::query_transfer(stationName st, stationName en, date curdate, std::string type){
    if(curdate < date(2, 1)){
        std::cout << "0" << std::endl;
        return;
    }
    using Tproposal = tuple<int, int, char64, char64, std::string, std::string>;
    using Tchoice = tuple<stationName, Time, int, int, char64, std::string>;
    std::vector<Tchoice> choices;
    std::vector<Tproposal> proposals;
    std::vector<sposer> all = stationer_.all_similar(sposer(st, char64()));
    int size = all.size();
    for(int i=0;i<size;i++){
        poser pos = released_.only(poser(all[i].value, 0));
        TtrainInfo info = trainMemory_.get(pos.value);
        mat tickets = ticketMemory_.get(pos.value);
        int spos = index(info, st);
        int number = get<0>(info);
        if(spos == number - 1){
            continue;
        }
        date startDate = curdate - leaving_gone_days(info, spos);
        stationNames &stations = get<1>(info);
        int100 &prices = get<3>(info);
        int100 &travelTimes = get<5>(info), &stopOverTimes = get<6>(info);
        dateRange saleDate = get<7>(info);
        if(!in(saleDate, startDate)){
            continue;
        }
        int idx = startDate - saleDate.l;
        Time start(curdate, leaving_time(info, spos)), arrival = start;
        int totalPrice = 0, maxTickets = int_max, accumulatedTime = 0;
        for(int j=spos+1;j<number;j++){
            arrival += travelTimes.v[j - 1];
            accumulatedTime += travelTimes.v[j - 1];
            totalPrice += prices.v[j - 1];
            maxTickets = min(maxTickets, tickets.v[idx].v[j - 1]);
            std::ostringstream oss;
            oss << all[i].value << ' ' << st << ' ' << start << " -> "
                << stations.ch[j] << ' ' << arrival << ' ' << totalPrice << ' ' << maxTickets;
            if(type == "cost"){
                choices.push_back(Tchoice(stations.ch[j], arrival, totalPrice, accumulatedTime, all[i].value, oss.str()));
            }
            else{
                choices.push_back(Tchoice(stations.ch[j], arrival, accumulatedTime, totalPrice, all[i].value, oss.str()));
            }
            if(j != number - 1){
                arrival += stopOverTimes.v[j - 1];
                accumulatedTime += stopOverTimes.v[j - 1];
            }
        }
    }
    sort(choices);
    //std::cout << "choices.size = " << choices.size() << std::endl;
    all = stationer_.all_similar(sposer(en, char64()));
    int nsize = all.size();
    for(int i=0;i<nsize;i++){
        poser pos = released_.only(poser(all[i].value, 0));
        TtrainInfo info = trainMemory_.get(pos.value);
        mat tickets = ticketMemory_.get(pos.value);
        int epos = index(info, en);
        int number = get<0>(info);
        if(epos == 0){
            continue;
        }
        stationNames &stations = get<1>(info);
        int100 &prices = get<3>(info);
        int100 &travelTimes = get<5>(info), &stopOverTimes = get<6>(info);
        dateRange saleDate = get<7>(info);
        int totalPrice = 0, maxTickets = int_max, accumulatedTime = 0;
        for(int j=epos-1;j>=0;j--){
            int firstPos = find_first(choices, stations.ch[j]);
            if(firstPos == -1){
                continue;
            }
            int nsize = choices.size();
            while(firstPos < nsize){
                Tchoice current = choices[firstPos];
                if(get<0>(current) != stations.ch[j]){
                    break;
                }
                Time arrival = get<1>(current), mem = arrival;
                int firstKeyword = get<2>(current), secondKeyword = get<3>(current);
                char64 thirdKeyword = get<4>(current);
                std::string firstOutput = get<5>(current);
                dailyTime dailyLeavingTime = leaving_time(info, j);
                if(arrival.t > dailyLeavingTime){
                    arrival.d += 1;
                }
                arrival.t = dailyLeavingTime;
                date startDate = arrival.d - leaving_gone_days(info, j);
                if(startDate > saleDate.r){
                    firstPos++;
                    continue;
                }
                if(startDate < saleDate.l){
                    arrival.d += saleDate.l - startDate;
                    startDate = saleDate.l;
                }
                //std::cerr << "startDate = " << startDate << ", saleDate.l = " << saleDate.l << ", saleDate.r = " << saleDate.r << std::endl;
                int idx = startDate - saleDate.l;
                std::ostringstream oss;
                int needed_time = pure_distance(info, j, epos), needed_cost = cost(info, j, epos);
                Time start(arrival), end = start + needed_time;
                oss << all[i].value << ' ' << stations.ch[j] << ' ' << start << " -> "
                    << en << ' ' << end << ' ' << needed_cost << ' ' << max_tickets(tickets, j, epos, idx);
                std::string secondOutput = oss.str();
                if(type == "cost"){
                    firstKeyword += needed_cost, secondKeyword += needed_time + (arrival - mem);
                }
                else{
                    firstKeyword += needed_time + (arrival - mem), secondKeyword += needed_cost;
                }
                proposals.push_back(Tproposal(firstKeyword, secondKeyword, thirdKeyword, all[i].value, firstOutput, secondOutput));
                firstPos++;
            }
        }
    }
    sort(proposals);
    int osize = proposals.size();
    for(int i=0;i<osize;i++){
        if(get<2>(proposals[i]) != get<3>(proposals[i])){
            std::cout << get<4>(proposals[i]) << std::endl << get<5>(proposals[i]) << std::endl;
            return;
        }
    }
    std::cout << '0' << std::endl;
    return;
}

void TrainSystem::clean(){
    hidden_.initialise(hidden_.file_name, true);
    released_.initialise(released_.file_name, true);
    stationer_.initialise(stationer_.file_name, true);
    ticketMemory_.initialise(ticketMemory_.file_name, true);
    trainMemory_.initialise(trainMemory_.file_name, true);
    return;
}