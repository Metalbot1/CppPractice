#include "threadbank.h"

///Test Functions
void add_one(int& num){
    num++;
}

void multiply(int& num, int factor){
    num = num*factor;
}

void wait(int sec){
    clock_t starttime = std::clock();
    while((std::clock() - starttime)/CLOCKS_PER_SEC < sec);
    std::cout << "Done waiting " << std::to_string(sec) << " seconds." << std::endl;
}

void print(int num){
    std::cout << std::to_string(num) << std::endl;
}

////
int main(){

    return 0;
}




