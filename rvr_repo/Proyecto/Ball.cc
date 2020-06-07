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

  pos_ = pos_ + vel_ * (double)(20.0/(double)radio_);//Del
}

void Ball::desfase(Vector2D d){
  desfase_ = d;
}
