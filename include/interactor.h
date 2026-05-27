#pragma once

#include "account.h"
#include "train.h"
#include "order.h"
#include <vector>

struct Interactor{
    bool end_;
    Interactor(){
        end_ = false;
        return;
    }
    ~Interactor() = default;
    std::vector<std::string> readLine();
    void tian(AccountSystem &, TrainSystem &, OrderSystem &);
};