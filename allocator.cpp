#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "allocator.h"
Pointer::Pointer(struct ptr *newelem)
{
    tmp = newelem;
}
Pointer::Pointer()
{
    tmp = NULL;
}
void *Pointer::get() const
{
    return (tmp->data).base;
} 
Allocator::Allocator(void *base, size_t size)
{
    this->base = base;
    this->size = size;
    list = NULL;
}
Pointer Allocator::alloc(size_t N)
{
	struct ptr *qmake = list;	
	if (!list) {
		struct ptr *newone = (struct ptr *)malloc(sizeof(*newone));
		newone->next = NULL;
		newone->data.base = base;
		newone->data.size = N;
		list = newone;
		return Pointer(newone);
	}
	while (qmake->next) {
		if((char *)qmake->next->data.base - ((char*)qmake->data.base + qmake->data.size) >= N) {
			struct ptr *newone = (struct ptr *)malloc(sizeof(*newone));
			newone->data.base = (char*)qmake->data.base + qmake->data.size;
			newone->data.size = N;
			newone->next = qmake->next;
			qmake->next = newone;
			return Pointer(newone);
		}
		qmake = qmake->next;
	}
	if(size + (char *)base - ((char*)qmake->data.base + qmake->data.size) >= N) {
		struct ptr *newone = (struct ptr *)malloc(sizeof(*newone));
		newone->data.base = (char*)qmake->data.base + qmake->data.size;
		newone->data.size = N;
		newone->next = qmake->next;
		qmake->next = newone;
		return Pointer(newone);
	}
	throw AllocError(AllocErrorType::NoMemory, "Wrong alloc.");
}
void Allocator::free(Pointer &p)
{
	struct ptr *x = (p).tmp;
	struct ptr *qmake = list;
	struct ptr *qstep = NULL;
	while(qmake && qmake != x) {
		qstep = qmake;
		qmake = qmake->next;
	}
	if (!qstep ) {
		list = NULL;
	} else {
		qstep ->next = qmake->next;
	}
	p.tmp->data.base = 0;
}
void Allocator::realloc(Pointer &p, size_t N)
{
	struct ptr *x = (p).tmp;
	if(!p.tmp) {
		p = alloc(N);
		return;
	}
	struct ptr *qmake = list;
	struct ptr *qstep = NULL;
	while(qmake && qmake != x) {
		qstep = qmake;
		qmake = qmake->next;
	}
	if (!qstep) {
		if (!qmake->next) {
			if (size >= N) {
				for (int i = 0; i < qmake->data.size; i++) {
					*((char *)base + i) = *((char*)qmake->data.base + i);
				}
				qmake->data.base = base;
				qmake->data.size = N;	
			}
		} else {
			if ((char *)qmake->next->data.base - (char*)base >= N) {
				for(int i = 0; i < qmake->data.size; i++) {
					*((char *)base + i) = *((char*)qmake->data.base + i);
				}
				qmake->data.base = base;
				qmake->data.size = N;
			} else {
				char *x = (char *)alloc(N).get();
				for(int i = 0; i < qmake->data.size; i++) {
					*((char *)x + i) = *((char*)qmake->data.base + i);
				}
				qmake->data.base = x;
				qmake->data.size = N;
			}
		}
	} else {
		if (!qmake->next) {
			if ((char *)base + size - ((char *)qmake->data.base) >= N) {
				qmake->data.size = N;
			}
		} else {
			if ((char *)qmake->next->data.base - (char *)qmake->data.base >= N) {
				qmake->data.size = N;
			} else {
				char *x = (char *)alloc(N).get();
				for(int i = 0; i < qmake->data.size; i++) {
					*((char *)x + i) = *((char*)qmake->data.base + i);
				}
				qmake->data.base = x;
				qmake->data.size = N;
			}
		}	
	}
}
void Allocator::defrag()
{
	struct ptr *qmake = list;
	if (qmake) {
		if (!(qmake->next)) {
			if((char *)qmake->data.base - (char *)base) {
				for(int i = 0; i < qmake->data.size; i++) {
					*((char *)base + i) = *((char*)qmake->data.base + i);
				}
				qmake->data.base = base;
			}
		}
		while(qmake && qmake->next) {
			if((char *)qmake->next->data.base - ((char*)qmake->data.base + qmake->data.size)) {
				for(int i = 0; i < qmake->next->data.size; i++) {
					*((char *)qmake->data.base + i) = *((char *)qmake->next->data.base + i);
				}
				qmake->next->data.base = (char*)qmake->data.base + qmake->data.size;
			}
			qmake = qmake->next;
		}
	}
}
