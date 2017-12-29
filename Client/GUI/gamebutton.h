#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include <QtWidgets>

class GameButton:public QPushButton
{
    int x,y;
public:
    GameButton(QWidget* parent = 0);
    GameButton(int _x, int _y, QWidget* parent = 0);
    void setX(const int& _x);
    void setY(const int& _y);
    const int& getX() const;
    const int& getY() const;
};

#endif // GAMEBUTTON_H
