#include "tree.h"

int main() {

    TreeXml t("test","book");
    
    t.imprimirTodo();

    t.borrar_ratings(4.5);
    cout << endl << "Ratings borrados" << endl << endl;

    t.imprimirTodo();
    
    return 0;
}
