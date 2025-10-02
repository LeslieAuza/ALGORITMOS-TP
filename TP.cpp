#include <iostream>
#include <cstdio>
#include <cstring>
#include <climits>
#include <iomanip>
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
    double tiempo;
    double diffPrimero;
    double diffAnterior;
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


void generarRanking(RegCorredores corredores[], int n, Resultado ranking[], int &m) {
    // Copiar solo terminados
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (convertirTiempoADecimas(corredores[i].llegada) >= 0) {
            ranking[k].corredor = corredores[i];
            ranking[k].tiempo = convertirTiempoADecimas(corredores[i].llegada);
            k++;
        }
    }
    m = k;

    // Ordenar por tiempo (burbuja simple para que se vea el método de archivos)
    for (int i = 0; i < m-1; i++) {
        for (int j = i+1; j < m; j++) {
            if (ranking[j].tiempo < ranking[i].tiempo) {
                Resultado aux = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = aux;
            }
        }
    }

    // Calcular posiciones
    for (int i = 0; i < m; i++) {
        ranking[i].posGeneral = i+1;
        // Pos género
        int posG = 1;
        for (int j = 0; j < i; j++) {
            if (ranking[j].corredor.genero == ranking[i].corredor.genero)
                posG++;
        }
        ranking[i].posGenero = posG;

        // Pos categoría
        int posC = 1;
        for (int j = 0; j < i; j++) {
            if (strcmp(ranking[j].corredor.categoria, ranking[i].corredor.categoria) == 0)
                posC++;
        }
        ranking[i].posCategoria = posC;

        // Diferencias
        ranking[i].diffPrimero = ranking[i].tiempo - ranking[0].tiempo;
        ranking[i].diffAnterior = (i > 0) ? ranking[i].tiempo - ranking[i-1].tiempo : 0;
    }
}


void Calculardiferenciasdetiempo (Resultado resultados[], int n)
{
	int tiempoprimero = convertirTiempoADecimas(resultados[0].corredor.llegada);
	for (int i = 0; i < n; i++) 
	{
		int tiempoactual = convertirTiempoADecimas(resultados[i].corredor.llegada);

        //Dif con el primero 
        resultados[i].diffPrimero = tiempoactual - tiempoprimero;

        //Dif con el anterior
        if (i == 0 ){
            resultados[i].diffAnterior = 0;
        } else {
            int tiempoanterior = convertirTiempoADecimas(resultados[i-1].corredor.llegada);
            resultados[i].diffAnterior = tiempoactual - tiempoanterior;
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

void guardarRanking(const char *filename, Resultado resultados[], int n) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        cout << "Error al crear archivo " << filename << endl;
        return;
    }
    for (int i = 0; i < n; i++) {
        fwrite(&resultados[i], sizeof(Resultado), 1, f);
    }
    fclose(f);
}


void generarPodios(const char *filename, Resultado resultados[], int n) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        cout << "Error al crear podios." << endl;
        return;
    }

    // Para cada categoría, tomamos los primeros 3
    for (int i = 0; i < n; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (strcmp(resultados[j].corredor.categoria, resultados[i].corredor.categoria) == 0) {
                fwrite(&resultados[j], sizeof(Resultado), 1, f);
                count++;
                if (count == 3) break;
            }
        }
    }
    fclose(f);
}


void mostrarResultados(Resultado resultados[], int n) {
    cout << "\n=== Resultados ===\n";
    for (int i = 0; i < n; i++) {
        cout << "PosG: " << resultados[i].posGeneral
             << " | PosGen: " << resultados[i].posGenero
             << " | PosCat: " << resultados[i].posCategoria
             << " | Num: " << resultados[i].corredor.numero
             << " | Nombre: " << resultados[i].corredor.nombreApellido
             << " | Cat: " << resultados[i].corredor.categoria
             << " | Genero: " << resultados[i].corredor.genero
             << " | Localidad: " << resultados[i].corredor.localidad
             << " | Llegada: " << resultados[i].corredor.llegada
             << " | DifPrim: " << formatearTiempo(resultados[i].diffPrimero)
             << " | DifAnt: " << formatearTiempo(resultados[i].diffAnterior)
             << endl;
    }
    cout << "=== Fin de resultados (" << n << " registros) ===\n";
}

void mostrarArchivoRanking(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        cout << "No se pudo abrir " << filename << endl;
        return;
    }

    Resultado r;
    int i = 0;
    cout << "\n=== Contenido de " << filename << " ===\n";
    while (fread(&r, sizeof(Resultado), 1, f) == 1) {
        cout << "PosG: " << r.posGeneral
             << " | PosGen: " << r.posGenero
             << " | PosCat: " << r.posCategoria
             << " | Num: " << r.corredor.numero
             << " | Nombre: " << r.corredor.nombreApellido
             << " | Cat: " << r.corredor.categoria
             << " | Genero: " << r.corredor.genero
             << " | Localidad: " << r.corredor.localidad
             << " | Llegada: " << r.corredor.llegada
             << " | DifPrim: " << formatearTiempo(r.diffPrimero)
             << " | DifAnt: " << formatearTiempo(r.diffAnterior)
             << endl;
        i++;
    }
    fclose(f);
    cout << "=== Fin de archivo (" << i << " registros) ===\n";
}

void mostrarArchivoPodios(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        cout << "No se pudo abrir " << filename << endl;
        return;
    }

    Resultado resultados[1000];
    int n = 0;
    while (fread(&resultados[n], sizeof(Resultado), 1, f) == 1 && n < 1000) {
        n++;
    }
    fclose(f);

    cout << "\n=== PODIOS POR CATEGORIA ===\n";

    // Recorrer categorías
    for (int i = 0; i < n; i++) {
        // Ver si ya mostramos esta categoría
        bool yaMostrada = false;
        for (int k = 0; k < i; k++) {
            if (strcmp(resultados[i].corredor.categoria, resultados[k].corredor.categoria) == 0) {
                yaMostrada = true;
                break;
            }
        }
        if (yaMostrada) continue;

        // Mostrar encabezado de la categoría
        cout << "\nCategoria: " << resultados[i].corredor.categoria << endl;
        cout << "----------------------------------------" << endl;

        // Mostrar los 3 primeros de la categoría
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (strcmp(resultados[j].corredor.categoria, resultados[i].corredor.categoria) == 0) {
                cout << "PosCat: " << resultados[j].posCategoria
                     << " | Num: " << resultados[j].corredor.numero
                     << " | Nombre: " << resultados[j].corredor.nombreApellido
                     << " | Tiempo: " << formatearTiempo(resultados[j].tiempo)
                     << endl;
                count++;
                if (count == 3) break;
            }
        }
    }

    cout << "\n=== Fin de podios ===\n";
}

int main() {
    const int MAX = 1000;
    RegCorredores corredores[MAX];
    Resultado resultados[MAX];
    int n, m;

    n = cargarCorredores("Archivo corredores 4Refugios.bin", corredores, MAX);
    if (n == 0) return 1;

    ordenarPorTiempo(corredores, n);
    generarRanking(corredores, n, resultados, m);
    Calculardiferenciasdetiempo(resultados, m);
    mostrarResultados(resultados, m);

    guardarRanking("Ranking.bin", resultados, m);
    generarPodios("Podios.bin", resultados, m);

    cout << "\nArchivos generados: Ranking.bin y Podios.bin\n";

    // Mostrar contenido de archivos
    mostrarArchivoRanking("Ranking.bin");
    mostrarArchivoPodios("Podios.bin");
	return 0;
    
}




