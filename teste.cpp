#include <iostream>
#include <fstream>
#include <list>

using namespace std;

list<string> nomeDosArquivos()
{

    /* ofstream arquivoSaida;

    arquivoSaida.open("dados.txt", ios::app); //ios::app para colocar o cursor no final

    //arquivo << "Texto inserido pelo COUT \n"; adicionar texto ao arquivo

    arquivoSaida.close();

    */

    ifstream arquivoEntrada;
    string linha;
    list<string> arquivos;

    arquivoEntrada.open("dados.txt");
    if (arquivoEntrada.is_open())
    {

        string quantidade;
        int contador = 1;
        getline(arquivoEntrada, quantidade); //Pega a primeira linha que é a quantidade de arquivos e coloca na variável quantidade

        int quant = stoi(quantidade); //converte de string para int

        cout << "Quantidade de arquivos : " << quant << endl;

        while (contador <= quant)
        {
            getline(arquivoEntrada, linha);
            cout << linha <<endl;
            arquivos.push_back(linha);
            contador++;
        }
        arquivoEntrada.close();
    }
    else
    {
        cout << "Não foi possível ler o arquivo";
    }
    return arquivos;

}

void lerArquivosTexto(list<string> lista) {
    ifstream arquivoAberto;
    cout << endl << lista.size();

}

int main()
{
    lerArquivosTexto(nomeDosArquivos());
}
