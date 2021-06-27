#include <bits/stdc++.h>
#include <stdlib.h>

#include <iostream>
#include <list>
#include <map>

using namespace std;

class Palavra {
   public:
    string palavra;
    map<int, int> repeticoes;

    Palavra(string novaPalavra, map<int, int> novasRepeticoes) {
        palavra = novaPalavra;
        repeticoes = novasRepeticoes;
    }
};