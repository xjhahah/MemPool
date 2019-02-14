#include "CentralCache.h"

CentralCache CentralCache::_inst;

// 从中心缓存获取一定数量的对象给thread cache
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t bytes)
{

}

//将一定数量的对象释放到Span跨度
void CentralCache::ReleaseListToSpans(void* start, size_t byte)
{

}
