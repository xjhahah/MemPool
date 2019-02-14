#pragma once

#include <iostream>
#include <assert.h>

using namespace std;

//管理自由链表长度
const size_t NLISTS = 128;
const size_t MAXBYTES = 64 * 1024 * 1024; //最大字节数

struct Span
{
	PageID _id;   //页号
	size_t _npage;  //表示当前内存被切成了多少页

	void* _list = nullptr;  //自由链表
	size_t _usecount = 0; //使用计数
};
static void* & NEXT_OBJ(void* obj)
{
	return (*(void**)obj);
}
class SpanList
{
private:
	Span* _head;
};
class FreeList
{
public:
	bool Empty()
	{
		return _list == nullptr;
	}
	void PushRange(void* start, void* end, size_t num)
	{
		NEXT_OBJ(end) = _list;
		_list = start;
		_size += num;
	}
	void* Pop()
	{
		void* obj = _list;
		_list = NEXT_OBJ(obj);
		--_size;
		return obj;
	}
	void Push(void* obj)
	{
		NEXT_OBJ(obj) = _list;
		_list = obj;
		++_size;
	}
	size_t Size()
	{
		return _size;
	}
	size_t MaxSize()
	{
		return _maxSize;
	}
private:
	void* _list = nullptr;
	size_t _size = 0;
	size_t _maxSize = 0;
};
//管理对齐映射
class ClassSize
{
	// 控制在12%左右的内碎片浪费
	// [1,128] 8byte对齐 freelist[0,16)
	// [129,1024] 16byte对齐 freelist[16,72)
	// [1025,8*1024] 128byte对齐 freelist[72,128)
	// [8*1024+1,64*1024] 512byte对齐 freelist[128,240)
public:
	//做到内存对齐,获得对齐数
	static inline size_t _Roundup(size_t size, size_t align)
	{
		return (size + align - 1)&(~(align - 1));
	}
	static inline size_t Roundup(size_t size)
	{
		assert(size <= MAXBYTES);
		if (size<=128)
		{
			return _Roundup(size, 8);
		}
		else if (size <= 1024) {
			return _Roundup(size, 16);
		}
		else if (size <= 8192) {
			return _Roundup(size, 128);
		}
		else if (size <= 65536) {
			return _Roundup(size, 512);
		}
		return -1;
	}
	static inline size_t _Index(size_t bytes, size_t align_shift)
	{
		return ((bytes + (1 << align_shift) - 1) >> align_shift) - 1;
	}
	static inline size_t Index(size_t bytes)
	{
		assert(bytes < MAXBYTES);

		//每个区间有多少个类
		static  int group_array[4] = {16,56,56,112};
		if (bytes <= 128)
		{
			return _Index(bytes, 3);
		}
		else if (bytes <= 1024)
		{
			return _Index(bytes-128,4)+group_array[0];
		}
		else if (bytes <= 8192) 
		{
			return _Index(bytes - 1024, 7) + group_array[1] + group_array[0];
		}
		else if (bytes <= 65536)
		{
			return _Index(bytes - 8192, 9) + group_array[2] + group_array[1] + group_array[0];
		}
		assert(false);
		return -1;
	}
};

