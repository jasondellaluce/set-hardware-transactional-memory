#ifndef COARSELOCKSET_H
#define COARSELOCKSET_H

#include <atomic>
#include "set.h"
#include "sequential-set.h"
#include "querylock.h"

class CoarseLockSet : public Set {
	public:
		CoarseLockSet() : Set() {
			set = new SequentialSet();
			lock = new QueryLock();
		}
		
		~CoarseLockSet() {
			delete set;
			delete lock;
		}
	
		bool add(int item) {
			bool result = false;
			lock->lock();
			result = set->add(item);
			lock->unlock();
			return result;
		}
		
		bool remove(int item) {
			bool result = false;
			lock->lock();
			result = set->remove(item);
			lock->unlock();
			return result;
		}
		
		bool contains(int item) {
			bool result = false;
			lock->lock();
			result = set->contains(item);
			lock->unlock();
			return result;
		}
		
		void print() {
			
		}
	
	private:
		SequentialSet* set;
		QueryLock* lock;
};


#endif