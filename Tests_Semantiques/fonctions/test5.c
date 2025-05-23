extern int printd( int i );

int add( int a, int b ) {
  return a + b;
}

int add (int a ) {
  return a + a;
}

int main() {
  printd(add(10, 12));
  return 0;
}
