#ifndef RESERVACION_H
#define RESERVACION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

class Reservacion
{
public:

    Reservacion(int idReservacion, int idUsuario, int idVuelo, string asiento, bool pagado = false);

    int getIdReservacion() const { return idReservacion; }
    int getIdUsuario() const { return idUsuario; }
    int getIdVuelo() const { return idVuelo; }
    string getAsiento() const { return asiento; }
    bool estaPagado() const { return pagado; }

    void setPagado(bool estado) { pagado = estado; }

    static bool CrearReservacion(int idUsuario, int idVuelo, const string& asiento);
    static vector<Reservacion> ObtenerReservaciones();
    static bool ActualizarReservaciones(const vector<Reservacion>& reservas);

private:
    int idReservacion;
    int idUsuario;
    int idVuelo;
    string asiento;
    bool pagado;
};

#endif
