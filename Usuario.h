#pragma once
#include <string>
#include <vector>
#include <memory> 
#include <iostream>


class Usuario
{
public:
	enum Tipo { Cliente, Administrador };

private:
	int id = 0;
	std::string nombre;
	std::string correo;
	std::string password;
	Tipo tipo;

public:
	Usuario(int id, std::string nombre, std::string correo,
		std::string password, Tipo tipo)
		: id(id), nombre(nombre), correo(correo), password(password), tipo(tipo) {
	};

	Usuario() : tipo(Cliente) {};

	bool IniciarSesion(Tipo& tipoUsuarioLogueado);

	bool ValidarInicioSesion(const std::string& enteredEmail,
		const std::string& enteredPassword) const {
		return (correo == enteredEmail && password == enteredPassword);
	};

	int getId() const;
	std::string getCorreo() const;
	std::string getNombre() const;
	std::string getPassword() const;

	Tipo getTipo() const { return tipo; }
	bool esAdmin() const { return tipo == Administrador; }

	bool RegistrarUsuario(std::string nombre, std::string correo, std::string password, Tipo tipoNuevo);

	std::vector<std::shared_ptr<Usuario>> ObtenerUsuarios();
};