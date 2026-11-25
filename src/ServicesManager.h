#pragma once

#include "AbstractService.h"

#include <boost/beast/http/verb.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <vector>

/// Gestionnaire de services : choisit le service et l'opération à exécuter
/// en fonction de l'URL et de la méthode HTTP de la requête.
class ServicesManager {
public:
    /// Ajoute un service à la liste (le manager en devient propriétaire).
    void registerService(std::unique_ptr<AbstractService> service);

    /// Renvoie le service dont le motif correspond à l'URL, sinon nullptr.
    AbstractService* findService(std::string_view url) const;

    /// Traite une requête.
    ///   out    : corps de la réponse (texte JSON), vide s'il n'y en a pas
    ///   in     : corps de la requête (texte JSON), vide s'il n'y en a pas
    ///   url    : cible de la requête, ex. "/user/1"
    ///   method : méthode HTTP (http::verb::get, http::verb::post, ...)
    /// Renvoie le statut HTTP de la réponse. Peut jeter une ServiceException.
    http::status queryService(std::string& out, const std::string& in,
                              std::string_view url, http::verb method);

private:
    std::vector<std::unique_ptr<AbstractService>> services_;
};
