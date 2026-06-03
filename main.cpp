#include "tree.h"

int main(int argc, char *argv[]) {
    int count = 0;

    //limite de archivos
    if (argc==2) count = stoi(argv[1]);

    TreeXml t("books_xml","book",count);

    while (true){

        int opcion;
        cout << endl << "seleccione consulta" << endl;
        cout << "1 - listar ids" << endl;
        cout << "2 - listar id precursores" << endl;
        cout << "3 - borrar libros con bajo rating" << endl;
        cout << "4 - imprimir TODO" <<endl;
        cout << "5 - salir" << endl;
        cin >> opcion;

        //ejecuta las peticiones
        switch (opcion)
        {
        case 1: //1 - listar ids
            t.listar();
            break;
        case 2: //2 - listar id precursores
            t.precursores();
            break;
        case 3: //3 - borrar libros con bajo rating
            float r;
            cout << "rating a borrar: ";
            cin >> r;
            t.borrar_ratings(r);
            break;
        case 4: //4 - imprimir TODO
            t.imprimirTodo();
            break;
        case 5: //5 - salir
            char s;
            cout << "esta seguro? (Y/n): ";
            cin >> s;
            if (s=='Y') return 0;
            break;
        default:
            break;
        }


    }

    return 0;
}
