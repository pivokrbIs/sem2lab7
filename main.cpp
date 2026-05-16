#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstdlib>
#include <string>

std::mutex cout_mutex;

void bubble_sort_and_print(double* arr, int left, int right,
                           const std::string& thread_name, int delay_ms)
{
    int n = right - left + 1;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = left; j < right - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }

    for (int i = left; i <= right; ++i) {
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << thread_name << " " << arr[i] << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
}

int main()
{
    const int N = 20;
    double* arr = new double[N];

    std::srand(42);
    for (int i = 0; i < N; ++i) {
        arr[i] = (std::rand() % 1000) / 1000.0;
    }

    std::cout << "Original array:\n";
    for (int i = 0; i < N; ++i) std::cout << arr[i] << " ";
    std::cout << "\n\n";

    const int delay = 10;

    std::thread t1(bubble_sort_and_print, arr, 0, N/2 - 1, "thread1", delay);
    std::thread t2(bubble_sort_and_print, arr, N/2, N - 1, "thread2", delay);

    t1.join();
    t2.join();

    std::thread t3(bubble_sort_and_print, arr, 0, N - 1, "thread3", delay);
    t3.join();

    std::cout << "\nAll child threads have finished. Main thread exiting.\n";

    delete[] arr;
    return 0;
}
