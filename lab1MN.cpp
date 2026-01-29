/*
Mustafa Nazeer
CSE - 3320 - 002
Lab 1 - Amdahl's Law
*/

#include <iostream>
#include <array>
#include <random>
#include <algorithm> // For std::sort
#include <cstddef> // For std::size_t
#include <utility> // For std::pair
#include <ctime>
#include <iomanip>
#include <cmath> // For std::abs


constexpr int ARRAY_SIZE{100'000};
std::array<int, ARRAY_SIZE> arr1;
std::array<int, ARRAY_SIZE> arr2;

// Random number generator
std::random_device rd{};
std::seed_seq ss{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
std::mt19937 mt{ss};
std::uniform_int_distribution range(0,10'000);

// Printing/Debugging functions
void printArray(const std::array<int, ARRAY_SIZE>&, bool=false);
void printResults(const std::pair<int, int>&, bool=false);
void printTimes(double, double, bool=false);
void printReport(double, double, double, double);


void loadArray(std::array<int, ARRAY_SIZE>& arr){
    for(auto& i : arr){
        i = range(mt);
    }
}

// Linear Search
const std::pair<int, int> unoptimized_algo(const std::array<int, ARRAY_SIZE>& arr, int target){
    for(std::size_t i{}; i < ARRAY_SIZE; ++i){
        if (arr[i] == target)
            return {i, i + 1};
    }
    // Target not found
    return {-1, ARRAY_SIZE - 1};
}

// Binary Search
const std::pair<int, int> optimized_algo(const std::array<int, ARRAY_SIZE>& arr, int target){

    std::size_t start{};
    std::size_t end{ARRAY_SIZE - 1};

    int iterations{};
    while(start <= end){

        std::size_t middle{start + (end - start) / 2};

        if(arr[middle] == target)
            return {middle, iterations};

        if(arr[middle] > target)
            end = middle - 1;

        if(arr[middle] < target)
            start = middle + 1;
        
        ++iterations;
    }

    // Target not found
    return {-1, iterations};
}

int main(){

    // Random index generator
    auto getRandomIndex = []() -> int {
        std::uniform_int_distribution arrayRange(0, ARRAY_SIZE - 1);
        return arrayRange(mt);
    };



    // Unoptimized

    clock_t start_total_unoptimized{clock()};
    loadArray(arr1);
    int target1 = arr1[getRandomIndex()];

    clock_t start_processing_unoptimized{clock()};
    std::pair<int, int> p1{unoptimized_algo(arr1, target1)};
    clock_t end_processing_unoptimized{clock()};

    clock_t end_total_unoptimized{clock()};

    double total_unoptimized{(static_cast<double>(end_total_unoptimized) - start_total_unoptimized) / CLOCKS_PER_SEC};
    double processing_unoptimized{(static_cast<double>(end_processing_unoptimized) - start_processing_unoptimized) / CLOCKS_PER_SEC};

    printTimes(processing_unoptimized, total_unoptimized);




    // Optimized, must sort first for a binary search

    clock_t start_total_optimized{clock()};
    loadArray(arr2);
    int target2 = arr2[getRandomIndex()];

    clock_t start_processing_optimized{clock()};
    std::sort(std::begin(arr2), std::end(arr2));
    std::pair<int, int> p2{optimized_algo(arr2, target2)};
    clock_t end_processing_optimized{clock()};

    clock_t end_total_optimized{clock()};

    double total_optimized{(static_cast<double>(end_total_optimized) - start_total_optimized) / CLOCKS_PER_SEC};
    double processing_optimized{(static_cast<double>(end_processing_optimized) - start_processing_optimized) / CLOCKS_PER_SEC};

    printTimes(processing_optimized, total_optimized, true);

    printReport(processing_unoptimized, total_unoptimized, processing_optimized, total_optimized);

    return 0;
}

void printReport(double processing_unoptimized, double total_unoptimized, double processing_optimized, double total_optimized){

    std::cout << "\n--- Amdahl's Law Analysis Data ---\n";

    // P: Proportion of optimizable code
    double P = processing_unoptimized / total_unoptimized;

    // S: Speedup of the optimized part
    double S = processing_unoptimized / processing_optimized;

    // Measured Overall Speedup
    double measured_speedup = total_unoptimized / total_optimized;

    // Theoretical Overall Speedup using Amdahl's Law
    double theoretical_speedup = 1 / ((1 - P) + (P / S));

    std::cout << "Proportion of Optimizable Code: " << P << ".\n";
    std::cout << "Speedup of Optimized Part: " << S << ".\n\n";
    std::cout << "Measured Overall Speedup: " << measured_speedup << ".\n";
    std::cout << "Theoretical Overall Speedup w/ Amdahl's Law: " << theoretical_speedup << ".\n";

    std::cout << "Measured and Theoretical Overall Speedup's Percent Difference: " << (std::abs(theoretical_speedup - measured_speedup) * 2 / (theoretical_speedup + measured_speedup)) * 100 << "%.\n\n";
}

// Printer for trial outcomes
void printTimes(double processing, double total, bool isOptimized){
    isOptimized ? std::cout << "---  Running Optimized Version ---\n" : std::cout << "---  Running Unoptimized Version ---\n";

    std::cout << "Processing Time: " <<  std::fixed << std::setprecision(6) << processing << " seconds.\n";
    std::cout << "Total Time: " <<  std::fixed << std::setprecision(6) << total << " seconds.\n\n";
}

// Printer for debugging
void printArray(const std::array<int, ARRAY_SIZE>& arr, bool isSorted){
    int j{};

    isSorted ? std::cout << "\n---Sorted " : std::cout << "\n---Unsorted ";
    std::cout << "Array---\n";

    for(const auto& i : arr){ 
        if(j == 20)
           std::cout << '\n';

        std::cout << i << ' ';
        ++j;
    }
    std::cout << '\n';
}

// Printer for search efficiency
void printResults(const std::pair<int, int>& p, bool isOptimized){

 isOptimized ? std::cout << "OPTIMIZED ARRAY:\n" : std::cout << "UNOPTIMIZED ARRAY:\n";

    if(p.first != -1){
        std::cout << "Target found at index " << p.first << ", in " << p.second << " iterations.\n\n";
    } else
        std::cout << "Target not found.\n";
}