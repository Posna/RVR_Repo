#include "Ball.h"

Ball::Ball(Vector2D pos, Texture* t, uint16_t radio): pos_(pos),
  radio_(radio), texture_(t)
{


}

Ball::Ball(Vector2D pos, bool r, uint16_t radio, uint32_t color): pos_(pos),
  radio_(radio), color_(color)
{
  if(r){
    color_ = 0xFF;
    color_ |= (rand() & 0xFF) << 8;
    color_ |= (rand() & 0xFF) << 16;
    color_ |= (rand() & 0xFF) << 24;
  }

}

Ball::~Ball(){

}

void Ball::render(SDL_Renderer* renderer){
  filledCircleColor(renderer, pos_.getX() - desfase_.getX(), pos_.getY() - desfase_.getY(), radio_, color_);



}

void Ball::update(uint32_t frameTime){
  int x, y;
  SDL_GetMouseState(&x, &y);
  vel_ = Vector2D(x - WIN_WIDTH/2, y - WIN_HEIGHT/2);
  vel_.normaliza();

  pos_ = pos_ + vel_ * (double)(50.0/(double)radio_);

  //comprobacion de que esta dentro del recuadro del juego
}

void Ball::to_bin(){
  alloc_data(MESSAGE_SIZE);

  memset(_data, 0, MESSAGE_SIZE);
  _size = MESSAGE_SIZE;
  char* tmp = _data;

  memcpy(tmp, &type, 1);
  tmp += sizeof(uint8_t);
  memcpy(tmp, &pos_.getX(), sizeof(uint16_t));
  tmp += sizeof(uint16_t);
  memcpy(tmp, &pos_.getY(), sizeof(uint16_t));
  tmp += sizeof(uint16_t);
  memcpy(tmp, &radio_, sizeof(uint16_t));
  tmp += sizeof(uint16_t);
}

void Ball::from_bin(char * data){
  alloc_data(MESSAGE_SIZE);

  memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);
  _size = MESSAGE_SIZE;
  //Reconstruir la clase usando el buffer _data

  char* tmp = _data;

  memcpy(&type, tmp, sizeof(uint8_t));
  tmp += sizeof(uint8_t);

  uint16_t x, y, r;
  memcpy(&x, tmp, sizeof(uint16_t));
  tmp += sizeof(uint16_t);

  memcpy(&y, tmp, sizeof(uint16_t));
  tmp += sizeof(uint16_t);

  memcpy(&r, tmp, sizeof(uint16_t));


}

uint16_t Ball::getRadius(){
  return radio_;
}

void Ball::setRadius(uint16_t r){
  radio_ = r;
}

void Ball::addRadius(uint16_t r){
   radio_+=r;
}

void Ball::desfase(Vector2D d){
  desfase_ = d;
}

Vector2D Ball:getPos(){
  return pos_;
}

void Ball::setPos(Vector2D v){
  pos_ = v;
}

void Ball::setGame(Game* g){
  g_ = g;
}
