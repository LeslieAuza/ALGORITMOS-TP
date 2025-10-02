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
    char nombreApellido[100] = {};
    char categoria[100] = {};
    char genero;
    char localidad[40] = {};
    char llegada[11] = {};
};

struct Resultado {
    RegCorredores corredor;
    int posGeneral;
    int posGenero;
    int posCategoria;
    int diferenciaprimero;
    int diferenciaanterior;
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

void Calculardiferenciasdetiempo (Resultado resultados[], int n)
{
	int tiempoprimero = convertirTiempoADecimas(resultados[0].corredor.llegada);
	for (int i = 0; i < n; i++) 
	{
		int tiempoactual = convertirTiempoADecimas(resultados[i].corredor.llegada);

        //Dif con el primero 
        resultados[i].diferenciaprimero = tiempoactual - tiempoprimero;

        //Dif con el anterior
        if (i == 0 ){
            resultados[i].diferenciaanterior = 0;
        } else {
            int tiempoanterior = convertirTiempoADecimas(resultados[i-1].corredor.llegada);
            resultados[i].diferenciaanterior = tiempoactual - tiempoanterior;
        }
	}
}

string formatearTiempo(int tiempo) {
    int totalSegs = tiempo / 10; // paso a segundos enteros
    int d = tiempo % 10; // resto (la decima)
    int hh = totalSegs / 3600; // horas
    int mm = (totalSegs % 3600) / 60; // minutos
    int ss = totalSegs % 60; // segundos

    char buffer[20];
    sprintf(buffer, "%02d:%02d:%02d.%d", hh, mm, ss, d);

    return string(buffer);
} 

#include <iostream>
#include <iomanip>
using namespace std;

void mostrarResultados(Resultado resultados[], int n) {
    // Anchos de columna
    const int wPosG = 6, wPosGen = 7, wPosCat = 8, wNumero = 8;
    const int wNombre = 20, wCategoria = 12, wGenero = 8;
    const int wLocalidad = 15, wLlegada = 10, wDifer = 15;

    // Cabecera
    cout << left
         << setw(wPosG) << "PosG" << "|"
         << setw(wPosGen) << "PosGen" << "|"
         << setw(wPosCat) << "PosCat" << "|"
         << setw(wNumero) << "Numero" << "|"
         << setw(wNombre) << "Nombre" << "|"
         << setw(wCategoria) << "Categoria" << "|"
         << setw(wGenero) << "Genero" << "|"
         << setw(wLocalidad) << "Localidad" << "|"
         << setw(wLlegada) << "Llegada" << "|"
         << setw(wDifer) << "DiferPrimero" << "|"
         << setw(wDifer) << "DiferAnterior"
         << endl;

    // Línea separadora
    cout << setfill('-') 
         << setw(wPosG) << "" << "+"
         << setw(wPosGen) << "" << "+"
         << setw(wPosCat) << "" << "+"
         << setw(wNumero) << "" << "+"
         << setw(wNombre) << "" << "+"
         << setw(wCategoria) << "" << "+"
         << setw(wGenero) << "" << "+"
         << setw(wLocalidad) << "" << "+"
         << setw(wLlegada) << "" << "+"
         << setw(wDifer) << "" << "+"
         << setw(wDifer) << "" 
         << setfill(' ') << endl;

    // Datos
    for (int i = 0; i < n; i++) {
        cout << left
             << setw(wPosG) << resultados[i].posGeneral << "|"
             << setw(wPosGen) << resultados[i].posGenero << "|"
             << setw(wPosCat) << resultados[i].posCategoria << "|"
             << setw(wNumero) << resultados[i].corredor.numero << "|"
             << setw(wNombre) << resultados[i].corredor.nombreApellido << "|"
             << setw(wCategoria) << resultados[i].corredor.categoria << "|"
             << setw(wGenero) << resultados[i].corredor.genero << "|"
             << setw(wLocalidad) << resultados[i].corredor.localidad << "|"
             << setw(wLlegada) << resultados[i].corredor.llegada << "|"
             << setw(wDifer) << formatearTiempo(resultados[i].diferenciaprimero) << "|"
             << setw(wDifer) << formatearTiempo(resultados[i].diferenciaanterior)
             << endl;
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
    Calculardiferenciasdetiempo(resultados, n);
    mostrarResultados(resultados, n);

    return 0;
}


