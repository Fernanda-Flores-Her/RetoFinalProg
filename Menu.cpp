#include "Menu.h"
#include "Reservacion.h"
#include "Vuelo.h" 
#include <iostream>
#include <cstdlib> 
#include <limits> 
#include <vector>
#include <algorithm> 
#include <cctype>    
#include <iomanip> 
#include <sstream> 

using namespace std;

const int ANIO_ACTUAL_SIMULADO = 25;
const int MES_ACTUAL_SIMULADO = 11;

bool VerificarAsientoOcupado(int idVuelo, const string& asientoDeseado) {
    vector<Reservacion> todasLasReservas = Reservacion::ObtenerReservaciones();

    for (const auto& reserva : todasLasReservas) {
        if (reserva.getIdVuelo() == idVuelo && reserva.getAsiento() == asientoDeseado) {
            return true;
        }
    }
    return false;
}

void Menu::MostrarMenu(Usuario& usuarioLogueado)
{
    int opcionMenu;

    int idPepe = usuarioLogueado.getId();
    string nombrePepe = usuarioLogueado.getNombre();

    do {
        system("cls");
        cout << "--- Menú de Vuelos | Bienvenido, " << nombrePepe << " ---\n"
            << "1 - Reservar vuelo\n"
            << "2 - Pagar boleto\n"
            << "3 - Ver mis Reservas\n"
            << "4 - Cerrar Sesión" << endl;

        cout << "Seleccione una opción: ";

        if (!(cin >> opcionMenu)) {
            cout << "Entrada inválida. Limpiando el buffer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcionMenu = 0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcionMenu)
        {
        case 1:
            ReservarVuelo(idPepe);
            break;
        case 2:
            PagarBoleto(idPepe);
            break;
        case 3:
            MostrarReservas(idPepe);
            break;
        case 4:
            cout << "\nCerrando sesión..." << endl;
            break;
        default:
            cout << "\nOpción no válida. Intente de nuevo." << endl;
            cout << "Presione ENTER para continuar...";
            cin.get();
            break;
        }
    } while (opcionMenu != 4);
}


void Menu::ReservarVuelo(int idUsuario)
{
    int idVuelo;
    string asiento = "N/A";
    vector<Vuelo> vuelos = Vuelo::ObtenerVuelos();

    system("cls");
    cout << "\n--- Reservar Vuelo ---" << endl;
    cout << "Vuelos disponibles:\n";
    cout << "ID | Fecha | Hora | Origen | Destino | Asientos Disp.\n";
    cout << "--------------------------------------------------------\n";

    int count = 0;
    for (const auto& v : vuelos) {
        if (count < 3) {
            cout << v.getId() << " | " << v.getFecha() << " | " << v.getHora() << " | "
                << v.getOrigen() << " | " << v.getDestino() << " | " << v.getAsientosDisponibles() << endl;
            count++;
        }
        else {
            break;
        }
    }

    if (count == 0) {
        cout << "No hay vuelos registrados para reservar. Consulte al Administrador." << endl;
        cout << "Presione ENTER para continuar...";
        cin.get();
        return;
    }

    cout << "--------------------------------------------------------\n";

    cout << "Ingrese el ID del vuelo a reservar: ";
    if (!(cin >> idVuelo)) {
        cout << "\nEntrada inválida." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Presione ENTER para continuar...";
        cin.get();
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    auto it = find_if(vuelos.begin(), vuelos.end(),
        [idVuelo](const Vuelo& v) { return v.getId() == idVuelo; });

    if (it != vuelos.end()) {
        if (it->getAsientosDisponibles() > 0) {

            do {
                cout << "Ingrese el número de asiento deseado (ej. A1): ";
                getline(cin, asiento);

                if (VerificarAsientoOcupado(idVuelo, asiento)) {
                    cout << "¡ERROR! El asiento " << asiento << " ya está ocupado en el vuelo ID " << idVuelo << ". Intente con otro." << endl;
                }
                else {
                    break;
                }
            } while (true);

            if (Reservacion::CrearReservacion(idUsuario, idVuelo, asiento)) {

                int asientosActuales = it->getAsientosDisponibles();
                if (asientosActuales > 0) {
                    it->setAsientosDisponibles(asientosActuales - 1);
                    Vuelo::ActualizarVuelos(vuelos);
                }

                cout << "\n¡Reserva creada exitosamente!" << endl;
                cout << "Vuelo ID: " << idVuelo << ", Asiento: " << asiento << endl;
            }
            else {
                cout << "\nERROR: No se pudo guardar la reserva." << endl;
            }
        }
        else {
            cout << "\nERROR: El vuelo " << idVuelo << " ya no tiene asientos disponibles." << endl;
        }
    }
    else {
        cout << "\nERROR: El ID de vuelo ingresado no existe." << endl;
    }

    cout << "Presione ENTER para continuar...";
    cin.get();
}


bool Menu::PagarBoleto(int idUsuario)
{
    system("cls");
    cout << "\n--- Pagar Boleto ---" << endl;

    vector<Reservacion> reservas = Reservacion::ObtenerReservaciones();
    vector<Vuelo> vuelos = Vuelo::ObtenerVuelos();
    vector<Reservacion> reservasUsuario;
    bool encontrado = false;
    int idReservaElegida;

    string numTarjeta;
    string fechaVencimiento;
    string cvv;

    cout << "ID_Reserva | Vuelo (Ruta)        | Asiento | Estado Pago\n";
    cout << "-------------------------------------------------------\n";
    for (const auto& reserva : reservas) {
        if (reserva.getIdUsuario() == idUsuario) {

            string rutaDetalle = "Vuelo ID " + to_string(reserva.getIdVuelo());

            auto itVuelo = find_if(vuelos.begin(), vuelos.end(),
                [&reserva](const Vuelo& v) { return v.getId() == reserva.getIdVuelo(); });

            if (itVuelo != vuelos.end()) {
                rutaDetalle = itVuelo->getOrigen() + " -> " + itVuelo->getDestino();
            }

            string estadoPago = reserva.estaPagado() ? "PAGADO" : "PENDIENTE";

            cout << "    " << reserva.getIdReservacion()
                << "      | " << left << setw(18) << rutaDetalle
                << " |  " << reserva.getAsiento()
                << "    | " << estadoPago << endl;

            reservasUsuario.push_back(reserva);
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No tienes reservas activas para pagar." << endl;
        cout << "\nPresione ENTER para continuar...";
        cin.get();
        return false;
    }

    cout << "\nIngrese el ID de la reserva que desea pagar: ";
    if (!(cin >> idReservaElegida)) {
        cout << "\nEntrada inválida." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Presione ENTER para continuar...";
        cin.get();
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    auto itReserva = find_if(reservas.begin(), reservas.end(),
        [idReservaElegida](const Reservacion& r) { return r.getIdReservacion() == idReservaElegida; });

    if (itReserva == reservas.end() || itReserva->getIdUsuario() != idUsuario) {
        cout << "\nERROR: El ID de reserva no es válido para su usuario." << endl;
        cout << "Presione ENTER para continuar...";
        cin.get();
        return false;
    }

    if (itReserva->estaPagado()) {
        cout << "\n🚫 ERROR: ¡Esta reserva (ID " << idReservaElegida << ") ya fue pagada! 🚫" << endl;
        cout << "Presione ENTER para continuar...";
        cin.get();
        return false;
    }

    cout << "\n--- Ingrese Datos de Pago ---" << endl;

    do {
        cout << "1. Tarjeta de Crédito (16 dígitos): ";
        getline(cin, numTarjeta);

        if (numTarjeta.length() != 16 || !all_of(numTarjeta.begin(), numTarjeta.end(), ::isdigit)) {
            cout << "ERROR: El número debe contener exactamente 16 dígitos numéricos sin espacios." << endl;
        }
    } while (numTarjeta.length() != 16 || !all_of(numTarjeta.begin(), numTarjeta.end(), ::isdigit));

    do {
        cout << "2. Fecha de Vencimiento (MM/AA): ";
        getline(cin, fechaVencimiento);

        if (fechaVencimiento.length() != 5 || fechaVencimiento[2] != '/') {
            cout << "ERROR: Formato de fecha incorrecto. Use MM/AA (ej. 12/26)." << endl;
            continue;
        }

        try {
            int mesVencimiento = stoi(fechaVencimiento.substr(0, 2));
            int anioVencimiento = stoi(fechaVencimiento.substr(3, 2));

            if (mesVencimiento < 1 || mesVencimiento > 12) {
                cout << "ERROR: El mes es inválido (debe ser 01 a 12)." << endl;
            }
            else if (anioVencimiento < ANIO_ACTUAL_SIMULADO) {
                cout << "ERROR: La tarjeta ha expirado. Año de vencimiento (20" << anioVencimiento << ") es anterior al " << (2000 + ANIO_ACTUAL_SIMULADO) << "." << endl;
            }
            else if (anioVencimiento == ANIO_ACTUAL_SIMULADO && mesVencimiento < MES_ACTUAL_SIMULADO) {
                cout << "ERROR: La tarjeta ha expirado este año. Mes de vencimiento es anterior al actual (" << MES_ACTUAL_SIMULADO << ")." << endl;
            }
            else {
                break;
            }
        }
        catch (const std::exception& e) {
            cout << "ERROR: No se pudieron leer los valores numéricos de la fecha." << endl;
        }

    } while (true);

    do {
        cout << "3. Código de Seguridad/CVV (3 o 4 dígitos): ";
        getline(cin, cvv);

        if (cvv.length() < 3 || cvv.length() > 4 || !all_of(cvv.begin(), cvv.end(), ::isdigit)) {
            cout << "ERROR: El CVV debe ser de 3 o 4 dígitos numéricos." << endl;
        }
        else {
            break;
        }
    } while (true);

    cout << "\nProcesando pago con Tarjeta: xxxx-xxxx-xxxx-" << numTarjeta.substr(12, 4) << endl;
    cout << "Fecha Vencimiento: " << fechaVencimiento << endl;

    itReserva->setPagado(true);
    Reservacion::ActualizarReservaciones(reservas);

    int idVueloPagado = itReserva->getIdVuelo();
    Vuelo* vueloPagado = nullptr;

    for (auto& v : vuelos) {
        if (v.getId() == idVueloPagado) {
            vueloPagado = &v;
            break;
        }
    }

    cout << "\n\n=======================================================" << endl;
    cout << "           PAGO REALIZADO para la RESERVA ID: " << itReserva->getIdReservacion() << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << "                    BOLETO DE AVIÓN " << endl;
    cout << "-------------------------------------------------------" << endl;

    if (vueloPagado) {
        cout << "RUTA: " << vueloPagado->getOrigen() << " -> " << vueloPagado->getDestino() << endl;
        cout << "VUELO ID: " << vueloPagado->getId() << " (" << vueloPagado->getNumeroAvion() << ")" << endl;
        cout << "FECHA: " << vueloPagado->getFecha() << endl;
        cout << "HORA DE ABORDAJE: " << vueloPagado->getHora() << endl;
    }
    else {
        cout << "VUELO ID: " << itReserva->getIdVuelo() << " (Detalles no disponibles)" << endl;
    }

    cout << "ASIENTO ASIGNADO: " << itReserva->getAsiento() << endl;
    cout << "=======================================================" << endl;

    cout << "\nPresione ENTER para regresar al Menú de Vuelos...";
    cin.get();
    return true;
}


void Menu::MostrarReservas(int idUsuario) const
{
    system("cls");
    cout << "\n--- Mis Reservas ---" << endl;

    vector<Reservacion> todasLasReservas = Reservacion::ObtenerReservaciones();
    bool encontrado = false;

    cout << "ID_Reserva | Vuelo (Detalle) | Asiento | Estado Pago\n";
    cout << "---------------------------------------------------\n";

    for (const auto& reserva : todasLasReservas) {
        if (reserva.getIdUsuario() == idUsuario) {

            vector<Vuelo> vuelos = Vuelo::ObtenerVuelos();
            auto it = find_if(vuelos.begin(), vuelos.end(),
                [&reserva](const Vuelo& v) { return v.getId() == reserva.getIdVuelo(); });

            string detalleVuelo = "ID " + to_string(reserva.getIdVuelo());
            if (it != vuelos.end()) {
                detalleVuelo = it->getOrigen() + " a " + it->getDestino();
            }

            string estadoPago = reserva.estaPagado() ? "✅ PAGADO" : "❌ PENDIENTE";

            cout << "    " << reserva.getIdReservacion()
                << "      | " << detalleVuelo
                << " |  " << reserva.getAsiento()
                << "    | " << estadoPago << endl;
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No tienes reservas activas." << endl;
    }

    cout << "\nPresione ENTER para continuar...";
    cin.get();
}


void Menu::RegistrarVueloAdmin() {
    string fecha, hora, origen, destino, numAvion;

    system("cls");
    cout << "\n--- REGISTRAR NUEVO VUELO ---" << endl;
    cout << "(Se crearán con 10 asientos disponibles)\n" << endl;

    cout << "Fecha (DD-MM-AAAA): ";
    getline(cin, fecha);
    cout << "Hora (HH:MM): ";
    getline(cin, hora);
    cout << "Origen (3 letras): ";
    getline(cin, origen);
    cout << "Destino (3 letras): ";
    getline(cin, destino);
    cout << "Número de Avión: ";
    getline(cin, numAvion);

    if (Vuelo::RegistrarVuelo(fecha, hora, origen, destino, numAvion)) {
        cout << "\n¡Vuelo registrado exitosamente! (10 asientos)." << endl;
    }
    else {
        cout << "\nERROR: No se pudo registrar el vuelo." << endl;
    }

    cout << "Presione ENTER para continuar...";
    cin.get();
}