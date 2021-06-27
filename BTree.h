#include <iostream>

#include "Queue.h"

using namespace std;

// Definição da classe que representa uma B-árvore
class BTree {
   private:
    int t;             // ordem da b-árvore
    int numberOfKeys;  // quantidade de chaves
    BTreeNode* root;

    BTreeNode* doSearch(BTreeNode*, string, int&);
    void doInsert(Palavra*);
    void splitChild(BTreeNode*, int);
    void insertNonFull(BTreeNode*, Palavra*);
    bool doRemove(BTreeNode*, string);
    void print(BTreeNode*, int);
    int depth(BTreeNode*);
    void merge(BTreeNode*, int);
    void DISK_READ(BTreeNode*);
    void DISK_WRITE(BTreeNode*);
    void printRepeticoes(BTreeNode*, int);

   public:
    BTree(int);
    ~BTree();
    // busca a chave que contem a palavra dada
    bool search(string);
    // insere a palavra
    bool insert(Palavra*);
    // remove a palavra
    bool remove(string);

    void update(string, int);
    void printRepeticoes();
    void print();
    void levelTraversal();
};
