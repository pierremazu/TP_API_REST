#include "ServicesManager.h"

#include <charconv>
#include <optional>
#include <utility>

void ServicesManager::registerService(std::unique_ptr<AbstractService> service)
{
    services_.push_back(std::move(service));
}

AbstractService* ServicesManager::findService(std::string_view url) const
{
    for (const auto& service : services_) {
        if (url== service->getPattern() || url.starts_with(service->getPattern()+"/")) {
            return service.get();
        }
    }
    return nullptr;
}

http::status ServicesManager::queryService(std::string& out, const std::string& in,
                                           std::string_view url, http::verb method)
{
    auto service = findService(url);

    if (service==nullptr) {
        return http::status::not_found;
    }

    json::value value = {};
    auto status=http::status::not_found;
    switch (method)
    {
    case http::verb::get:
        status=service->get(value,std::nullopt);
        out=json::serialize(value);
        return status;
    case http::verb::post:
        status=service->post(value,json::parse(in));
        out=json::serialize(value);
        return status;
    case http::verb::patch:
        status=service->patch(std::nullopt,json::parse(in));
        return status;
    case http::verb::delete_:
        status=service->remove(std::nullopt);
        return status;
    default :
        return status;

    }
}
