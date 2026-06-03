#include "tree.h"


//lista los libros que son mas antiguos a sus libros similares
void TreeXml::precursores(){
    //accede a los libros
    for(Nodo *libro : raiz.hijos){
        bool es_precursor=true;
        int publication_year;
        //obtiene año del libro
        try{
            publication_year = stoi((*libro)["publication_year"].valor);
        }catch(exception e){
            continue;
        }

        //itera sobre los libros similares
        for(Nodo *sim_libro : (*libro)["similar_books"].hijos){
            //obtiene año del libro similar
            int sim_publication_year;
            try{
                sim_publication_year = stoi((*sim_libro)["publication_year"].valor);
            }catch(exception e){
                continue;
            }

            //compara
            if (sim_publication_year<=publication_year){
                es_precursor=false;
                break;
            }
        }

        //lista los que si son precursores
        if (es_precursor)
            cout << (*libro)["id"].valor << endl;
    }
}

//elimina todos los libros con rating promedio menor o igual a r
void TreeXml::borrar_ratings(double r){
    //accede a los libros
    auto it = raiz.hijos.begin();
    while (it != raiz.hijos.end()) {
        //obtiene su rating
        double rating = atof((**it)["average_rating"].valor.c_str());

        //elimina si rating es menor o igual
        if (rating <= r){
            Nodo* borrar = *it;
            //remueve de la lista del padre
            it = raiz.hijos.erase(it);
            //llama destructor
            delete borrar;
        }else
            ++it;
    }

}

void TreeXml::imprimir_subtree(Nodo *start, string prefix){
    cout << prefix << start->tag << " : " << start->valor << endl;
    for(auto hijo : start->hijos){
        imprimir_subtree(hijo, prefix+"  ");
    }
}

void TreeXml::imprimirTodo(){
    imprimir_subtree(&raiz);
}
void TreeXml::imprimirLibro(size_t index){
    imprimir_subtree(raiz.hijos[index]);
}


bool TreeXml::esAtributoValido(const string& tag, const vector<string>& atris) {
        for (const auto& a : atris)
            if (a == tag) return true;
        return false;
    }

void TreeXml::listar() {
    stack<Nodo*> pila;
    pila.push(&raiz);

    //recorre preorder
    while (!pila.empty()) {
        Nodo* nodo = pila.top();
        pila.pop();

        if (nodo->tag == "id")
            cout << nodo->valor << endl;

        for (auto hijo : nodo->hijos) {
            pila.push(hijo);
        }
    }
}

void TreeXml::procesarHasta(const string& palabra, ifstream* archivo){
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

TreeXml::TreeXml(string folder,string initialNode, size_t limit) : raiz("root"){

        int i=0;
        int ip=0;

        for (const auto& entry : filesystem::directory_iterator(folder)) {
            if (limit>0 && i>=limit) break;

            ifstream archivo(entry.path());
            if (!archivo.is_open()) return;

            i++;
            if (i>=ip+10){
                cout << '\r' << "xml procesados: " << i;
                ip=i;
            }

            //inicializa el arbol en un nodo raiz
            Nodo* currNode = &raiz;

            //señala que esta leyendo el valor de un nodo
            bool reading_value=false;

            //señala cierre de nodos instantaneos
            bool autocierre=false;

            //señala cual de los dos filtros de atributos estan siendo ocupados al momento de crear los nodos hijos
            vector<string> filtroactual=book_atributes;
            string word;
            word.reserve(1024);
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
        cout << '\r' << "xml procesados: " << i << endl;
   }
