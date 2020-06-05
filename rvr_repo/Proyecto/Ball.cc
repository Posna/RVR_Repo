#include "Ball.h"

Ball::Ball(Vector2D pos, Texture* t, uint16_t radio): pos_(pos),
  radio_(radio), texture_(t)
{


}

Ball::Ball(Vector2D pos, uint16_t radio, uint32_t color): pos_(pos),
  radio_(radio), color_(color)
{


}

Ball::~Ball(){

}

void Ball::render(SDL_Renderer* renderer){
  filledCircleColor(renderer, pos_.getX(), pos_.getY(), radio_, color_);

  /*SDL_Rect destRect;
  destRect.w = radio_;
  destRect.h = radio_;
  destRect.x = pos_.getX();
  destRect.y = pos_.getY();
  texture_->render(destRect);*/

  //aalineRGBA(renderer, pos_.getX(), pos_.getY(), 50, 50, 255, 255, 255, 255);

}

void Ball::update(){
  int x, y;
  SDL_GetMouseState(&x, &y);
  vel_ = Vector2D(x - pos_.getX(), y - pos_.getY());
  vel_.normaliza();

  pos_ = pos_ + vel_ * speed_;

}
