#include <iostream>
#include <stack>
#include <vector>

using namespace std;

struct binomy {
  stack<int> index;
  stack<string> operacion;
  stack<string> estadoPila;
  stack<string> variablesLocales;
  stack<string> accionPendiente;
};

int counter = 0;

vector<binomy> binomial;

int obtenerCoeficiente(int n, int k) {
  if (k == 0 || k == n) {
    return 1;
  }
  return obtenerCoeficiente(n - 1, k - 1) + obtenerCoeficiente(n - 1, k);
}

string formatearTermino(int coef, int expA, int expB, bool esPrimero) {
  string resultado = "";

  if (!esPrimero) {
    resultado += " + ";
  }

  if (coef > 1 || (expA == 0 && expB == 0)) {
    resultado += to_string(coef);
  }

  if (expA == 1) {
    resultado += "a";
  } else if (expA > 1) {
    resultado += "a^" + to_string(expA);
  }

  if (expB == 1) {
    resultado += "b";
  } else if (expB > 1) {
    resultado += "b^" + to_string(expB);
  }

  return resultado;
}

void imprimirFila(int n, int k, binomy bin) {
  if (k > n) {
    return;
  }

  int coef = obtenerCoeficiente(n, k);
  int expA = n - k;
  int expB = k;

  cout << formatearTermino(coef, expA, expB, k == 0);

  imprimirFila(n, k + 1, bin);
}

void imprimirTorre(int nivelActual, int nivelMaximo, binomy bin) {

  int espacios;

  if (nivelActual > nivelMaximo) {
    return;
  }

  string estado =
      "imprimirTorre(" + to_string(nivelActual) + ", " + to_string(nivelMaximo);

  string variables_locaes = "nivelActual: " + to_string(nivelActual) +
                            "nivelMaximo: " + to_string(nivelMaximo);

  string accion = "imprimirTorre(" + to_string(nivelActual) + " + 1, " +
                  to_string(nivelMaximo);

  bin.index.push(nivelActual);
  bin.operacion.push("PUSH");
  bin.estadoPila.push(estado);
  bin.variablesLocales.push(variables_locaes);

  if (nivelActual == 0) {
    espacios = (nivelMaximo - 1 - nivelActual) * 4;
    cout << string(espacios + 2, ' ');
  } else {
    espacios = (nivelMaximo - nivelActual) * 4;
    cout << string(espacios, ' ');
  }

  cout << "(a + b)^" << nivelActual << " = ";

  imprimirFila(nivelActual, 0, bin);

  cout << '\n';

  imprimirTorre(nivelActual + 1, nivelMaximo, bin);
}

void pila() {
  cout << "todas las piramides";
  int i = 0;
  for (binomy bin : binomial) {
    cout << "piramide ingresada: " << i;
    // for (int i = 0; i < bin.index[]; i++) {
    // }
    // cout << bin.variablesLocales;
    i++;
  }
}

void menu() {
  int opcion = 0;
  while (opcion != 3) {
    cout << "seleccione una opcion:" << endl;
    cout << "1. mostrar piramide" << endl;
    cout << "2. ver pila" << endl;
    cout << "3. salir" << endl;
    cin >> opcion;
    if (opcion == 1) {
      int n;
      cout << "numero de niveles: ";
      cin >> n;
      if (n < 10) {
        binomy bin;
        imprimirTorre(0, n, bin);
        binomial.push_back(bin);
        counter++;
      }
    } else if (opcion == 2) {
      pila();
    } else if (opcion == 3) {
      return;
    }
  }
}

int main(int argc, char *argv[]) {

  menu();
  return 0;
}
