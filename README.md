# books_tree

## compilacion
```bash
g++ main.cpp tree.cpp -o main
./main
```

En el caso de querer procesar solo una cantidad especifica de los libros se puede ejecutar

```bash
./main cantidad
```


## como usar
La carpeta "books_xml" que contiene los libros debe estar en el mismo directorio que el ejecutable


se dan 5 opciones al terminar de procesar los libros

1 - listar ids                    - para mostrar las ids de todos los libros

2 - listar id precursores         - para mostrar las id de los libros que son precursores

3 - borrar libros con bajo rating - para eliminar libros que tengan un menor rating que el numero especificado

4 - imprimir TODO                 - para mostrar todo el arbol

5 - salir                         - para salir del programa

