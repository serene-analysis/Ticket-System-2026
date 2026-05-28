#pragma once
#include "bpt.hpp"
#include "utils.h"
#include "account.h"
#include "train.h"

using Torder = string_info<int>;
using TtimeOrder = int_info<int>;
using TorderInfo = tuple<int, char64, date, stationName, Time, stationName, Time, int, int, char64>;
using TorderMemory = int_info<TorderInfo>;

struct OrderSystem{
    BPT<Torder> userorder_, queue_;
    BPT<TtimeOrder> timequeue_, refunded_;
    BPT<TorderMemory> orderMemory_;
    OrderSystem(){
        userorder_.initialise("userorder.data");
        queue_.initialise("queue.data");
        timequeue_.initialise("timequeue.data");
        refunded_.initialise("refunded.data");
        orderMemory_.initialise("orderMemory.data");
        return;
    }
    ~OrderSystem(){

        return;
    }
    void buy_ticket(char64, char64, date, stationName, stationName, int, std::string, AccountSystem &, TrainSystem &, int);
    void query_order(char64, AccountSystem&);
    TorderInfo back_order(char64, int, AccountSystem&, TrainSystem&);
    void refund_ticket(char64, int, AccountSystem&, TrainSystem&);
    void clean();
};