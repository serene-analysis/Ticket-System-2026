#pragma once
#include "bpt.hpp"
#include "utils.h"

using Ttrain = string_info<int>;
using TtrainInfo = tuple<int, stationNames, int, int100, dailyTime, int100, int100, dateRange, char>;
using TtrainMemory = int_info<TtrainInfo>;
using TstationerInfo = stationName_info<char64>;
//stationNum, stations[], seatNum, prices[], startTime, travelTimes[],
//    stopoverTimes[], saleDate, type, remaining_tickets[max_date][max_num]

struct TrainSystem{
    BPT<Ttrain> hidden_, released_;
    BPT<TstationerInfo> stationer_;
    ARRAY<mat> ticketMemory_;
    BPT<TtrainMemory> trainMemory_;
    TrainSystem(){
        hidden_.initialise("hidden.data");
        released_.initialise("released.data");
        stationer_.initialise("stationer.data");
        trainMemory_.initialise("trainMemory.data");
        ticketMemory_.initialise("ticketMemory.data");
        return;
    }
    ~TrainSystem(){
        
        return;
    }
    void add_train(char64, TtrainInfo, mat);
    void delete_train(char64);
    void release_train(char64);
    void query_train(char64, date);
    void query_ticket(stationName, stationName, date, std::string);
    void query_transfer(stationName, stationName, date, std::string);
    void clean();
};

int index(TtrainInfo &info, stationName station);

int pure_distance(TtrainInfo &info, int st, int en);

int cost(TtrainInfo &info, int st, int en);

int max_tickets(mat tickets, int st, int en, int idx);

int leaving_gone_days(TtrainInfo &info, int x);

dailyTime leaving_time(TtrainInfo &info, int x);

int leaving_time_since_begin(TtrainInfo &info, int x);