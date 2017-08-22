run:
	g++ -o seq seq.cpp -std=c++11 -lpthread
	g++ -o thread thread.cpp -std=c++11 -lpthread
	g++ -o proc proc.cpp -std=c++11 -lpthread


	time ./seq 64
	time ./thread 64 4
	time ./proc 64 4
