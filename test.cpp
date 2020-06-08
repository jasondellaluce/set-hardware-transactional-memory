#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <string.h>
#include <sys/time.h>
#include "lock-free-set.h"
#include "coarse-lock-set.h"
#include "itm-htm-set.h"
#include "rtm-htm-set.h"

void threadJob(Set *set, int id)
{
	for(int i = 0; i < 10; i++) {
		set->add(id + 1);
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
		if(set->contains(id + 1))
			set->remove(id + 1);
	}
}

bool testConcurrent(Set* set) {
	std::vector<std::thread> v(8);
	int i = 0;
    for(auto& t: v) {
        t = std::thread([set, i]{
			threadJob(set, i);
		});
		i++;
	}
    for(auto& t: v) {
        t.join();
	}
	for(int i = 8; i > 0; i--)
		if(set->contains(i))
			return false;
	return true;
}

bool testSequential(Set* set) {
	for(int i = 100; i > 0; i--)
		if(!set->add(i))
			return false;
	for(int i = 100; i > 0; i--)
		if(set->add(i))
			return false;
	for(int i = 100; i > 0; i--)
		if(!set->contains(i))
			return false;
	for(int i = 95; i > 0; i--)
		if(!set->remove(i))
			return false;
	for(int i = 95; i > 0; i--)
		if(set->remove(i))
			return false;
	if(set->remove(101))
		return false;
	if(set->remove(-1))
		return false;
	if(!set->remove(99))
		return false;
	return true;
}

void threadBenchmark(Set* set, int id,  int upperLimit, int addPerc, int removePerc) {
	for(int i = 0; i < upperLimit; i++) {
		if((i % addPerc) == 0) {
			set->add(rand() % 100);
		}
		else if((i % removePerc) == 0) {
			set->remove(rand() % 100);
		}
		else {
			set->contains(rand() % 100);
		}
	}
}

void benchmark(Set* set, int upperLimit, int threadCount, int addPerc, int removePerc) {
	struct timeval stop, start;
	srand(time(NULL));
	
	/* Obtain time before execution of the task */
	gettimeofday(&start, NULL);
	
	std::vector<std::thread> v(threadCount);
	int i = 0;
    for(auto& t: v) {
        t = std::thread([set, i, upperLimit, addPerc, removePerc] {
			threadBenchmark(set, i, upperLimit, addPerc, removePerc);
		});
		i++;
	}
    for(auto& t: v) {
        t.join();
	}
	
	/* Obtain time difference between start and finish of task execution */
	gettimeofday(&stop, NULL);
	double exec_time_in_seconds = ((double)(stop.tv_sec - start.tv_sec) * 1000000 
		+ (double)(stop.tv_usec - start.tv_usec)) / 1000000.0;
	
	/* Print time elapsed in seconds*/
	std::cout << exec_time_in_seconds << std::endl << std::flush;
}

int main(int argc, char** argv)
{	
	if(argc != 6)
	{
		std::cout << "Usage: test <coarse-lock | lock-free | itm-htm | rtm-htm> <threads_count> <upper_limit> <add_percentage> <remove_percentage>\n";
		exit(1);
	}
	Set *set;
	if(strcmp(argv[1], "coarse-lock") == 0) {
		set = new CoarseLockSet();
	}
	else if(strcmp(argv[1], "lock-free") == 0) {
		set = new LockFreeSet();
	}
	else if(strcmp(argv[1], "itm-htm") == 0) {
		set = new ItmHtmSet();
	}
	else if(strcmp(argv[1], "rtm-htm") == 0) {
		set = new RtmHtmSet();
	}
	else {
		std::cout << "The lock implementation is unknown.\n";
		exit(1);
	}
	
	srand(time(NULL));
	
	int threadCount = atoi(argv[2]);
	int upperLimit = atoi(argv[3]);
	int addPerc = atoi(argv[4]);
	int removePerc = atoi(argv[5]);
    
	/*std::cout << "Test sequential... ";
	std::cout << (testSequential(set) ? "OK" : "FAIL") << std::endl << std::flush;
	
	std::cout << "Test concurrent... ";
	std::cout << (testConcurrent(set) ? "OK" : "FAIL") << std::endl << std::flush;
	
	std::cout << "Benchmark... ";*/
	benchmark(set, upperLimit, threadCount, addPerc, removePerc);
	
	delete set;
	return 0;
}