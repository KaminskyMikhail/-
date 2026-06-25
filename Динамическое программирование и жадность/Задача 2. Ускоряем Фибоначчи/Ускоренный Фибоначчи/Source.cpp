#include <iostream>
#include <vector>
class Fibonacсi {
private:
	int count;
	std::vector<int>arr;

	void filling() {
		for (int i = 0; i < count; i++) {
			arr[i] = fibonacci(i);
		}
	}

	int fibonacci(int i) {
		if (i == 0) return 0;
		if (i == 1) return 1;
		return fibonacci(i - 1) + fibonacci(i - 2);
	}

public:

	Fibonacсi(int count) : count(count), arr(count){}
	void print() {
		filling();
		for (int i = 0; i < count; i++) {
			std::cout << arr[i] << "\t";
		}
	}
};

int main() {
	Fibonacсi fib(5);
	fib.print();
}
