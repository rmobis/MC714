#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <thread>
#include <vector>
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

void tProcessMemoryRange(uint64_t *mem, size_t offset, size_t length, uint_fast64_t *rangeSum) {
	*rangeSum = processMemoryRange(mem, offset, length);
}

double processMemory(uint64_t *mem, size_t size, uint_fast64_t numThreads) {
	std::vector<std::pair<std::thread, uint_fast64_t>> threads(numThreads);
	for (size_t i = 0; i < numThreads; ++i) {
		threads[i].second = 0;
		// TODO: handle remaining
		if (i == numThreads - 1) {
			threads[i].first = std::thread(tProcessMemoryRange, mem, (size / numThreads) * i,  (size / numThreads) + (size % numThreads), &threads[i].second);
		} else {
			threads[i].first = std::thread(tProcessMemoryRange, mem, (size / numThreads) * i,  (size / numThreads), &threads[i].second);
		}
	}

	uint_fast64_t totalSum = 0;
	for (size_t i = 0; i < numThreads; ++i) {
		threads[i].first.join();
		totalSum += threads[i].second;
	}

	return static_cast<double>(totalSum) / size;
}

uint64_t *allocateMemory(size_t size) {
	return static_cast<uint64_t *>(malloc(size * sizeof(uint64_t)));
}

int main(int argc, char const *argv[]) {
	if (argc != 3) {
		std::cerr << "Not enough arguments!" << std::endl;
		return 1;
	}

	uint_fast64_t N = std::atoll(argv[1]);
	if (N == 0) {
		std::cerr << "Invalid arguments! N must be an integer greater than 0." << std::endl;
		return 1;
	}

	uint_fast64_t k = std::atoll(argv[2]);
	if (k == 0) {
		std::cerr << "Invalid arguments! k must be an integer greater than 0." << std::endl;
		return 1;
	}

	uint64_t *mem = allocateMemory(N * _2TO20);

	double mean = processMemory(mem, N * _2TO20, k);

	std::cout << mean << std::endl;

	return 0;
}
