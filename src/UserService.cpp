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

http::status UserService::post(json::value& out, const json::value& in) {
    const json::object* obj = in.if_object();
    if (!obj)
        throw ServiceException(http::status::bad_request, "Body must be a JSON object");

    const json::value* jname = obj->if_contains("name");
    const json::value* jage  = obj->if_contains("age");
    if (!jname || !jage || !jname->is_string() || !jage->is_int64())
        throw ServiceException(http::status::bad_request, "Bad Input");

    std::string name(jname->get_string());
    std::int64_t age = jage->get_int64();
    if (name.empty() || age < 0 || age > 150)
        throw ServiceException(http::status::bad_request, "Invalid name or age");

    int id = userDB_.addUser(User{name, static_cast<int>(age)});
    out = {{"id", id}};
    return http::status::created;
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
