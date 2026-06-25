#include <iostream>

//экспоненциальная сложность по времени, линейная по памяти

int fibonacci(int n);

int main() {
    int count;

    std::cin >> count;

    for (int i = 0; i < count; i++) {
        std::cout << fibonacci(i) << '\t';

    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}

int fibonacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}