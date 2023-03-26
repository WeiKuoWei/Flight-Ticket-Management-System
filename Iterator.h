#ifndef ITERATOR
#define ITERATOR

#include<iostream>
#include<iomanip>
using namespace std;

template <typename T>
class Node
{
	private:
		T elem; //data element 
		Node* next; //Link (pointer) to the next Node
		Node* prev; //Link (pointer) to the previous Node
		template <typename T2> friend class DoublyLinkedList;
		
	public:
		Node(): next(NULL), prev(NULL)
		{}
		Node(T elem) : elem(elem),next(NULL), prev(NULL)
		{}
		Node& operator=(Node& other)
		{
			this->elem = other.elem;
			return *this;
		}
        T getElem(){
            return elem;
        }
};
//==============================================================
template <typename T> 
class DoublyLinkedList
{
	private:
		Node<T>* head; 	// pointer to the head of List
		Node<T>* tail; 	// pointer to the tail of List
	public:
		DoublyLinkedList (); // empty list constructor
		~DoublyLinkedList (); // destructor to clean up all nodes
		bool empty() const; // is list empty?
		const T& front() const; // get the value (element) from front Node<T> of list
		const T& back() const;  // get the value (element) from last Node<T> of the List 
		void addBefore(Node<T> *ptr, const T& elem);	 //add a new node before ptr
        void addAfter(Node<T> *ptr, const T& elem); // add a new node before ptr
		void addFront(const T& elem); // add a new node to the front of list
		void addBack(const T & elem); //add a new node to the back of the list
		void removeFront(); // remove front node from list
		void removeBack();  // remove last node from list
        void erase(Node<T> *ptr); // remove a node from a from a certain iterator ***
		void reverseList();	// reverse the list
		bool isPalindrome(); //check if the List is Palindrome or not		
        
        //Iterator
		class iterator;   //Forwarding declarion of iterator class // NOTICE in order to use begin() and end()?
		iterator begin()  
		{
			return iterator(head->next); 
		}
		iterator end()
		{
			return iterator(tail);
		}
        iterator secondLast(){
            return iterator(tail->prev);
        }
		class iterator
		{
			private:
                Node<T>* ptr;

            public: //iterator traits //NOTICE what's the significance of these traits and their usage?
                using iterator_category = bidirectional_iterator_tag;
                using difference_type = ptrdiff_t;
				using value_type = T;
                using pointer = T*;
                using reference = T&;

            public:
                iterator(){
					this->ptr = nullptr;
				}
				iterator(Node<T>* node){
                    this->ptr = node; 
                }
                T& operator*(){
                    return ptr->elem;
                }
                T& getElem(){
                    return ptr->elem;
                }
                Node<T>* getPtr(){
                    return this->ptr;
                }
                iterator& getNext(){
                    ptr = ptr->next;
                    return *this;
                }
                iterator& operator++(){
                    ptr = ptr->next;
                    return *this; 
                }
                iterator& getPrev(){
                    ptr = ptr->prev;
                    return *this;
                }
                iterator& operator--(){
                    ptr = ptr->prev;
                    return *this;
                }
                bool operator==(const iterator& other) const{
                    return this->ptr == other.ptr;
                }
                bool operator!=(const iterator& other) const{
                    return this->ptr != other.ptr;
                }
		};
};


#endif
