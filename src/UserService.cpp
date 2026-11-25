#include "UserService.h"

#include <boost/beast/http/field.hpp>

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

http::status UserService::patch(std::optional<int> id, const json::value& in) {
    if (!id)
        throw ServiceException(http::status::method_not_allowed, "No id");
    const User* user = userDB_.getUser(*id);
    if (!user) {
        throw ServiceException(http::status::not_found, "User not found");
    }
    User userM = User(user->name, user->age);
    const json::object* obj = in.if_object();
    if (!obj)
        throw ServiceException(http::status::bad_request, "Body must be a JSON object");
    for (const auto& [key, value] : *obj){
        if (key == "name" && value.is_string()) {
            std::string name(value.get_string());
            if (name.empty()) {
                throw ServiceException(http::status::bad_request, "Invalid name");
            }
            userM.name = name;
        }
        else if (key == "age" && value.is_int64()){
            std::int64_t age = value.get_int64();
            if (age < 0 || age > 150){
                throw ServiceException(http::status::bad_request, "Invalid age");
            }
            userM.age = static_cast<int>(age);
        }
        else {
            throw ServiceException(http::status::bad_request, "Bad argument");
        }
    }
    userDB_.setUser(id.value(), userM);
    return http::status::no_content;

}

http::status UserService::remove(std::optional<int> id)
{
    // TODO Question 12
    (void)id;
    throw ServiceException(http::status::not_implemented, "Not implemented yet");
}
