#include <iostream>

int search_function(int arr[], int size, int var);

int main() {
    const int size = 9; int var, count = 0;
    std::cout << "Enter number: "; std::cin >> var;
    int arr[size] = { 14, 16, 19, 32, 32, 32, 56, 69, 72 };

    int target = search_function(arr, size, var);
    if (arr[target] == arr[target + 1]) {
        for (int i = target + 1; i < size - 1; i++) {
            count++;
        }
    }
    else {
        for (int i = target; i < size - 1; i++) {
            count++;
        }
    }
    std::cout << "Number of elements in the array greater than " 
              << var << ": " << count << std::endl;

	return EXIT_SUCCESS;
}
int search_function(int arr[], int size, int var) {
    int left = 0, right = size - 1;
    while (left < right) {
        int middle = (left + right) / 2;
        if (arr[middle] > var) {
            right = middle - 1;
        }
        else if (arr[middle] < var) {
            left = middle + 1;
        }
        else if (arr[middle] == var) {
            return middle;
        }
    }
}