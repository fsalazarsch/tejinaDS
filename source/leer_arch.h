#include <3ds.h>
#include <stdio.h>

void read_file() {

    // Escribir en un archivo
    FILE *file = fopen("sdmc:/data.dat", "w+");
    if (file) {
        fprintf(file, "Mario,1500,3\n");
        fclose(file);
        printf("Archivo escrito con éxito.\n");
    } else {
        printf("Error al abrir el archivo para escribir.\n");
    }

    // Leer del archivo
    file = fopen("sdmc:/data.dat", "r");
    if (file) {
        char name[50];
        int score, level;
        fscanf(file, "%[^,],%d,%d", name, &score, &level);
        printf("Nombre: %s, Puntaje: %d, Nivel: %d\n", name, score, level);
        fclose(file);
    } else {
        printf("Error al leer el archivo.\n");
    }

    return;
}