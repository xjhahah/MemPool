//高并发内存池

#pragma once
#include <iostream>

#include "Common.h"

class ThreadCache
{
public:
	void* Allocate(size_t size);               //分配内存
	void Deallocate(void* ptr, size_t size);  //解除分配
	void* FetchFromCentralCache(size_t size, size_t index);
private:
	FreeList _freelist[NLISTS];
};
