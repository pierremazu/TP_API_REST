#include "UserDB.h"

#include <utility>

const User* UserDB::getUser(int id) const
{
    auto it = users_.find(id);
    if (it == users_.end())
        return nullptr;
    return &it->second;
}

int UserDB::addUser(User user)
{
    int id = idseq_++;
    users_.emplace(id, std::move(user));
    return id;
}

bool UserDB::setUser(int id, User user)
{
    auto it = users_.find(id);
    if (it == users_.end())
        return false;
    it->second = std::move(user);
    return true;
}

bool UserDB::removeUser(int id)
{
    return users_.erase(id) > 0;
}
