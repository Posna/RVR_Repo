#include "Game.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include "Serializable.h"

class Ball: public Serializable{

public:
  static const size_t MESSAGE_SIZE = sizeof(uint16_t)*3 + sizeof(uint8_t) + sizeof(uint32_t);

  enum MessageType
  {
      LOGIN   = 0,
      POSITION = 1,
      LOGOUT  = 2,
      DEAD = 3,
      EAT = 4,
      ID = 5
  };


  Ball(Vector2D pos, Texture* t, uint16_t radio = 5);
  Ball(Vector2D pos = Vector2D(0,0), bool r = true, uint16_t radio = 5, uint32_t color = 0x550000FF); //Las primeras dos (0xFF..)son el alpha
  Ball(const Ball& obj);
  ~Ball();

  void render(SDL_Renderer* cam) const;
  void update(uint32_t frameTime);
  void handleInput();

  virtual void to_bin();
  virtual int from_bin(char * data);

  const uint16_t getRadius();
  void setRadius(uint16_t r);
  void addRadius(uint16_t r);
  //Este metodo se usa para que el jugador sea el punto medio
  //y todo vaya en funcion del mismo
  void desfase(Vector2D d);

  const Vector2D getPos();
  void setPos(Vector2D v);

  void setGame(Game* g);

  void setType(uint8_t t);
  const uint8_t getType();

  void setRandomColor();
  void setColor(uint32_t c);
  const uint32_t getColor();

  const uint32_t getId();
  void setId(uint32_t id);
private:
  Game* g_;
  Texture* texture_;
  SDL_Texture* t_;
  Vector2D desfase_ = Vector2D(0, 0);
  uint32_t color_;
  Vector2D vel_;
  uint16_t speed_ = 1;

  //Variables a serializar
  uint8_t type;
  Vector2D pos_;
  uint16_t radio_;
  uint32_t id_ = -1;
};
