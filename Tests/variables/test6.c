extern int printd( int i );

int main() {
  int i;
  i = 0;
  if (i == 0) {
    int j;
    j = 42;
    printd(i);
  } else {
    int k;
    k = 43;
    printd(k);
  }
  return 0;
}
