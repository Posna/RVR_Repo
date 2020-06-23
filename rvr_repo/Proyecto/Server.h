#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Serializable.h"
#include "Socket.h"
#include "Ball.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/**
 *  Clase para el servidor de chat
 */
class Server
{
public:
    static const int NUM_BOLITAS = 100;
    Server(const char * s, const char * p): socket(s, p)
    {
        id_actual = NUM_BOLITAS;
        socket.bind();
    };

    /*
     *  Thread principal del servidor recive mensajes en el socket y
     *  lo distribuye a los clientes. Mantiene actualizada la lista de clientes
     */
    void recieve_messages();
    void update();
    void collision_detection();
    void send_positions();

    Ball getRandomBall();
    Ball getRandomBall(int id);

private:
    uint32_t id_actual;
    /**
     *  Lista de clientes conectados al servidor de Chat, representados por
     *  su socket
     */
    std::vector<Socket *> clients;
    std::vector<Ball> jugadores;
    std::vector<Ball> comida;
    /**
     * Socket del servidor
     */
    Socket socket;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
