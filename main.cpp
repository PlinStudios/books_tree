#include "tree.h"

int main(int argc, char *argv[]) {
    int count = 0;

    //limite de archivos
    if (argc==2) count = stoi(argv[1]);

    TreeXml t("books_xml","book",count);
    
    //t.imprimirTodo();

    t.borrar_ratings(4.5);
    cout << endl << "Ratings borrados" << endl << endl;

    //t.imprimirTodo();
    t.precursores();
    
    return 0;
}
