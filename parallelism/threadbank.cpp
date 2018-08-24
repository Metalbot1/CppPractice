#include "threadbank.h"

void add_one(int& num){
    num++;
}

int main(){
    job J;

    int counter = 0;

    J.add_task(task<int&>(add_one, std::make_tuple(std::ref(counter))));

    std::cout << counter << std::endl;
    J.run_tasks();
    std::cout << counter << std::endl;
    J.run_tasks();
    std::cout << counter << std::endl;

    return 0;
}




