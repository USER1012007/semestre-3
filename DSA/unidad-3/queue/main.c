#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CLIENTES 20

typedef struct {
  char (*cliente)[2];
  char (*ventanilla)[2];
  char (*turno)[5];

} Clientes;

Clientes colaGeneral[MAX_CLIENTES];
Clientes ventanilla1[MAX_CLIENTES], ventanilla2[MAX_CLIENTES],
    ventanilla3[MAX_CLIENTES];
Clientes colaAtendidos;

int obtenerNumero(const char *turno) { return atoi(turno + 2); }

int estaDuplicado(Clientes *s, char turno[10]) {}

void ingresarClientes(Clientes *s) {
  char turno[10];

  for (int i = 0; i < 20; i++) {
    int randomNumber = rand() % 1000;
    snprintf(turno, sizeof(turno), "C-%03d", randomNumber);
  }
}

void mostrarTurnos(Clientes *s, char stack) {
  if (s == NULL) {
    return;
  }

  printf("\nPila %c\n", stack);
  printf("Cliente Turno\n");
  for (int i = 0; i <= 20; i++) {
    printf("[%05d]      %s[%05d]      %s[%05d]      %s\n", s->cliente,
           s->ventanilla, s->turno);
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
  // if (origen->tope + 1 > maxAltura)
  //   maxAltura = origen->tope + 1;
  // if (aux->tope + 1 > maxAltura)
  //   maxAltura = aux->tope + 1;
  // if (destino->tope + 1 > maxAltura)
  //   maxAltura = destino->tope + 1;

  // for (int i = maxAltura - 1; i >= 0; i--) {
  //   if (i <= origen->tope) {
  //     printf("              %02d |  %-5s  |", i + 1, origen->turnos[i]);
  //   } else {
  //     printf("              %02d |  %-5s  |", i + 1, "");
  //   }
  //
  //   if (i <= aux->tope) {
  //     printf("       %02d |  %-5s  |", i + 1, aux->turnos[i]);
  //   } else {
  //     printf("       %02d |  %-5s  |", i + 1, "");
  //   }
  //
  //   if (i <= destino->tope) {
  //     printf("       %02d |  %-5s  |", i + 1, destino->turnos[i]);
  //   } else {
  //     printf("       %02d |  %-5s  |", i + 1, "");
  //   }
  //
  //   printf("\n");
  // }
  printf("\n");
}

void menu() {

  int op;
  do {
    printf("\nMenu:\n");
    printf("1. ingresar clientes al banco\n");
    printf("2. formar clientes a las ventanillas\n");
    printf("3. atender clientes\n");
    printf("4. Salir\n");
    printf("Opcion: ");
    if (scanf("%d", &op) != 1) {
      break;
    }

    switch (op) {
    case 1:
      ingresarClientes();
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    default:
      printf("opcion invalida\n");
    }
  } while (op != 4);
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  menu();
  return 0;
}
