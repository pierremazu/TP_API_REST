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
        if (url== service->getPattern() || (url.size() > service->getPattern().size() && url.starts_with(service->getPattern()) && url[service->getPattern().size()]=='/')  ) {
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
        throw ServiceException(http::status::not_found,"error");
    }

    std::optional<int> id = 0;
    std::string_view rest = url.substr(service->getPattern().size());
    if (rest.size() > 1) {
        rest.remove_prefix(1);
        int value = 0;
        auto [ptr, ec] = std::from_chars(rest.data(), rest.data() + rest.size(), value);
        if (ec != std::errc{} || ptr != rest.data() + rest.size())
            throw ServiceException(http::status::bad_request, "Bad id");
        id = value;
    }

    json::value value;
    http::status status;
    switch (method)
    {
    case http::verb::get:
        status=service->get(value,id);
        out=json::serialize(value);
        break;
    case http::verb::post:
        status=service->post(value,json::value_from(in));
        out=json::serialize(value);
        break;
    case http::verb::patch:
        status=service->patch(id,json::value_from(in));
        break;
    case http::verb::delete_:
        status=service->remove(id);
        break;
    default :
        throw ServiceException(http::status::not_implemented, "Method not implemented");
    }

    if (!value.is_null())
        out = json::serialize(value);
    return status;
}
