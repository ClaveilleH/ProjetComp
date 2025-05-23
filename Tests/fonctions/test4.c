extern int printd( int i );

int add( int a, int b ) {
  return a + b;
}

int main() {
  printd(add(10));
  return 0;
}
