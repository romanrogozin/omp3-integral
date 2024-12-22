#include <iostream>
#include <vector>
#include <random>
#include <omp.h>
#include <chrono>
#include <iomanip>

#define _USE_MATH_DEFINES
#include <math.h>


// Функция, подлежащая интегрированию
double func(double x) {
    return std::sin(x); // Например, интеграл sin(x)
}

// Метод прямоугольников с OpenMP
double integrate(double a, double b, int n) {
    double h = (b - a) / n; // Ширина прямоугольника
    double integral = 0.0;

#pragma omp parallel for reduction(+:integral)
    for (int i = 0; i < n; ++i) {
        double x = a + i * h; // Точка слева для текущего прямоугольника
        integral += func(x) * h;
    }

    return integral;
}

int main() {
    std::vector<int> thread_experiments = { 1, 2, 4, 8, 16, 24, 32, 64, 128, 256, 512, 1024 };
    std::vector<int> vector_size_experiments = { 1'000'000, 10'000'000, 100'000'000, 500'000'000, 1'000'000'000 };
    int runs_count = 5;

    // Границы интегрирования
    double a = 0.0;  // Нижний предел
    double b = M_PI; // Верхний предел (Pi)

    for (int i = 0; i < vector_size_experiments.size(); i++)
    {
        int vector_size_experiment = vector_size_experiments[i];

        for (int j = 0; j < thread_experiments.size(); j++)
        {
            int current_thread_experiment = thread_experiments[j];
            omp_set_num_threads(current_thread_experiment);
            //simple
            double total_execution_time = 0;
            double result = 0;
            for (int k = 0; k < runs_count; k++)
            {
                auto start = std::chrono::high_resolution_clock::now();

                result = integrate(a, b, vector_size_experiment);

                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> duration = end - start;
                total_execution_time += duration.count();
            }
            double avg_exexution_time = total_execution_time / runs_count;

            std::cout << std::setprecision(10) << vector_size_experiment << ";" << current_thread_experiment << ";" << avg_exexution_time << ";" << std::endl;
        }
    }

    std::cout << "Waiting for exit...";
    int temp;
    std::cin >> temp;
    return 0;
}