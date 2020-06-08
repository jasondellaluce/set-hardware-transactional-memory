#ifndef ITMHTMSET_H
#define ITMHTMSET_H

#include <atomic>
#include <mutex> 
#include "set.h"
#include "sequential-set.h"

class ItmHtmSet : public Set {
	public:
		ItmHtmSet() : Set() {
			tail = new SequentialSetNode(-1 & ~(1 << (sizeof(int) * 8 - 1)), NULL);
			head = new SequentialSetNode(-1, tail);
		}
		
		~ItmHtmSet() {
			SequentialSetNode* cur = head;
			while(cur != NULL) {
				SequentialSetNode* temp = cur->next;
				delete cur;
				cur = temp;			
			}
		}
	
		bool add(int item) {
			atomic_noexcept {
				SequentialSetNode* cur = head->next;
				SequentialSetNode* pred = head;
				while(cur != NULL) {
					if(item < cur->item) {
						if(pred->item == item)
							return false;
						SequentialSetNode* node = new SequentialSetNode(item, cur);
						pred->next = node;
						return true;
					}
					pred = cur;
					cur = cur->next;
				}
				return false;
			}
		}
		
		bool remove(int item) {
			atomic_noexcept {
				SequentialSetNode* cur = head->next;
				SequentialSetNode* pred = head;
				while(cur != tail) {
					if(item == cur->item) {
						pred->next = cur->next;
						delete cur;
						return true;
					}
					pred = cur;
					cur = cur->next;
				}
				return false;
			}
		}
		
		bool contains(int item) {
			atomic_noexcept {
				SequentialSetNode* cur = head->next;
				while(cur != tail) {
					if(cur->item == item) {
						return true;
					}
					cur = cur->next;
				}
				return false;
			}
		}
		
		void print() {
			
		}
	
	private:
		SequentialSetNode* head;
		SequentialSetNode* tail;
};


#endif