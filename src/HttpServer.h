#pragma once

#include "ServicesManager.h"

#include <string>

/// Serveur HTTP/1.1 minimal basé sur Boost.Beast et les coroutines C++20.
///
/// Chaque requête reçue est transmise à ServicesManager::queryService().
/// Les ServiceException sont converties en réponse { "error": "..." } avec
/// le statut associé ; toute autre exception donne 500.
///
/// Le serveur tourne sur un seul thread (un seul io_context) : les services
/// n'ont donc pas besoin de verrous pour protéger leurs données.
///
/// Ce fichier est fourni : il n'y a rien à modifier pour le TP.
class HttpServer {
public:
    HttpServer(ServicesManager& manager, std::string address, unsigned short port);

    /// Lance le serveur ; rend la main après Ctrl+C (SIGINT) ou SIGTERM.
    void run();

private:
    ServicesManager& manager_;
    std::string address_;
    unsigned short port_;
};
