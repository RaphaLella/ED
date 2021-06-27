#include "Queue.h"

//Constrói uma fila vazia
Queue::Queue()
{
    head = tail = 0;
    numberOfElements = 0;
}

//Destrói a fila
Queue::~Queue()
{    
    QueueNode* ptr;
    
    while (head != 0)
    {
        ptr = head;
        
        head = ptr->next;        
        
        delete ptr;        
    }
    
    head = tail = 0;
    
    numberOfElements = 0;
}

//Verifica se a fila está vazia
bool Queue::isEmpty()
{
    if (numberOfElements == 0)
        return true;
    
    return false;
}

//Enfileira um novo elemento
void Queue::enqueue(BTreeNode* node)
{
    QueueNode* ptr = new QueueNode(node);
    
    if (tail == 0)
    {
        head = tail = ptr;
    }
    else
    {
        tail->next = ptr;
        tail = ptr;
    }
    
    numberOfElements++;
}

//Desenfileira um elemento
BTreeNode* Queue::dequeue()
{
    if (isEmpty())
        return 0;
    
    QueueNode* ptr = head;
    BTreeNode* info = ptr->node;
    
    head = head->next;
    
    if (head == 0)
        tail = 0;
    
    delete ptr;
    
    numberOfElements--;
    
    return info;
}
