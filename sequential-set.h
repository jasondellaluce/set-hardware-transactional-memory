#ifndef SEQUENTIALSET_H
#define SEQUENTIALSET_H

#include <atomic>
#include "set.h"

class SequentialSetNode {
	public:
		SequentialSetNode* next;
		int item;
		
		SequentialSetNode(int item, SequentialSetNode* next) {
			this->item = item;
			this->next = next;
		}
};

class SequentialSet : public Set {
	public:
		SequentialSet() : Set() {
			tail = new SequentialSetNode(-1 & ~(1 << (sizeof(int) * 8 - 1)), NULL);
			head = new SequentialSetNode(-1, tail);
		}
		
		~SequentialSet() {
			SequentialSetNode* cur = head;
			while(cur != NULL) {
				SequentialSetNode* temp = cur->next;
				delete cur;
				cur = temp;			
			}
		}
	
		bool add(int item) {
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
		
		bool remove(int item) {
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
		
		bool contains(int item) {
			SequentialSetNode* cur = head->next;
			while(cur != tail) {
				if(cur->item == item) {
					return true;
				}
				cur = cur->next;
			}
			return false;
		}
		
		void print() {
			SequentialSetNode* cur = head->next;
			while(cur != tail) {
				std::cout << cur->item << std::endl;
				cur = cur->next;
			}
		}
	
	private:
		SequentialSetNode* head;
		SequentialSetNode* tail;
};


#endif