// librerias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// constantes para definir los tamaños de los arreglos
#define MAX_PILA 100
#define MAX_HIST 2000
#define MAX_ARR 20

char pila_frames[MAX_PILA][40];
int tope = 0;

// objeto para almacenar la informacion de cada evento
typedef struct {
  int indice;
  char operacion[5];
  char pila[400];
  char variables[60];
  char pendiente[60];
  char retorno[20];
} Evento;

Evento historial[MAX_HIST];
int hist_count = 0;
int contador = 0;

// funcion para agregar un frame al string
void push_frame(const char *frame) {
  strcpy(pila_frames[tope], frame);
  tope++;
}

// funcion para quitar un frame al string
void pop_frame() { tope--; }

// funcion para generar un snapshot de la pila
void snapshot_pila(char *destino) {
  destino[0] = '\0';
  strcat(destino, "      ");
  strcat(destino, pila_frames[tope - 1]);
  if (tope == 0)
    strcpy(destino, "pila vacia");
}

// funcion para registrar un evento en el historial
void registrar(int idx, const char *op, const char *vars, const char *pendiente,
               const char *retorno) {
  Evento *e = &historial[hist_count++];
  e->indice = idx;

  strcpy(e->operacion, op);
  snapshot_pila(e->pila);
  strcpy(e->variables, vars);
  strcpy(e->pendiente, pendiente);
  strcpy(e->retorno, retorno);
}

// funcion para mostrar el historial de eventos
void mostrar_stack() {
  printf("\n--- pila ---\n");
  printf("%-4s %-4s %-25s %-22s %-25s %-8s\n", "indice", "operacion",
         "estado de la pila", "variables locales", "accion pendiente",
         "valor de retorno");
  for (int i = 0; i < hist_count; i++) {
    Evento *e = &historial[i];
    // formateo de la tabla de la pila
    printf("%-4d     %-4s %-32s %-22s %-25s %-8s\n", e->indice, e->operacion,
           e->pila, e->variables, e->pendiente, e->retorno);
  }
}

// funcion auxiliar
void reiniciar_traza() {
  tope = 0;
  hist_count = 0;
  contador = 0;
}

// funcion recursiva para el mcd
int mcd(int a, int b) {
  contador++;
  int idx = contador;
  char frame[40];
  sprintf(frame, "mcd(%d,%d)", a, b);
  char vars[40];
  sprintf(vars, "a=%d, b=%d", a, b);
  char pend[60];
  if (b == 0)
    sprintf(pend, "caso base (b==0)");
  else
    sprintf(pend, "mcd(%d, %d %% %d)", b, a, b);

  push_frame(frame);
  registrar(idx, "PUSH", vars, pend, "-");

  int resultado;
  if (b == 0) {
    resultado = a;
  } else {
    resultado = mcd(b, a % b);
  }

  char ret[20];
  sprintf(ret, "%d", resultado);
  registrar(idx, "POP", vars, "retorno de la llamada", ret);
  pop_frame();
  return resultado;
}

// funcion para pedir los datos del mcd y llamar por primera vez la funcion
// recursiva
void opcion_mcd() {
  int a, b;
  printf("ingresa dos enteros positivos:\n");
  printf("valor A: ");
  scanf("%d", &a);
  printf("valor B: ");
  scanf("%d", &b);
  if (a <= 0 || b <= 0) {
    printf("ambos valores deben ser enteros positivos\n");
    return;
  }
  reiniciar_traza();
  int r = mcd(a, b);
  printf("el MCD de %d y %d es: %d\n", a, b, r);
  mostrar_stack();
}

// funcion recursiva del fibonacci
int fib(int n) {
  contador++;
  int idx = contador;
  char frame[30];
  sprintf(frame, "fib(%d)", n);
  char vars[30];
  sprintf(vars, "n=%d", n);
  char pend[60];
  if (n <= 1)
    sprintf(pend, "caso base (n<=1)");
  else
    sprintf(pend, "fib(%d)+fib(%d)", n - 1, n - 2);

  push_frame(frame);
  registrar(idx, "PUSH", vars, pend, "-");

  int resultado;
  if (n <= 1) {
    resultado = n;
  } else {
    resultado = fib(n - 1) + fib(n - 2);
  }

  char ret[20];
  sprintf(ret, "%d", resultado);
  registrar(idx, "POP", vars, "retorno de la llamada", ret);
  pop_frame();
  return resultado;
}

// funcion para pedir los datos de la serie de fibonacci y llamar por primera
// vez la funcion recursiva
void opcion_fibonacci() {
  int n;
  printf("ingresa el termino de fibonacci a calcular (0-15): ");
  scanf("%d", &n);
  if (n < 0 || n > 15) {
    printf("usa un valor entre 0 y 15 (para no saturar la traza)\n");
    return;
  }
  reiniciar_traza();
  int r = fib(n);
  printf("el termino %d de fibonacci es: %d\n", n, r);
  mostrar_stack();
}

// funcion para hacer la suma de los numeros aleatorios de forma recursiva
int suma(int arr[], int n) {
  contador++;
  int idx = contador;
  char frame[30];
  sprintf(frame, "suma(n=%d)", n);
  char vars[50];
  if (n > 0)
    sprintf(vars, "n=%d, arr[%d]=%d", n, n - 1, arr[n - 1]);
  else
    sprintf(vars, "n=%d", n);
  char pend[40];
  if (n == 0)
    sprintf(pend, "caso base (n==0)");
  else
    sprintf(pend, "suma(arr,%d)", n - 1);

  push_frame(frame);
  registrar(idx, "PUSH", vars, pend, "-");

  int resultado;
  if (n == 0) {
    resultado = 0;
  } else {
    resultado = arr[n - 1] + suma(arr, n - 1);
  }

  char ret[20];
  sprintf(ret, "%d", resultado);
  registrar(idx, "POP", vars, "retorno de la llamada", ret);
  pop_frame();
  return resultado;
}

// funcion para pedir la cantidad de numeros aleatorios, generar los numeros
// aleatorios y llamar por primera vez a la funcion recursiva
void opcion_suma() {
  int n, arreglo[MAX_ARR];
  printf("ingresa el numero de elementos (1-%d): ", MAX_ARR);
  scanf("%d", &n);
  if (n <= 0 || n > MAX_ARR) {
    printf("el tamaño debe estar entre 1 y %d\n", MAX_ARR);
    return;
  }
  srand((unsigned int)time(NULL));
  printf("lista: ");
  for (int i = 0; i < n; i++) {
    arreglo[i] = rand() % 100;
    printf("%d ", arreglo[i]);
  }
  printf("\n");

  reiniciar_traza();
  int r = suma(arreglo, n);
  printf("la suma de los elementos es: %d\n", r);
  mostrar_stack();
}

int main() {
  int opcion;
  do {
    // Menu principal
    printf("1. maximo comun divisor\n");
    printf("2. serie de fibonacci\n");
    printf("3. sumatoria de elementos\n");
    printf("4. salir\n");
    printf("teclee una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
    case 1:
      opcion_mcd();
      break;
    case 2:
      opcion_fibonacci();
      break;
    case 3:
      opcion_suma();
      break;
    case 4:
      break;
    }
  } while (opcion != 4);

  return 0;
}
