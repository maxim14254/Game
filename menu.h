#include <QPaintDevice>
#include <string>
#ifndef MENU_H
#define MENU_H


struct TBtn
{
    void AddButton(std::string name, int x, int y, int width, int height);
    void print_string(int x, int y, int h, int w, int fontSize, const char* text, int r, int g, int b, QPaintDevice* cc);
    void ShowButton(QPaintDevice* cc);
    bool IsHover = false;
    bool IsDown = false;
    float vert[8];

private:
    std::string Name;
    int Width;
    int Height;
    int textWidth;
};



#endif // MENU_H
