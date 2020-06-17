#include "Server.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

std::mutex mutex_clients;
std::mutex mutex_jugadores;
std::mutex mutex_comida;

void Server::recieve_messages()
{
  //Recibir Mensajes en y en función del tipo de mensaje
  // - LOGIN: Añadir al vector clients y jugadores
  // - LOGOUT: Eliminar del vector clients y jugadores
  // - POSITION: Cambia la posicion de las bolas dentro del vector de jugadores
    socket.bind();
    char buffer[Ball::MESSAGE_SIZE];

    mutex_comida.lock();
    //Genera la comida aleatoria por el mapa
    for (size_t i = 0; i < NUM_BOLITAS; i++) {
      comida.push_back(getRandomBall());
      comida[i].setId(i);
    }
    mutex_comida.unlock();

    while (true)
    {
      Ball cm;
      Socket* s;
      socket.recv(cm, s);

      switch(cm.getType())
      {
        case Ball::LOGIN:{
          mutex_clients.lock();
          clients.push_back(s);
          mutex_clients.unlock();

          mutex_comida.lock();
          for(Ball b: comida){
            Ball aux = b;
            aux.setType(Ball::LOGIN);
            socket.send(aux, *s);
          }
          mutex_comida.unlock();

          mutex_jugadores.lock();
          cm.setId(id_actual);
          id_actual++;
          jugadores.push_back(cm);
          mutex_jugadores.unlock();

          cm.setType(Ball::ID);
          mutex_clients.lock();
          socket.send(cm, *s);
          cm.setType(Ball::LOGIN);
          int i = 0;
          for (auto it = clients.begin(); it < clients.end(); it++)
          {
            if (!((*(*it)) == *s))
            {
              socket.send(cm, *(*it));
              jugadores[i].setType(Ball::LOGIN);
              socket.send(jugadores[i], *s);
            }
            i++;
          }
          mutex_clients.unlock();
        }
        break;

        //Hay que mandar mensaje a todos los jugadores de que un id se ha desconectado para que no lo pinten
        case Ball::LOGOUT:{
          int i = 0;
          mutex_clients.lock();
          auto it = clients.begin();
          while(it != clients.end() && !((*(*it)) == *s))
          {
            i++;
            it++;
          }
          if(it == clients.end())
          {
            printf("No está conectado\n");
            mutex_clients.unlock();
          }
          else{
            printf("Cliente %d desconectado\n", cm.getId());
            for(Socket* s1: clients){
              socket.send(cm, *s1);
            }
            clients.erase(it);
            cm.setType(Ball::DEAD);
            socket.send(cm, *s);
            mutex_clients.unlock();

            mutex_jugadores.lock();
            jugadores.erase(jugadores.begin()+i);
            mutex_jugadores.unlock();
            delete *it;
          }

        }
        break;

        case Ball::POSITION:
        int i = 0;
        mutex_jugadores.lock();
        auto it = clients.begin();
        while(jugadores.size() > i && cm.getId() != jugadores[i].getId()){
          i++;
        }
        if(i != jugadores.size()){
          jugadores[i].setPos(cm.getPos());
        }
        mutex_jugadores.unlock();
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
  for(auto it = jugadores.begin(); it != jugadores.end(); ++it){
    Ball o1 = *it;
    y = 0;
    for(auto itt = jugadores.begin() + x; itt != jugadores.end(); ++itt){
      Ball o2 = *itt;
      if (o1.getId() != o2.getId())
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
          jugadores[x] = o1;
          mutex_clients.unlock();
        }
      }
      y++;
    }
    y = 0;
    mutex_comida.lock();
    for(Ball b: comida){
      Vector2D aux = o1.getPos() - b.getPos();
      if((o1.getRadius() > b.getRadius() + o1.getRadius()/4)&&
      (aux.magnitude() < o1.getRadius() + b.getRadius()))
      {
        mutex_clients.lock();
        o1.setType(Ball::EAT);
        o1.addRadius(b.getRadius());
        socket.send(o1, *clients[x]);
        mutex_clients.unlock();

        jugadores[x] = o1;
        comida[y] = getRandomBall();
        Ball aux = comida[y];

        mutex_clients.lock();
        aux.setType(Ball::POSITION);
        socket.send(aux, *clients[x]);
        mutex_clients.unlock();
      }
      y++;
    }
    mutex_comida.unlock();
    x++;
  }
  mutex_jugadores.unlock();
}

void Server::send_positions()
{
  int i = 0;
  mutex_jugadores.lock();
  for(auto it = jugadores.begin(); it != jugadores.end(); ++it){
    Ball o1 = *it;
    for(auto itt = jugadores.begin(); itt != jugadores.end(); ++itt){
      Ball o2 = *itt;
      if (o1.getId() != o2.getId())
      {
        mutex_clients.lock();
        o2.setType(Ball::POSITION);
        socket.send(o2, *clients[i]);
        mutex_clients.unlock();
      }
    }
    i++;
  }
  mutex_jugadores.unlock();
}


Ball Server::getRandomBall(){
  int x = rand() % WIN_WIDTH;
  int y = rand() % WIN_HEIGHT;
  return Ball(Vector2D(x, y), true);

}
