#pragma once

#include "AbstractService.h"
#include "UserDB.h"

/// Service "/user" : opérations CRUD sur la base d'utilisateurs.
class UserService : public AbstractService {
public:
    explicit UserService(UserDB& userDB);

    http::status get(json::value& out, std::optional<int> id) const override;
    http::status post(json::value& out, const json::value& in) override;
    http::status patch(std::optional<int> id, const json::value& in) override;
    http::status remove(std::optional<int> id) override;

private:
    UserDB& userDB_;
};
