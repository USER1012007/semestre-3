#include <iostream>
#include <stack>
#include <tuple>
#include <vector>

using namespace std;

struct binomy {
  int potencia;
  stack<tuple<int, string, string, string, string, int>> state;
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

void imprimirFila(int n, int k) {
  if (k > n) {
    return;
  }

  int coef = obtenerCoeficiente(n, k);
  int expA = n - k;
  int expB = k;

  cout << formatearTermino(coef, expA, expB, k == 0);

  imprimirFila(n, k + 1);
}

void imprimirTorre(int nivelActual, int nivelMaximo, binomy bin) {

  if (nivelActual > nivelMaximo) {
    return;
  }

  cout << "(a + b)^" << nivelActual << " = ";

  imprimirFila(nivelActual, 0);

  cout << '\n';

  bin.potencia = nivelActual;
  bin.state.push(make_tuple(nivelActual, "a", "b", "+", "=", nivelActual));

  imprimirTorre(nivelActual + 1, nivelMaximo, bin);
}

void pila() {}

void menu() {
  int opcion = 0;
  while (opcion != 3) {
    cout << "Seleccione una opción:" << endl;
    cout << "1. Mostrar piramide" << endl;
    cout << "2. Ver pila" << endl;
    cout << "3. Salir" << endl;
    cin >> opcion;
    if (opcion == 1) {
      int n;
      cout << "numero de niveles: ";
      cin >> n;
      if (n < 10) {
        binomy bin;
        imprimirTorre(0, n, bin);
        counter++;
      }
    } else if (opcion == 2) {
      pila();
    } else {
      return;
    }
  }
}

int main(int argc, char *argv[]) {

  menu();
  return 0;
}
