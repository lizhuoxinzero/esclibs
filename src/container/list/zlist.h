/*
 * zlist.h
 *
 *  Created on: 2015年12月3日
 *      Author: zero
 */

#ifndef LIST_ZLIST_H_
#define LIST_ZLIST_H_

#include <stdio.h>
#include <exception>

template<class T>
struct NODE
{
	T data;
	NODE<T> *next;
	NODE<T> *last;
};

template<class T>
class CList;

template<class T>
class CListIterator
{
	friend class CList<T>;
private:
	CListIterator(CList<T> *list);
public:
	~CListIterator();
	T Last();
	void ToHead();
	void ToTail();
	bool Next();
	bool Istail();
	T* operator ->();
	T& operator *();
private:
	NODE<T> *m_p_node;
	CList<T> *m_p_list;

	NODE<T> *m_p_last;	//为防止迭代器delete后失效，next采用链表备份来实现，
	NODE<T> *m_p_next;	//为此额外消耗16Bit(不考虑对齐问题，64位环境)
};

template<class T>
class CList
{
	friend class CListIterator<T>;
public:
	CList();
	~CList();

	bool Push_tail(T data);
	bool Push_front(T data);
	bool Insert(unsigned long index,T data);

	void Delete_tail();
	void Delete_front();
	bool Delete(unsigned long index);
	bool Delete(CListIterator<T> iterator );
	bool Delete_all();


	T Get_tail() throw(const char*);
	T Get_front() throw(const char*);
	T Get(unsigned long index) throw(const char*);

	unsigned long Length();
	bool Empty();

	CListIterator<T> GetIterator();

private:
	unsigned long m_length;
	NODE<T> *m_p_head;
	NODE<T> *m_p_tail;
};

template<class T>
CList<T>::CList() //*
{
	m_p_head = new NODE<T>;
	m_p_tail = new NODE<T>;
	m_p_head->next = m_p_tail;
	m_p_head->last = NULL;
	m_p_tail->last = m_p_head;
	m_p_tail->next = NULL;
	m_length = 0;
}

template<class T>
CList<T>::~CList()
{
	NODE<T> *temp;
	while (m_p_tail)
	{
		temp = m_p_tail;
		m_p_tail = m_p_tail->last;
		delete temp;
	}
}

template<class T>
bool CList<T>::Push_tail(T data) //*
{
	try
	{
		NODE<T> *temp = new NODE<T>;
		temp->data = data;
		temp->last = m_p_tail->last;
		temp->next = m_p_tail;
		m_p_tail->last->next = temp;
		m_p_tail->last = temp;
		++m_length;
		return true;
	}
	catch (  std::exception &e )
	{
		return false;
	}
}

template<class T>
bool CList<T>::Push_front(T data) //*
{
	try
	{
		NODE<T> *temp = new NODE<T>;
		temp->data = data;
		temp->last = m_p_head;
		temp->next = m_p_head->next;
		m_p_head->next->last = temp;
		m_p_head->next = temp;
		++m_length;
		return true;
	}
	catch (std::exception &e)
	{
		return false;
	}
}

template<class T>
bool CList<T>::Insert(unsigned long index, T data) //*
{
	if( index > m_length || index < 0 )
	{
		return false;
	}

	NODE<T> *temp = m_p_head;
	for( int i=0; i<index; ++i )
	{
		temp = temp->next;
	}
	NODE<T> *temp2 = temp->next, *temp3;
	try
	{
		temp->next = temp3 = new NODE<T>;
	}
	catch (  std::exception &e )
	{
		return false;
	}
	temp3->last = temp;
	temp3->next = temp2;
	temp2->last = temp3;
	temp3->data = data;
	++m_length;
	return true;

}

template<class T>
void CList<T>::Delete_tail()	// s-1-2-3-4-e  delete 4
{
	if(m_p_tail->last == m_p_head)	//the list is empty;
		return;

	NODE<T> *temp = m_p_tail->last;	//temp=4
	m_p_tail->last = temp->last;		//3<-e
	m_p_tail->last->next = m_p_tail;	//3->e
	delete temp;							//delete temp
	--m_length;
}

template<class T>
void CList<T>::Delete_front()	//like delete_tail
{
	if(m_p_tail->last == m_p_head)	//the list is empty;
		return;
	NODE<T> *temp = m_p_head->next;
	m_p_head->next = temp->next;
	m_p_head->next->last = m_p_head;
	delete temp;
	--m_length;
}

template<class T>
bool CList<T>::Delete(CListIterator<T> iterator )
{
	NODE<T> *temp = iterator.m_p_node;
	if( (temp != m_p_head) && (temp != m_p_tail) )
	{
		temp->last->next = temp->next;
		temp->next->last = temp->last;
		delete temp;
		--m_length;
	}
	else
	{
		return false;
	}
}

template<class T>
bool CList<T>::Delete_all() ///*
{
	NODE<T> *temp;
	while(m_p_tail->last != m_p_head)
	{
		temp = m_p_head->next;
		m_p_head->next = temp->next;
		m_p_head->next->last = m_p_head;
		delete temp;
	}
	m_length = 0;
}

template<class T>
T CList<T>::Get_front() throw(const char*)	//*
{
	if(m_p_head->next != m_p_tail)
	{
		return m_p_head->next->data;
	}
	else
	{
		throw "list is null";
	}
}

template<class T>
T CList<T>::Get_tail() throw(const char*)	//*
{
	if( m_p_tail->last != m_p_head)
	{
		return m_p_tail->last->data;
	}
	else
	{
		throw "list is null";
	}
}

template<class T>
unsigned long CList<T>::Length()	//*
{
	return m_length;
}

template<class T>
bool CList<T>::Empty()
{
	if( m_length == 0)
		return true;
	else
		return false;
}

template<class T>
CListIterator<T> CList<T>::GetIterator()
{
	CListIterator<T> temp(this);
	return temp;
}

template<class T>
CListIterator<T>::CListIterator(CList<T> *list)  : m_p_list(list)	//*
{
	m_p_node = list->m_p_head;
	m_p_next = m_p_node->next;
	m_p_last = m_p_node->last;
}

template<class T>
CListIterator<T>::~CListIterator() //*
{
	//printf("fdsf\n");
}

template<class T>
bool CListIterator<T>::Next()	//*
{
	m_p_node = m_p_next;
	m_p_next = m_p_next->next;
	if(m_p_node != m_p_list->m_p_tail)
		return true;
	else
		return false;
}

template<class T>
bool CListIterator<T>::Istail()//*
{
	if(m_p_node == m_p_list->m_p_tail)
		return true;
	else
		return false;
}

template<class T>
void CListIterator<T>::ToHead()
{
	m_p_node = m_p_list->m_p_head;
	m_p_next = m_p_node->next;
	m_p_last = m_p_node->last;
}

template<class T>
void CListIterator<T>::ToTail()
{
	m_p_node = m_p_list->m_p_tail;
	m_p_next = m_p_node->next;
	m_p_last = m_p_node->last;
}

template<class T>
T* CListIterator<T>::operator ->()	//*
{
	return &(m_p_node->data);
}

template<class T>
T& CListIterator<T>::operator *()	//*
{
	return m_p_node->data;
}
#endif /* LIST_ZLIST_H_ */
