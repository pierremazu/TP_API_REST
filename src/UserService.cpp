#include "UserService.h"

UserService::UserService(UserDB& userDB) : AbstractService("/user"), userDB_(userDB) {}

http::status UserService::get(json::value& out, std::optional<int> id) const {
    if (!id)
        throw ServiceException(http::status::bad_request, "No id");
    const User* user = userDB_.getUser(*id);
    if (!user) {
        throw ServiceException(http::status::not_found, "User not found");
    }
    out = json::value_from(*user);
    return http::status::ok;
}

http::status UserService::post(json::value& out, const json::value& in)
{
    // TODO Question 8
    (void)out; (void)in;
    throw ServiceException(http::status::not_implemented, "Not implemented yet");
}

http::status UserService::patch(std::optional<int> id, const json::value& in)
{
    // TODO Question 10
    (void)id; (void)in;
    throw ServiceException(http::status::not_implemented, "Not implemented yet");
}

http::status UserService::remove(std::optional<int> id)
{
    // TODO Question 12
    (void)id;
    throw ServiceException(http::status::not_implemented, "Not implemented yet");
}
