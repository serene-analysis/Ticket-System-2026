#include "order.h"

using poser = string_info<int>;
using timePoser = int_info<int>;
using accountPointer = int_info<Taccountinfo>;
using trainPointer = int_info<TtrainInfo>;
using traintPointer = int_info<mat>;
using orderPointer = int_info<TorderInfo>;

void OrderSystem::buy_ticket(char64 username, char64 trainId, date curDate, stationName st, stationName en, int number, std::string type, AccountSystem& account, TrainSystem& train, int timestamp){
    if(!account.stack_.have(poser(username, 0))){
        std::cout << "-1" << std::endl;
        return;
    }
    if(!train.released_.have(poser(trainId, 0))){
        std::cout << "-1" << std::endl;
        return;
    }
    poser pos = train.released_.only(poser(trainId, 0));
    TtrainInfo info = train.trainMemory_.get(pos.value);
    mat tickets = train.ticketMemory_.get(pos.value);
    dateRange saleDate = get<7>(info);
    int spos = index(info, st), epos = index(info, en);
    if(spos > epos || spos == -1 || epos == -1){
        std::cout << "-1" << std::endl;
        return;
    }
    dailyTime startTime = get<4>(info);
    date startDate = curDate - leaving_gone_days(info, spos);
    if(!in(saleDate, startDate)){
        std::cout << "-1" << std::endl;
        return;
    }
    int idx = startDate - saleDate.l;
    int prices = cost(info, spos, epos), maxNumber = max_tickets(tickets, spos, epos, idx);
    Time start(startDate, startTime);
    Time leavingTime = start + leaving_time_since_begin(info, spos), arrivalTime = leavingTime + pure_distance(info, spos, epos);
    TorderInfo mem(timestamp, trainId, startDate, st, leavingTime,
        en, arrivalTime, prices, number, username);
    if(maxNumber < number){
        if(type != "true" || number > get<2>(info)){
            std::cout << "-1" << std::endl;
            return;
        }
    }
    int cou = orderMemory_.number;
    userorder_.insert(poser(username, cou));
    orderMemory_.insert(orderPointer(cou, mem));
    if(maxNumber < number){
        std::cout << "queue" << std::endl;
        queue_.insert(poser(trainId, cou));
        timequeue_.insert(timePoser(timestamp, cou));
    }
    else{
        //std::cout << "number = " << number << ", prices = " << prices << std::endl;
        //std::cout << "spos = " << spos << ", epos = " << epos << std::endl;
        std::cout << 1ll * number * prices << std::endl;
        //std::cout << "idx = " << idx << ", spos = " << spos << ", epos = " << epos << std::endl;
        //std::cout << "ok" << std::endl;
        //std::cerr << "buy : trainId = " << trainId << ", idx = " << idx << ", spos = " << spos << ", epos = " << epos
        //    << ", number = " << number << ", maxNumber = " << maxNumber << std::endl;
        //std::cerr << "original : " << std::endl;
        //int size = get<0>(info);
        //for(int i=0;i<size;i++){
        //    std::cerr << tickets.v[idx].v[i] << " ";
        //}
        //std::cerr << std::endl;
        for(int i=spos;i<epos;i++){
            tickets.v[idx].v[i] -= number;
            if(tickets.v[idx].v[i] < 0){
                //std::cout << "idx = " << idx << ", i = " << i << std::endl;
                throw false;
            }
        }
        train.ticketMemory_.set(pos.value, tickets);
    }
    return;
}

void OrderSystem::query_order(char64 username, AccountSystem& account){
    if(!account.stack_.have(poser(username, 0))){
        std::cout << "-1" << std::endl;
        return;
    }
    std::vector<poser> pos = userorder_.all_similar(poser(username, 0));
    int size = pos.size();
    std::cout << size << std::endl;
    for(int i=size-1;i>=0;i--){
        int npos = pos[i].value;
        TorderInfo info = orderMemory_.only(orderPointer(npos, TorderInfo())).value;
        int timestamp = get<0>(info);
        char64 trainId = get<1>(info);
        Time st = get<4>(info), en = get<6>(info);
        stationName stName = get<3>(info), enName = get<5>(info);
        int prices = get<7>(info), number = get<8>(info), type = 1;
        if(refunded_.have(timePoser(timestamp, 0))){
            type = 3;
        }
        else if(timequeue_.have(timePoser(timestamp, 0))){
            type = 2;
        }
        static std::string outs[4] = {"", "[success]", "[pending]", "[refunded]"};
        std::cout << outs[type] << ' ' << trainId << ' ' << stName << ' ' << st
            << " -> " << enName << ' ' << en << ' ' << prices << ' ' << number << std::endl;
        //std::cout << "timestamp = " << timestamp << std::endl;
    }
    return;
}

TorderInfo OrderSystem::back_order(char64 username, int num, AccountSystem& account, TrainSystem& train){
    std::vector<poser> pos = userorder_.all_similar(poser(username, 0));
    int size = pos.size();
    if(size < num){
        throw false;
    }
    return orderMemory_.only(orderPointer(pos[size - num].value, TorderInfo())).value;
}

void OrderSystem::refund_ticket(char64 username, int num, AccountSystem& account, TrainSystem& train){
    if(!account.stack_.have(poser(username, 0))){
        std::cout << "-1" << std::endl;
        return;
    }
    try{
        TorderInfo test = back_order(username, num, account, train);
    }catch(bool){
        std::cout << "-1" << std::endl;
        return;
    }
    TorderInfo orderInfo = back_order(username, num, account, train);
    int timestamp = get<0>(orderInfo);
    if(refunded_.have(timePoser(timestamp, 0))){
        std::cout << "-1" << std::endl;
        return;
    }
    if(timequeue_.have(timePoser(timestamp, 0))){
        queue_.remove(poser(username, timequeue_.only(timePoser(timestamp, 0)).value));
        timequeue_.remove(timequeue_.only(timePoser(timestamp, 0)));
        refunded_.insert(timePoser(timestamp, 0));
        std::cout << "0" << std::endl;
        return;
    }
    char64 trainId = get<1>(orderInfo);
    poser pos = train.released_.only(poser(trainId, 0));
    date startDate = get<2>(orderInfo);
    Time st = get<4>(orderInfo), en = get<6>(orderInfo);
    stationName stName = get<3>(orderInfo), enName = get<5>(orderInfo);
    int prices = get<7>(orderInfo), number = get<8>(orderInfo);

    mat tickets = train.ticketMemory_.get(pos.value);
    TtrainInfo trainInfo = train.trainMemory_.get(pos.value);
    int spos = index(trainInfo, stName), epos = index(trainInfo, enName);
    dateRange saleDate = get<7>(trainInfo);
    int idx = startDate - saleDate.l;
    for(int i=spos;i<epos;i++){
        tickets.v[idx].v[i] += number;
    }

    std::vector<Torder> queue = queue_.all_similar(Torder(trainId, 0));
    int size = queue.size();
    for(int i=0;i<size;i++){
        Torder now = queue[i];
        TorderInfo alternative = orderMemory_.only(orderPointer(now.value, TorderInfo())).value;
        int nowTimestamp = get<0>(alternative);
        stationName nowStName = get<3>(alternative), nowEnName = get<5>(alternative);
        int nowNumber = get<8>(alternative);
        int nspos = index(trainInfo, nowStName), nepos = index(trainInfo, nowEnName);
        date nowStartDate = get<2>(alternative);
        if(nowStartDate != startDate){
            continue;
        }
        int maxNumber = int_max;
        for(int j=nspos;j<nepos;j++){
            maxNumber = min(maxNumber, tickets.v[idx].v[j]);
        }
        if(maxNumber >= nowNumber){
            for(int j=nspos;j<nepos;j++){
                tickets.v[idx].v[j] -= nowNumber;
            }
            timequeue_.remove(timequeue_.only(timePoser(nowTimestamp, 0)));
            queue_.remove(now);
        }
    }

    train.ticketMemory_.set(pos.value, tickets);
    refunded_.insert(timePoser(timestamp, 0));

    std::cout << '0' << std::endl;
    return;
}

void OrderSystem::clean(){
    userorder_.initialise(userorder_.file_name, true);
    queue_.initialise(queue_.file_name, true);
    timequeue_.initialise(timequeue_.file_name, true);
    refunded_.initialise(refunded_.file_name, true);
    orderMemory_.initialise(orderMemory_.file_name, true);
    return;
}