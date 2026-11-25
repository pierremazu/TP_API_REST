#include "ServicesManager.h"

#include <charconv>
#include <optional>
#include <utility>

void ServicesManager::registerService(std::unique_ptr<AbstractService> service)
{
    // TODO Question 3
    (void)service;
}

AbstractService* ServicesManager::findService(std::string_view url) const
{
    // TODO Question 3
    (void)url;
    return nullptr;
}

http::status ServicesManager::queryService(std::string& out, const std::string& in,
                                           std::string_view url, http::verb method)
{
    // TODO Questions 1, 3, 4, 5, 7, 9, 11
    (void)out; (void)in; (void)url; (void)method;
    if (url == "/version") {
        if (method == http::verb::get) {
            json::value obj = {{"major", "1"}, {"minor", 0}};
            out = json::serialize(obj);
            return http::status::ok;
        }
    }
    else {
        throw ServiceException(http::status::not_implemented, "Not implemented yet");
    }
}
