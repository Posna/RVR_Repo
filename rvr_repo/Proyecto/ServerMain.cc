#include <string>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <thread>

#include "Serializable.h"
#include "Socket.h"
#include "Server.h"

extern "C" void * _server_thread(void *arg)
{
    Server * server = static_cast<Server *>(arg);

    server->update();

    return 0;
}


int main(int argc, char **argv)
{

    Server es(argv[1], argv[2]);
    pthread_attr_t attr;
    pthread_t id;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&id, &attr, _server_thread, static_cast<void *>(&es));
    //sleep(20);
    //std::thread ms(&Server::update, &es);
    //es.update();
    es.recieve_messages();
    //ms.join();


    return 0;
}
