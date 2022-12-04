#include "tcell.h"

TCell::TCell(int id) : Id(id)
{
    Left = nullptr;
    Right = nullptr;
    Up = nullptr;
    Down = nullptr;

    WeigthLeft = INT32_MAX;
    WeigthRight = INT32_MAX;
    WeigthUp = INT32_MAX;
    WeigthDown = INT32_MAX;
}

void TCell:: Add(TCell* Right, TCell* Up, int WeigthRight, int WeigthUp)
{
    Right != nullptr ? Right->Left = this : nullptr;
    Right != nullptr ? Right->WeigthLeft = WeigthRight : INT32_MAX;
    Up != nullptr ? Up->Down = this : nullptr;
    Up != nullptr ? Up->WeigthDown = WeigthUp : INT32_MAX;

    Right != nullptr ? this->Right = Right : nullptr;
    Right != nullptr ? this->WeigthRight = WeigthRight : INT32_MAX;
    Up != nullptr ? this->Up = Up : nullptr;
    Up != nullptr ? this->WeigthUp = WeigthUp : INT32_MAX;
}

