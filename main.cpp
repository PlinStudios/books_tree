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
};


class TreeXml {
private:
    vector<string>book_atributes={"id","title","isbn","publication_year","description","average_rating","num_pages","similar_books"};
    vector<string>similarbooks_atributes={"book","title","isbn","publication_year"};

    bool esAtributoValido(const string& tag,vector<string>atris) {
        for (const auto& a : atris)
            if (a == tag) return true;
        return false;
    }

    void procesarHasta(string palabra, ifstream* archivo){
    string auxw="";
    char c;

    while(archivo->get(c)){
        auxw += c;

        if(auxw.size() > palabra.size()){
            auxw.erase(0,1);
        }
        if(auxw == palabra){
            break;
        }
    }
    }

public:
    Nodo raiz;
    //los argumentos son el nombre de la carpeta y el nodo por el que se inicia
    TreeXml(string folder,string initialNode) : raiz("root"){

        for (const auto& entry : filesystem::directory_iterator(folder)) {
            ifstream archivo(entry.path());
            if (!archivo.is_open()) return;

            //inicializa el arbol en un nodo raiz
            Nodo* currNode = &raiz;

            //señala que esta leyendo el valor de un nodo
            bool reading_value=false;

            //señala cierre de nodos instantaneos
            bool autocierre=false;

            //señala cual de los dos filtros de atributos estan siendo ocupados al momento de crear los nodos hijos
            vector<string> filtroactual=book_atributes;
            string word;
            char c;

            procesarHasta("<"+initialNode+">",&archivo);

            Nodo* auxN = new Nodo(initialNode);
            currNode->agregarHijo(auxN);
            currNode=auxN;

            while (archivo.get(c)) {
                if (c == '\n') {
                    word = "";
                    continue;
                }
                else if (c=='<'){
                    //ingresa el valor en el nodo
                    if(reading_value && word!=""){
                        currNode->valor=word;
                        reading_value=false;
                    }

                    word = "";
                    archivo.get(c);


                    switch(c){
                    //cierre de tag
                    case'/':
                        word = "";
                        if (currNode->tag=="similar_books") filtroactual=book_atributes;
                        currNode=currNode->padre;
                        while (archivo.get(c) && c != '>') {}
                        break;

                    //declaracion inicial del xml
                    case'?':

                        while (archivo.get(c) && c != '>') {
                        }
                        break;
                    // se procesa el texto de un CDATA y se ingresa el valor en el nodo
                    case'!':
                        while (archivo.get(c) && c != '[') {}
                        while (archivo.get(c) && c != '[') {}
                        word = "";
                        //cierre del CDATA
                        while(archivo.get(c)){

                            if(c==']'){
                                string auxS="]";
                                archivo.get(c);
                                if((auxS+=c)=="]]"){
                                    archivo.get(c);
                                    if((auxS+=c)=="]]>"){

                                        break;
                                    }
                                    else{
                                        word+=auxS;
                                        continue;
                                    }
                                }
                                else
                                {
                                    word+=auxS;
                                    continue;
                                }
                            }
                            if(c=='<'){
                                string auxw="";
                                while(archivo.get(c) && c != '>'){
                                    auxw += c;
                                }
                                if(auxw=="br /"){
                                    word+='\n';
                                    auxw="";
                                }
                            }
                            if(c!='>'){word+=c;}
                        }
                        reading_value=true;
                        break;
                    //se crea nodo
                    default:
                        word += c;
                        bool leyendonombre = true;

                        while(archivo.get(c) && c != '>' ){
                            //esto es para los tags que se cierran en el mismo tag
                            if (c=='/'){
                                autocierre=true;
                            }
                            if(c==' '){
                                leyendonombre = false;
                            }
                            if(leyendonombre){
                                word += c;
                            }
                        }
                        //descarta los nodos que no se encuentran en la lista de atributos validos
                        if (!esAtributoValido(word,filtroactual)){
                            if(!autocierre){
                                //cout <<"saltandose tag "+word+" . . ."<<endl;
                                procesarHasta("</"+word+">",&archivo);
                            }
                            continue;
                        }

                        //crea el nodo y lo vuelve el nodo seleccionado
                        Nodo* auxN = new Nodo(word);
                        currNode->agregarHijo(auxN);

                        if (word=="similar_books") {filtroactual=similarbooks_atributes;}
                        //cout <<"se creo "+auxN->tag+" hijo de "+currNode->tag+" en archivo "+entry.path().filename().string()<<endl ;
                        if (!autocierre){
                            currNode=auxN;
                        }
                        autocierre=false;
                        reading_value=true;
                        word = "";
                        break;
                    }
                }
                else {
                    word += c;
                }
            }
            archivo.close();
        }
   }
};


int main() {

    TreeXml t("test","book");
    for(auto hijo : t.raiz.hijos[0]->hijos){

        cout << hijo->valor << endl;
    }
    return 0;
}
