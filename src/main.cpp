#include "HttpServer.h"
#include "ServicesManager.h"
#include "UserDB.h"
#include "UserService.h"
#include "VersionService.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char* argv[])
{
    // Usage : ./server [port] [adresse]
    unsigned short port = 8080;
    if (argc > 1)
        port = static_cast<unsigned short>(std::atoi(argv[1]));

    std::string address = "127.0.0.1";
    if (argc > 2)
        address = argv[2];

    UserDB userDB;
    userDB.addUser({"Paul", 23});                // id 1

    ServicesManager manager;
    // TODO Question 3 : enregistrer une instance de VersionService
    // TODO Question 6 : enregistrer une instance de UserService

    try {
        HttpServer server(manager, address, port);
        server.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
