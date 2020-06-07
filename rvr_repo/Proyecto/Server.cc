#include "Server.h"
#include "Ball.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void Server::do_messages()
{
  //Recibir Mensajes en y en función del tipo de mensaje
  // - LOGIN: Añadir al vector clients
  // - LOGOUT: Eliminar del vector clients
  // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
    socket.bind();
    char buffer[Ball::MESSAGE_SIZE];

    while (true)
    {
      //cm = ChatMessage();
      Ball cm;
      Socket* s;
      socket.recv(cm, s);
      switch(cm.type)
      {
        case Ball::LOGIN:
        printf("Se ha conectado\n");
        clients.push_back(s);
        jugadores.push_back(cm);
        break;

        case Ball::LOGOUT:{
          int i = 0;
          auto it = clients.begin();
          while(it != clients.end() && !(*(*it) == *s))
          {
            i++;
            it++;
          }
          if(it == clients.end())
          {
            printf("No está conectado\n");
          }
          else{
            //printf("%s Se ha desconectado\n", &cm.nick[0]);
            clients.erase(it);
            jugadores.erase(jugadores.begin()+i);
            delete *it;
            //*it = nullptr;
          }
        }
        break;

        case Ball::POSITION:
        int i = 0;
        auto it = clients.begin();
        while (it != clients.end() && !((*(*it)) == *s)) {
          i++;
        }
        if(it != clients.end()){
          jugadores[i].setPos(cm.getPos());
        }
        break;
      }
    }
}

void Server::game(){
  while(){

  }

}

/*void ChatClient::login()
{
    std::string msg("-SE HA UNIDO-");
    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
    em.type = ChatMessage::MESSAGE;
    socket.send(em, socket);
}

void ChatClient::logout()
{
  std::string msg("-SE HA DESCONECTADO-");

  ChatMessage em(nick, msg);
  em.type = ChatMessage::MESSAGE;
  socket.send(em, socket);

  em.type = ChatMessage::LOGOUT;
  socket.send(em, socket);
}

void ChatClient::input_thread()
{
    while (!terminar)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::string msg;
        std::getline(std::cin, msg);

        ChatMessage cm (nick, msg);
        cm.type = ChatMessage::MESSAGE;
        if(msg.compare("/q") == 0){
          logout();
          terminar = true;
        }
        else
          socket.send(cm, socket);
    }
}

void ChatClient::net_thread()
{
    while(!terminar)
    {
      //Recibir Mensajes de red
      //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
      ChatMessage cm;
      Socket* s;
      socket.recv(cm);
      std::cout << &cm.nick[0] << ": " << &cm.message[0] << "\n";
    }
}*/
