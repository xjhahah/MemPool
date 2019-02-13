#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

#include <malloc.h>
//定长的内存池
template <class T>
class ObjectPool
{
protected:
	struct Block
	{
		//内存块管理问题
		char* _start = nullptr;
		size_t _bytesize = 0;
		size_t _byteleft = 0;  //剩余字节数
		Block* _next = nullptr;

		Block(size_t bytesize)
		{
			_start = (char*)malloc(bytesize);
			_bytesize = bytesize;
			_byteleft = bytesize;
			_next = nullptr;
		}
	};
public:
	ObjectPool(size_t initnum = 16)
	{
		_head = _tail = new Block(initnum*sizeof(T));
	}
	T* OBJ_NEXT(T* obj)
	{
		return (*(T**)obj);
	}
	T* New()
	{
		T* obj;
		if (_freelist)
		{
			//取一个内存块
			obj = _freelist;

		}
		else  //表示自由链表中没有
		{
			Block* tail = _tail;
			if (tail->_byteleft == 0)
			{
				Block newBlock = new Block(tail->_bytesize * 2);
				_tail->next = newBlock;
				_tail = newBlock;
			}
			obj = (T*)(_tail->_start + (_tail->_bytesize - _tail->byteleft));
			_tail->_byteleft -= sizeof(T);
		}
		return obj;
	}
	void Delete(T* ptr)
	{
		if (_freelist == nullptr)
		{
			_freelist = ptr;
			//(*(T**)ptr) = nullptr;
			OBJ_NEXT(ptr) = nullptr;
		}
		else
		{
			(*(T**)ptr) = _freelist;
			_freelist = ptr;
		}
	}

protected:	
	//自由链表
	T* _freelist = nullptr;
	//块管理
	Block* _head = nullptr;
	Block* _tail = nullptr;
};
#endif
