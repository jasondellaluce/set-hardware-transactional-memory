#ifndef LOCKFREESET_H
#define LOCKFREESET_H

#include <atomic>
#include "set.h"

using namespace std;

class LockFreeSetNode {
	public:
		atomic_ulong next;
		int item;
};

class LockFreeSetWindow {
	public:
		LockFreeSetNode* pred;
		LockFreeSetNode* curr;
};

class LockFreeSet : public Set {
	public:
		LockFreeSet() : Set() {
			this->head = new LockFreeSetNode();
			this->tail = new LockFreeSetNode();
			this->head->item = -1;
			atomic_init(&this->head->next, (ulong) tail);
			this->tail->item = -1 & ~(1 << (sizeof(int) * 8 - 1));
			atomic_init(&this->tail->next, (ulong) NULL);
		}
		
		~LockFreeSet() {
			LockFreeSetNode* cur = head;
			while(cur != NULL) {
				LockFreeSetNode* temp = parseAtomic(atomic_load(&cur->next), NULL);
				delete cur;
				cur = temp;
			}
		}
		
		bool add(int item) {
			LockFreeSetNode* node = new LockFreeSetNode();
			ulong temp;
			while(true) {
				LockFreeSetWindow window;
				find(&window, this->head, item);
				if(window.curr->item == item) {
					delete node;
					return false;
				}
				else {
					temp = formatAtomic(window.curr, false);
					node->item = item;
					atomic_store(&node->next, temp);
					if(atomic_compare_exchange_strong(&window.pred->next, &temp, formatAtomic(node, false))) {
						return true;
					}
				}
			}
		}
		
		bool remove(int item) {
			bool snip;
			while(true) {
				LockFreeSetWindow window;
				find(&window, this->head, item);
				if(window.curr->item != item) {
					return false;
				}
				else {
					LockFreeSetNode* succ = parseAtomic(atomic_load(&window.curr->next), NULL);
					ulong temp = formatAtomic(succ, false);
					snip = atomic_compare_exchange_strong(&window.curr->next, &temp, formatAtomic(succ, true));
					if(!snip)
						continue;
					temp = formatAtomic(window.curr, false);
					atomic_compare_exchange_strong(&window.pred->next, &temp, formatAtomic(succ, false));
					return true;
				}
			}
		}
		
		bool contains(int item) {
			bool marked = false;
			LockFreeSetNode* curr = head;
			while(curr->item < item) {
				curr = parseAtomic(atomic_load(&curr->next), NULL);
				LockFreeSetNode* succ = parseAtomic(atomic_load(&curr->next), &marked);
			}
			return curr->item == item && !marked;
		}
		
		void print() {
			bool marked;
			LockFreeSetNode* cur = parseAtomic(atomic_load(&head->next), &marked);
			while(cur != this->tail) {
				if(!marked)
					cout << cur->item << endl << flush;
				cur = parseAtomic(atomic_load(&cur->next), &marked);
			}
		}
		
	private:
		LockFreeSetNode* head;
		LockFreeSetNode* tail;
		
		LockFreeSetNode* parseAtomic(ulong val, bool* mark) {
			if(mark != NULL) {
				*mark = val & 1;
			}
			return (LockFreeSetNode*)(val & ~1);
		}
		
		ulong formatAtomic(LockFreeSetNode* node, bool mark) {
			ulong val = 0;
			val |= ((ulong) node) & ~1;
			val |= mark & 1;
			return val;
		}
		
		void find(LockFreeSetWindow* window, LockFreeSetNode* head, int item) {
			LockFreeSetNode* pred = NULL;
			LockFreeSetNode* curr = NULL;
			LockFreeSetNode* succ = NULL;
			ulong temp;
			bool snip;
			bool retry = false;
			bool marked = false;
			while(true) {
				pred = head;
				curr = parseAtomic(atomic_load(&pred->next), NULL);
				retry = false;
				while(!retry) {
					succ = parseAtomic(atomic_load(&curr->next), &marked);
					while(marked && !retry) {
						temp = formatAtomic(curr, false);
						snip = atomic_compare_exchange_strong(&pred->next, &temp, formatAtomic(succ, false));
						if(!snip) {
							retry = true;
						}
						else {
							delete curr;
							curr = succ;
							succ = parseAtomic(atomic_load(&curr->next), &marked);
						}
					}
					if(!retry) {
						if(curr->item >= item) {
							window->pred = pred;
							window->curr = curr;
							return;
						}
						pred = curr;
						curr = succ;
					}
				}
			}
		}
		
};

#endif