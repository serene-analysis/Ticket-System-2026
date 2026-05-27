#include "interactor.h"
#include "account.h"
#include "train.h"
#include "order.h"
AccountSystem account;
TrainSystem train;
OrderSystem order;
Interactor interactor;
signed main(){
    //freopen("test.in","r",stdin);
    //freopen("mine.out","w",stdout);
    //std::cout << "???" << std::endl;
    interactor.tian(account, train, order);
    return 0;
}