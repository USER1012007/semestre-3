// librerias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

// constantes para definir tamaños
#define MAX_PILA 50
#define MAX_BIN 20
#define MAX_FILA 512

/* registro de un momento del stack de memoria */
typedef struct {
  int paso;

  char operacion[10];
  char funcion[80];

  char estadoPila[500];
  char variablesLocales[120];
  char accionPendiente[120];

  char valorRetorno[30];
} RegistroMemoria;

/* registro de cada piramide que se pida (registros con crecimiento dinamico) */
typedef struct {
  int niveles;
  RegistroMemoria *registros;
  int numRegistros;
  int capacidad;
} Binomy;

/* arreglo principal de piramides guardadas */
Binomy binomial[MAX_BIN];
int numBinomiales = 0;

/* pila de llamadas activa  */
char pila[MAX_PILA][60];
int tope = 0;

/* obtiene el ancho actual de la terminal, si no se puede
   determinar, usa 80 como valor por defecto */
int obtenerAnchoPantalla(void) {
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
    return w.ws_col;
  }
  return 80;
}

// funciones auxiliares
void push_pila(const char *llamada) {
  strcpy(pila[tope], llamada);
  tope++;
}

void pop_pila(void) { tope--; }

/* funcion para obtener el estado de la pila */
void obtenerEstadoPila(char *destino) {
  if (tope == 0) {
    strcpy(destino, "sin elementos");
    return;
  }

  destino[0] = '\0';
  for (int i = tope - 1; i >= 0; i--) {
    strcat(destino, pila[i]);
    if (i != 0) {
      strcat(destino, " | ");
    }
  }
}

/* funcion para registrar eventos en la pila de memoria */
void registrarEvento(Binomy *bin, int *contador, const char *operacion,
                     const char *funcion, const char *variablesLocales,
                     const char *accionPendiente, const char *valorRetorno) {
  if (bin->numRegistros == bin->capacidad) {
    bin->capacidad = (bin->capacidad == 0) ? 64 : bin->capacidad * 2;
    bin->registros =
        realloc(bin->registros, bin->capacidad * sizeof(RegistroMemoria));
  }

  RegistroMemoria *r = &bin->registros[bin->numRegistros++];

  r->paso = (*contador)++;
  strcpy(r->operacion, operacion);
  strcpy(r->funcion, funcion);

  obtenerEstadoPila(r->estadoPila);

  strcpy(r->variablesLocales, variablesLocales);
  strcpy(r->accionPendiente, accionPendiente);
  strcpy(r->valorRetorno, valorRetorno);
}

/* funcion recursiva para obtener el coeficiente de cada elemento de la fila */
int obtenerCoeficiente(int n, int k, Binomy *bin, int *contador) {
  char llamada[40];
  sprintf(llamada, "obtenerCoeficiente(%d, %d)", n, k);
  push_pila(llamada);

  char vars[40];
  sprintf(vars, "n = %d, k = %d", n, k);
  registrarEvento(bin, contador, "PUSH", llamada, vars, "Verificar caso base",
                  "");

  if (k == 0 || k == n) {
    registrarEvento(bin, contador, "RETURN", llamada, vars, "Retornar 1", "1");

    pop_pila();
    registrarEvento(bin, contador, "POP", llamada, "La funcion termino",
                    "Regresar a la llamada anterior", "1");

    return 1;
  }

  int izquierda = obtenerCoeficiente(n - 1, k - 1, bin, contador);
  int derecha = obtenerCoeficiente(n - 1, k, bin, contador);
  int resultado = izquierda + derecha;

  char varsRes[100];
  sprintf(varsRes, "izquierda = %d, derecha = %d, resultado = %d", izquierda,
          derecha, resultado);
  char accion[30];
  sprintf(accion, "Retornar %d", resultado);
  char ret[20];
  sprintf(ret, "%d", resultado);

  registrarEvento(bin, contador, "RETURN", llamada, varsRes, accion, ret);

  pop_pila();
  registrarEvento(bin, contador, "POP", llamada, "La funcion termino",
                  "Regresar a la llamada anterior", ret);

  return resultado;
}

/* funcion para darle forma a cada termino de la fila, el resultado se
   concatena al final del buffer "destino" */
void formatearTermino(char *destino, int coef, int expA, int expB,
                      int esPrimero) {
  char temp[40] = "";

  if (!esPrimero) {
    strcat(temp, " + ");
  }

  if (coef > 1 || (expA == 0 && expB == 0)) {
    char num[12];
    sprintf(num, "%d", coef);
    strcat(temp, num);
  }

  if (expA == 1) {
    strcat(temp, "a");
  } else if (expA > 1) {
    char e[16];
    sprintf(e, "a^%d", expA);
    strcat(temp, e);
  }

  if (expB == 1) {
    strcat(temp, "b");
  } else if (expB > 1) {
    char e[16];
    sprintf(e, "b^%d", expB);
    strcat(temp, e);
  }

  strcat(destino, temp);
}

/* funcion recursiva para armar cada fila de la torre, "fila" es el buffer
   donde se va concatenando el texto de la fila completa */
void imprimirFila(int n, int k, Binomy *bin, int *contador, char *fila) {
  char llamada[30];
  sprintf(llamada, "imprimirFila(%d, %d)", n, k);
  push_pila(llamada);

  char vars[30];
  sprintf(vars, "n = %d, k = %d", n, k);
  registrarEvento(bin, contador, "PUSH", llamada, vars, "Verificar si k > n",
                  "");

  if (k > n) {
    char msg[50];
    sprintf(msg, "k = %d es mayor que n = %d", k, n);
    registrarEvento(bin, contador, "RETURN", llamada, msg,
                    "Terminar imprimirFila", "");

    pop_pila();
    registrarEvento(bin, contador, "POP", llamada, "La funcion termino",
                    "Regresar a imprimirTorre", "");
    return;
  }

  int coef = obtenerCoeficiente(n, k, bin, contador);
  int expA = n - k;
  int expB = k;

  formatearTermino(fila, coef, expA, expB, k == 0);

  // recursividad
  imprimirFila(n, k + 1, bin, contador, fila);

  registrarEvento(bin, contador, "RETURN", llamada, vars,
                  "Terminar imprimirFila", "");

  pop_pila();
  registrarEvento(bin, contador, "POP", llamada, "La funcion termino",
                  "Regresar a imprimirTorre", "");
}

/* funcion recursiva que arma cada fila y la imprime centrada respecto
   al ancho real de la pantalla */
void imprimirTorre(int nivelActual, int nivelMaximo, Binomy *bin,
                   int *contador) {
  char llamada[40];
  sprintf(llamada, "imprimirTorre(%d, %d)", nivelActual, nivelMaximo);
  push_pila(llamada);

  char vars[60];
  sprintf(vars, "nivelActual = %d, nivelMaximo = %d", nivelActual, nivelMaximo);
  registrarEvento(bin, contador, "PUSH", llamada, vars, "Verificar caso base",
                  "");

  if (nivelActual > nivelMaximo) {
    registrarEvento(bin, contador, "RETURN", llamada, vars,
                    "Terminar imprimirTorre", "");

    pop_pila();
    registrarEvento(bin, contador, "POP", llamada, "La funcion termino",
                    "Fin de ejecucion", "");
    return;
  }

  char fila[MAX_FILA];
  sprintf(fila, "(a + b)^%d = ", nivelActual);
  imprimirFila(nivelActual, 0, bin, contador, fila);

  int anchoPantalla = obtenerAnchoPantalla();
  int espacios = (anchoPantalla - (int)strlen(fila)) / 2;
  if (espacios < 0) {
    espacios = 0;
  }

  printf("%*s%s\n", espacios, "", fila);

  imprimirTorre(nivelActual + 1, nivelMaximo, bin, contador);

  registrarEvento(bin, contador, "RETURN", llamada, vars,
                  "Terminar imprimirTorre", "");

  pop_pila();
  registrarEvento(bin, contador, "POP", llamada, "La funcion termino",
                  "Regresar al nivel anterior", "");
}

/* funcion para mostrar la tabla de la pila de memoria */
void mostrarTabla(const Binomy *bin) {
  if (bin->numRegistros == 0) {
    printf("\nno hay registros para mostrar.\n");
    return;
  }

  printf("niveles de la piramide: %d\n\n", bin->niveles);

  printf("%-8s%-12s%-35s%-35s%-40s%-20s\n", "indice", "operacion",
         "estado de la pila", "variables locales", "accion pendiente",
         "valor de retorno");

  printf("--------------------------------------------------------------------"
         "-----------------------------------------------------------------"
         "------------\n");

  for (int i = 0; i < bin->numRegistros; i++) {
    const RegistroMemoria *r = &bin->registros[i];

    char estadoPila[500];
    strcpy(estadoPila, r->estadoPila);
    if (strlen(estadoPila) > 33) {
      estadoPila[30] = '\0';
      strcat(estadoPila, "...");
    }

    char variables[120];
    strcpy(variables, r->variablesLocales);
    if (strlen(variables) > 33) {
      variables[30] = '\0';
      strcat(variables, "...");
    }

    char accion[120];
    strcpy(accion, r->accionPendiente);
    if (strlen(accion) > 38) {
      accion[35] = '\0';
      strcat(accion, "...");
    }

    char retorno[30];
    strcpy(retorno, r->valorRetorno);
    if (strlen(retorno) > 18) {
      retorno[15] = '\0';
      strcat(retorno, "...");
    }

    printf("%-8d%-12s%-35s%-35s%-40s%-20s\n", r->paso, r->operacion, estadoPila,
           variables, accion, retorno);
  }
}

/* funcion para mostrar el menu para que el usuario elija que pila mostrar */
void pilaMenu(void) {
  if (numBinomiales == 0) {
    printf("\nno hay ejecuciones guardadas\n");
    return;
  }

  printf("\npilas guardadas\n");
  for (int i = 0; i < numBinomiales; i++) {
    printf("%d. piramide con %d niveles\n", i + 1, binomial[i].niveles);
  }

  int opcion;
  printf("\nseleccione una piramide: ");
  scanf("%d", &opcion);

  if (opcion < 1 || opcion > numBinomiales) {
    printf("\nOpcion invalida\n");
    return;
  }

  mostrarTabla(&binomial[opcion - 1]);
}

/* funcion del menu */
void menu(void) {
  int opcion = 0;

  while (opcion != 3) {
    printf("1. Mostrar piramide\n");
    printf("2. Ver simulacion de pila\n");
    printf("3. Salir\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    if (opcion == 1) {
      int n;
      printf("numero de niveles: ");
      scanf("%d", &n);

      if (n >= 0 && n < 10 && numBinomiales < MAX_BIN) {
        Binomy *bin = &binomial[numBinomiales];
        bin->niveles = n;
        bin->registros = NULL;
        bin->numRegistros = 0;
        bin->capacidad = 0;

        int contador = 1;
        tope = 0;

        imprimirTorre(0, n, bin, &contador);
        numBinomiales++;

        printf("piramide guardada\n");
      } else {
        printf("el numero debe estar entre 0 y 9\n");
      }
    } else if (opcion == 2) {
      pilaMenu();
    } else if (opcion == 3) {
      break;
    }
  }
}

int main(void) {
  menu();

  /* liberar memoria reservada dinamicamente */
  for (int i = 0; i < numBinomiales; i++) {
    free(binomial[i].registros);
  }

  return 0;
}
