#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CLIENTES 20
#define NUM_VENT 3

typedef struct {
  int cliente;
  int ventanilla;
  int turno;
} Cliente;

typedef struct {
  Cliente elementos[MAX_CLIENTES];
  int frente;
  int final;
  int tamano;
} Cola;

Cola colaGeneral;
Cola ventanilla[NUM_VENT];
Cola colaAtendidos;

void inicializarCola(Cola *c) {
  c->frente = 0;
  c->final = -1;
  c->tamano = 0;
}

void inicializarTodo() {
  inicializarCola(&colaGeneral);
  inicializarCola(&colaAtendidos);
  for (int i = 0; i < NUM_VENT; i++)
    inicializarCola(&ventanilla[i]);
}

int push(Cola *c, Cliente nuevo) {
  if (c->tamano == MAX_CLIENTES)
    return 0;
  c->final = (c->final + 1) % MAX_CLIENTES;
  c->elementos[c->final] = nuevo;
  c->tamano++;
  return 1;
}

int pop(Cola *c, Cliente *atendido) {
  if (c->tamano == 0)
    return 0;
  *atendido = c->elementos[c->frente];
  c->frente = (c->frente + 1) % MAX_CLIENTES;
  c->tamano--;
  return 1;
}

Cliente *elemento(Cola *c, int r) {
  return &c->elementos[(c->frente + r) % MAX_CLIENTES];
}

void mostrarCola(Cola *c, const char *titulo) {
  printf("\n        %s\n", titulo);
  printf("   %-8s %-8s %-10s\n", "Cliente", "Turno", "Ventanilla");
  printf("   ----------------------------\n");
  for (int r = c->tamano - 1; r >= 0; r--) {
    Cliente *p = elemento(c, r);
    printf("   | %02d       F-%03d      %02d     |\n", p->cliente, p->turno,
           p->ventanilla);
  }
  printf("   ----------------------------\n");
}

void mostrarVentanillas() {
  int max = 0;
  for (int w = 0; w < NUM_VENT; w++)
    if (ventanilla[w].tamano > max)
      max = ventanilla[w].tamano;

  printf("\n");
  for (int w = 0; w < NUM_VENT; w++)
    printf(" %-5s %-7s %-5s   ", "Cli.", "Turno", "Vent.");
  printf("\n");

  for (int r = max - 1; r >= 0; r--) {
    for (int w = 0; w < NUM_VENT; w++) {
      if (r < ventanilla[w].tamano) {
        Cliente *p = elemento(&ventanilla[w], r);
        char a[8], b[8], c[8];
        snprintf(a, sizeof a, "%02d", p->cliente);
        snprintf(b, sizeof b, "F-%03d", p->turno);
        snprintf(c, sizeof c, "%02d", p->ventanilla);
        printf(" %-5s %-7s %-5s   ", a, b, c);
      } else {
        printf("%-20s   ", "");
      }
    }
    printf("\n");
  }
  for (int w = 0; w < NUM_VENT; w++)
    printf("   [Ventanilla %02d]      ", w + 1);
  printf("\n");
}

void ingresarClientes() {
  inicializarTodo();
  int turno = 0;
  for (int i = 1; i <= MAX_CLIENTES; i++) {
    turno += 1 + rand() % 49;
    Cliente nuevo;
    nuevo.cliente = i;
    nuevo.turno = turno;
    nuevo.ventanilla = rand() % NUM_VENT + 1;
    push(&colaGeneral, nuevo);
  }
  mostrarCola(&colaGeneral, "Fila General");
}

void formarVentanillas() {
  if (colaGeneral.tamano == 0) {
    printf("\nFila general vacia. Usar opcion 1.\n");
    return;
  }
  printf("\nOpcion 2: Formar clientes en ventanillas\n");
  int paso = 0;
  printf("\nPaso %02d:\n", paso);
  mostrarCola(&colaGeneral, "Fila General");
  mostrarVentanillas();

  Cliente tmp;
  while (pop(&colaGeneral, &tmp)) {
    push(&ventanilla[tmp.ventanilla - 1], tmp);
    paso++;
    printf("\nPaso %02d:\n", paso);
    mostrarCola(&colaGeneral, "Fila General");
    mostrarVentanillas();
  }
}

void atenderClientes() {
  int total = 0;
  for (int w = 0; w < NUM_VENT; w++)
    total += ventanilla[w].tamano;

  if (total == 0) {
    printf("\nventanillas vacias\n");
    return;
  }

  int paso = 0;
  printf("\nPaso %02d:\n", paso);
  mostrarVentanillas();
  mostrarCola(&colaAtendidos, "Fila Clientes Atendidos");

  while (1) {
    int mejor = -1;
    for (int w = 0; w < NUM_VENT; w++) {
      if (ventanilla[w].tamano == 0)
        continue;
      if (mejor == -1 || elemento(&ventanilla[w], 0)->turno <
                             elemento(&ventanilla[mejor], 0)->turno)
        mejor = w;
    }
    if (mejor == -1)
      break;

    Cliente tmp;
    pop(&ventanilla[mejor], &tmp);
    push(&colaAtendidos, tmp);

    paso++;
    printf("\nPaso %02d:\n", paso);
    mostrarVentanillas();
    mostrarCola(&colaAtendidos, "Fila Clientes Atendidos");
  }
}

void menu() {
  int op;
  do {
    printf("\nMenu:\n");
    printf("1. Ingresar clientes al banco\n");
    printf("2. Formar clientes en ventanillas\n");
    printf("3. Atender clientes\n");
    printf("4. Salir\n");
    printf("Opcion: ");
    if (scanf("%d", &op) != 1)
      break;

    switch (op) {
    case 1:
      ingresarClientes();
      break;
    case 2:
      formarVentanillas();
      break;
    case 3:
      atenderClientes();
      break;
    case 4:
      break;
    default:
      printf("Opcion invalida\n");
    }
  } while (op != 4);
}

int main() {
  srand(time(NULL));
  inicializarTodo();
  menu();
  return 0;
}
