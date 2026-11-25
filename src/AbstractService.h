#pragma once

#include "ServiceException.h"

#include <boost/json.hpp>

#include <optional>
#include <string>

namespace json = boost::json;

/// Classe de base de tous les services REST du TP.
///
/// Un service est associé à un motif d'URL (pattern), par exemple "/user".
/// Il traite les URL "/user" (la collection) et "/user/<id>" (un élément).
/// L'identifiant est passé sous forme de std::optional<int> : vide pour la
/// collection, renseigné pour un élément.
///
/// Correspondance HTTP -> méthode C++ :
///   GET    -> get()     lire        (200 OK)
///   POST   -> post()    créer       (201 Created)
///   PATCH  -> patch()   modifier    (204 No Content)
///   DELETE -> remove()  supprimer   (204 No Content)
///
/// Par défaut, chaque opération répond 405 Method Not Allowed :
/// un service ne surcharge que les opérations qu'il supporte.
class AbstractService {
public:
    explicit AbstractService(std::string pattern);
    virtual ~AbstractService() = default;

    const std::string& getPattern() const { return pattern_; }

    /// Lecture : remplit `out` avec la ressource demandée.
    virtual http::status get(json::value& out, std::optional<int> id) const;

    /// Création : `in` contient la ressource à créer ; remplir `out`
    /// (typiquement avec l'identifiant attribué).
    virtual http::status post(json::value& out, const json::value& in);

    /// Modification partielle de la ressource `id` avec les champs de `in`.
    virtual http::status patch(std::optional<int> id, const json::value& in);

    /// Suppression de la ressource `id`.
    virtual http::status remove(std::optional<int> id);

protected:
    std::string pattern_;
};
