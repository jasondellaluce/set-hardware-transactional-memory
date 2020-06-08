#ifndef QUERYLOCK_H
#define QUERYLOCK_H

#include <mutex> 

class QueryLock {
	public:

		QueryLock() {
			atomic_store(&locked, false);
		}

		void lock() {
			mutex.lock();
			atomic_store(&locked, true);
		}

		void unlock() {
			atomic_store(&locked, false);
			mutex.unlock();
		}

		bool isLocked() {
			return atomic_load(&locked);
		}
	
	private:
		atomic_bool locked;
		std::mutex mutex;
};


#endif