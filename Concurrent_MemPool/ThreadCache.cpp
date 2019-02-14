
#include "ThreadCache.h"
#include "CentralCache.h"

void* ThreadCache::FetchFromCentralCache(size_t size, size_t index)
{
	FreeList& freelist = _freelist[index];
	size_t num = 10;

	void* start, *end;
	size_t fetchNum = CentralCache::GetInstance()->FetchRangeObj(start, end);
	if (fetchNum == 1)
	{
		return start;
	}
	freelist.PushRange(NEXT_OBJ(start), end, fetchNum);
	return start;
}
void* ThreadCache::Allocate(size_t size)
{
	assert(size <= MAXBYTES);

	//重新对齐取整
	size = ClassSize::Roundup(size);
	size_t index = ClassSize::Index(size);
	FreeList& freelist = _freelist[index];
	if (!freelist.Empty())
	{
		return freelist.Pop();
	}
	else
	{
		return FetchFromCentralCache(size,index);
	}
}
void ThreadCache::Deallocate(void* ptr, size_t size)
{
	assert(size <= MAXBYTES);
	size_t index = ClassSize::Index(size);
	FreeList& freelist = _freelist[index];

	freelist.Push(ptr);

	//满足某个条件,ListTooLong

}
int main()
{
	return 0;
}
