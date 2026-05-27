#pragma once
#include "bpt.hpp"
#include "utils.h"

using Torder = string_info<int>;
using TorderMemory = int_info<tuple<int, char20, char20, int, stationName, stationName, int>>;

struct OrderSystem{
    BPT<Torder> userorder_, queue_;
    BPT<TorderMemory> orderMemory_;
    OrderSystem(){
        userorder_.initialise("userorder.data");
        queue_.initialise("queue.data");
        orderMemory_.initialise("orderMemory.data");
        return;
    }
    ~OrderSystem(){

        return;
    }
};