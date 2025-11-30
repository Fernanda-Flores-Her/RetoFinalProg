#include "Vuelo.h"
#include <iostream>
#include <algorithm>

using namespace std;

Vuelo::Vuelo(int id, string fecha, string hora, string origen, string destino, string numeroAvion, int asientosDisponibles)
    : id(id), fecha(fecha), hora(hora), origen(origen), destino(destino), numeroAvion(numeroAvion), asientosDisponibles(asientosDisponibles) {
}

bool Vuelo::RegistrarVuelo(const string& fecha, const string& hora, const string& origen, const string& destino, const string& numeroAvion)
{
    vector<Vuelo> vuelos = ObtenerVuelos();
    int newId = 1;

    for (const Vuelo& v : vuelos) {
        if (v.id >= newId) {
            newId = v.id + 1;
        }
    }

    ofstream outputFile("vuelos.txt", ios::app);

    if (outputFile.is_open())
    {
        outputFile << newId << " " << fecha << " " << hora << " " << origen << " " << destino << " " << numeroAvion << " " << 10 << "\n";
        outputFile.close();
        return true;
    }
    else {
        cerr << "Error: No se pudo abrir el archivo 'vuelos.txt' para escribir." << endl;
        return false;
    }
}

vector<Vuelo> Vuelo::ObtenerVuelos()
{
    vector<Vuelo> vuelos;
    ifstream inputFile("vuelos.txt");

    if (!inputFile.is_open()) {
        ofstream tempFile("vuelos.txt");
        tempFile.close();
        return vuelos;
    }

    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);

        int idV, asientosD;
        string fecha, hora, origen, destino, numAvion;

        if (ss >> idV >> fecha >> hora >> origen >> destino >> numAvion >> asientosD) {
            vuelos.emplace_back(idV, fecha, hora, origen, destino, numAvion, asientosD);
        }
    }
    inputFile.close();
    return vuelos;
}

bool Vuelo::ActualizarVuelos(const std::vector<Vuelo>& vuelos) {
    ofstream outputFile("vuelos.txt", ios::trunc);

    if (!outputFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo 'vuelos.txt' para actualizar." << endl;
        return false;
    }

    for (const auto& v : vuelos) {
        outputFile << v.id << " " << v.fecha << " " << v.hora << " " << v.origen << " "
            << v.destino << " " << v.numeroAvion << " " << v.asientosDisponibles << "\n";
    }

    outputFile.close();
    return true;
}