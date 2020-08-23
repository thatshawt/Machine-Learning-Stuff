#pragma once
#include <vector>
#include <random>
#include <chrono>

template <class T>
static void fill_n(std::vector<T> v, T n) {
	for (int i = 0; i < v.capacity(); i++) {
		v.push_back(n);
	}
}

template<class type>
static void print_array(type* begin, int size) {
	for (int i = 0; i < size;i++) {
		printf("%d, ", *((begin + i)));
	}
	printf("\n");
}

static int m = pow(2, 32);
static int a = 1664525;
static int c = 1013904223;
static int x = time(NULL);
static int irand() {
	x = (a * x + c) % m;
	return x;
}
//this thing is so trash lol
static float frand(int max) {
	return (float)abs((((double)abs(irand()) / (double)m) * (double)max));
}

class Timer {
private:
	std::chrono::steady_clock::time_point time;
public:
	void start(void) {
		this->time = std::chrono::high_resolution_clock::now();
	}

	long long getNano(void) {
		auto finish = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(finish - time).count();
	}

	long long getMilli(void) {
		auto finish = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(finish - time).count();
	}
};