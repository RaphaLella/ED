// Definição da classe que representa um nó da B-árvore
#include "Palavra.cpp"
class BTreeNode {
   public:
    int n;
    bool leaf;
    Palavra** key;
    BTreeNode** c;
    BTreeNode* parent;

    BTreeNode(int);
    ~BTreeNode();
};
