#pragma once

//中心缓存，采用设计模式来设计 --- 单例模式（饿汉模式）
#include "Common.h"
typedef size_t PageID;


class CentralCache
{
public:
	static CentralCache* GetInstance()
	{
		return &_inst;
	}
	//从中心缓存中取出一定数量的对象给 ThreadCache
	size_t FetchRangeObj(void*& start, void*& end,size_t n,size_t byte);

	//将一定数量的对象释放到Span跨度
	void ReleaseListToSpans(void* start, size_t byte);

private:
	//中心缓存自由链表
	SpanList _freeList[NLISTS];
private:
	CentralCache() = default;
	CentralCache(const CentralCache&) = delete;
	CentralCache& operator=(const CentralCache&) = delete;

	static CentralCache _inst;
};



