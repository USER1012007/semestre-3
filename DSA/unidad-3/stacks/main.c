#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  int tope;
  int capacidad;
  char (*turnos)[10];
} Clientes;

Clientes pilaPrincipal, pilaOrdenada;

void inicializar(Clientes *s) {
  s->tope = -1;
  s->capacidad = 2;
  s->turnos = malloc(s->capacidad * sizeof(*s->turnos));
}

void liberar(Clientes *s) {
  if (s->turnos != NULL) {
    free(s->turnos);
    s->turnos = NULL;
  }
  s->tope = -1;
  s->capacidad = 0;
}

void push(Clientes *s, const char turno[10]) {
  s->tope++;
  if (s->tope >= s->capacidad) {
    s->capacidad *= 2;
    char (*temp)[10] = realloc(s->turnos, s->capacidad * sizeof(*s->turnos));
    if (temp == NULL) {
      printf("Error de asignacion de memoria.\n");
      exit(1);
    }
    s->turnos = temp;
  }
  strncpy(s->turnos[s->tope], turno, 10);
  s->turnos[s->tope][9] = '\0';
}

int pop(Clientes *s, char turnoDestino[10]) {
  if (s->tope < 0) {
    return 0;
  }
  if (turnoDestino != NULL) {
    strncpy(turnoDestino, s->turnos[s->tope], 10);
    turnoDestino[9] = '\0';
  }
  s->tope--;
  return 1;
}

int estaDuplicado(Clientes *s, char turno[10]) {
  for (int i = 0; i <= s->tope; i++) {
    if (strcmp(s->turnos[i], turno) == 0) {
      return 1;
    }
  }
  return 0;
}

int obtenerNumero(const char *turno) { return atoi(turno + 2); }

void generarTurno(Clientes *s) {
  int randomNumber = rand() % 1000;
  char turno[10];

  snprintf(turno, sizeof(turno), "C-%03d", randomNumber);

  if (estaDuplicado(s, turno)) {
    generarTurno(s);
    return;
  }

  push(s, turno);
  printf("Turno generado: %s\n", s->turnos[s->tope]);
}

void mostrarTurnos(Clientes *s, char stack) {
  if (s == NULL || s->tope < 0) {
    return;
  }

  printf("\nPila %c\n", stack);
  printf("Cliente Turno\n");
  for (int i = 0; i <= s->tope; i++) {
    printf("[%02d]      %s\n", i + 1, s->turnos[i]);
  }
}

void formatearTabla(int paso, Clientes *origen, Clientes *aux,
                    Clientes *destino) {
  if (paso == 0) {
    printf("\nPaso 0:\n\n");
  } else {
    printf("\nPaso %02d:\n\n", paso);
  }

  printf("                 %-16s %-16s %-16s\n", "Pila Origen", "Pila Auxiliar",
         "Pila destino");
  printf("                    %-13s    %-13s    %-13s\n", "Turno", "Turno",
         "Turno");

  int maxAltura = 10;
  if (origen->tope + 1 > maxAltura)
    maxAltura = origen->tope + 1;
  if (aux->tope + 1 > maxAltura)
    maxAltura = aux->tope + 1;
  if (destino->tope + 1 > maxAltura)
    maxAltura = destino->tope + 1;

  for (int i = maxAltura - 1; i >= 0; i--) {
    if (i <= origen->tope) {
      printf("              %02d |  %-5s  |", i + 1, origen->turnos[i]);
    } else {
      printf("              %02d |  %-5s  |", i + 1, "");
    }

    if (i <= aux->tope) {
      printf("       %02d |  %-5s  |", i + 1, aux->turnos[i]);
    } else {
      printf("       %02d |  %-5s  |", i + 1, "");
    }

    if (i <= destino->tope) {
      printf("       %02d |  %-5s  |", i + 1, destino->turnos[i]);
    } else {
      printf("       %02d |  %-5s  |", i + 1, "");
    }

    printf("\n");
  }
  printf("\n");
}

int obtenerMaxNum(Clientes *s, int Bool) {
  int max_val = -1;
  int max_index = -1;
  for (int i = 0; i <= s->tope; i++) {
    int val = obtenerNumero(s->turnos[i]);
    if (val > max_val) {
      max_val = val;
      max_index = i;
    }
  }
  if (Bool) {
    return max_index;
  }
  return max_val;
}

void ordenarTurnos(Clientes *s) {
  if (s->tope < 0) {
    printf("No hay turnos registrados para ordenar.\n");
    return;
  }

  liberar(&pilaOrdenada);
  inicializar(&pilaOrdenada);

  Clientes pilaAuxiliar, pilaTrabajo;
  inicializar(&pilaAuxiliar);
  inicializar(&pilaTrabajo);

  for (int i = 0; i <= s->tope; i++) {
    push(&pilaTrabajo, s->turnos[i]);
  }

  int paso = 0;
  formatearTabla(paso++, &pilaTrabajo, &pilaAuxiliar, &pilaOrdenada);

  while (pilaTrabajo.tope >= 0) {

    int max_index = obtenerMaxNum(&pilaTrabajo, 1);
    int max_val = obtenerMaxNum(&pilaTrabajo, 0);

    while (pilaTrabajo.tope > max_index) {
      char temp[10];
      pop(&pilaTrabajo, temp);
      push(&pilaAuxiliar, temp);
    }

    char max_turno[10];
    pop(&pilaTrabajo, max_turno);
    push(&pilaOrdenada, max_turno);

    formatearTabla(paso++, &pilaTrabajo, &pilaAuxiliar, &pilaOrdenada);

    if (pilaAuxiliar.tope >= 0) {
      while (pilaAuxiliar.tope >= 0) {
        char temp[10];
        pop(&pilaAuxiliar, temp);
        push(&pilaTrabajo, temp);
      }
      formatearTabla(paso++, &pilaTrabajo, &pilaAuxiliar, &pilaOrdenada);
    }
  }

  liberar(&pilaAuxiliar);
  liberar(&pilaTrabajo);
  printf("Turnos ordenados exitosamente.\n");
}

void menu() {
  inicializar(&pilaPrincipal);
  inicializar(&pilaOrdenada);

  int op;
  do {
    printf("\nMenu:\n");
    printf("1. Generar turno\n");
    printf("2. Ordenar turnos\n");
    printf("3. Salir\n");
    printf("Opcion: ");
    if (scanf("%d", &op) != 1) {
      break;
    }

    switch (op) {
    case 1:
      generarTurno(&pilaPrincipal);
      mostrarTurnos(&pilaPrincipal, 'P');
      break;
    case 2:
      ordenarTurnos(&pilaPrincipal);
      break;
    case 3:
      break;
    default:
      printf("opcion invalida\n");
    }
  } while (op != 3);

  liberar(&pilaPrincipal);
  liberar(&pilaOrdenada);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  menu();
  return 0;
}
