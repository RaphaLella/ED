#include <time.h>

#include <fstream>
#include <iostream>
#include <list>

#include "BTree.h"

using namespace std;

list<string> nomeDosArquivos() {
    ifstream arquivoEntrada;
    string linha;
    list<string> arquivos;

    arquivoEntrada.open("dados.txt");
    if (arquivoEntrada.is_open()) {
        string quantidade;
        int contador = 1;
        getline(arquivoEntrada,
                quantidade);  // Pega a primeira linha que é a quantidade de
                              // arquivos e coloca na variável quantidade

        int quant = stoi(quantidade);  // converte de string para int

        cout << "Quantidade de arquivos : " << quant << endl << endl;

        while (contador <= quant) {
            getline(arquivoEntrada, linha);
            arquivos.push_back(linha);
            contador++;
        }
        arquivoEntrada.close();
    } else {
        cout << "Não foi possível ler o arquivo";
    }
    return arquivos;
}

void lerArquivosTexto(list<string> lista) {
    BTree* tree = new BTree(2);
    ifstream abrirArquivo;
    string text;

    int indice = 1;

    list<string>::iterator k;

    for (k = lista.begin(); k != lista.end(); k++) {
        abrirArquivo.open(*k);

        if (abrirArquivo.is_open()) {
            while (getline(abrirArquivo, text)) {
                // separarPalavras(tree, text);
                cout << text << endl;
            }
            cout << indice << endl;
            indice++;
            abrirArquivo.close();
        }
    }
}

int main() { lerArquivosTexto(nomeDosArquivos()); }
