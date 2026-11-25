#include "HttpServer.h"

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>

#include <iostream>

namespace asio  = boost::asio;
namespace beast = boost::beast;
namespace json  = boost::json;
using tcp       = asio::ip::tcp;

namespace {

/// Exécute la requête et construit la réponse HTTP.
http::response<http::string_body>
handleRequest(ServicesManager& manager, const http::request<http::string_body>& req)
{
    http::response<http::string_body> res;
    res.version(req.version());
    res.keep_alive(req.keep_alive());
    res.set(http::field::server, "TP-REST (Boost.Beast)");

    std::string out;
    http::status status;
    try {
        status = manager.queryService(out, req.body(), req.target(), req.method());
    }
    catch (const ServiceException& e) {
        status = e.status();
        out = json::serialize(json::object{{"error", e.what()}});
    }
    catch (const std::exception& e) {
        status = http::status::internal_server_error;
        out = json::serialize(json::object{{"error", e.what()}});
    }

    res.result(status);
    // 204 No Content et 304 Not Modified ne doivent pas avoir de corps.
    if (!out.empty() && status != http::status::no_content
                     && status != http::status::not_modified) {
        res.set(http::field::content_type, "application/json");
        res.body() = std::move(out);
        if (res.body().back() != '\n')
            res.body() += '\n';
    }
    res.prepare_payload();

    std::cout << req.method_string() << ' ' << req.target() << " -> "
              << res.result_int() << ' ' << res.reason() << std::endl;
    return res;
}

/// Une connexion TCP : lit des requêtes et y répond, tant que le client
/// garde la connexion ouverte (keep-alive).
asio::awaitable<void> session(tcp::socket socket, ServicesManager& manager)
{
    beast::flat_buffer buffer;
    try {
        for (;;) {
            http::request<http::string_body> req;
            co_await http::async_read(socket, buffer, req, asio::use_awaitable);

            auto res = handleRequest(manager, req);
            co_await http::async_write(socket, res, asio::use_awaitable);

            if (!res.keep_alive())
                break;
        }
    }
    catch (const boost::system::system_error& e) {
        if (e.code() != http::error::end_of_stream
            && e.code() != asio::error::connection_reset)
            std::cerr << "session: " << e.what() << std::endl;
    }
    boost::system::error_code ec;
    socket.shutdown(tcp::socket::shutdown_send, ec);
}

/// Accepte les connexions entrantes et lance une session pour chacune.
asio::awaitable<void> listener(tcp::acceptor acceptor, ServicesManager& manager)
{
    for (;;) {
        tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);
        asio::co_spawn(acceptor.get_executor(),
                       session(std::move(socket), manager),
                       asio::detached);
    }
}

} // namespace

HttpServer::HttpServer(ServicesManager& manager, std::string address, unsigned short port)
    : manager_(manager), address_(std::move(address)), port_(port) {}

void HttpServer::run()
{
    asio::io_context ioc;

    tcp::endpoint endpoint{asio::ip::make_address(address_), port_};
    tcp::acceptor acceptor{ioc, endpoint};  // reuse_address activé par défaut

    asio::co_spawn(ioc, listener(std::move(acceptor), manager_), asio::detached);

    asio::signal_set signals{ioc, SIGINT, SIGTERM};
    signals.async_wait([&](auto, auto) { ioc.stop(); });

    std::cout << "Serveur en écoute sur http://" << address_ << ':' << port_
              << "  (Ctrl+C pour arrêter)" << std::endl;
    ioc.run();
}
