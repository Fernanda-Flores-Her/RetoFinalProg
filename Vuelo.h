#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>

class Vuelo
{
private:
    int id;
    std::string fecha;
    std::string hora;
    std::string origen;
    std::string destino;
    std::string numeroAvion;
    int asientosDisponibles = 10;

public:
    Vuelo(int id, std::string fecha, std::string hora, std::string origen, std::string destino, std::string numeroAvion, int asientosDisponibles = 10);

    int getId() const { return id; }
    std::string getFecha() const { return fecha; }
    std::string getHora() const { return hora; }
    std::string getOrigen() const { return origen; }
    std::string getDestino() const { return destino; }
    std::string getNumeroAvion() const { return numeroAvion; }
    int getAsientosDisponibles() const { return asientosDisponibles; }

    void setAsientosDisponibles(int count) { asientosDisponibles = count; }

    static bool RegistrarVuelo(const std::string& fecha, const std::string& hora, const std::string& origen, const std::string& destino, const std::string& numeroAvion);
    static std::vector<Vuelo> ObtenerVuelos();
    static bool ActualizarVuelos(const std::vector<Vuelo>& vuelos);
};