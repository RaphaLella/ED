#include "Queue.h"
#include <iostream>

using namespace std;

//Definição da classe que representa uma B-árvore
class BTree
{
    private:
        int t; //ordem da b-árvore
        int numberOfKeys;   // quantidade de chaves
        BTreeNode* root;

        BTreeNode* doSearch(BTreeNode*, int, int&);
        void doInsert(int);
        void splitChild(BTreeNode*, int);
        void insertNonFull(BTreeNode*, int);
        bool doRemove(BTreeNode*, int);
        void print(BTreeNode*, int);
        int depth(BTreeNode*);
        void merge(BTreeNode*, int);
        void DISK_READ(BTreeNode*);
        void DISK_WRITE(BTreeNode*);

    public:
        BTree(int);
        ~BTree();

        bool search(int);
        bool insert(int);
        bool remove(int);

        void print();
        void levelTraversal();
};

