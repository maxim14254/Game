#ifndef CONTROL_H
#define CONTROL_H

#include <QPaintDevice>



class Control
{
public:
    void ShowControl(QPaintDevice* cc);
    void Add(float x, float y, int count);
    void EditColor(int numb, float r, float g, float b);
    void Delete();
private:
    float x, y;
    int count;
    float r, g, b;
    Control* cubes;
};

#endif // CONTROL_H
