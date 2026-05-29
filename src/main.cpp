#include "interactor.h"
#include "account.h"
#include "train.h"
#include "order.h"
bool mbeg;
AccountSystem account;
TrainSystem train;
OrderSystem order;
Interactor interactor;
bool mend;
signed main(){
    //fprintf(stderr, "mem : %d\n", &mbeg-&mend);
    freopen("test.in","r",stdin);
    freopen("mine.out","w",stdout);
    //std::cout << "???" << std::endl;
    interactor.tian(account, train, order);
    return 0;
}