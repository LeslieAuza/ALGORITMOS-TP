#include <iostream>
#include <cstdio>
#include <cstring>
#include <climits>
using namespace std;

// =======================
// ESTRUCTURAS
// =======================
struct RegCorredores {
    int numero;
    char nombreApellido[50];
    char categoria[50];
    char genero;
    char localidad[40];
    char llegada[11];
};

struct Resultado {
    RegCorredores corredor;
    int posGeneral;
    int posGenero;
    int posCategoria;
};

// =======================
// FUNCIONES AUXILIARES
// =======================

int convertirTiempoADecimas(const char llegada[11]) {
    if (strcmp(llegada, "No Termino") == 0) return INT32_MAX;

    int hh = ((llegada[0] - '0') * 10) + (llegada[1] - '0');
    int mm = ((llegada[3] - '0') * 10) + (llegada[4] - '0');
    int ss = ((llegada[6] - '0') * 10) + (llegada[7] - '0');
    int d  = (llegada[9] - '0');

    return (((hh * 60 + mm) * 60) + ss) * 10 + d;
}

int cargarCorredores(const char *filename, RegCorredores corredores[], int maxCorredores) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        cout << "Error al abrir archivo." << endl;
        return 0;
    }

    int cant = 0;
    while (cant < maxCorredores && fread(&corredores[cant], sizeof(RegCorredores), 1, f) == 1) {
        cant++;
    }

    fclose(f);
    return cant;
}

void ordenarPorTiempo(RegCorredores corredores[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int t1 = convertirTiempoADecimas(corredores[j].llegada);
            int t2 = convertirTiempoADecimas(corredores[j+1].llegada);
            if (t1 > t2) {
                RegCorredores aux = corredores[j];
                corredores[j] = corredores[j+1];
                corredores[j+1] = aux;
            }
        }
    }
}

void procesarResultados(RegCorredores corredores[], Resultado resultados[], int n) {
    int posGenero[256] = {0};
    char categorias[100][50];
    int posCat[100] = {0};
    int cantCat = 0;

    for (int i = 0; i < n; i++) {
        resultados[i].corredor = corredores[i];
        resultados[i].posGeneral = i + 1;

        unsigned char g = (unsigned char) corredores[i].genero;
        posGenero[g]++;
        resultados[i].posGenero = posGenero[g];

        int idx = -1;
        for (int c = 0; c < cantCat; c++) {
            if (strcmp(categorias[c], corredores[i].categoria) == 0) {
                idx = c;
                break;
            }
        }
        if (idx == -1) {
            strcpy(categorias[cantCat], corredores[i].categoria);
            idx = cantCat;
            cantCat++;
        }
        posCat[idx]++;
        resultados[i].posCategoria = posCat[idx];
    }
}

void mostrarResultados(Resultado resultados[], int n) {
    cout << "PosG\tPosGen\tPosCat\tNumero\tNombre\tCategoria\tGenero\tLocalidad\tLlegada\n";
    for (int i = 0; i < n; i++) {
        cout << resultados[i].posGeneral << "\t"
             << resultados[i].posGenero << "\t"
             << resultados[i].posCategoria << "\t"
             << resultados[i].corredor.numero << "\t"
             << resultados[i].corredor.nombreApellido << "\t"
             << resultados[i].corredor.categoria << "\t"
             << resultados[i].corredor.genero << "\t"
             << resultados[i].corredor.localidad << "\t"
             << resultados[i].corredor.llegada << endl;
    }
}

// =======================
// MAIN
// =======================
int main() {
    const int MAX = 1000;
    RegCorredores corredores[MAX];
    Resultado resultados[MAX];

    int n = cargarCorredores("Archivo corredores 4Refugios.bin", corredores, MAX);
    if (n == 0) return 1;

    ordenarPorTiempo(corredores, n);
    procesarResultados(corredores, resultados, n);
    mostrarResultados(resultados, n);

    return 0;
}


