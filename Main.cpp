#include "Menu.h"
#include "Usuario.h"
#include <iostream>
#include <string>
#include <cstdlib> 
#include <limits>  
#include <cctype>  

using namespace std;

void MenuAdministrador(Usuario& adminUsuario);

void MostrarMenuPrincipal() {
	Usuario usuarioActual;
	Menu menu;
	int opcion;

	do {
		system("cls");
		cout << "--- Sistema de Vuelos ---" << endl;
		cout << "1. Iniciar Sesión" << endl;
		cout << "2. Salir de la Aplicación" << endl;
		cout << "Seleccione una opción: ";

		if (!(cin >> opcion)) {
			cout << "Entrada inválida. Limpiando el buffer." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			opcion = 0;
			continue;
		}

		switch (opcion) {
		case 1: {
			cout << "\n--- INICIAR SESIÓN ---" << endl;

			Usuario::Tipo tipoUsuario;

			if (usuarioActual.IniciarSesion(tipoUsuario)) {

				if (tipoUsuario == Usuario::Administrador) {
					MenuAdministrador(usuarioActual);
				}
				else {
					menu.MostrarMenu(usuarioActual);
				}
			}
			else {
				cout << "\nPresione ENTER para volver al Menú Principal...";
				cin.get();
			}
			break;
		}
		case 2:
			cout << "\nGracias por usar el sistema. Saliendo..." << endl;
			break;
		default:
			cout << "\nOpción no válida. Intente de nuevo." << endl;
			cout << "Presione ENTER para continuar...";
			cin.get();
			break;
		}

	} while (opcion != 2);
}

void MenuAdministrador(Usuario& adminUsuario) {
	int opcion;
	Menu menu;

	do {
		system("cls");
		cout << "--- Menú de Administrador: " << adminUsuario.getNombre() << " ---" << endl;
		cout << "1. Registrar Nuevo Usuario" << endl;
		cout << "2. Registrar Nuevo Vuelo" << endl;
		cout << "3. Ir al Menú de Vuelos (Comportamiento de Cliente)" << endl;
		cout << "4. Cerrar Sesión" << endl;
		cout << "Seleccione una opción: ";

		if (!(cin >> opcion)) {
			cout << "Entrada inválida. Limpiando el buffer." << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			opcion = 0;
			continue;
		}

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		switch (opcion) {
		case 1: {
			string nombre, correo, password;
			Usuario::Tipo tipoNuevo;
			char tipoChar;

			cout << "\n--- REGISTRAR NUEVO USUARIO ---" << endl;
			cout << "Tipo (A=Admin, C=Cliente): ";
			if (!(cin >> tipoChar)) {
				tipoChar = 'C';
			}
			tipoChar = toupper(tipoChar);

			if (tipoChar == 'A') {
				tipoNuevo = Usuario::Administrador;
			}
			else if (tipoChar == 'C') {
				tipoNuevo = Usuario::Cliente;
			}
			else {
				cout << "Tipo de usuario inválido. Por defecto se registrará como Cliente." << endl;
				tipoNuevo = Usuario::Cliente;
			}

			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "Nombre: ";
			getline(cin, nombre);
			cout << "Correo: ";
			getline(cin, correo);
			cout << "Contraseña: ";
			getline(cin, password);

			Usuario tempUser;
			tempUser.RegistrarUsuario(nombre, correo, password, tipoNuevo);
			cout << "\nPresione ENTER para continuar...";
			cin.get();
			break;
		}
		case 2: {
			menu.RegistrarVueloAdmin();
			break;
		}
		case 3: {
			menu.MostrarMenu(adminUsuario);
			break;
		}
		case 4:
			cout << "Cerrando sesión de Administrador." << endl;
			break;
		default:
			cout << "\nOpción no válida. Intente de nuevo." << endl;
			cout << "Presione ENTER para continuar...";
			cin.get();
			break;
		}

	} while (opcion != 4);
}


int main() {
	MostrarMenuPrincipal();
	return 0;
}