#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  int tope;
  int capacidad;
  char (*turnos)[10];
} Clientes;

void inicializar(Clientes *s) {
  s->tope = -1;
  s->capacidad = 2;
  s->turnos = malloc(s->capacidad * sizeof(*s->turnos));
}

void push(Clientes *s, char turno[10]) {
  s->tope++;

  if (s->tope >= s->capacidad) {
    s->capacidad *= 2;
    char (*temp)[10] = realloc(s->turnos, s->capacidad * sizeof(*s->turnos));

    s->turnos = temp;
  }

  strncpy(s->turnos[s->tope], turno, 10);
  s->turnos[s->tope][9] = '\0';
}

char pop(Clientes *s, char turno[10]) {
  if (s->tope < 0) {
    printf("No hay turnos registrados.\n");
    return '\0';
  }

  s->tope--;

  char (*temp)[10] = realloc(s->turnos, s->capacidad - 1 * sizeof(*s->turnos));

  s->turnos = temp;
  return *s->turnos[s->tope + 1];
}

int estaDuplicado(Clientes *s, char turno[10]) {
  for (int i = 0; i <= s->tope; i++) {
    if (strcmp(s->turnos[i], turno) == 0) {
      return 1;
    }
  }
  return 0;
}

void generarTurno(Clientes *s) {
  int randomNumber = rand() % 1000;
  char turno[10];

  snprintf(turno, sizeof(turno), "C-%03d", randomNumber);

  if (estaDuplicado(s, turno)) {
    printf("turno duplicado generado: %s. generando otro turno\n", turno);
    generarTurno(s);
    return;
  }

  push(s, turno);
  printf("turno generado: %s\n", s->turnos[s->tope]);
}

void ordenarTurnos(Clientes *s) {
  if (s->tope < 0) {
    printf("No hay turnos registrados.\n");
    return;
  }

  Clientes pilaAux, pilaOrdenada;
  inicializar(&pilaAux);
  inicializar(&pilaOrdenada);
  for (int i = 0; i <= s->tope; i++) {
  }
}

void mostrarTurnos(Clientes *s) {
  if (s->tope < 0) {
    printf("No hay turnos registrados.\n");
    return;
  }
  printf("\nlista de turnos\n");
  for (int i = 0; i <= s->tope; i++) {
    printf("[%d] %s\n", i + 1, s->turnos[i]);
  }
}

void liberar(Clientes *s) {
  free(s->turnos);
  s->turnos = NULL;
  s->tope = -1;
  s->capacidad = 0;
}

void menu() {
  Clientes s;
  inicializar(&s);

  int op;
  do {
    printf("\nMenu:\n");
    printf("1. Generar turno\n");
    printf("2. Ordenar turnos\n");
    printf("3. Mostrar turnos\n");
    printf("4. Salir\n");
    printf("Opcion: ");
    scanf("%d", &op);

    switch (op) {
    case 1:
      generarTurno(&s);
      break;
    case 2:
      ordenarTurnos(&s);
      break;
    case 3:
      mostrarTurnos(&s);
      break;
    default:
      printf("Opcion invalida.\n");
    }
  } while (op != 4);

  liberar(&s);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  menu();
  return 0;
}
