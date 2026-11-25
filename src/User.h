#pragma once

#include <boost/json.hpp>

#include <string>

namespace json = boost::json;

/// Un utilisateur de la mini base : un simple type valeur.
struct User {
    std::string name;
    int age = 0;
};

/// Conversion User -> JSON, utilisée par json::value_from(user).
/// Boost.JSON trouve cette fonction par ADL (« tag_invoke »).
///
///   json::value v = json::value_from(user);   // {"name":"Paul","age":23}
inline void tag_invoke(json::value_from_tag, json::value& jv, const User& u)
{
    jv = {{"name", u.name}, {"age", u.age}};
}
