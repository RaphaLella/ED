#include <time.h>
#include <map>

#include <fstream>
#include <iostream>
#include <list>

#include "BTree.h"

using namespace std;

list<string> pegarNomeDosArquivos() {
    ifstream arquivoEntrada;
    string nomeDoArquivo;
    list<string> arquivos;

    arquivoEntrada.open("dados.txt");
    if (arquivoEntrada.is_open()) {
        string quantidade;
        int contador = 1;
        getline(arquivoEntrada,
                quantidade);  // Pega a primeira linha que é a quantidade de
                              // arquivos e coloca na variável quantidade

        int quant = stoi(quantidade);  // converte de string para int

        while (contador <= quant) {
            getline(arquivoEntrada, nomeDoArquivo);
            arquivos.push_back(nomeDoArquivo);
            contador++;
        }
        arquivoEntrada.close();
    } else {
        cout << "Não foi possível ler o arquivo";
    }

    return arquivos;

}

void lerArquivosTexto(list<string> lista) {
    //BTree* tree = new BTree(2);
    ifstream abrirArquivo;
    string textoArquivo, aux;
    map<string, int> mapArquivo;

    int indice = 1;

    list<string>::iterator k;

    for (k = lista.begin(); k != lista.end(); k++) {
        // ver se o arq existe
        abrirArquivo.open(*k);

        if (!abrirArquivo.fail() && abrirArquivo.is_open()) {
            while (getline(abrirArquivo, aux)) {
                // separarPalavras(tree, text);
                textoArquivo += aux;
            }
            mapArquivo.insert(pair<string, int>(textoArquivo, indice));
            textoArquivo = "";
            indice++;
            abrirArquivo.close();
        }
    }

    for (const auto& x : mapArquivo) {
        std::cout <<"Texto: " << "\n" << x.first << "\n" << "Indice: " << "\n" << x.second << "\n" << "\n";
    }
}

int main() { 
    list<string> nomeDosArquivos = pegarNomeDosArquivos();

    lerArquivosTexto(nomeDosArquivos);
}
