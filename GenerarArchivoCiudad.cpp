#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct CorredoresCiudad {
    int numero;
    char nombreApellido[50];
    char localidad[40];
    char ciudad[11];
};

int main() {
    FILE *f = fopen("Ciudades.bin", "wb");
    if (!f) {
        cout << "No se pudo crear el archivo Ciudades.bin" << endl;
        return 1;
    }

    CorredoresCiudad c;

    // Relacionar los corredores que ya tenemos en Archivo corredores 4Refugios.bin
    // Corredor 1
    c.numero = 1;
    strcpy(c.nombreApellido, "Juan Perez");
    strcpy(c.localidad, "Bariloche");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 2;
    strcpy(c.nombreApellido, "Ana Gomez");
    strcpy(c.localidad, "Neuquen");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 3;
    strcpy(c.nombreApellido, "Carlos Lopez");
    strcpy(c.localidad, "Buenos Aires");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 4;
    strcpy(c.nombreApellido, "Maria Diaz");
    strcpy(c.localidad, "Cordoba");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 5;
    strcpy(c.nombreApellido, "Pedro Sanchez");
    strcpy(c.localidad, "Mendoza");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 6;
    strcpy(c.nombreApellido, "Laura Martinez");
    strcpy(c.localidad, "San Luis");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 7;
    strcpy(c.nombreApellido, "Roberto Torres");
    strcpy(c.localidad, "Rosario");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    // Los extras que agregaste (8 a 21)
    c.numero = 8;
    strcpy(c.nombreApellido, "Luis Fernandez");
    strcpy(c.localidad, "Villa La Angostura");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 9;
    strcpy(c.nombreApellido, "Sofia Alvarez");
    strcpy(c.localidad, "Esquel");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 10;
    strcpy(c.nombreApellido, "Martin Herrera");
    strcpy(c.localidad, "Mar del Plata");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 11;
    strcpy(c.nombreApellido, "Gabriela Torres");
    strcpy(c.localidad, "Santa Fe");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 12;
    strcpy(c.nombreApellido, "Diego Ramirez");
    strcpy(c.localidad, "Tandil");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 13;
    strcpy(c.nombreApellido, "Patricia Suarez");
    strcpy(c.localidad, "La Plata");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 14;
    strcpy(c.nombreApellido, "Esteban Molina");
    strcpy(c.localidad, "Salta");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 15;
    strcpy(c.nombreApellido, "Andres Gutierrez");
    strcpy(c.localidad, "San Martin");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 16;
    strcpy(c.nombreApellido, "Valeria Castro");
    strcpy(c.localidad, "Villa Regina");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 17;
    strcpy(c.nombreApellido, "Javier Morales");
    strcpy(c.localidad, "Olavarria");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 18;
    strcpy(c.nombreApellido, "Claudia Mendez");
    strcpy(c.localidad, "Parana");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 19;
    strcpy(c.nombreApellido, "Facundo Arias");
    strcpy(c.localidad, "Godoy Cruz");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 20;
    strcpy(c.nombreApellido, "Luciana Perez");
    strcpy(c.localidad, "Merlo");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    c.numero = 21;
    strcpy(c.nombreApellido, "Hernan Lopez");
    strcpy(c.localidad, "Jujuy");
    strcpy(c.ciudad, "Argentina");
    fwrite(&c, sizeof(c), 1, f);

    fclose(f);
    cout << "Archivo 'Ciudades.bin' creado con datos de prueba." << endl;
    return 0;
}

