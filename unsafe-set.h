#ifndef UNSAFESET_H
#define UNSAFESET_H

#include <atomic>
#include "set.h"

class UnsafeSetNode {
	public:
		UnsafeSetNode* next;
		int item;
		
		UnsafeSetNode(int item, UnsafeSetNode* next) {
			this->item = item;
			this->next = next;
		}
};

class UnsafeSet : public Set {
	public:
		UnsafeSet() : Set() {
			tail = new UnsafeSetNode(-1 & ~(1 << (sizeof(int) * 8 - 1)), NULL);
			head = new UnsafeSetNode(-1, tail);
		}
		
		~UnsafeSet() {
			UnsafeSetNode* cur = head;
			while(cur != NULL) {
				UnsafeSetNode* temp = cur->next;
				delete cur;
				cur = temp;			
			}
		}
	
		bool add(int item) {
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
		
		bool remove(int item) {
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
		
		bool contains(int item) {
			UnsafeSetNode* cur = head->next;
			while(cur != tail) {
				if(cur->item == item) {
					return true;
				}
				cur = cur->next;
			}
			return false;
		}
		
		void print() {
			UnsafeSetNode* cur = head->next;
			while(cur != tail) {
				std::cout << cur->item << std::endl;
				cur = cur->next;
			}
		}
	
	private:
		UnsafeSetNode* head;
		UnsafeSetNode* tail;
};


#endif