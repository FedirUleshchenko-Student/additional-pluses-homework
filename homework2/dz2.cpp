#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <execution> // Обов'язково для std::execution::par та seq
#include <chrono>
#include <random>


// Допоміжна функція для вимірювання часу
template <typename Func>
double measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    return duration.count();
}


// ============================================================================
// TASK 1: Послідовна редукція
// ============================================================================
// TODO: Обчисліть та поверніть суму всіх елементів вектора 'data', 
// використовуючи std::reduce з політикою std::execution::seq (або std::accumulate).
long long sumSequential(const std::vector<int>& data) {
    // TODO: Ваш код тут
    return std::reduce(std::execution::seq, data.begin(), data.end());
}


// ============================================================================
// TASK 2: Паралельна редукція
// ============================================================================
// TODO: Обчисліть та поверніть суму всіх елементів вектора 'data', 
// використовуючи std::reduce з політикою std::execution::par.
long long sumParallel(const std::vector<int>& data) {
    // TODO: Ваш код тут
    return std::reduce(std::execution::par, data.begin(), data.end());
}


// ============================================================================
// TASK 3: Паралельна модифікація елементів
// ============================================================================
// TODO: Піднесіть кожен елемент вектора 'data' до квадрата (x = x * x), 
// використовуючи std::for_each з політикою std::execution::par.
void squareParallel(std::vector<int>& data) {
    // TODO: Ваш код тут
    std::for_each(std::execution::par, data.begin(), data.end(),
        [](int& x) {x = x * x; });
}


int main() {
    // Створюємо вектор на 10 мільйонів елементів
    const size_t SIZE = 10'000'000;
    std::cout << "Generating " << SIZE << " elements...\n";
    
    std::vector<int> numbers(SIZE);
    std::mt19937 rng(42); // Фіксований seed для повторюваності
    std::uniform_int_distribution<int> dist(1, 10);
    
    for (size_t i = 0; i < SIZE; ++i) {
        numbers[i] = dist(rng);
    }


    std::cout << "\n=================== TASK 1 & 2: Sum Reduction ===================\n";
    long long seqSum = 0;
    long long parSum = 0;


    // TODO TASK 4a: Вирядіть виклики sumSequential та sumParallel через measureTime 
    // та порівняйте отримані результати й час виконання.


    double seqTime = measureTime([&]() {
        seqSum = sumSequential(numbers);
    });


    double parTime = measureTime([&]() {
        parSum = sumParallel(numbers);
    });


    std::cout << "Sequential Sum: " << seqSum << " | Time: " << seqTime << " ms\n";
    std::cout << "Parallel Sum:   " << parSum << " | Time: " << parTime << " ms\n";
    if (parTime > 0) {
        std::cout << "Speedup: " << seqTime / parTime << "x\n";
    }


    std::cout << "\n=================== TASK 3: Parallel Square ===================\n";
    // TODO TASK 4b: Заміряйте час виконання squareParallel для вектора 'numbers'.


    double squareTime = measureTime([&]() {
         squareParallel(numbers);
    });


    std::cout << "Parallel Square Time: " << squareTime << " ms\n";
    std::cout << "First 5 elements after square: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << numbers[i] << " ";
    }
    std::cout << "\n";


    return 0;
}