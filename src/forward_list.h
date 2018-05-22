#pragma once

#include <iostream>
#include <memory>

template <typename T, typename MyAllocator = std::allocator<T>>
class ForwardList
{
    template <typename El>
    struct Node;
    
    using alloc_t = typename MyAllocator::template rebind<Node<T>>::other;
    alloc_t my_allocator;
    
    typename Node<T>::unique_node_ptr root; // the lower is value, the early dtor
        
    template <typename El>
    struct Node
    {
        Node(const T& _data, alloc_t* _alloc = nullptr)
            : data{_data}
            , alloc{_alloc}
            , next(nullptr)
        {
            //std::cout << "Node constructor invoked.\n";  
        }
        
        ~Node() 
        { 
            //std::cout << "Node destructor invoked.\n"; 
        }        
        
        struct NodeDeleter 
        {  
            void operator()(Node<El>* p) 
            {
                //delete p;
                p->~Node(); 
                p->alloc->deallocate(p, 1);
            }
        };
        
        using unique_node_ptr = std::unique_ptr<Node<El>, NodeDeleter>;
        
        El data;
        alloc_t* alloc;
        unique_node_ptr next;
    };        
        
    public:
    class iterator
    {
        friend class ForwardList;

        iterator(Node<T>* node) : m_node(node) {}

        public:
        iterator() : m_node(0) {}

        iterator& operator ++() 
        {
            m_node = m_node->next.get();
            return *this;
        }

        T operator * () 
        {
            return m_node->data; // Dereferncing the end is undefined behavior.
        }

        bool operator==(const iterator& rhg) const
        {
            return m_node == rhg.m_node;
        }

        bool operator!=(const iterator& rhg) const
        {
            return m_node != rhg.m_node;
        }        

        private:
        Node<T>* m_node;
    };       
        
    public:

    iterator begin() 
    {
        return iterator(root.get()); 
    }

    iterator end() 
    {
        return nullptr; 
    }

    void append(T value)
    {
        auto newNode = my_allocator.allocate(1);       
        my_allocator.construct(newNode, value, &my_allocator);  

        typename Node<T>::unique_node_ptr newNodeUnique(newNode);

        if (!root) 
        {
            root = std::move(newNodeUnique);
        } 
        else 
        {
            Node<T>* curr(root.get());

            while (curr->next) 
            {
                curr = curr->next.get();
            }

            curr->next = std::move(newNodeUnique);
        }     
    }
};