#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

using namespace std;

class Nodo {
public:
    string valor;
    string tag;
    Nodo* padre=nullptr;

    vector<Nodo*> hijos;


    Nodo(string t, string v = "") : tag(t), valor(v) {}

    void agregarHijo(Nodo* v) {
        v->padre = this;
        hijos.push_back(v);
    }

    Nodo operator[](int index){
        return *hijos[index];
    }
    Nodo operator[](string tag){
        for (Nodo *hijo : hijos)
            if (hijo->tag == tag)
                return *hijo;
        return *this;
    }
};


class TreeXml {
private:
    vector<string>book_atributes={"id","title","isbn","publication_year","language_code","description","average_rating","num_pages","similar_books"};
    vector<string>similarbooks_atributes={"book","title","isbn","publication_year"};

    bool esAtributoValido(const string& tag,vector<string>atris);

    void procesarHasta(string palabra, ifstream* archivo);

    //imprime los valores de un subarbol
    void imprimir_subtree(Nodo *start, string prefix = "");

public:
    Nodo raiz;
    //los argumentos son el nombre de la carpeta y el nodo por el que se inicia
    TreeXml(string folder,string initialNode);

    //muestra la jerarquia completa
    void imprimirTodo();
    //muestra la jerarquia del libro dado por el indice
    void imprimirLibro(size_t index);

    //lista los libros que son mas antiguos a sus libros similares
    void precursores();
};