#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

typedef struct {
  int tope;
  int capacidad;
  char (*turnos)[10];
} Pila;

void inicializarCola(Cola *c) {
  c->frente = 0;
  c->final = -1;
  c->tamano = 0;
}

int isEmptyCola(Cola *c) { return c->tamano == 0; }

int pushQueue(Cola *c, Cliente nuevo) {
  if (c->tamano == MAX_CLIENTES)
    return 0;
  c->final = (c->final + 1) % MAX_CLIENTES;
  c->elementos[c->final] = nuevo;
  c->tamano++;
  return 1;
}

int popQueue(Cola *c, Cliente *atendido) {
  if (isEmptyCola(c))
    return 0;
  *atendido = c->elementos[c->frente];
  c->frente = (c->frente + 1) % MAX_CLIENTES;
  c->tamano--;
  return 1;
}

void mostrarCola(Cola *c, const char *titulo) {
  printf("\n        %s\n", titulo);
  printf("   %-8s %-8s %-10s\n", "Cliente", "Turno", "Ventanilla");
  printf("   ----------------------------\n");
  for (int r = c->tamano - 1; r >= 0; r--) {
    Cliente *p = &c->elementos[(c->frente + r) % MAX_CLIENTES];
    printf("   | %02d       F-%03d      %02d     |\n", p->cliente, p->turno,
           p->ventanilla);
  }
  printf("   ----------------------------\n");
}

void mostrarVentanillas(Cola ventanilla[NUM_VENT]) {
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
        Cliente *p =
            &ventanilla[w].elementos[(ventanilla[w].frente + r) % MAX_CLIENTES];
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

void procesarAtencion(Cola *colaGeneral, Cola *colaAtendidos,
                      Cola ventanillas[NUM_VENT]) {
  if (isEmptyCola(colaGeneral)) {
    printf("\nFila general vacía.\n");
  } else {
    printf("\n--- FORMANDO CLIENTES EN VENTANILLAS ---\n");
    int paso = 0;
    while (!isEmptyCola(colaGeneral)) {
      Cliente tmp;
      popQueue(colaGeneral, &tmp);
      pushQueue(&ventanillas[tmp.ventanilla - 1], tmp);
      mostrarVentanillas(ventanillas);
    }
  }

  int total = 0;
  for (int w = 0; w < NUM_VENT; w++)
    total += ventanillas[w].tamano;
  if (total == 0)
    return;

  printf("\n--- ATENDIENDO CLIENTES POR TURNO ---\n");
  int paso = 0;
  while (1) {
    int mejor = -1;
    for (int w = 0; w < NUM_VENT; w++) {
      if (ventanillas[w].tamano == 0)
        continue;
      if (mejor == -1 ||
          ventanillas[w].elementos[ventanillas[w].frente].turno <
              ventanillas[mejor].elementos[ventanillas[mejor].frente].turno)
        mejor = w;
    }
    if (mejor == -1)
      break;

    Cliente tmp;
    popQueue(&ventanillas[mejor], &tmp);
    pushQueue(colaAtendidos, tmp);
  }
  mostrarCola(colaAtendidos, "Fila Clientes Atendidos");
}

void inicializarPila(Pila *p) {
  p->tope = -1;
  p->capacidad = 2;
  p->turnos = malloc(p->capacidad * sizeof(*p->turnos));
}

void liberarPila(Pila *p) {
  free(p->turnos);
  p->turnos = NULL;
}

int isEmptyPila(Pila *p) { return p->tope < 0; }

void pushStack(Pila *p, const char turno[10]) {
  p->tope++;
  if (p->tope >= p->capacidad) {
    p->capacidad *= 2;
    p->turnos = realloc(p->turnos, p->capacidad * sizeof(*p->turnos));
  }
  strncpy(p->turnos[p->tope], turno, 10);
}

int popStack(Pila *p, char turnoDestino[10]) {
  if (isEmptyPila(p))
    return 0;
  if (turnoDestino)
    strncpy(turnoDestino, p->turnos[p->tope], 10);
  p->tope--;
  return 1;
}

void mostrarPila(Pila *p) {
  printf("\n--- PILA DE DOCUMENTOS ---\n");
  if (isEmptyPila(p))
    printf("La pila está vacía.\n");
  else {
    for (int i = p->tope; i >= 0; i--)
      printf("| %s |\n", p->turnos[i]);
    printf("-------\n");
  }
}

int main() {
  srand(time(NULL));
  Cola colaGeneral, colaAtendidos, ventanillas[NUM_VENT];
  Pila pila;

  inicializarCola(&colaGeneral);
  inicializarCola(&colaAtendidos);
  for (int i = 0; i < NUM_VENT; i++)
    inicializarCola(&ventanillas[i]);
  inicializarPila(&pila);

  int op, sub;
  do {
    printf("\nMENÚ\n1. Atención a clientes (cola)\n2. Gestión de documentos "
           "(pila)\n3. Salir\nOpción: ");
    scanf("%d", &op);
    if (op == 1) {
      printf("A. Registrar (1)\nB. Atender (2)\nOpción: ");
      scanf("%d", &sub);
      if (sub == 1) {
        Cliente nuevo;
        printf("Folio (3 dígitos): ");
        scanf("%d", &nuevo.cliente);
        nuevo.turno = rand() % 900 + 100;
        nuevo.ventanilla = rand() % NUM_VENT + 1;
        pushQueue(&colaGeneral, nuevo);
        mostrarCola(&colaGeneral, "Fila General");
      } else if (sub == 2) {
        procesarAtencion(&colaGeneral, &colaAtendidos, ventanillas);
      }
    } else if (op == 2) {
      printf("A. Agregar (1)\nB. Liberar (2)\nOpción: ");
      scanf("%d", &sub);
      if (sub == 1) {
        char doc[10];
        printf("Folio (D01): ");
        scanf("%s", doc);
        pushStack(&pila, doc);
      } else if (sub == 2) {
        char doc[10];
        popStack(&pila, doc);
      }
      mostrarPila(&pila);
    }
  } while (op != 3);

  liberarPila(&pila);
  return 0;
}
