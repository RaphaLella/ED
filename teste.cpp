#include <bits/stdc++.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <fstream>
#include <iostream>
#include <list>
#include <map>

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

void separarPalavras(BTree *tree, string texto, int indice) {
    // remove pontuação da string
    for (int i = 0, len = texto.size(); i < len; i++) {
        if (ispunct(texto[i])) {
            texto.erase(i--, 1);
            len = texto.size();
        }
    }
    // deixa todas as letras minusculas
    std::transform(texto.begin(), texto.end(), texto.begin(), ::tolower);

    // cout << texto << endl;
    stringstream ss(texto);
    string word;
    // itera palavra a palavra
    while (ss >> word) {
        // cout << word << endl;
        tree->update(word, indice);
    }
    cout << endl;
}

void lerArquivosTexto(list<string> lista) {
    BTree *tree = new BTree(2);
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
                separarPalavras(tree, aux, indice);
                textoArquivo += aux;
            }
            indice++;
            abrirArquivo.close();
        }
    }
    tree->printRepeticoes();
}

int main() {
    list<string> nomeDosArquivos = pegarNomeDosArquivos();

    lerArquivosTexto(nomeDosArquivos);
}
