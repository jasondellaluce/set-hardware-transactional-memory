#ifndef ITMHTMSET_H
#define ITMHTMSET_H

#include <atomic>
#include <mutex> 
#include "set.h"
#include "unsafe-set.h"

class ItmHtmSet : public Set {
	public:
		ItmHtmSet() : Set() {
			tail = new UnsafeSetNode(-1 & ~(1 << (sizeof(int) * 8 - 1)), NULL);
			head = new UnsafeSetNode(-1, tail);
		}
		
		~ItmHtmSet() {
			UnsafeSetNode* cur = head;
			while(cur != NULL) {
				UnsafeSetNode* temp = cur->next;
				delete cur;
				cur = temp;			
			}
		}
	
		bool add(int item) {
			atomic_noexcept {
				UnsafeSetNode* cur = head->next;
				UnsafeSetNode* pred = head;
				while(cur != NULL) {
					if(item < cur->item) {
						if(pred->item == item)
							return false;
						UnsafeSetNode* node = new UnsafeSetNode(item, cur);
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
				UnsafeSetNode* cur = head->next;
				UnsafeSetNode* pred = head;
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
				UnsafeSetNode* cur = head->next;
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
		UnsafeSetNode* head;
		UnsafeSetNode* tail;
};


#endif