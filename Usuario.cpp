#include "Usuario.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory> 
#include <algorithm> 
#include <limits>

using namespace std;

bool Usuario::IniciarSesion(Tipo& tipoUsuarioLogueado)
{
	string enteredCorreo;
	string enteredPassword;

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Usuario (Correo): ";
	getline(cin, enteredCorreo);

	cout << "Password: ";
	getline(cin, enteredPassword);

	vector<shared_ptr<Usuario>> usuarios = ObtenerUsuarios();

	for (const shared_ptr<Usuario>& usuario : usuarios)
	{

		if (usuario->ValidarInicioSesion(enteredCorreo, enteredPassword))
		{

			this->id = usuario->getId();
			this->nombre = usuario->getNombre();
			this->correo = usuario->getCorreo();
			this->password = usuario->getPassword();
			this->tipo = usuario->getTipo();
			tipoUsuarioLogueado = this->tipo;

			cout << "\nInicio de sesión exitoso! Bienvenido, " << this->nombre << "." << endl;
			return true;
		}
	}

	cout << "\nError: Correo o contraseña incorrectos." << endl;
	return false;
}

int Usuario::getId() const
{
	return id;
}

string Usuario::getCorreo() const
{
	return correo;
}

string Usuario::getNombre() const
{
	return nombre;
}

string Usuario::getPassword() const
{
	return password;
}

bool Usuario::RegistrarUsuario(string nombre, string correo, string password, Tipo tipoNuevo)
{
	vector<shared_ptr<Usuario>> usuarios = ObtenerUsuarios();
	int newId = 1;

	for (const shared_ptr<Usuario>& usuario : usuarios)
	{
		if (usuario->getId() >= newId) {
			newId = usuario->getId() + 1;
		}
		if (usuario->getCorreo() == correo) {
			cerr << "Error: El correo '" << correo << "' ya está registrado." << endl;
			return false;
		}
	}

	ofstream outputFile("usuarios.txt", ios::app);

	if (outputFile.is_open())
	{
		char tipoChar = (tipoNuevo == Administrador) ? 'A' : 'C';
		outputFile << newId << " " << tipoChar << " " << nombre << " " << correo << " " << password << "\n";
		outputFile.close();
		cout << "\nUsuario registrado exitosamente con ID: " << newId << endl;
		return true;
	}
	else {
		cerr << "Error: No se pudo abrir el archivo 'usuarios.txt' para escribir." << endl;
		return false;
	}
}

vector<shared_ptr<Usuario>> Usuario::ObtenerUsuarios()
{
	vector<shared_ptr<Usuario>> usuariosReg;
	ifstream inputFile("usuarios.txt");

	if (!inputFile.is_open()) {
		ofstream tempFile("usuarios.txt");
		tempFile.close();
		return usuariosReg;
	}

	string line;
	while (getline(inputFile, line)) {
		stringstream ss(line);

		int idFromFile;
		char tipoCharFromFile;
		string nombreFromFile, correoFromFile, passwordFromFile;

		if (ss >> idFromFile >> tipoCharFromFile >> nombreFromFile >> correoFromFile >> passwordFromFile) {

			Tipo tipoFromFile = (tipoCharFromFile == 'A' || tipoCharFromFile == 'a')
				? Administrador : Cliente;

			usuariosReg.push_back(make_shared<Usuario>(idFromFile,
				nombreFromFile, correoFromFile, passwordFromFile, tipoFromFile));
		}
		else {
			cerr << "Advertencia: Línea con formato incorrecto encontrada y omitida: " << line << endl;
		}
	}
	inputFile.close();
	return usuariosReg;
}