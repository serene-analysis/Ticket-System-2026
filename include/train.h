#pragma once
#include "bpt.hpp"
#include "utils.h"

using Ttrain = string_info<int>;
using TtrainMemory = int_info<tuple<int, stationNames, int, int100, int, int100, int100, int, int, char, mat>>;
using TstationerInfo = string_info<char20>;

struct TrainSystem{
    BPT<Ttrain> hidden_, released_;
    BPT<TstationerInfo> stationer_;
    BPT<TtrainMemory> trainMemory_;
    TrainSystem(){
        hidden_.initialise("hidden.data");
        released_.initialise("released.data");
        stationer_.initialise("stationer.data");
        trainMemory_.initialise("trainMemory.data");
        return;
    }
    ~TrainSystem(){
        
        return;
    }
};