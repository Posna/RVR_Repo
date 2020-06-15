#include "Server.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

std::mutex mutex_clients;
std::mutex mutex_jugadores;

void Server::recieve_messages()
{
  //Recibir Mensajes en y en función del tipo de mensaje
  // - LOGIN: Añadir al vector clients y jugadores
  // - LOGOUT: Eliminar del vector clients y jugadores
  // - POSITION: Cambia la posicion de las bolas dentro del vector de jugadores
    socket.bind();
    char buffer[Ball::MESSAGE_SIZE];

    while (true)
    {
      Ball cm;
      Socket* s = &socket;
      //printf("preRecv\n");
      socket.recv(cm, s);
      switch(cm.getType())
      {
        case Ball::LOGIN:{

          mutex_clients.lock();
          clients.push_back(s);
          mutex_clients.unlock();

          mutex_jugadores.lock();
          cm.setId(id_actual);
          id_actual++;
          jugadores.push_back(cm);
          mutex_jugadores.unlock();

          cm.setType(Ball::ID);
          mutex_clients.lock();
          //printf("preSend\n");
          socket.send(cm, *s);
          mutex_clients.unlock();
        }
        break;

        case Ball::LOGOUT:{
          int i = 0;
          printf("que lo desconecto!\n");
          printf("lockclientes\n");
          mutex_clients.lock();
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
            printf("Se ha desconectado\n");
            //mutex_clients.lock();
            clients.erase(it);
            //mutex_clients.unlock();
            printf("lockJugadores\n");
            mutex_jugadores.lock();
            jugadores.erase(jugadores.begin()+i);
            printf("unlockJugadores\n");
            mutex_jugadores.unlock();
            delete *it;
            //*it = nullptr;
          }
          printf("unlockclientes\n");
          mutex_clients.unlock();
        }
        break;

        case Ball::POSITION:
        int i = 0;
          mutex_jugadores.lock();
        //mutex_clients.lock();
        auto it = clients.begin();
        //Probablemente este bucle se pueda cambiar usando los ID
        while(jugadores.size() < i && cm.getId() != jugadores[i].getId()){
          if(cm.getId() == 51){
            printf("ey\n");
          }
          i++;
        }
        /*while (it != clients.end() && !((*(*it)) == *s)) {
          i++;
          it++;
        }
        if(it != clients.end()){

          jugadores[i].setPos(cm.getPos());

        }*/
        if(i != jugadores.size()){
          jugadores[i].setPos(cm.getPos());
        }
        mutex_jugadores.unlock();
        //mutex_clients.unlock();
        break;
      }
    }
}

void Server::update(){
  while(true){
    collision_detection();
    send_positions();
  }

}

void Server::collision_detection()
{
  int x = 0, y = 0;
  mutex_jugadores.lock();
  //printf("De momento bien\n");
  /*for (Ball o1 : jugadores)
  {
    //for (Ball o2 : jugadores)
    //{
      /*if (o1.getId() != o2.getId())
      {
        Vector2D aux = o1.getPos() - o2.getPos();
        if((o1.getRadius() > o2.getRadius() + o1.getRadius()/4)&&
        (aux.magnitude() < o1.getRadius() + o2.getRadius()))
        {
          mutex_clients.lock();
          o2.setType(Ball::DEAD);
          socket.send(o2, *clients[y]);
          o1.setType(Ball::EAT);
          o1.addRadius(o2.getRadius());
          socket.send(o1, *clients[x]);
          mutex_clients.unlock();
        }
      }
      //y++;
    //}
    x++;
  }*/
    for(auto it = jugadores.begin(); it != jugadores.end(); ++it){
      //Ball o1 = *it;
      for(auto itt = jugadores.begin(); itt != jugadores.end(); ++itt){
        //Ball o2 = *itt;
        //printf("comparacion\n");
        if ((*it).getId() != (*itt).getId())
        {
          //printf("distancias\n");
          Vector2D aux = (*it).getPos() - (*itt).getPos();
          if(((*it).getRadius() > (*itt).getRadius() + (*it).getRadius()/4)&&
          (aux.magnitude() < (*it).getRadius() + (*itt).getRadius()))
          {
            //printf("lock\n");
            mutex_clients.lock();
            (*itt).setType(Ball::DEAD);
            //printf("mensaje muerto\n");
            socket.send((*itt), *clients[y]);
            (*it).setType(Ball::EAT);
            (*it).addRadius((*itt).getRadius());
            //printf("mensaje comido\n");
            socket.send((*it), *clients[x]);
            //printf("unlock\n");
            mutex_clients.unlock();
        }
      }
    }
  }
  //printf("unlock jugadores\n");
  mutex_jugadores.unlock();
}

void Server::send_positions()
{
  int i = 0;
  mutex_jugadores.lock();
  //printf("De momento bien2\n");
  for(auto it = jugadores.begin(); it != jugadores.end(); ++it){
    //Ball o1 = *it;
    for(auto itt = jugadores.begin(); itt != jugadores.end(); ++itt){
      //Ball o2 = *itt;
      if ((*it).getId() != (*itt).getId())
      {
        mutex_clients.lock();
        //printf("lock\n");
        (*itt).setType(Ball::POSITION);
        //printf("que envia su pos!!!\n");
        socket.send((*itt), *clients[i]);
        //printf("eeeenviadaaa\n");
        mutex_clients.unlock();
        //printf("unlock\n");
      }
    }
    i++;
  }
  /*for (Ball o1 : jugadores)
  {
    for (Ball o2 : jugadores)
    {
      if (o1.getId() != o2.getId())
      {
        mutex_clients.lock();
        o2.setType(Ball::POSITION);
        socket.send(o2, *clients[i]);
        mutex_clients.unlock();
      }
    }
    i++;
  }*/
  //printf("unlock jugadores\n");
  mutex_jugadores.unlock();
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
