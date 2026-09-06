#include <stdio.h>

int factorial(int n) {
  if (n == 0)
    return 1;
  else
    return n * factorial(n - 1);
}

int main(int argc, char *argv[]) {
  int n;
  printf("ingrese entero: ");
  scanf("%d", &n);

  printf("factorial de %d: %d", n, factorial(n));
  return 0;
}
