#pragma once
#include <string>
#include "Usuario.h" 


class Menu
{
private:
    void MostrarReservas(int idUsuario) const;
    void ReservarVuelo(int idUsuario);
    bool PagarBoleto(int idUsuario);

public:
    void RegistrarVueloAdmin();
    void MostrarMenu(Usuario& usuarioLogueado);
};