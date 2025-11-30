#pragma once
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "Usuario.h"

class AdministradorDeSesion
{
private:
    std::map<int, std::shared_ptr<Usuario>> sesionesActivas;

public:
    bool login(const std::string& correo, const std::string& password,
        const std::vector<std::shared_ptr<Usuario>>& usuarios)
    {
        for (const auto& usuario : usuarios)
        {
            if (usuario->ValidarInicioSesion(correo, password))
            {
                sesionesActivas[usuario->getId()] = usuario;
                return true;
            }
        }
        return false;
    }

    void logout(int userId)
    {
        sesionesActivas.erase(userId);
    }

    bool isLoggedIn(int userId) const
    {
        return sesionesActivas.find(userId) != sesionesActivas.end();
    }

    std::shared_ptr<Usuario> getSesionUsuario(int idUsuario) const
    {
        auto it = sesionesActivas.find(idUsuario);
        return (it != sesionesActivas.end()) ? it->second : nullptr;
    }
};