#include "VersionService.h"

VersionService::VersionService() : AbstractService("/version") {}

http::status VersionService::get(json::value& out, std::optional<int> /*id*/) const
{
    // TODO Question 2
    (void)out;
    throw ServiceException(http::status::not_implemented, "Not implemented yet");
}
