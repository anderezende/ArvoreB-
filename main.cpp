/*
Disciplina: Banco de dados II
Alunos: Anderson A, Guilherme M.
Professor: Marcos R.

*/

#include <iostream>
#include <sstream>
#include "BPlusTree.hpp"
#include "Definitions.hpp"
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <omp.h>

using namespace std;
string introMessage(int aOrder) {
    ostringstream oss;
    oss << "Arvore B+ de Ordem: " << aOrder << endl;
    return oss.str();
}

string usageMessage() {
    string message =
    "Entre com uma tecla correspondente a acao abaixo> :\n"
    "\tp <k>  -- Encontre um valor por igualdade.\n"
    "\ti <k1> <k2> -- Mostrar as chaves no intervalo, insira uma e de espaco para inserir a segunda\n"
    "\tm -- Mostrar a Arvore B+.\n"
    "\tc <k> -- Mostre o caminho ate a folha procurada.\n"
    "\tf -- Printe a folha da arvore.\n"
    "\ts -- Sair.\n";
    return message;
}

int getOrder(int argc, const char * argv[]) {
    if (argc > 1) {
        int order = 0;
        istringstream iss(argv[1]);
        if ((iss >> order) && iss.eof() &&
            order >= MIN_ORDER && order <= MAX_ORDER) {
            return order;
        } else {
            //Objeto da classe ostreamque representa o fluxo de erros padrão orientado para caracteres estreitos (de tipo char). Isso corresponde ao fluxo C stderr.
            cerr << "Ordem Invalida: "
            << argv[1] << endl;
            cerr << "A ordem deve ser inteiro" << MIN_ORDER
            << " <= <order> <= " << MAX_ORDER << endl;
            cerr << "Ordem padrao: " << DEFAULT_ORDER << endl;
        }
    }
    return DEFAULT_ORDER;
}

vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}


int main(int argc, const char * argv[]) {
    ifstream fileReader ("output.csv");
    string line;
    vector<string> sep;
    char instruction;
    int key = 0;
    bool quit = false;
    bool verbose = false;
    int order = getOrder(argc, argv);
    string table;
    BPlusTree tree(order);
    int key2;
    int num_colunas =10;
    if(fileReader.is_open()){//Se o arquivo foi aberto com sucesso

        while (getline(fileReader,line)){//Vai pegando as linhas do arquivo
            cout <<line <<'\n';//Imprime cada linha
            sep=split(line, ','); // explode as linhas na virgula
                if(sep[0]=="+"){
                key2 = atoi(sep[1].c_str()); //converte a string em um inteiro
                table = "";
                for(int i=2;i<=num_colunas;i++){
                    table = table + " | "+sep[i]; //insere o resto da string associado a um chave
                }
                cout <<"table:"<<table;
                   // for(int i=1;i<=num_colunas;i++){
                        tree.insert(key2, table);
                        //tree.print(verbose);

                   // }
                        cout<<endl;

                }
                if(sep[0]=="-"){
                    cout<<"Selecionou deletar" <<endl;
                    //for(int i=1;i<=10;i++){
                    key2 = atoi(sep[1].c_str()); //converte a string em um inteiro
                        tree.remove(key2);
                       // tree.print(verbose);
                    //}
               // cout<<endl;
                }
        }
    }
    fileReader.close();
    cout <<endl;
    cout << introMessage(order);
    cout << usageMessage();
    while (!quit)
        {
        cout << "> ";
        cin >> instruction;
        switch (instruction) {

            case 'p':
                cin >> key;
                tree.printValue(key);
                break;
            case 'f':
                tree.printLeaves(verbose);
                break;
            case 'c':
                cin >> key;
                tree.printPathTo(key, verbose);
                break;
            case 's':
                quit = true;
                break;
            case 'i':
                int key2;
                cin >> key;
                cin >> key2;
                tree.printRange(key, key2);
                break;
            case 'm':
                tree.print(verbose);
                break;
            default:
                cin.ignore(256, '\n');
                cout << usageMessage();
                break;
        }
    }


    return 0;
}

