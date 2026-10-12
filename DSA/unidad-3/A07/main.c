#include <stdio.h>
#include <stdlib.h>
// Definición de la estructura de TNodo
typedef struct TNodo {
  int dato;
  struct TNodo *prevNodo;
  struct TNodo *nextNodo;
} TNodo;
// Buscar un nodo de la lista:
// 1. Devuelve el nodo si lo encuentra.
// 2. Devuelve NULL si no existe.
void buscarNodo(TNodo **first, TNodo **nodo, int dato) {
  if ((*first)->dato == dato) {
    *nodo = *first;
  } else {
    *nodo = *first;
    while (((*nodo)->dato != dato) && ((*nodo)->nextNodo != *first)) {
      *nodo = (*nodo)->nextNodo;
    }
    if (!((*nodo)->dato == dato))
      *nodo = NULL;
  }
}
// Insertar un nuevo nodo a la lista
void insertarNodo(TNodo **first, int dato) {
  TNodo *newNodo, *nodoActual;
  newNodo = (TNodo *)malloc(sizeof(TNodo));
  newNodo->dato = dato;
  newNodo->prevNodo = newNodo->nextNodo = NULL;
  if (*first == NULL) {
    *first = newNodo;
    newNodo->nextNodo = newNodo->prevNodo = *first;
  } else {
    nodoActual = *first;
    if (nodoActual->nextNodo == *first) {
      newNodo->prevNodo = newNodo->nextNodo = nodoActual;
      nodoActual->prevNodo = nodoActual->nextNodo = newNodo;
      if (newNodo->dato < nodoActual->dato)
        *first = newNodo;
    } else {
      if (newNodo->dato < nodoActual->dato) {
        newNodo->nextNodo = nodoActual;
        newNodo->prevNodo = nodoActual->prevNodo;
        nodoActual->prevNodo->nextNodo = nodoActual->prevNodo = *first =
            newNodo;
      } else {
        while ((nodoActual->nextNodo != *first) &&
               (nodoActual->nextNodo->dato < newNodo->dato)) {

          nodoActual = nodoActual->nextNodo;
        }
        if (nodoActual->nextNodo == *first) {

          newNodo->nextNodo = *first;
          newNodo->prevNodo = nodoActual;
          nodoActual->nextNodo = (*first)->prevNodo = newNodo;
        } else {
          newNodo->nextNodo = nodoActual->nextNodo;
          newNodo->prevNodo = nodoActual;
          nodoActual->nextNodo->prevNodo = nodoActual->nextNodo = newNodo;
        }
      }
    }
  }
}
// Evaluar si la lista está vacía
int listaVacia(TNodo *first) { return (first == NULL); }
// Mostrar el contenido de la lista
void mostrarLista(TNodo *first) {
  TNodo *nodoActual = first;
  if (nodoActual == NULL) {
    printf("La lista está vacía. \n");
  } else {
    printf("%4d", nodoActual->dato);
    while (nodoActual->nextNodo != first) {
      nodoActual = nodoActual->nextNodo;
      printf("%4d", nodoActual->dato);
    }
    printf("\n");
    nodoActual = first->prevNodo;
    printf("%4d", nodoActual->dato);
    while (nodoActual != first) {
      nodoActual = nodoActual->prevNodo;
      printf("%4d", nodoActual->dato);
    }
    printf("\n");
  }
}
// Eliminar un nodo de la lista
void eliminarNodo(TNodo **first, TNodo **nodo) {
  TNodo *nodoActual = *nodo;
  if ((*first)->nextNodo == *first) {
    *first = NULL;
  } else if (nodoActual == *first) {
    *first = (*first)->nextNodo;
    (*first)->prevNodo = nodoActual->prevNodo;
    nodoActual->prevNodo->nextNodo = *first;
  }

  else if (nodoActual->nextNodo == *first) {
    nodoActual->prevNodo->nextNodo = *first;
    (*first)->prevNodo = nodoActual->prevNodo;
  } else {
    nodoActual->prevNodo->nextNodo = nodoActual->nextNodo;
    nodoActual->nextNodo->prevNodo = nodoActual->prevNodo;
  }
  nodoActual->prevNodo = nodoActual->nextNodo = *nodo = NULL;
  free(nodoActual);
}
// Eliminar los nodos de la lista
void eliminarLista(TNodo **first) {
  TNodo *nodoActual;
  (*first)->prevNodo->nextNodo = NULL;
  while (*first != NULL) {
    nodoActual = *first;
    *first = (*first)->nextNodo;
    free(nodoActual);
  }
}
// Ingresar el dato desde el teclado
int leerDato() {
  int dato;
  printf("Ingrese el dato: ");
  scanf("%d", &dato);
  return dato;
}
// Función Menu
int menu() {
  int opcion;
  printf("\n----MENÚ----\n");
  printf("\n1. Insertar elemento");
  printf("\n2. Mostrar lista");
  printf("\n3. Buscar elemento");
  printf("\n4. Eliminar elemento");
  printf("\n5. Eliminar lista");
  printf("\n6. Salir\n");
  printf("\nIngresa la opción: ");
  scanf("%d", &opcion);
  return opcion;
}
// Función principal donde inicia la ejecución del programa
int main() {
  int dato, opcion;
  TNodo *first, *nodo;
  first = nodo = NULL;
  do {

    opcion = menu();
    switch (opcion) {
    case 1:
      dato = leerDato();
      insertarNodo(&first, dato);
      printf("\nSe agregó el elemento %d a la lista... \n", dato);
      break;
    case 2:
      if (!listaVacia(first))
        mostrarLista(first);
      else
        printf("\nNo existe la lista...\n");
      break;
    case 3:
      if (!listaVacia(first)) {
        dato = leerDato();
        buscarNodo(&first, &nodo, dato);
        printf("%s ", nodo != NULL ? "\n Elemento encontrado...\n"
                                   : "\n Elemento NO encontrado...\n");
      } else {
        printf("\nNo existe la lista...\n");
      }
      break;
    case 4:
      if (!listaVacia(first)) {
        dato = leerDato();
        buscarNodo(&first, &nodo, dato);
        if (!(nodo == NULL))
          eliminarNodo(&first, &nodo);
        else
          printf("\n Elemento NO encontrado...\n");
      } else {
        printf("\nNo existe la lista...\n");
      }
      break;
    case 5:
      if (!listaVacia(first)) {
        eliminarLista(&first);
        printf("\nSe eliminó la lista cargada...\n");
      } else {
        printf("\nNo existe la lista...\n");
      }
      break;
    case 6:
      printf("\nSaliendo...\n");
      break;
    default:
      printf("\nOpción No válida.");
      break;
    }
  } while (opcion != 6);
  return 0;
}
