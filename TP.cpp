#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// ESTRUCTURAS
struct RegCorredores {
    int numero;
    char nombreApellido[50];
    char categoria[50];
    char genero;
    char localidad[40];
    char llegada[11];
};

// Convierte "HH:MM:SS.D" en décimas de segundo (entero)
int convertirTiempoADecimas(const char llegada[11]) {
    if (strcmp(llegada, "No Termino") == 0) return INT32_MAX;

    int hh = ((llegada[0] - '0') * 10) + (llegada[1] - '0');
    int mm = ((llegada[3] - '0') * 10) + (llegada[4] - '0');
    int ss = ((llegada[6] - '0') * 10) + (llegada[7] - '0');
    int d  = (llegada[9] - '0');

    return (((hh * 60 + mm) * 60) + ss) * 10 + d;
}

// Ordenamiento Burbuja (ESTO NOSE ME LO DIO CHAT GPT))
void ordenarPorTiempo(vector<RegCorredores> &corredores) {
    int n = corredores.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int t1 = convertirTiempoADecimas(corredores[j].llegada);
            int t2 = convertirTiempoADecimas(corredores[j+1].llegada);
            if (t1 > t2) {
                swap(corredores[j], corredores[j+1]);
            }
        }
    }
}

int main() {
    vector<RegCorredores> corredores;

    // 1) Cargar datos
    cargarCorredores("Archivo corredores 4Refugios.bin", corredores);

    // 2) Ordenar por tiempo de llegada
    ordenarPorTiempo(corredores);

    // 3) Mostrar orden general
    cout << "Listado General (ordenado por tiempo):" << endl;
    mostrarCorredores(corredores);

    cout << "Procesamiento terminado." << endl;
    return 0;
}
