extern int printd(int i);

int main() {
   int i,j;
   i=3;
   switch(i) {
      default: printd(0); break;
      case 0 : printd(0); break;
      default: printd(-1);
   }
   
}
