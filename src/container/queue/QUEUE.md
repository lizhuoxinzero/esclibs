# queue
	队列结构

-----------------------------------------------------------------------------------

### listqueue.h
	一个可存放不同大小数据的队列结构，存放的数据都将拷贝一份进来。适合做大量需要存储的内容，但长度不一的情况。

##### 工作原理
	* 通过使用buffer_t结构，形成一个大块内存的链表，当队列头块内容使用完后会暂时接到一个临时大块队列里去，当尾部内容不够时，会先从临时队列找块，没有则再申请。
	* 在大块里存的内容为单向链表结构，由一个节点接一个节点。


--------------------------------------------------------------------------------------