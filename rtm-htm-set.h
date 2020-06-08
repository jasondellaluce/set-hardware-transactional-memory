#ifndef RTMHTMSET_H
#define RTMHTMSET_H

#include <atomic>
#include <mutex> 
#include <immintrin.h>
#include "set.h"
#include "unsafe-set.h"
#include "querylock.h"

#define RTM_ATTEMPTS    3
#define RTM_WAITNSEC    50

class RtmHtmSet : public Set {
	public:
		RtmHtmSet() : Set() {
			set = new UnsafeSet();
			lock = new QueryLock();
		}
		
		~RtmHtmSet() {
			delete set;
			delete lock;
		}
	
		bool add(int item) {
			bool result;
			int status, attempts = 0;
			while(true) {
				status = _xbegin();
				if (status == _XBEGIN_STARTED) {
					if (lock->isLocked()) {
						_xabort(0xff); 
					}			
					result = set->add(item);  
					_xend ();
					return result;
				}
				if(attempts++ >= RTM_ATTEMPTS)
					break;
				
				std::this_thread::sleep_for (std::chrono::nanoseconds((rand() % RTM_WAITNSEC) + 1));
			}
			lock->lock();
			result = set->add(item);
			lock->unlock();
			return result;
		}
		
		bool remove(int item) {
			bool result;
			int status, attempts = 0;
			while(true) {
				status = _xbegin();
				if (status == _XBEGIN_STARTED) {
					if (lock->isLocked()) {
						_xabort(0xff); 
					}			
					result = set->remove(item);  
					_xend ();
					return result;
				}
				if(attempts++ >= RTM_ATTEMPTS)
					break;
				
				std::this_thread::sleep_for (std::chrono::nanoseconds((rand() % RTM_WAITNSEC) + 1));
			}
			lock->lock();
			result = set->remove(item);
			lock->unlock();
			return result;
		}
		
		bool contains(int item) {
			bool result;
			int status, attempts = 0;
			while(true) {
				status = _xbegin();
				if (status == _XBEGIN_STARTED) {
					if (lock->isLocked()) {
						_xabort(0xff); 
					}			
					result = set->contains(item);  
					_xend ();
					return result;
				}
				if(attempts++ >= RTM_ATTEMPTS)
					break;
				
				std::this_thread::sleep_for (std::chrono::nanoseconds((rand() % RTM_WAITNSEC) + 1));
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
		UnsafeSet* set;
		QueryLock* lock;
};


#endif