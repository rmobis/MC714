#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/mman.h>
#include <random>
#include <cstring>

#define _2TO20 1048576

uint64_t *allocateMemory(size_t size) {
	return static_cast<uint64_t *>(mmap(NULL, size * sizeof(uint64_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
}

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

double processMemory(uint64_t *mem, size_t size, uint_fast64_t numProcs) {
	uint64_t *rangeSums = allocateMemory(numProcs);
	memset(rangeSums, 0, numProcs * sizeof(uint64_t));

	for (size_t i = 0; i < numProcs; ++i) {
		switch (fork()) {
			case 0:
				// TODO: handle remaining
				rangeSums[i] = processMemoryRange(mem, (size / numProcs) * i,  (size / numProcs));
				if (i == numProcs - 1) {
					rangeSums[i] = processMemoryRange(mem, (size / numProcs) * i,  (size / numProcs) + (size % numProcs));
				} else {
					rangeSums[i] = processMemoryRange(mem, (size / numProcs) * i,  (size / numProcs));
				}
				exit(0);
				break;

			default:
				break;
		}
	}

	for (size_t i = 0; i < numProcs; ++i) {
		wait(NULL);
	}

	uint_fast64_t totalSum = 0;
	for (size_t i = 0; i < numProcs; ++i) {
		totalSum += rangeSums[i];
	}

	return static_cast<double>(totalSum) / size;
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
