#ifndef TCELL_H
#define TCELL_H
#include <ostream>

class TCell
{
public:
    bool IsVizit = false;
    bool isAdded = false;
    bool isPaint = false;
    int Id;
    int WeigthLeft;
    int WeigthRight;
    int WeigthUp;
    int WeigthDown;
    float Colors[3] = { 0.0f,0.0f,0.0f };
    TCell* Left;
    TCell* Right;
    TCell* Up;
    TCell* Down;

    TCell(int id);
    void Add(TCell* Right, TCell* Up, int WeigthRight, int WeigthUp);
};

#endif // TCELL_H
