#pragma once

#include <boost/beast/http/status.hpp>

#include <stdexcept>
#include <string>

namespace http = boost::beast::http;

/// Exception à jeter depuis n'importe quel service pour interrompre la requête.
/// Le serveur l'intercepte et renvoie au client le statut HTTP associé,
/// avec un corps JSON de la forme { "error": "<message>" }.
///
///   throw ServiceException(http::status::not_found, "Invalid user id");
class ServiceException : public std::runtime_error {
public:
    ServiceException(http::status status, const std::string& msg)
        : std::runtime_error(msg), status_(status) {}

    http::status status() const noexcept { return status_; }

private:
    http::status status_;
};
