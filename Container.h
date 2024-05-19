#include <iostream>

#ifndef CONTAINER_H
#define CONTAINER_H

template <class T>
class Container
{

public:
    struct Node
    {
        T *data;
        Node *next;
        Node *prev;

        explicit Node(T *val, Node *p = nullptr, Node *n = nullptr)
        {
            data = val;
            prev = p;
            next = n;
        }
    };

    class iterator
    {
    public:
        iterator() : current{nullptr} {}

        T *operator*()
        {
            return current->data;
        }

        iterator &operator++()
        {
            current = current->next;
            return *this;
        }

        bool operator==(const iterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const iterator &other) const
        {
            return !(*this == other);
        }

        iterator &operator--()
        {
            current = current->prev;
            return *this;
        }

    protected:
        Node *current;
        explicit iterator(Node *p) : current{p} {}
        friend class Container<T>;
    };

public:
    iterator begin() noexcept
    {
        return iterator(head);
    }

    iterator end() noexcept
    {
        return iterator(tail->next);
    }

public:
    Container()
    {
        create();
    }

    Container &operator=(const Container &other)
    {
        Container copy = other;
        std::swap(*this, copy);
        return *this;
    }

    ~Container()
    {
        clear();
    }

public:
    uint32_t get_size()
    {
        return container_size;
    }

    bool is_empty()
    {
        return container_size == 0;
    }

    void pop_front()
    {
        if (container_size > 0)
        {
            Node *t = head;
            head = head->next;
            if (head)
                head->prev = nullptr;
            delete t;
            container_size -= 1;
        }
        else
            throw std::out_of_range("PopFrontError: container is empty");
    }

    void pop_back()
    {
        if (container_size > 0)
        {
            Node *t = tail;
            tail = tail->prev;
            if (tail)
                tail->next = nullptr;
            delete t;
            container_size -= 1;
        }
        else
            throw std::out_of_range("PopBackError: container is empty");
    }

    void push_front(T *val)
    {
        if (is_empty())
            create_first_element(val);
        else
        {
            Node *t = new Node(val);
            t->next = head;
            head->prev = t;
            head = t;
            container_size += 1;
        }
    }

    void push_back(T *val)
    {
        if (is_empty())
            create_first_element(val);
        else
        {
            Node *t = new Node(val);
            t->prev = tail;
            tail->next = t;
            tail = t;
            container_size += 1;
        }
    }

    T *get_first_element()
    {
        if (container_size > 0)
        {
            return head->data;
        }
        else
            throw std::out_of_range("GetFirstElementError: container is empty");
    }

    T *get_last_element()
    {
        if (container_size > 0)
        {
            return tail->data;
        }
        else
            throw std::out_of_range("GetLastElementError: container is empty");
    }

    void clear()
    {
        while (container_size > 0)
        {
            pop_front();
        }
    }

    void swap(Container &other)
    {
        swap_elems(head, other.head);
        swap_elems(tail, other.tail);
        swap_elems(container_size, other.container_size);
    }

    void printList()
    {
        Node *node;
        node = head;
        while (node != NULL)
        {
            std::cout << "* " << *node->data << std::endl;
            node = node->next;
        }
    }

    void reverse()
    {
        struct Node *temp = NULL;
        struct Node *current = head;

        while (current != NULL)
        {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        }

        if (temp != NULL)
            head = temp->prev;
    }

private:
    void create()
    {
        container_size = 0;
        head = nullptr;
        tail = nullptr;
    }

    void create_first_element(T *val)
    {
        head = new Node(val, nullptr, nullptr);
        tail = head;
        container_size = 1;
    }

    void swap_elems(Node *&a, Node *&b)
    {
        Node *t;
        t = a;
        a = b;
        b = t;
    }

    void swap_elems(unsigned int &a, unsigned int &b)
    {
        int t = a;
        a = b;
        b = t;
    }

private:
    Node *head;
    Node *tail;
    uint32_t container_size{};
};

#endif