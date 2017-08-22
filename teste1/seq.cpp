#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <random>

#define _2TO20 1048576


uint_fast64_t processMemoryRange(uint64_t *mem, size_t offset, size_t length) {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<uint64_t> dist(0, 1001);

	for (size_t i = 0; i < length; ++i) {
		mem[offset + i] = dist(mt);
	}

	uint_fast64_t rangeSum = 0;
	for (size_t i = 0; i < length; ++i) {
		rangeSum += mem[offset + i];
	}

	return rangeSum;
}

double processMemory(uint64_t *mem, size_t size) {
	return static_cast<double>(processMemoryRange(mem, 0, size)) / size;
}

uint64_t *allocateMemory(size_t size) {
	return (uint64_t *) malloc(size * sizeof(uint64_t));
}

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		std::cerr << "Not enough arguments!" << std::endl;
		return 1;
	}

	uint_fast64_t N = std::atoll(argv[1]);
	if (N == 0) {
		std::cerr << "Invalid arguments! N must be an integer greater than 0." << std::endl;
		return 1;
	}

	uint64_t *mem = allocateMemory(N * _2TO20);

	double mean = processMemory(mem, N * _2TO20);

	std::cout << mean << std::endl;

	return 0;
}
