#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TOTAL_FICHAS 28

typedef struct TNodo {
  int dato;
  int dato1;
  struct TNodo *prevNodo;
  struct TNodo *nextNodo;
} TNodo;

void pause(void) {
#if defined(_WIN32) || defined(_WIN64)
  system("pause");
#else
  printf("Presione Enter para continuar...");
  fflush(stdout);
  system("read -n 1 -s -p \"\"");
#endif
}

int listaVacia(TNodo *first) { return (first == NULL); }

void insertarNodo(TNodo **first, int dato, int dato1) {
  TNodo *newNodo = (TNodo *)malloc(sizeof(TNodo));
  newNodo->dato = dato;
  newNodo->dato1 = dato1;

  if (*first == NULL) {
    *first = newNodo;
    newNodo->nextNodo = *first;
    newNodo->prevNodo = *first;
    return;
  }

  TNodo *ultimo = (*first)->prevNodo;
  newNodo->nextNodo = *first;
  newNodo->prevNodo = ultimo;
  ultimo->nextNodo = newNodo;
  (*first)->prevNodo = newNodo;
}

void mostrarLista(TNodo *first, int isDestino) {
  if (first == NULL) {
    printf("Lista vacia\n");
    return;
  }

  TNodo *nodoActual = first;
  do {
    printf("[%d|%d]", nodoActual->dato, nodoActual->dato1);
    if (nodoActual->nextNodo != first) {
      printf(isDestino ? " - " : " , ");
    }
    nodoActual = nodoActual->nextNodo;
  } while (nodoActual != first);
  printf("\n");
}

void extraerNodo(TNodo **first, TNodo *nodoAEliminar) {
  if (*first == NULL || nodoAEliminar == NULL)
    return;

  if ((*first)->nextNodo == *first && *first == nodoAEliminar) {
    *first = NULL;
  } else {
    nodoAEliminar->prevNodo->nextNodo = nodoAEliminar->nextNodo;
    nodoAEliminar->nextNodo->prevNodo = nodoAEliminar->prevNodo;

    if (*first == nodoAEliminar) {
      *first = nodoAEliminar->nextNodo;
    }
  }

  free(nodoAEliminar);
}

void eliminarLista(TNodo **first) {
  if (*first == NULL)
    return;

  TNodo *actual = *first;
  TNodo *siguiente = NULL;

  (*first)->prevNodo->nextNodo = NULL;

  while (actual != NULL) {
    siguiente = actual->nextNodo;
    free(actual);
    actual = siguiente;
  }

  *first = NULL;
}

void generarFichas(TNodo **first) {
  if (*first != NULL) {
    eliminarLista(first);
  }

  int fichas[TOTAL_FICHAS][2];
  int index = 0;

  for (int i = 0; i <= 6; i++) {
    for (int j = i; j <= 6; j++) {
      fichas[index][0] = i;
      fichas[index][1] = j;
      index++;
    }
  }

  for (int i = TOTAL_FICHAS - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp0 = fichas[i][0];
    int temp1 = fichas[i][1];
    fichas[i][0] = fichas[j][0];
    fichas[i][1] = fichas[j][1];
    fichas[j][0] = temp0;
    fichas[j][1] = temp1;
  }

  for (int i = 0; i < TOTAL_FICHAS; i++) {
    insertarNodo(first, fichas[i][0], fichas[i][1]);
  }

  printf("\nLista Origen\n");
  mostrarLista(*first, 0);
}

TNodo *buscarFichaCompatible(TNodo *first, int val1, int val2,
                             int *ladoCoincide) {
  if (first == NULL)
    return NULL;

  TNodo *actual = first;
  do {
    if (val1 == val2) {
      if (actual->dato == val1 && actual->dato1 == val2) {
        *ladoCoincide = 1;
        return actual;
      }
    } else {
      if (actual->dato == val2) {
        *ladoCoincide = 2;
        return actual;
      }
      if (actual->dato1 == val2) {
        *ladoCoincide = 3;
        return actual;
      }
    }
    actual = actual->nextNodo;
  } while (actual != first);

  return NULL;
}

void iniciarJuego(TNodo **listaOrigen, TNodo **listaDestino) {
  if (listaVacia(*listaOrigen)) {
    printf("\nNo hay fichas generadas en la lista origen.\n");
    return;
  }

  if (*listaDestino != NULL) {
    eliminarLista(listaDestino);
  }

  int lado = 0;
  TNodo *fichaEncontrada = buscarFichaCompatible(*listaOrigen, 6, 6, &lado);

  if (fichaEncontrada != NULL) {
    insertarNodo(listaDestino, 6, 6);
    extraerNodo(listaOrigen, fichaEncontrada);
    printf("\nSe colocó la ficha [6|6] en la lista destino.\n");
  } else {
    printf("\nNo se pudo iniciar el juego: la ficha 6|6 no está disponible.\n");
    return;
  }

  printf("\nLista Origen:\n");
  mostrarLista(*listaOrigen, 0);
  printf("Lista Destino:\n");
  mostrarLista(*listaDestino, 1);

  pause();

  while (!listaVacia(*listaOrigen)) {
    int extremoLibre = (*listaDestino)->prevNodo->dato1;

    fichaEncontrada =
        buscarFichaCompatible(*listaOrigen, -1, extremoLibre, &lado);

    if (fichaEncontrada == NULL) {
      printf("\nEl juego se ha cerrado (no hay fichas compatibles en la lista "
             "origen).\n");
      return;
    }

    int d1 = fichaEncontrada->dato;
    int d2 = fichaEncontrada->dato1;

    if (lado == 3) {
      int temp = d1;
      d1 = d2;
      d2 = temp;
    }

    insertarNodo(listaDestino, d1, d2);
    extraerNodo(listaOrigen, fichaEncontrada);

    printf("\nLista Origen:\n");
    mostrarLista(*listaOrigen, 0);
    printf("Lista Destino:\n");
    mostrarLista(*listaDestino, 1);

    if (!listaVacia(*listaOrigen)) {
      pause();
    }
  }

  printf(
      "\nEl juego ha terminado (se agotaron las fichas de la lista origen).\n");
}

int menu() {
  int opcion;
  printf("\n---- MENÚ ----\n");
  printf("1. Generar fichas\n");
  printf("2. Iniciar juego\n");
  printf("3. Eliminar fichas\n");
  printf("4. Salir\n");
  printf("Ingresa la opción: ");
  if (scanf("%d", &opcion) != 1)
    return 4;

  return opcion;
}

int main() {
  srand((unsigned int)time(NULL));
  int opcion;
  TNodo *listaOrigen = NULL;
  TNodo *listaDestino = NULL;

  do {
    opcion = menu();
    switch (opcion) {
    case 1:
      generarFichas(&listaOrigen);
      break;
    case 2:
      iniciarJuego(&listaOrigen, &listaDestino);
      break;
    case 3:
      if (listaVacia(listaDestino) && listaVacia(listaOrigen)) {
        printf("\nNo hay fichas por eliminar.\n");
      } else {
        eliminarLista(&listaDestino);
        printf("\nSe han eliminado todas las fichas de la lista DESTINO.\n");
      }
      break;
    case 4:
      printf("\nSaliendo...\n");
      eliminarLista(&listaOrigen);
      eliminarLista(&listaDestino);
      break;
    default:
      printf("\nOpción no válida.\n");
      break;
    }
  } while (opcion != 4);

  return 0;
}
