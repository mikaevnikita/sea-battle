#include "gamebutton.h"

GameButton::GameButton(QWidget* parent)
    :QPushButton(parent),x(-1),y(-1)
{

}

GameButton::GameButton(int _x, int _y, QWidget* parent)
    :QPushButton(parent),x(_x),y(_y)
{
}

void GameButton::setX(const int& _x){
    x = _x;
}

void GameButton::setY(const int& _y){
    y = _y;
}

const int& GameButton::getX() const{
    return x;
}
const int& GameButton::getY() const{
    return y;
}
