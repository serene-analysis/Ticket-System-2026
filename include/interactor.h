#pragma once

#include "vector.hpp"
#include "account.h"
#include "train.h"
#include "order.h"

struct Interactor{
    bool end_;
    Interactor(){
        end_ = false;
        return;
    }
    ~Interactor() = default;
    sjtu::vector<std::string> readLine();
    void tian(AccountSystem &, TrainSystem &, OrderSystem &);
};