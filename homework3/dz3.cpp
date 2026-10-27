#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <semaphore>
#include <chrono>
#include <syncstream>


// Спільний ресурс: спільний лог-звіт
std::vector<std::string> deliveryLog;


// TODO 1: Створіть класичний семафор із лімітом 3 (максимум 3 кур'єри пакують товари одночасно)
 std::counting_semaphore<3> terminalSemaphore(3);


// TODO 2: Створіть мютекс для захисту запису у спільний вектор deliveryLog
 std::mutex logMutex;


// Функція, яку виконує кожен кур'єр (потік)
void courierTask(int courierId) {
    std::osyncstream(std::cout) << "[Courier " << courierId << "] Arrived at hub, waiting for free packing terminal...\n";


    // TODO 3: Захопіть перепустку у семафора (acquire). 
    // Якщо всі 3 термінали зайняті, потік зупиниться тут.
    terminalSemaphore.acquire();


    std::osyncstream(std::cout) << "  >>> [Courier " << courierId << "] STARTED packing at terminal!\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(800)); // Імітація пакування
    
    std::string report = "Courier #" + std::to_string(courierId) + " successfully delivered package.";


    // TODO 4: Безпечно додайте звіт у спільний вектор deliveryLog.
    // Використайте std::lock_guard з вашим мютексом для захисту цієї критичної секції.
    {
        // Ваш код тут (lock_guard + push_back)
        std::lock_guard<std::mutex> lock(logMutex);
        deliveryLog.push_back(report);
    }


    std::osyncstream(std::cout) << "  <<< [Courier " << courierId << "] FINISHED packing. Freeing terminal.\n";


    // TODO 5: Поверніть перепустку семафору (release).
    terminalSemaphore.release();
}


int main() {
    const int TOTAL_COURIERS = 7;
    
    // Створюємо вектор для зберігання потоків (будемо використовувати і std::thread, і std::jthread)
    // У цьому ДЗ задіяно 7 кур'єрів. Перші 4 нехай будуть std::thread, а наступні 3 — std::jthread.
    std::vector<std::thread> classicThreads;
    std::vector<std::jthread> modernJThreads;


    std::cout << "[Hub] Starting dispatch for " << TOTAL_COURIERS << " couriers...\n\n";


    // TODO 6: Запустіть перших 4 кур'єрів за допомогою std::thread та додайте їх у вектор classicThreads.
    // Наприклад, у циклі від 1 до 4: classicThreads.emplace_back(courierTask, i);
    for (int i = 1; i <= 4; i++)
        classicThreads.emplace_back(courierTask, i);



    // TODO 7: Запустіть наступних 3 кур'єрів (від 5 до 7) за допомогою std::jthread та додайте у vector modernJThreads.
    // Використовуйте jthread, щоб побачити його автоматичний join у дії.
    for (int i = 5; i <= 7; i++)
        modernJThreads.emplace_back(std::jthread(courierTask, i));



    // Чекаємо на завершення класичних потоків
    for (auto& t : classicThreads) {
        if (t.joinable()) {
            t.join();
        }
    }


    // Примітка: для modernJThreads явний join писати необов'язково, але можна залишити для порядку.
    modernJThreads.clear();

    std::cout << "\n[Hub] All couriers finished work! Final Delivery Log:\n";
    std::cout << "--------------------------------------------------\n";
    for (const auto& record : deliveryLog) {
        std::cout << record << "\n";
    }
    std::cout << "--------------------------------------------------\n";


    return 0;
}