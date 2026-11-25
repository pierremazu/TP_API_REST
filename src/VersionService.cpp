#include "VersionService.h"

VersionService::VersionService() : AbstractService("/version") {}



http::status VersionService::get(json::value& out, std::optional<int> /*id*/) const
{
    out = {{"major",1},{"minor",0}};
    return http::status::ok;
}
