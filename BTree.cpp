#include "BTree.h"

#include <map>
// Constrói uma B-árvore contendo apenas a raiz vazia.
BTree::BTree(int order) {
    t = order;
    numberOfKeys = 0;
    root = new BTreeNode(t);

    DISK_WRITE(root);
}

// Destrói a B-árvore
BTree::~BTree() { delete root; }

// Inicia a busca por uma chave (value) na B-árvore
bool BTree::search(string value) {
    int i;
    if (doSearch(root, value, i) != 0) return true;

    return false;
}

// Efetivamente busca, recursivamente, uma chave na B-árvore
// Retorna um ponteiro para o nó encontrado explicitamente e
// a posição da chave no vetor de chaves no parâmetro por
// referência i
BTreeNode* BTree::doSearch(BTreeNode* node, string value, int& i) {
    i = 0;
    while (i < node->n && value > node->key[i]->palavra) i++;

    if (i < node->n && value == node->key[i]->palavra)
        return node;
    else {
        if (node->leaf == true) return 0;

        DISK_READ(node->c[i]);

        return doSearch(node->c[i], value, i);
    }
}

// Não faz nada, apenas marca o local onde uma leitura do
// disco deveria ser feita
void BTree::DISK_READ(BTreeNode* node) { return; }

// Não faz nada, apenas marca o local onde uma escrita no
// disco deveria ser feita
void BTree::DISK_WRITE(BTreeNode* node) { return; }

// Inicia o processo de inserção de uma nova chave (value).
// Busca para saber se não existe na árvore e somente nesse
// momento chama doInsert para efetivamente inserir.
bool BTree::insert(Palavra* key) {
    if (search(key->palavra) == true) return false;

    doInsert(key);

    return true;
}

// Inicia o processo de inserção na raiz, verificando
// inicialmente, se ela não precisa ser quebrada antes
// de invocar insertNonFull. Pode fazer a árvore aumentar
// em altura
void BTree::doInsert(Palavra* value) {
    if (root->n == 2 * t - 1) {
        BTreeNode* s = new BTreeNode(t);

        s->leaf = false;
        s->n = 0;
        s->c[0] = root;
        root->parent = s;

        root = s;

        splitChild(s, 0);

        insertNonFull(s, value);
    } else {
        insertNonFull(root, value);
    }
}

// Quebra um nó cheio em dois, sobe a mediana para o pai
void BTree::splitChild(BTreeNode* x, int i) {
    BTreeNode* z = new BTreeNode(t);
    BTreeNode* y = x->c[i];

    z->leaf = y->leaf;
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++) z->key[j] = y->key[j + t];

    if (!y->leaf) {
        for (int j = 0; j < t; j++) z->c[j] = y->c[j + t];
    }
    y->n = t - 1;

    for (int j = x->n; j > i; j--) x->c[j + 1] = x->c[j];

    x->c[i + 1] = z;
    z->parent = x;

    for (int j = x->n - 1; j >= i; j--) x->key[j + 1] = x->key[j];

    x->key[i] = y->key[t - 1];
    x->n++;

    DISK_WRITE(y);
    DISK_WRITE(z);
    DISK_WRITE(x);
}

// Recursivamente, insere uma nova chave em um nó não cheio
void BTree::insertNonFull(BTreeNode* x, Palavra* value) {
    int i = x->n - 1;

    if (x->leaf) {
        // printf("\nFolha -> n = %d", x->n);
        // fflush(NULL);
        while (i >= 0 && value < x->key[i]) {
            x->key[i + 1] = x->key[i];
            i--;
        }
        x->key[i + 1] = value;
        x->n++;

        DISK_WRITE(x);
    } else {
        while (i >= 0 && value < x->key[i]) i--;

        i++;
        DISK_READ(x->c[i]);

        if (x->c[i]->n == 2 * t - 1) {
            splitChild(x, i);

            if (value > x->key[i]) i++;
        }
        insertNonFull(x->c[i], value);
    }
}

// Inicia o processo de remoção através de uma chamada
// inicial à busca. Caso encontre, chama o método que vai
// realizar a remoção
bool BTree::remove(string x) {
    if (!root) return false;

    return doRemove(root, x);
}

// Recursivamente, realiza a remoção da chave da árvore.
// Somente desce para a subárvore para continuar a busca
// caso a raiz da subárvore tenha pelo menos t chaves. A
// remoção também será realizada somente se a folha tiver
// pelo menos t elementos.
bool BTree::doRemove(BTreeNode* pt, string x) {
    BTreeNode* u;
    BTreeNode* w;
    BTreeNode* y;
    BTreeNode* z;

    int i = 0;
    while (i < pt->n && x > pt->key[i]->palavra) i++;

    if (i < pt->n && x == pt->key[i]->palavra) {
        if (pt->leaf) {
            // Caso 1
            for (int j = i; j < pt->n - 1; j++) pt->key[j] = pt->key[j + 1];

            pt->n--;

            DISK_WRITE(pt);

            return true;
        } else {
            // Caso 2
            DISK_READ(pt->c[i]);

            if (pt->c[i]->n > t - 1) {
                // Caso 2a
                u = pt->c[i];

                while (!u->leaf) {
                    DISK_READ(u->c[u->n]);
                    u = u->c[u->n];
                }

                pt->key[i] = u->key[u->n - 1];
                DISK_WRITE(pt);

                return doRemove(pt->c[i], u->key[u->n - 1]->palavra);
            } else {
                DISK_READ(pt->c[i + 1]);

                if (pt->c[i + 1]->n > t - 1) {
                    // Caso 2b
                    z = pt->c[i + 1];

                    while (!z->leaf) {
                        DISK_READ(z->c[0]);
                        z = z->c[0];
                    }

                    pt->key[i] = z->key[0];

                    DISK_WRITE(pt);

                    return doRemove(pt->c[i + 1], z->key[0]->palavra);
                } else {
                    // Caso 2c
                    merge(pt, i);

                    return doRemove(pt->c[i], x);
                }
            }
        }
    } else {
        if (pt->leaf) return false;

        DISK_READ(pt->c[i]);

        if (pt->c[i]->n > t - 1)
            return doRemove(pt->c[i], x);
        else {
            // Caso 3
            y = pt->c[i];

            if (i > 0) {
                DISK_READ(pt->c[i - 1]);

                u = pt->c[i - 1];
            }

            if (i < pt->n) {
                DISK_READ(pt->c[i + 1]);

                z = pt->c[i + 1];
            }

            if (i > 0 && pt->c[i - 1]->n > t - 1) {
                // Caso 3a - esquerda
                y->c[t] = y->c[t - 1];

                for (int j = t - 2; j >= 0; j--) {
                    y->key[j + 1] = y->key[j];
                    y->c[j + 1] = y->c[j];
                }

                y->key[0] = pt->key[i - 1];
                y->c[0] = u->c[u->n];
                y->n++;

                pt->key[i - 1] = u->key[u->n - 1];
                u->n--;

                DISK_WRITE(pt);
                DISK_WRITE(u);
                DISK_WRITE(y);

                return doRemove(pt->c[i], x);
            } else {
                if (i < pt->n && pt->c[i + 1]->n > t - 1) {
                    // Caso 3a - direita
                    y->n++;
                    y->key[t - 1] = pt->key[i];
                    y->c[t] = z->c[0];

                    pt->key[i] = z->key[0];

                    for (int j = 0; j < z->n - 1; j++) {
                        z->key[j] = z->key[j + 1];
                        z->c[j] = z->c[j + 1];
                    }

                    z->c[z->n - 1] = z->c[z->n];
                    z->n--;

                    DISK_WRITE(pt);
                    DISK_WRITE(z);
                    DISK_WRITE(y);

                    return doRemove(pt->c[i], x);
                } else {
                    // Caso 3b
                    if (i < pt->n) {
                        merge(pt, i);

                        w = pt->c[i];
                    } else {
                        merge(pt, i - 1);

                        w = pt->c[i - 1];
                    }

                    if (pt->n == 0) {
                        root = w;

                        delete pt;
                    }

                    return doRemove(w, x);
                }
            }
        }
    }
}

void BTree::merge(BTreeNode* pt, int i) {
    BTreeNode* y;
    BTreeNode* z;

    y = pt->c[i];
    z = pt->c[i + 1];

    y->key[t - 1] = pt->key[i];

    for (int j = t; j < 2 * t - 1; j++) {
        y->c[j] = z->c[j - t];
        y->key[j] = z->key[j - t];
    }

    y->c[2 * t - 1] = z->c[t - 1];
    y->n = 2 * t - 1;

    for (int j = i; j < pt->n - 1; j++) pt->key[j] = pt->key[j + 1];

    for (int j = i + 1; j < pt->n; j++) pt->c[j] = pt->c[j + 1];

    pt->n--;

    delete z;

    DISK_WRITE(pt);
    DISK_WRITE(y);
    DISK_WRITE(z);
}

void BTree::print() { print(root, 0); }

void BTree::print(BTreeNode* node, int spaces) {
    if (node != 0) {
        for (int i = 0; i < spaces; i++) cout << " ";

        for (int i = 0; i < node->n; i++) {
            cout << node->key[i]->palavra << " ";
        }

        if (node->parent)
            cout << "(" << node->leaf << ") (" << node->n << ") ("
                 << node->parent->key[0]->palavra << ")";
        else
            cout << "(" << node->leaf << ") (" << node->n << ") (nulo)";
        cout << endl;

        for (int i = 0; i < node->n + 1; i++) print(node->c[i], spaces + 5);
    }
}

void BTree::printRepeticoes() { printRepeticoes(root, 0); }

void BTree::printRepeticoes(BTreeNode* node, int spaces) {
    if (node != 0) {
        for (int i = 0; i < spaces; i++) cout << " ";

        for (int i = 0; i < node->n; i++) {
            cout << node->key[i]->palavra << " " << endl;

            for (const auto& x : node->key[i]->repeticoes) {
                cout << "Indice do documento: "
                     << "\n"
                     << x.first << "\n"
                     << "Quantidade de repeticoes: "
                     << "\n"
                     << x.second << "\n"
                     << "\n";
            }
            cout << endl;
        }

        // if (node->parent)
        //     cout << "(" << node->leaf << ") (" << node->n << ") ("
        //          << node->parent->key[0]->palavra << ")";
        // else
        //     cout << "(" << node->leaf << ") (" << node->n << ") (nulo)";
        // cout << endl;

        // for (int i = 0; i < node->n + 1; i++)
        //     printRepeticoes(node->c[i], spaces + 5);
    }
}

// Retorna a profundidade de um nó
int BTree::depth(BTreeNode* node) {
    if (node == root) return 0;

    return 1 + depth(node->parent);
}

// Imprime os nós e as chaves da árvore por nível
void BTree::levelTraversal() {
    Queue* q = new Queue();

    q->enqueue(root);

    BTreeNode* prev = root;

    // Enquanto houver alguém na pilha
    while (!q->isEmpty()) {
        BTreeNode* ptr = q->dequeue();

        if (depth(prev) != depth(ptr)) cout << endl;

        cout << "|";

        for (int i = 0; i < ptr->n; i++) cout << ptr->key[i] << " ";

        cout << "|";

        if (!ptr->leaf) {
            for (int i = 0; i <= ptr->n; i++) q->enqueue(ptr->c[i]);
        }
        prev = ptr;
    }
}
void BTree::update(string palavra, int docId) {
    int i;

    BTreeNode* pointer = doSearch(root, palavra, i);
    if (pointer != 0) {
        // já existia palavra na árvoere, apenas as repetições vao mudar
        map<int, int> mapRepeticoes = pointer->key[i]->repeticoes;

        auto it = mapRepeticoes.find(docId);
        if (it != mapRepeticoes.end()) {
            // a chave já existe no mapa, apenas incrementar no contador
            it->second++;
        } else {
            mapRepeticoes.insert(pair<int, int>(docId, 1));
        }
        pointer->key[i]->repeticoes = mapRepeticoes;
    } else {
        map<int, int> mapRepeticoes;
        mapRepeticoes[docId] = 1;
        Palavra* novo = new Palavra(palavra, mapRepeticoes);

        doInsert(novo);
    }
}