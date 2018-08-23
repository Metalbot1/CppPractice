#pragma once

#include <thread>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <random>
#include "../util/utilities.h"
#include <mutex>

int main();

void hello_from_thread(int thread_id);
void hello_from_thread_handler(unsigned int num_threads);

void add_vector_test(bool printout, unsigned int vec_size, unsigned int num_threads);
void vector_adding_thread_handler(bool printout, unsigned int vec_size, unsigned int num_threads, std::vector<double>& A, std::vector<double>& B);
void add_element(std::mutex& mu, unsigned int begin, unsigned int end, const std::vector<double>& A, const std::vector<double>& B, std::vector<double>& output);

void delay_thread_handler(unsigned int num_threads);
void delay(unsigned int sec);
