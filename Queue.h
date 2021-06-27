#include "BTreeNode.h"

//Definição de um nó da fila
class QueueNode
{
    public:
        BTreeNode* node;
        QueueNode* next;
        
        QueueNode(BTreeNode* node)
        {
            this->node = node;
            next = 0;
        }
};

//Definição de uma classe que representa uma fila encadeada 
//de nós de B-árvore
class Queue
{
    private: 
        QueueNode* head;
        QueueNode* tail;
        int numberOfElements;
        
    public:
        Queue();
        ~Queue();
        
        bool isEmpty();
        
        void enqueue(BTreeNode*);
        BTreeNode* dequeue();
};
