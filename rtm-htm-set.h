#ifndef RTMHTMSET_H
#define RTMHTMSET_H

#include <atomic>
#include <mutex> 
#include <random>
#include <immintrin.h>
#include "set.h"
#include "sequential-set.h"
#include "querylock.h"

#define RTM_ATTEMPTS    3
#define RTM_WAITNSEC    50

class RtmHtmSet : public Set {
	public:
		RtmHtmSet() : Set(), rng{std::random_device{}()}, dist(1, RTM_WAITNSEC) {
			set = new SequentialSet();
			lock = new QueryLock();
		}
		
		~RtmHtmSet() {
			delete set;
			delete lock;
		}
	
		bool add(int item) {
			bool result;
			size_t status;
			for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
			{
				status = _xbegin();
				if (status == _XBEGIN_STARTED) {
					if (!(lock->isLocked())) {
						result = set->add(item);
						_xend();
						return result;
					}
					_xabort(0xFF);
				}
				if((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
					std::this_thread::sleep_for(std::chrono::nanoseconds(dist(rng)));
				}
				else if(!(status & _XABORT_RETRY)) {
					break;
				}
			}
			lock->lock();
			result = set->add(item);
			lock->unlock();
			return result;
		}
		
		bool remove(int item) {
			bool result;
			size_t status;
			for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
			{
				status = _xbegin();
				if (status == _XBEGIN_STARTED) {
					if (!(lock->isLocked())) {
						result = set->remove(item);
						_xend();
						return result;
					}
					_xabort(0xFF);
				}
				if((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
					std::this_thread::sleep_for(std::chrono::nanoseconds(dist(rng)));
				}
				else if(!(status & _XABORT_RETRY)) {
					break;
				}
			}
			lock->lock();
			result = set->remove(item);
			lock->unlock();
			return result;
		}
		
		bool contains(int item) {
			bool result;
			size_t status;
			for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
			{
				status = _xbegin();
				if (status == _XBEGIN_STARTED) {
					if (!(lock->isLocked())) {
						result = set->contains(item);
						_xend();
						return result;
					}
					_xabort(0xFF);
				}
				if((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
					std::this_thread::sleep_for(std::chrono::nanoseconds(dist(rng)));
				}
				else if(!(status & _XABORT_RETRY)) {
					break;
				}
			}
			lock->lock();
			result = set->contains(item);
			lock->unlock();
			return result;
		}
		
		void print() {
			set->print();
		}
	
	private:
		SequentialSet* set;
		QueryLock* lock;
		std::mt19937 rng;
		std::uniform_int_distribution<std::mt19937::result_type> dist;
};


#endif