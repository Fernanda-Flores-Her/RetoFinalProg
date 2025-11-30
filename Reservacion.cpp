#include "Reservacion.h"

Reservacion::Reservacion(int idReservacion, int idUsuario, int idVuelo, string asiento, bool pagado)
    : idReservacion(idReservacion), idUsuario(idUsuario), idVuelo(idVuelo), asiento(asiento), pagado(pagado) {
}

bool Reservacion::CrearReservacion(int idUsuario, int idVuelo, const string& asiento)
{
    vector<Reservacion> reservas = ObtenerReservaciones();
    int newId = 1;

    for (const Reservacion& r : reservas) {
        if (r.idReservacion >= newId) {
            newId = r.idReservacion + 1;
        }
    }

    ofstream outputFile("reservaciones.txt", ios::app);

    if (outputFile.is_open())
    {
        outputFile << newId << " " << idUsuario << " " << idVuelo << " " << asiento << " " << 0 << "\n";
        outputFile.close();
        return true;
    }
    return false;
}

vector<Reservacion> Reservacion::ObtenerReservaciones()
{
    vector<Reservacion> reservas;
    ifstream inputFile("reservaciones.txt");

    if (!inputFile.is_open()) {
        ofstream tempFile("reservaciones.txt");
        tempFile.close();
        return reservas;
    }

    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);

        int idR, idU, idV, pagadoInt;
        string asiento;

        if (ss >> idR >> idU >> idV >> asiento >> pagadoInt) {
            bool pagadoBool = (pagadoInt == 1);
            reservas.emplace_back(idR, idU, idV, asiento, pagadoBool);
        }
    }
    inputFile.close();
    return reservas;
}

bool Reservacion::ActualizarReservaciones(const vector<Reservacion>& reservas) {
    ofstream outputFile("reservaciones.txt", ios::trunc);

    if (!outputFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo 'reservaciones.txt' para actualizar." << endl;
        return false;
    }

    for (const auto& r : reservas) {
        int pagadoInt = r.estaPagado() ? 1 : 0;
        outputFile << r.getIdReservacion() << " " << r.getIdUsuario() << " " << r.getIdVuelo() << " "
            << r.getAsiento() << " " << pagadoInt << "\n";
    }

    outputFile.close();
    return true;
}