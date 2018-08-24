#include "threadbank.h"

void add_one(int& num){
    num++;
}

void multiply(int& num, int factor){
    num = num*factor;
}

int main(){
    job J;

    int counter = 0;

    J.add_task(add_one, std::make_tuple(std::ref(counter)));
    J.add_task(multiply, std::make_tuple(std::ref(counter), 2));

    std::cout << counter << std::endl;
    J.run_all_tasks();
    std::cout << counter << std::endl;
    J.run_all_tasks();
    std::cout << counter << std::endl;

    return 0;
}




