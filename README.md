# MemPool
C++项目之内存池技术
 开发环境：Windows10 、VS2017   
	使用技术：list、unordered_map、TLS、单例模式
	项目简介：该项目是Google的一个开源项目截取核心内容而成。该项目主要有三个部分构成：
1.	thread cache：每个线程独享的cache，用于小于64K的内存的分配，这里申请内存不需要加锁
2.	central cache：中心缓存所有线程共享，用于thread cache内存不够的情况下的申请，可以周期性的从thread cache回收资源，这里申请内存是需要加锁的
3.	page cache：页缓存是封装于central cache上的一层缓存，central cache没有内存对象时，从page cache中获取一定数量的page供central cache调用，能够合并切分出去的span，从而解决内存碎片问题
	项目缺陷：没有完全脱离malloc，内部结构的申请仍然使用的是new
