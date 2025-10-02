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

struct CorredoresCiudad {
    int numero;
    char nombreApellido[50];
    char localidad[40];
    char ciudad[11];
};

struct EstadisticaCiudades {
    char ciudad[11];
    char localidad[40];
    int cantidadCorredores;
    double sumaTiempos;
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

string formatearTiempoAString(int tiempo) {
    int totalSegs = tiempo / 10; // paso a segundos enteros
    int d = tiempo % 10; // resto (la decima)
    int hh = totalSegs / 3600; // horas
    int mm = (totalSegs % 3600) / 60; // minutos
    int ss = totalSegs % 60; // segundos

    char buffer[20];
    sprintf(buffer, "%02d:%02d:%02d.%d", hh, mm, ss, d); // formateo el string

    return string(buffer);
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

int cargarCiudades(const char *filename, CorredoresCiudad ciudades[], int maxCorredores) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        cout << "Error al abrir archivo ciudades." << endl;
        return 0;
    }

    int cant = 0;
    while (cant < maxCorredores && fread(&ciudades[cant], sizeof(CorredoresCiudad), 1, f) == 1) {
        cant++;
    }

    fclose(f);
    return cant;
}

int generarReporteCiudades(Resultado resultados[], int n, CorredoresCiudad ciudades[], int m, EstadisticaCiudades estadisticas[]) {
    int cantEst = 0;

    // acumuladores ciudad/localidad/datos
    for(int i = 0; i < n; i++){
        int numeroCorredor = resultados[i].corredor.numero;

        for(int j = 0; j < m; j++){

            if(ciudades[j].numero == numeroCorredor) {
                // chequeo si ya tengo la ciudad/localidad
                for(int k = 0; k < cantEst; k++){
                    if(strcmp(estadisticas[k].ciudad, ciudades[j].ciudad) == 0 && strcmp(estadisticas[k].localidad, ciudades[j].localidad == 0)) {
                        break;
                    }
                }

                // si no esta la agrego
                if (k == cantEst) {
                    strcpy(estadisticas[k].ciudad, ciudades[j].ciudad);
                    strcpy(estadisticas[k].localidad, ciudades[j].localidad);
                    estadisticas[k].cantidadCorredores = 0;
                    estadisticas[k].sumaTiempos = 0;
                    cantEst++;
                }

                // acumulo datos
                estadisticas[k].cantidadCorredores++;
                estadisticas[k].sumaTiempos += resultados[i].tiempo;
                break;
            }
        }
    }
    return cantEst;
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

void mostrarReporteCiudades(EstadisticaCiudades estadisticas[], int cantEst) {
    cout << "\n=== Reporte por Ciudad y Localidad ===\n\n";
    
    cout << left
         << setw(15) << "Ciudad"
         << setw(15) << "Localidad"
         << setw(20) << "Cantidad corredores"
         << setw(15) << "Tiempo promedio" << endl;

    cout << string(65, '-') << endl;


    for(int i=0; i < cantEst; i++){
        bool primeraVez = true;
        for(int j = 0; j < i; j++){
            if(strcmp(estadisticas[i].ciudad, estadisticas[j].ciudad) == 0)) {
                primeraVez = false;
                break;
            }
        }
        if(!primeraVez) continue;

        // Acumular totales de la ciudad
        int totalCant = 0;
        double totalSuma = 0;

        for (int k = 0; k < cantEst; k++) {
            if (strcmp(estadisticas[i].ciudad, estadisticas[k].ciudad) == 0) {
                double prom = estadisticas[k].sumaTiempos / estadisticas[k].cantidadCorredores;

                cout << left
                     << setw(15) << estadisticas[k].ciudad
                     << setw(15) << estadisticas[k].localidad
                     << setw(20) << estadisticas[k].cantidadCorredores
                     << setw(15) << formatearTiempo((int)prom)
                     << endl;

                totalCant += estadisticas[k].cantidadCorredores;
                totalSuma += estadisticas[k].sumaTiempos;
            }
        }

        // Mostrar línea de totales
        double promTotal = totalSuma / totalCant;
        cout << left
             << setw(15) << ("Total " + string(estadisticas[i].ciudad))
             << setw(15) << "-"
             << setw(20) << totalCant
             << setw(15) << formatearTiempo((int)promTotal)
             << endl;

        cout << string(65, '-') << endl;
    }
}

int main() {
    const int MAX = 1000;
    RegCorredores corredores[MAX];
    Resultado resultados[MAX];
    CorredoresCiudad ciudades[MAX];
    EstadisticaCiudades estadisticas[200];
    int n, m, c, cantEst;

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

    c = cargarCiudades("Ciudades.bin", ciudades, MAX);
    if(c > 0) {
        cantEst = generarReporteCiudades(resultados, m, ciudades, c, estadisticas);
        mostrarReporteCiudades(estadisticas, cantEst);
    }
	return 0;
    
}




