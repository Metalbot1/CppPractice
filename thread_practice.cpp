#include "thread_practice.h"

int run_thread_practice(){
    //add_vector_test(true, 10000, 3);

    delay_thread_handler(10);

    return 0;
}

//--------------------------------------------------------------------------------------------------------------------//

//Let's use multithreading for a simple hello world program.
//This is a run function which will launch a specified number of threads and handle them all.
void hello_from_thread_handler(unsigned int num_threads){
    std::vector<std::thread> threadbank;

    for(int i = 0; i < num_threads; i++){
        threadbank.emplace_back(hello_from_thread, i);
    }

    std::cout << "Hello from main!" << std::endl;

    for(auto& t : threadbank){
        t.join();
    }
}

//The hello function to be called by the thread
void hello_from_thread(int thread_id){
    std::cout << "Hello from thread #" + std::to_string(thread_id) << std::endl;
}

//--------------------------------------------------------------------------------------------------------------------//

//I'm going to test some things about threads, by building a simple random-delay kernel.
//I will run this kernel on several threads to see how the joining process works more thoroughly.

void delay_thread_handler(unsigned int num_threads){
    std::vector<std::thread> threadbank;
    std::vector<int> times = std::vector<int>(num_threads, 0);

    for(int ti = 0; ti < num_threads; ti++) {
        double range = 10.0;
        times[ti] = static_cast<int>(std::round(
                (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX)) * range));
    }

    print_functions::print_vector(times, "Times");

    for(int ti = 0; ti < num_threads; ti++){
        threadbank.push_back(std::thread(delay, times[ti]));
    }

    std::cout << "Threads launched" << std::endl;

    //Join the threads sequentially.
    /*for(int ti = 0; ti < num_threads; ti++){
        threadbank[ti].join();
        std::cout << "Joined thread #" << ti << std::endl;
    }//*/

    //Join the threads individually
    bool all_joined = false;
    std::vector<bool> joined = std::vector<bool>(num_threads, false);
    while(!all_joined){
        for(int ti = 0; ti < num_threads; ti++){
            if(!joined[ti]){
                if(threadbank[ti].joinable()) {
                    threadbank[ti].join();
                    joined[ti] = true;
                    std::cout << "Joined thread #" << ti << std::endl;
                }
            }
        }

        all_joined = true;
        for(int ti = 0; ti < num_threads; ti++){
            all_joined = all_joined && joined[ti];
        }
    }


    std::cout << "Done random delay test." << std::endl;
}

//Delays for a random amount of seconds between 0 and 10.
void delay(unsigned int sec){
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}


//--------------------------------------------------------------------------------------------------------------------//

//Now let's use multithreading to add two input vectors together.
//This is an example of an embarrassingly parallel problem
void add_vector_test(bool printout, unsigned int vec_size, unsigned int num_threads){
    std::vector<double> A, B;

    //then, we need to initialize our vectors
    A = std::vector<double>(vec_size, 0.0);
    B = std::vector<double>(vec_size, 0.0);

    //now, we will populate A and B with random doubles between 0 and 1
    for(int i = 0; i < vec_size; i++){
        A[i] = (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX));
        B[i] = (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX));
    }

    //Run multithreaded vector addition and time it.
    print_functions::print_linebreak(30);
    time_t start_time = std::clock();
    vector_adding_thread_handler(printout, vec_size, num_threads, A, B);
    float runtime = static_cast<float>(start_time) / static_cast<float>(CLOCKS_PER_SEC);
    std::cout << "Finished multithreaded calculation in " + std::to_string(runtime) << " seconds" << std::endl;

    print_functions::print_linebreak(30);

    start_time = std::clock();
    vector_adding_thread_handler(printout, vec_size, 1, A, B);
    runtime = static_cast<float>(start_time) / static_cast<float>(CLOCKS_PER_SEC);
    std::cout << "Finished single threaded calculation in " + std::to_string(runtime) << " seconds" << std::endl;

    print_functions::print_linebreak(30);

    std::cout << "Done vector addition test!";

}

void vector_adding_thread_handler(bool printout, unsigned int vec_size, unsigned int num_threads, std::vector<double>& A, std::vector<double>& B){
    //first, we should declare our variables
    std::vector<std::thread> threadbank;
    std::vector<double> output;
    std::mutex mu;

    //Split our thread calls into appropriately sized chunks
    std::vector<int> index_chunks;
    {
        unsigned int current_index, ideal_chunk;
        current_index = 0;
        ideal_chunk = static_cast<unsigned int>(std::ceil(static_cast<double>(vec_size)/static_cast<double>(num_threads)));

        while(current_index < vec_size){
            if(current_index + ideal_chunk < vec_size){
                index_chunks.push_back(ideal_chunk);
                current_index += ideal_chunk;
            }else{
                index_chunks.push_back(vec_size - current_index);
                break;
            }
        }
    }

    //Print out the intended chunk configuration.
    if(printout)
        print_functions::print_vector(index_chunks, std::string("index_chunks"));

    if(index_chunks.size() != num_threads){
        std::cout << "Error in index_chunk creation. Size does not match the specified number of threads." << std::endl;
        return;
    }

    //Initialize threads
    unsigned int current_index = 0;
    for(unsigned int ti = 0; ti < num_threads; ti++){
        threadbank.emplace_back(add_element, std::ref(mu), current_index, current_index + index_chunks[ti], A, B, std::ref(output));
        threadbank[ti].detach();
        current_index += index_chunks[ti];
    }

    //Printout that we've finished initializing the threads
    if(printout)
        std::cout << "Threads initialized up to index " << std::to_string(current_index) << std::endl;

    //Rejoin detached daemon threads
    for(unsigned int ti = 0; ti < num_threads; ti++){
        while(!threadbank[ti].joinable()) {}
        threadbank[ti].join();
        std::cout << "Joined thread #" << std::to_string(ti) << std::endl;
    }

    if(printout) {
        print_functions::print_vector(std::ref(output), "output");
    }
}

void add_element(std::mutex& mu, unsigned int begin, unsigned int end, const std::vector<double>& A, const std::vector<double>& B, std::vector<double>& output){
    while(begin != end){
        double op = A[begin] + B[begin];

        mu.lock();
        output[begin] = op;
        mu.unlock();

        begin++;
    }
}

//--------------------------------------------------------------------------------------------------------------------//