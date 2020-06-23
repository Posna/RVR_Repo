#include "Ball.h"
#include <math.h>
Ball::Ball(Vector2D pos, Texture* t, float radio): pos_(pos),
  radio_(radio), texture_(t)
{


}

Ball::Ball(Vector2D pos, bool r, float radio, uint32_t color): pos_(pos),
  radio_(radio), color_(color)
{
  if(r){
    setRandomColor();
  }

}

Ball::Ball(const Ball& obj){
  pos_ = obj.pos_;
  radio_ = obj.radio_;
  color_ = obj.color_;
  id_ = obj.id_;
}

Ball::~Ball(){

}

void Ball::render(SDL_Renderer* renderer) const{
  filledCircleColor(renderer, pos_.getX() - desfase_.getX(), pos_.getY() - desfase_.getY(), radio_, color_);



}

void Ball::update(uint32_t frameTime){
  int x, y;
  SDL_GetMouseState(&x, &y);
  vel_ = Vector2D(x - WIN_WIDTH/2, y - WIN_HEIGHT/2);
  vel_.normaliza();

  pos_ = pos_ + vel_ * (radio_/(pow(radio_, 1.1f)));

  //comprobacion de que esta dentro del recuadro del juego
  x = pos_.getX();
  y = pos_.getY();

  bool si = false;
  if(x < 0){
    si = true;
    x = 0;
  }
  if(x > LEVEL_WIDTH){
    si = true;
    x = LEVEL_WIDTH;
  }

  if(y < 0){
    si = true;
    y = 0;
  }
  if(y > LEVEL_HEIGHT){
    si = true;
    y = LEVEL_HEIGHT;
  }
  if(si)
    pos_ = Vector2D(x, y);
}

void Ball::to_bin(){
  alloc_data(MESSAGE_SIZE);

  memset(_data, 0, MESSAGE_SIZE);
  _size = MESSAGE_SIZE;
  char* tmp = _data;

  memcpy(tmp, &type, sizeof(uint8_t));
  tmp += sizeof(uint8_t);
  uint16_t x, y;
  x = pos_.getX();
  y = pos_.getY();
  memcpy(tmp, &x, sizeof(uint16_t));
  tmp += sizeof(uint16_t);
  memcpy(tmp, &y, sizeof(uint16_t));
  tmp += sizeof(uint16_t);
  memcpy(tmp, &radio_, sizeof(float));
  tmp += sizeof(float);
  memcpy(tmp, &id_, sizeof(uint32_t));
  tmp += sizeof(uint32_t);
  //printf("Vector2D(%d, %d) r: %d id: %d\n", pos_.getX(), pos_.getY(), radio_, id_);
  //printf("%d\n", *tmp);
}

int Ball::from_bin(char * data){
  alloc_data(MESSAGE_SIZE);

  memcpy(static_cast<void *>(_data), data, MESSAGE_SIZE);
  _size = MESSAGE_SIZE;
  //Reconstruir la clase usando el buffer _data

  char* tmp = _data;

  memcpy(&type, tmp, sizeof(uint8_t));
  tmp += sizeof(uint8_t);

  uint16_t x, y;
  float r;
  memcpy(&x, tmp, sizeof(uint16_t));
  tmp += sizeof(uint16_t);

  memcpy(&y, tmp, sizeof(uint16_t));
  tmp += sizeof(uint16_t);

  memcpy(&r, tmp, sizeof(float));
  tmp += sizeof(float);

  memcpy(&id_, tmp, sizeof(uint32_t));

  radio_ = r;
  pos_ = Vector2D(x, y);

  //printf("Vector2D(%d, %d) r: %d id: %d\n", pos_.getX(), pos_.getY(), radio_, id_);

  return 0;

}

const float Ball::getRadius(){
  return radio_;
}

void Ball::setRadius(float r){
  radio_ = r;
}

void Ball::addRadius(float r){
   radio_+=r;
}

void Ball::desfase(Vector2D d){
  desfase_ = d;
}

const Vector2D Ball::getPos(){
  return pos_;
}

void Ball::setPos(Vector2D v){
  pos_ = v;
}

void Ball::setGame(Game* g){
  g_ = g;
}

void Ball::setType(uint8_t t)
{
  type = t;
}

const uint8_t Ball::getType()
{
  return type;
}

void Ball::setRandomColor(){
  color_ = 0xFF;
  color_ |= (rand() & 0xFF) << 8;
  color_ |= (rand() & 0xFF) << 16;
  color_ |= (rand() & 0xFF) << 24;
}

void Ball::setColor(uint32_t c){
  color_ = c;
}

const uint32_t Ball::getColor(){
  return color_;
}

const uint32_t Ball::getId(){
  return id_;
}

void Ball::setId(uint32_t id){
  id_ = id;
}
