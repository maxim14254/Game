#ifndef WAY_H
#define WAY_H

#include "tcell.h"


class Way
{
public:
    int SumWeigth = INT32_MAX;
    std::vector<TCell*> Cells;
    Way();
};

#endif // WAY_
