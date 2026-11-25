#include "AbstractService.h"

#include <utility>

AbstractService::AbstractService(std::string pattern) : pattern_(std::move(pattern)) {}

http::status AbstractService::get(json::value&, std::optional<int>) const
{
    throw ServiceException(http::status::method_not_allowed, "GET not supported by " + pattern_);
}

http::status AbstractService::post(json::value&, const json::value&)
{
    throw ServiceException(http::status::method_not_allowed, "POST not supported by " + pattern_);
}

http::status AbstractService::patch(std::optional<int>, const json::value&)
{
    throw ServiceException(http::status::method_not_allowed, "PATCH not supported by " + pattern_);
}

http::status AbstractService::remove(std::optional<int>)
{
    throw ServiceException(http::status::method_not_allowed, "DELETE not supported by " + pattern_);
}
