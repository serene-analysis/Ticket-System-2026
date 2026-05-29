//#pragma GCC optimize(3)
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
    //freopen("test.in","r",stdin);
    //freopen("mine.out","w",stdout);
    //std::cout << "???" << std::endl;
    std::ios::sync_with_stdio(false);
    std::cin.tie(0), std::cout.tie(0);
    interactor.tian(account, train, order);
    return 0;
}