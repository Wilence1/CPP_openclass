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
	struct ptr *q = list;	
	if(!list)
	{
		struct ptr *newone = (struct ptr *)malloc(sizeof(*newone));
		newone->next = NULL;
		newone->data.base = base;
		newone->data.size = N;
		list = newone;
		return Pointer(newone);
	}
	while(q->next)
	{
		if((char *)q->next->data.base - ((char*)q->data.base + q->data.size) >= N)
		{
			struct ptr *newone = (struct ptr *)malloc(sizeof(*newone));
			newone->data.base = (char*)q->data.base + q->data.size;
			newone->data.size = N;
			newone->next = q->next;
			q->next = newone;
			return Pointer(newone);
		}
		q = q->next;
	}
	if(size + (char *)base - ((char*)q->data.base + q->data.size) >= N)
	{
		struct ptr *newone = (struct ptr *)malloc(sizeof(*newone));
		newone->data.base = (char*)q->data.base + q->data.size;
		newone->data.size = N;
		newone->next = q->next;
		q->next = newone;
		return Pointer(newone);

	}
	throw AllocError(AllocErrorType::NoMemory, "Wrong alloc.");
}

void Allocator::free(Pointer &p)
{
	struct ptr *x = (p).tmp;
	
	struct ptr *q = list;
	struct ptr *qq = NULL;
	
	while(q && q != x)
	{
		qq = q;
		q = q->next;
	}
	if(!qq)
	{
		list = NULL;
	}
	else
	{
		qq->next = q->next;
	}
	p.tmp->data.base = 0;
}

void Allocator::realloc(Pointer &p, size_t N)
{

	struct ptr *x = (p).tmp;
	if(!p.tmp)
	{
		p = alloc(N);
		return;
	}
	struct ptr *q = list;
	struct ptr *qq = NULL;
	//printf("ira\n");
	if(q == x)printf("irka");
	while(q && q != x)
	{
		//printf("ira\n");
		qq = q;
		q = q->next;
	}
	if(!qq)
	{
		
		if(!q->next)
		{
			//printf("ira\n");
			if(size >= N)
			{
				for(int i = 0; i < q->data.size; i++)
				{
					*((char *)base + i) = *((char*)q->data.base + i);
					
				}
				q->data.base = base;
				q->data.size = N;	
			}
		}
		else
		{
			if((char *)q->next->data.base - (char*)base >= N)
			{
				for(int i = 0; i < q->data.size; i++)
				{
					*((char *)base + i) = *((char*)q->data.base + i);
				}
				q->data.base = base;
				q->data.size = N;
			}
			else
			{
				char *x = (char *)alloc(N).get();
				for(int i = 0; i < q->data.size; i++)
				{
					*((char *)x + i) = *((char*)q->data.base + i);
				}
				q->data.base = x;
				q->data.size = N;

			}
			
		}
	}
	else
	{
		if(!q->next)
		{
			if((char *)base + size - ((char *)q->data.base) >= N)
			{
				q->data.size = N;
			}

		}
		else
		{
			if((char *)q->next->data.base - (char *)q->data.base >= N)
			{
				q->data.size = N;
			}
			else
			{
				char *x = (char *)alloc(N).get();
				for(int i = 0; i < q->data.size; i++)
				{
					*((char *)x + i) = *((char*)q->data.base + i);
				}
				q->data.base = x;
				q->data.size = N;
			}
		}	
	}
}
void Allocator::defrag()
{
	struct ptr *q = list;
	if(q){

	if(!(q->next))
	{
		if((char *)q->data.base - (char *)base)
		{
			for(int i = 0; i < q->data.size; i++)
			{
				*((char *)base + i) = *((char*)q->data.base + i);
			}
			q->data.base = base;
		}
	}
	while(q && q->next)
	{
		if((char *)q->next->data.base - ((char*)q->data.base + q->data.size))
		{
			for(int i = 0; i < q->next->data.size; i++)
			{
				*((char *)q->data.base + i) = *((char *)q->next->data.base + i);
			}
			q->next->data.base = (char*)q->data.base + q->data.size;
		}
		q = q->next;
	}}
}
