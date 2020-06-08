#include <string>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <thread>

#include "Serializable.h"
#include "Socket.h"
#include "Server.h"

int main(int argc, char **argv)
{

    Server es(argv[1], argv[2]);
    std::thread ms(&Server::recieve_messages, &es);
    es.update();

    ms.join();

    return 0;
}
