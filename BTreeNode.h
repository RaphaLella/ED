//Definição da classe que representa um nó da B-árvore

class BTreeNode
{
    public:
        int n;
        bool leaf;
        int* key;        
        BTreeNode** c;
        BTreeNode* parent;

        BTreeNode(int);
        ~BTreeNode();
};
