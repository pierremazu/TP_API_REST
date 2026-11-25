#pragma once

#include "AbstractService.h"

/// Service "/version" : renvoie la version de l'API en JSON.
class VersionService : public AbstractService {
public:
    VersionService();

    http::status get(json::value& out, std::optional<int> id) const override;
};
