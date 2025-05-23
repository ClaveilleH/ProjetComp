extern int printd(int i);

/* Cas d'un switch difficile */

int main() {
    int jour, ferie;

    switch (jour) {
        case 1:
        case 2:
        case 3:
            if (jour == 3) {
                printd(300);
                break;
            }
            printd(100 + jour);
            break;

        case 4:
            printd(400);
            if (ferie == ferie) break;

        case 5:
            printd(500);
            break;

        case 6:
        case 7:
            printd(jour * 10);
            break;

        default:
            break;
    }

    return 0;
}
