// linked_list.h
// author:  Joseph Perry
// desc:    This file defines a custom singly-linked LinkedList class

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <string>
#include <cstddef>
#include <exception>

struct ZeroLengthException : public std::exception {
   const char * what () const throw () {
      return "Linked List has no elements";
   }
};

template <typename T>
class LinkedList {
    private:
        template <typename U>
        class Node {
            private:
                U data;
                Node *next;
            public:
                Node(U data) : data(data), next(nullptr) {}
                friend class LinkedList;
        };
        Node<T> *head;
        Node<T> *tail;
        size_t num_nodes;
    public:
        LinkedList();                                       // empty constructor
        LinkedList(T);                                      // element constructor
        LinkedList(const LinkedList<T>&);                   // copy constructor
        LinkedList<T>& operator=(const LinkedList<T>&);     // assignment operator
        bool operator != (const LinkedList<T>&) const;      // not equal operator
        bool operator == (const LinkedList<T>&) const;      // is equal operator
        void print_list();                                  // list printer
        void push_back(T);                                  // add element to end of list
        void push_front(T);                                 // add element to front of list
        T find_first(T);                                    // find the first instance of item
        T find_last(T);                                     // find the last instance of item
        T pop_front();                                      // normally returns void
        T pop_back();                                       // normally returns void
        T front();                                          // return first element
        T back();                                           // return last element
        void clear();                                       // clear all elements from list
        void reverse();                                     // reverse the list in place
        void bubble_sort();                                 // bubble sort list in place
        void selection_sort();                              // select sort list in place
        inline size_t size();
};

// empty constructor
template <typename T>
LinkedList<T>::LinkedList(){
    head = nullptr;
    tail = head;
    num_nodes = 0;
}

// element constructor
template <typename T>
LinkedList<T>::LinkedList(T a){
    head = new Node<T>(a);
    tail = head;
    num_nodes = 1;
}

// copy constructor
template <typename T>
LinkedList<T>::LinkedList(const LinkedList &other){
    Node<T> *n, *p;

    p = other.head;
    num_nodes = 0;

    // assign head
    if(p != nullptr) {
        head = new Node<T>(p->data);
        ++num_nodes;
    } else {
        head = nullptr;
    }

    n = head;

    // assign body
    while(p != nullptr){
        p = p->next;

        // keep n for use w/ tail
        if(p != nullptr){
            ++num_nodes;
            n->next = new Node<T>(p->data);
            n = n->next;
        }
    }

    // assign tail
    tail = n;
}

// assignment
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T> &other){
    Node<T> *n, *p;

    p = other.head;

    // detect self-assignment, prevent self-deletion
    if(this == &other)
        return *this;

    // if this LinkedList already has elements, clear the memory
    if(head != nullptr && p != nullptr)
        clear();

    // assign head
    if(p != nullptr) {
        head = new Node<T>(p->data);
        ++num_nodes;
    }

    n = head;

    // assign body
    while(p != nullptr){
        p = p->next;

        // keep n for use w/ tail
        if(p != nullptr){
            ++num_nodes;
            n->next = new Node<T>(p->data);
            n = n->next;
        }
    }

    // assign tail
    tail = n;

    return *this;
}

template <typename T>
bool LinkedList<T>::operator!=(const LinkedList<T> &other) const {
    bool flag = false;
    Node<T> *n = head;
    Node<T> *p = other.head;

    while(!flag){
        // if either node is null and the other is not -> different lengths
        if((n == nullptr && p != nullptr) || (n != nullptr && p == nullptr))
            flag = true;
        // if both are null -> both lists same length
        else if(n == nullptr && p == nullptr)
            break;
        // if data are not equal
        else if(n->data != p->data)
            flag = true;
        // if data are equal, advance
        else if(n->data == p->data) {
            n = n->next;
            p = p->next;
        }
    }

    return flag;
}

template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T> &other) const {
    bool flag = true;
    Node<T> *n = head;
    Node<T> *p = other.head;

    while(flag){
        // if either node is null and the other is not -> different lengths
        if((n == nullptr && p != nullptr) || (n != nullptr && p == nullptr))
            flag = false;
        // if both are null -> both lists same length
        else if(n == nullptr && p == nullptr)
            break;
        // if data are not equal
        else if(n->data != p->data)
            flag = false;
        // if data are equal, advance
        else if(n->data == p->data) {
            n = n->next;
            p = p->next;
        }
    }

    return flag;
}

template <typename T>
void LinkedList<T>::clear(){
    Node<T> *node = head;
    Node<T> *prev = node;

    while(node != nullptr){
        node = node->next;
        delete prev;
        prev = node;
        --num_nodes;
    }
}

template <typename T>
void LinkedList<T>::push_front(T data){
    Node<T> *node = new Node<T>(data);
    node->next = head;
    head = node;
    ++num_nodes;
}

template <typename T>
void LinkedList<T>::push_back(T data){
    if(num_nodes > 0){
        tail->next = new Node<T>(data);
        tail = tail->next;
    }
    else {
        tail = new Node<T>(data);
        head = tail;
    }
    ++num_nodes;
}

template <typename T>
T LinkedList<T>::front(){
    if(head != nullptr)
        return head->data;
    else
        throw ZeroLengthException();
}

template <typename T>
T LinkedList<T>::back(){
    if(tail != nullptr)
        return tail->data;
    else
        throw ZeroLengthException();
}

template <typename T>
void LinkedList<T>::print_list(){
    Node<T> *p = head;

    if(p != nullptr)
        while(p != nullptr){
            std::cout << p->data << " ";
            p = p->next;
        }
    else
        std::cout << "Empty list";
    std::cout << std::endl;
}

template <typename T>
T LinkedList<T>::find_first(T a){
    Node<T> *p = head;

    while(p != nullptr && p->data != a)
        p = p->next;

    if(p != nullptr)
        return p->data;
    else
        throw ZeroLengthException();
}

template <typename T>
T LinkedList<T>::pop_front(){
    if(head != nullptr){
        T data = head->data;
        Node<T> *node = head;
        head = head->next;
    
        delete node;
        --num_nodes;
        return data;
    } else {
        throw ZeroLengthException();
    }
}

template <typename T>
T LinkedList<T>::pop_back(){
    if(tail != nullptr){
        T data;
        int i = 1;
        Node<T> *node = head;
    
        while(i++ < num_nodes-1)
            node = node->next;
    
        data = node->next->data;
        tail = node;
    
        delete node->next;
        tail->next = nullptr;
        --num_nodes;
        return data;
    } else {
        throw ZeroLengthException();
    }
}

template <typename T>
T LinkedList<T>::find_last(T a){
    if(head != nullptr){
        Node<T> *p = head, *n = nullptr;
    
        while(p != nullptr){
            if(p->data == a)
                n = p;
            p = p->next;
        }
    
        return n->data;
    } else {
        throw ZeroLengthException();
    }
}

template <typename T>
void LinkedList<T>::reverse(){
    Node<T> *prev, *cur, *next;

    prev = nullptr;
    cur = head;
    tail = head;

    while(cur != nullptr){
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    
    head = prev;
}

template <typename T>
inline size_t LinkedList<T>::size(){ return num_nodes; }

template <typename T>
void LinkedList<T>::bubble_sort(){
    Node<T> *a, *b;
    T tmp;
    bool flag = false;

    if(head == nullptr)
        return;

    if(head->next == nullptr)
        return;

    do {
        a = head;
        b = a->next;
        flag = false;
        while(b != nullptr){
            if(a->data > b->data){
                tmp = a->data;
                a->data = b->data;
                b->data = tmp;
                flag = true;
            }
            a = b;
            b = b->next;
        }
    } while(flag);
}

template <typename T>
void LinkedList<T>::selection_sort(){
    Node<T> *a, *b, *c;

    T min;
    bool flag = false;

    if(head == nullptr)
        return;

    if(head->next == nullptr)
        return;

    a = head;

    while(a != nullptr){
        min = a->data;
        b = a->next;

        while(b != nullptr){
            if(b->data < min){
                min = b->data;
                c = b;
                flag = true;
            }
            b = b->next;
        }

        if(flag){
            c->data = a->data;
            a->data = min;
            flag = false;
        }

        a = a->next;
    }
}

#endif