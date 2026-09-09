#include <iostream>
// #include <tuple>
// #include <vector>
using namespace std;

// struct binomy {
//   int potencia;
//   tuple<int, string, string, string, string, int> state;
// };
//
// vector<binomy> binomial;

void pila() {}

int exponente(int n, int k) {
  if (n == 0 || k == 0) {
    return 1;

  } else {
    exponente(n - 1, k - 1);
    exponente(n - 1, k);
  }
}

void piramide() {}

void solve(int exponente) {}

void menu() {
  int opcion = 0;
  while (opcion != 3) {
    cout << "Seleccione una opción:" << endl;
    cout << "1. Mostrar piramide" << endl;
    cout << "2. Ver pila" << endl;
    cout << "3. Salir" << endl;
    cin >> opcion;
    if (opcion == 1) {
      piramide();
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
