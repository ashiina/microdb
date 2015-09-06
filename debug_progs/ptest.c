void ptrch ( char ** point) {
    *point = "asd";
}

int main() {
    char * point;
    ptrch(&point);
    printf("%s\n", point);
    return 0;
}

