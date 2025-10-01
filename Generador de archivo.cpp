#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct RegCorredores {
    int numero;
    char nombreApellido[50];
    char categoria[50];
    char genero;
    char localidad[40];
    char llegada[11]; // "HH:MM:SS.D" o "No Termino"
};

int main() {
    FILE *f = fopen("Archivo corredores 4Refugios.bin", "wb");
    if (!f) {
        cout << "No se pudo crear el archivo." << endl;
        return 1;
    }

    RegCorredores c;

    // Corredor 1
    c.numero = 1;
    strcpy(c.nombreApellido, "Juan Perez");
    strcpy(c.categoria, "Senior");
    c.genero = 'M';
    strcpy(c.localidad, "Bariloche");
    strcpy(c.llegada, "02:15:30.5");
    fwrite(&c, sizeof(RegCorredores), 1, f);

    // Corredor 2
    c.numero = 2;
    strcpy(c.nombreApellido, "Ana Gomez");
    strcpy(c.categoria, "Senior");
    c.genero = 'F';
    strcpy(c.localidad, "Neuquen");
    strcpy(c.llegada, "02:20:10.3");
    fwrite(&c, sizeof(RegCorredores), 1, f);

    // Corredor 3
    c.numero = 3;
    strcpy(c.nombreApellido, "Carlos Lopez");
    strcpy(c.categoria, "Master");
    c.genero = 'M';
    strcpy(c.localidad, "Buenos Aires");
    strcpy(c.llegada, "02:18:45.9");
    fwrite(&c, sizeof(RegCorredores), 1, f);

    // Corredor 4 (No terminó)
    c.numero = 4;
    strcpy(c.nombreApellido, "Maria Diaz");
    strcpy(c.categoria, "Master");
    c.genero = 'F';
    strcpy(c.localidad, "Cordoba");
    strcpy(c.llegada, "No Termino");
    fwrite(&c, sizeof(RegCorredores), 1, f);

    fclose(f);
    cout << "Archivo 'Archivo corredores 4Refugios.bin' creado con datos de prueba." << endl;
    return 0;
}

