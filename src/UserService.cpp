#include "UserService.h"

UserService::UserService(UserDB& userDB) : AbstractService("/user"), userDB_(userDB) {}

http::status UserService::get(json::value& out, std::optional<int> id) const
{
    // TODO Question 6 (et Question 13 pour GET /user)
    (void)out; (void)id;
    throw ServiceException(http::status::not_implemented, "Not implemented yet");
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
