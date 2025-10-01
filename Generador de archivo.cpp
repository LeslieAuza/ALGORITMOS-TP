#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct RegCorredores {
    int numero;
    char nombreApellido[100] = {};
    char categoria[100] = {} ;   // Carrera + genero + rango edad
    char genero;
    char localidad[40] = {};
    char llegada[11] = {};      // "HH:MM:SS.D" o "No Termino"
};

int main() {
    FILE *f = fopen("Archivo corredores 4Refugios.bin", "wb");
    if (!f) {
        cout << "No se pudo crear el archivo." << endl;
        return 1;
    }

    RegCorredores c = {};

    // --- Clasica - Caballeros (Hasta 29 años) ---
    c.numero = 1;
    strcpy(c.nombreApellido, "Juan Perez");
    strcpy(c.categoria, "4 Refugios Clasica - Caballeros (Hasta 29 años)");
    c.genero = 'M';
    strcpy(c.localidad, "Bariloche");
    strcpy(c.llegada, "02:15:30.5");
    fwrite(&c, sizeof(RegCorredores), 1, f);

    // --- Clasica - Damas (Hasta 29 años) ---
    c.numero = 2;
    strcpy(c.nombreApellido, "Ana Gomez");
    strcpy(c.categoria, "4 Refugios Clasica - Damas (Hasta 29 años)");
    c.genero = 'F';
    strcpy(c.localidad, "Neuquen");
    strcpy(c.llegada, "02:20:10.3");
    fwrite(&c, sizeof(RegCorredores), 1, f);

    // --- Clasica - Caballeros (De 30 a 39 años) ---
    c.numero = 3;
    strcpy(c.nombreApellido, "Carlos Lopez");
    strcpy(c.categoria, "4 Refugios Clasica - Caballeros (De 30 a 39 años)");
    c.genero = 'M';
    strcpy(c.localidad, "Buenos Aires");
    strcpy(c.llegada, "02:18:45.9");
    fwrite(&c, sizeof(RegCorredores), 1, f);

    // --- Clasica - Damas (De 40 a 49 años) - No Termino ---
    c.numero = 4;
    strcpy(c.nombreApellido, "Maria Diaz");
    strcpy(c.categoria, "4 Refugios Clasica - Damas (De 40 a 49 años)");
    c.genero = 'F';
    strcpy(c.localidad, "Cordoba");
    strcpy(c.llegada, "No Termino");
    fwrite(&c, sizeof(RegCorredores), 1, f);

    // --- NonStop - Caballeros (Hasta 29 años) ---
    c.numero = 5;
    strcpy(c.nombreApellido, "Pedro Sanchez");
    strcpy(c.categoria, "4 Refugios NonStop - Caballeros (Hasta 29 años)");
    c.genero = 'M';
    strcpy(c.localidad, "Mendoza");
    strcpy(c.llegada, "03:05:12.4");
    fwrite(&c, sizeof(RegCorredores), 1, f);

    // --- NonStop - Damas (De 30 a 39 años) ---
    c.numero = 6;
    strcpy(c.nombreApellido, "Laura Martinez");
    strcpy(c.categoria, "4 Refugios NonStop - Damas (De 30 a 39 años)");
    c.genero = 'F';
    strcpy(c.localidad, "San Luis");
    strcpy(c.llegada, "03:22:40.1");
    fwrite(&c, sizeof(RegCorredores), 1, f);

    // --- NonStop - Caballeros (De 40 a 49 años) ---
    c.numero = 7;
    strcpy(c.nombreApellido, "Roberto Torres");
    strcpy(c.categoria, "4 Refugios NonStop - Caballeros (De 40 a 49 años)");
    c.genero = 'M';
    strcpy(c.localidad, "Rosario");
    strcpy(c.llegada, "03:10:55.7");
    fwrite(&c, sizeof(RegCorredores), 1, f);

    fclose(f);
    cout << "Archivo 'Archivo corredores 4Refugios.bin' creado con datos de prueba." << endl;
    return 0;
}

