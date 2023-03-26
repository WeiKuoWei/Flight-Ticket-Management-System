#include <iostream>
#include "Iterator.h"
using namespace std;

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList ()
{
	this->head = new Node<T>();
	this->tail = new Node<T>();
	this->head->next = tail;
	this->tail->prev = head;
}
//============================================================
template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList () // destructor to clean up all nodes
{
	while(!empty())
		removeFront();
}
//============================================================
template <typename T>
bool DoublyLinkedList<T>::empty() const // is list empty?
{
	return this->head->next == tail;
}
//============================================================
template <typename T>
const T& DoublyLinkedList<T>::front() const // get front element
{
	if(!empty())
		return this->head->next->elem;
	else
		throw runtime_error("List is Empty");
}
//============================================================
template <typename T>
const T& DoublyLinkedList<T>::back() const // get front element
{
	if(!empty())
		return this->tail->prev->elem;
	else
		throw runtime_error("List is Empty");
}
//============================================================
template <typename T>
void DoublyLinkedList<T>::addBefore(Node<T> *ptr, const T& elem)
{
	Node<T> *tmp = new Node<T>(elem);
	tmp->next = ptr;
	tmp->prev = ptr->prev;
	ptr->prev->next = tmp;
	ptr->prev=tmp;
}
//============================================================
template <typename T>
void DoublyLinkedList<T>::addFront(const T& elem) // add to front of list
{
	addBefore(this->head->next, elem);
}
//============================================================
template <typename T>
void DoublyLinkedList<T>::addBack(const T& elem) // add to Back of the list
{
	addBefore(this->tail, elem);
}
//============================================================
template <typename T>
void DoublyLinkedList<T>::removeFront() // remove front item from list
{
	if(!empty())
	{
		Node<T> *tmp = this->head->next;
		this->head->next = tmp->next;
		tmp->next->prev = head;
		delete tmp;
	}
}
//============================================================
template <typename T>
void DoublyLinkedList<T>::removeBack() // remove last item from list
{
	if(!empty())
	{
		Node<T> *tmp = this->tail->prev;
		this->tail->prev = tmp->prev;
		tmp->prev->next = tail;
		delete tmp;
	}
}

 template <typename T>
 void DoublyLinkedList<T>::erase(Node<T> *ptr){
     Node<T> *prev = ptr->prev;
     Node<T> *next= ptr->next;
     prev->next = next;
     next->prev = prev;
     delete ptr;
 }
