#include <stdio.h>

// Asumimos que p es una función que devuelve un puntero a char (string)
const char* p(const char* str) {
    printf("Función p llamada con: %s\n", str);
    return str;
}

// Asumimos que z es una función que recibe tres punteros a const char
void z(const char* arg1, const char* arg2, const char* arg3) {
    printf("Función z llamada con:\n");
    printf("  arg1: %s\n", arg1);
    printf("  arg2: %s\n", arg2);
    printf("  arg3: %s\n", arg3);
}

int main() {
    z(p("a"), p("b"), p("c"));
    return 0;
}
