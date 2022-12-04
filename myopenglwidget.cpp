#include <myopenglwidget.h>
#include <menu.h>
#include <way.h>
#include <tcell.h>
#include <thread>
#include <chrono>
#include <control.h>

#define mapW 10
#define mapH 12

TCell* cells;
Way* ways;
Control control;
std::allocator<TCell> AllocCells;
std::allocator<Way> AllocWays;
std::vector<int> indexes;
TBtn btn;
TBtn btn1;
TBtn btn2;
TBtn text;
float mainColors[3][3] = {
    {0.9f,0.9f,0.9f},
    {0.9f,0.9f,0.9f},
    {0.9f,0.9f,0.9f}
};
float AllColrors[7][3] = {
    {0.9f,0.9f,0.9f},
    {1.0f,0.0f,0.0f},
    {0.0f,1.0f,0.0f},
    {0.0f,0.0f,1.0f},
    {1.0f,1.0f,0.0f},
    {1.0f,0.0f,1.0f},
    {0.0f,1.0f,1.0f}
};
std::chrono::time_point<std::chrono::steady_clock> animTime;
int colorNumb = 0;
size_t index = 0;
int maxId = 0;
bool Anim = false;
bool RevAnim = false;


void Go(TCell* cell, TCell* cellGo, int way, std::allocator<Way>& sss2, bool reverse = false)
{
    if (cellGo == nullptr)
        return;

    if(!reverse)
    {
        std::allocator_traits<std::allocator<Way>>::construct(sss2, &ways[cellGo->Id]);

        int r = rand() % 7;
        cellGo->Colors[0] = AllColrors[r][0];
        cellGo->Colors[1] = AllColrors[r][1];
        cellGo->Colors[2] = AllColrors[r][2];
    }

    if (!cellGo->IsVizit && way + ways[cell->Id].SumWeigth < ways[cellGo->Id].SumWeigth)
    {
        ways[cellGo->Id].SumWeigth = way + ways[cell->Id].SumWeigth;
        if (ways[cell->Id].Cells.size() > 0)
        {
            ways[cellGo->Id].Cells.clear();
            ways[cellGo->Id].Cells.insert(ways[cellGo->Id].Cells.end(), ways[cell->Id].Cells.begin(), ways[cell->Id].Cells.end());
        }
        ways[cellGo->Id].Cells.push_back(cellGo);
    }
}

void Game_New(size_t GameLevel)
{
    typedef std::allocator_traits<std::allocator<TCell>> Alloc;

    size_t i = 0;
    size_t j = 1;
    Alloc::construct(AllocCells, &cells[i], i);
    while (i < mapH * mapW - 1)
    {
        if (j == mapH)
        {
            cells[i].Add(&cells[i+1], nullptr, rand() % 10 + 1, INT32_MAX);
            ++i;
        }
        else
        {
            if (i == mapW * j - 1)
            {
                Alloc::construct(AllocCells, &cells[i+mapW], i+mapW);
                cells[i].Add(nullptr, &cells[i+mapW], rand() % 10 + 1, rand() % 10 + 1);
                ++j;
            }
            else if(i < mapW)
            {
                Alloc::construct(AllocCells, &cells[i+1], i+1);
                Alloc::construct(AllocCells, &cells[i+mapW], i+mapW);
                cells[i].Add(&cells[i+1], &cells[i+mapW], rand() % 10 + 1, rand() % 10 + 1);
            }
            else
            {
                Alloc::construct(AllocCells, &cells[i+mapW], i+mapW);
                cells[i].Add(&cells[i+1], &cells[i+mapW], rand() % 10 + 1, rand() % 10 + 1);
            }
            ++i;
        }
    }

    int start = rand() % mapW;
    int finish = rand() % mapW + 110;
    cells[start].IsVizit = true;

    std::allocator_traits<std::allocator<Way>>::construct(AllocWays, &ways[start]);
    ways[start].SumWeigth = 0;

    for (size_t i = start; i < mapW; ++i)
    {
        Go(&cells[i], cells[i].Right, cells[i].WeigthRight, AllocWays);
        Go(&cells[i], cells[i].Up, cells[i].WeigthUp, AllocWays);
    }

    for (int i = start; i >= 0; --i)
    {
        Go(&cells[i], cells[i].Left, cells[i].WeigthLeft, AllocWays);
        Go(&cells[i], cells[i].Up, cells[i].WeigthUp, AllocWays);
    }

    for (size_t i = 1; i < mapH; ++i)
    {
        for (size_t j = i * mapW; j < i * mapW + mapW; ++j)
        {
            Go(&cells[j], cells[j].Right, cells[j].WeigthRight, AllocWays, true);
            Go(&cells[j], cells[j].Up, cells[j].WeigthUp, AllocWays);
        }

        for (size_t j = i * mapW + mapW - 1; j >= i * mapW; --j)
        {
            Go(&cells[j], cells[j].Left, cells[j].WeigthLeft, AllocWays, true);
            Go(&cells[j], cells[j].Up, cells[j].WeigthUp, AllocWays, true);
        }
    }

    auto iter1 = ways[finish].Cells.cbegin();
    ways[finish].Cells.insert(iter1, &cells[start]);

    int a = -1;
    int b = -1;
    float** RGB = new float* [GameLevel];
    for (size_t i = 0; i < GameLevel; ++i)
    {
        int r = rand() % 7;
        if (a == r || b == r)
            --i;
        else
        {
            RGB[i] = AllColrors[r];
            if (GameLevel > 2 && i == 0)
                b = r;
            else
                a = r;
        }
    }

    for (i = 0; i < ways[finish].Cells.size(); ++i)
    {
        if (GameLevel > 0)
        {
            ways[finish].Cells[i]->Colors[0] = RGB[0][0];
            ways[finish].Cells[i]->Colors[1] = RGB[0][1];
            ways[finish].Cells[i]->Colors[2] = RGB[0][2];

            if (GameLevel > 1 && i + 1 < ways[finish].Cells.size())
            {
                i = i + 1;
                ways[finish].Cells[i]->Colors[0] = RGB[1][0];
                ways[finish].Cells[i]->Colors[1] = RGB[1][1];
                ways[finish].Cells[i]->Colors[2] = RGB[1][2];

                if (GameLevel > 2 && i + 1 < ways[finish].Cells.size())
                {
                    i = i + 1;
                    ways[finish].Cells[i]->Colors[0] = RGB[2][0];
                    ways[finish].Cells[i]->Colors[1] = RGB[2][1];
                    ways[finish].Cells[i]->Colors[2] = RGB[2][2];
                }
            }
        }
    }
    delete[] RGB;
}

void ShowField(float red, float green, float blue)
{
    glBegin(GL_TRIANGLE_STRIP);

    glColor4f(red - 0.2f, green - 0.2f, blue - 0.2f, 0.85f); glVertex2f(0, 1);
    glColor4f(red - 0.3f, green - 0.3f, blue - 0.3f, 0.85f); glVertex2f(1, 1); glVertex2f(0, 0);
    glColor4f(red - 0.6f, green - 0.6f, blue - 0.6f, 0.85f); glVertex2f(1, 0);

    glEnd();
}

void Game_Show()
{
    glLoadIdentity();
    glScalef(1.85f / mapW, 1.7f / mapH, 1.0f);
    glTranslated(-mapW * 0.5, -mapH * 0.45, 0);


    for (int y = 0; y < mapH; ++y)
    {
        for (int x = 0; x < mapW; ++x)
        {
            glPushMatrix();
            glTranslated(x, y, 0);

            int index = y * mapW + x;
            cells[index].IsVizit = false;
            cells[index].isAdded = false;
            ShowField(cells[index].Colors[0], cells[index].Colors[1], cells[index].Colors[2]);

            glPopMatrix();
        }
    }
}

void ChangeColor(TCell* color, float col)
{
    color->Colors[0] += col;
    color->Colors[1] += col;
    color->Colors[2] += col;
}

void AnimateChangeColor(TCell* cell, bool revers = false)
{
    auto time = std::chrono::steady_clock::now();
    double deltaTime = std::chrono::duration<double>(time - animTime).count();

    if (deltaTime < 0.2)
        return;
    else if (cell->isPaint && !revers)
    {
        ++index;
        return;
    }
    animTime = time;

    float col;
    if (revers)
    {
        col = -0.9f;
        cell->isPaint = false;
        --index;
        RevAnim = true;
    }
    else
    {
        col = 0.9f;
        cell->isPaint = true;
        ++index;
        RevAnim = false;
    }

    ChangeColor(cell, col);
}


void MyOpenGLWidget::Scene1()
{
    btn.AddButton("Easy", -150, 25, 300, 50);
    btn1.AddButton("Normal", -150, -125, 300, 50);
    btn2.AddButton("Difficult", -150, -275, 300, 50);

    glScalef(0.015f, -0.015f, 1.0f);

    text.print_string(29, 120, 105, 724, 80, "Choose the difficulty", 128, 128, 255, this);
    glLoadIdentity();

    btn.ShowButton(this);
    btn1.ShowButton(this);
    btn2.ShowButton(this);

    update();
}

void MyOpenGLWidget::Scene2()
{
    control.ShowControl(this);

    Game_Show();

    if (Anim && indexes.size() > index && !RevAnim)
    {
        if (cells[indexes[index]].Id > maxId)
            maxId = cells[indexes[index]].Id;

        AnimateChangeColor(&cells[indexes[index]]);
    }
    else if ((Anim && indexes.size() <= index) || RevAnim)
    {
        AnimateChangeColor(&cells[indexes[index - 1]], true);
    }

    if (Anim && indexes.size() == index && maxId > mapW * mapH - mapW - 1)
    {
        maxId = 0;
        Anim = false;
        index = 0;
        gameNew = true;
    }

    if (index == 0)
    {
        indexes.clear();
        RevAnim = false;
        Anim = false;
    }



    update();
}

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setSamples(1);
    this->setFormat(format);

    this->setMouseTracking(true);
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    std::allocator_traits<std::allocator<TCell>>::deallocate(AllocCells, cells, mapH * mapW);
    std::allocator_traits<std::allocator<Way>>::deallocate(AllocWays, ways, mapH * mapW);
};

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    cells = std::allocator_traits<std::allocator<TCell>>::allocate(AllocCells, mapH * mapW);
    ways = std::allocator_traits<std::allocator<Way>>::allocate(AllocWays, mapH * mapW);
};

void MyOpenGLWidget::paintGL()
{
    auto begin = std::chrono::steady_clock::now();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(GameLevel == 0)
    {
        Scene1();

        index = 0;
        maxId = 0;
    }
    else
    {
        Scene2();
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::this_thread::sleep_for(std::chrono::milliseconds(std::chrono::milliseconds(17) - elapsed_ms));
};

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event)
{   
    auto xy = event->position();

    int width = this->width()/2;
    int height = this->height()/2;

    if (event->buttons() == Qt::RightButton && !Anim && GameLevel > 0)
    {
        if (xy.x() <= 30 || xy.x() >= (this->height() - 30) || xy.y() <= (this->height() - 95) - ((this->height() - 120) / mapH) || xy.y() >= this->height() - 95)
        {
            --colorNumb;
            if (colorNumb < 0)
                colorNumb = 6;
            return;
        }
    }
    else if (event->buttons() == Qt::LeftButton && !Anim && GameLevel > 0)
    {
        if (gameNew)
        {
            gameNew = false;
            Game_New(GameLevel);
            return;
        }
        if (xy.x() <= 30 || xy.x() >= (this->height() - 30) || xy.y() <= (this->height() - 95) - ((this->height() - 120) / mapH) || xy.y() >= this->height() - 95)
        {
            ++colorNumb;
            if (colorNumb > 6)
                colorNumb = 0;
            return;
        }
        size_t x = (xy.x() - 30) / ((this->width() - 60) / mapW);

        if (!(cells[x].Colors[0] == mainColors[0][0] && cells[x].Colors[1] == mainColors[0][1] && cells[x].Colors[2] == mainColors[0][2]))
            return;

        Anim = true;
        indexes.push_back(x);
        cells[indexes[0]].IsVizit = true;
        cells[indexes[0]].isAdded = true;
        for (size_t i = 0; i < indexes.size(); ++i)
        {
            int a;
            std::equal(std::begin(cells[indexes[i]].Colors), std::end(cells[indexes[i]].Colors), std::begin(mainColors[0])) && GameLevel > 1 ? a = 1 : a = 0;
            std::equal(std::begin(cells[indexes[i]].Colors), std::end(cells[indexes[i]].Colors), std::begin(mainColors[1])) && GameLevel > 2 ? a = 2 : a = a;
            std::equal(std::begin(cells[indexes[i]].Colors), std::end(cells[indexes[i]].Colors), std::begin(mainColors[2])) && GameLevel > 2 ? a = 0 : a = a;

            if (cells[indexes[i]].Up != nullptr &&
                    !cells[indexes[i]].Up->IsVizit &&
                    !cells[indexes[i]].Up->isAdded &&
                    cells[indexes[i]].Up->Colors[0] == mainColors[a][0] &&
                    cells[indexes[i]].Up->Colors[1] == mainColors[a][1] &&
                    cells[indexes[i]].Up->Colors[2] == mainColors[a][2])
            {
                cells[indexes[i]].Up->IsVizit = true;
                cells[indexes[i]].Up->isAdded = true;
                indexes.push_back(cells[indexes[i]].Up->Id);
            }
            if (cells[indexes[i]].Left != nullptr &&
                    !cells[indexes[i]].Left->IsVizit &&
                    !cells[indexes[i]].Left->isAdded &&
                    cells[indexes[i]].Left->Colors[0] == mainColors[a][0] &&
                    cells[indexes[i]].Left->Colors[1] == mainColors[a][1] &&
                    cells[indexes[i]].Left->Colors[2] == mainColors[a][2])
            {
                cells[indexes[i]].Left->IsVizit = true;
                cells[indexes[i]].Left->isAdded = true;
                indexes.push_back(cells[indexes[i]].Left->Id);
            }
            if (cells[indexes[i]].Right != nullptr &&
                    !cells[indexes[i]].Right->IsVizit &&
                    !cells[indexes[i]].Right->isAdded &&
                    cells[indexes[i]].Right->Colors[0] == mainColors[a][0] &&
                    cells[indexes[i]].Right->Colors[1] == mainColors[a][1] &&
                    cells[indexes[i]].Right->Colors[2] == mainColors[a][2])
            {
                cells[indexes[i]].Right->IsVizit = true;
                cells[indexes[i]].Right->isAdded = true;
                indexes.push_back(cells[indexes[i]].Right->Id);
            }
            if (cells[indexes[i]].Down != nullptr &&
                    !cells[indexes[i]].Down->IsVizit &&
                    !cells[indexes[i]].Down->isAdded &&
                    cells[indexes[i]].Down->Colors[0] == mainColors[a][0] &&
                    cells[indexes[i]].Down->Colors[1] == mainColors[a][1] &&
                    cells[indexes[i]].Down->Colors[2] == mainColors[a][2])
            {
                cells[indexes[i]].Down->IsVizit = true;
                cells[indexes[i]].Down->isAdded = true;
                indexes.push_back(cells[indexes[i]].Down->Id);
            }
        }
    }
    else if (event->buttons() == Qt::LeftButton && GameLevel == 0)
    {
        if (xy.x() > (btn.vert[0] * width) + width &&
                xy.x() < (btn.vert[2] * width) + width &&
                xy.y() >(-btn.vert[5] * height) + height &&
                xy.y() < (-btn.vert[1] * height) + height)
            btn.IsDown = true;
        else if (xy.x() > (btn1.vert[0] * width) + width &&
                 xy.x() < (btn1.vert[2] * width) + width &&
                 xy.y() >(-btn1.vert[5] * height) + height &&
                 xy.y() < (-btn1.vert[1] * height) + height)
            btn1.IsDown = true;
        else if (xy.x() > (btn2.vert[0] * width) + width &&
                 xy.x() < (btn2.vert[2] * width) + width &&
                 xy.y() >(-btn2.vert[5] * height) + height &&
                 xy.y() < (-btn2.vert[1] * height) + height)
            btn2.IsDown = true;
    }
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    auto xy = event->position();

    int width = this->width()/2;
    int height = this->height()/2;

    if (event->button() == Qt::LeftButton && GameLevel == 0)
    {
        if (btn.IsDown &&
                xy.x() > (btn.vert[0] * width) + width &&
                xy.x() < (btn.vert[2] * width) + width &&
                xy.y() >(-btn.vert[5] * height) + height &&
                xy.y() < (-btn.vert[1] * height) + height)
        {
            this->GameLevel = 1;
            control.Add(0 - ((GameLevel * 40.0f / 2.0f) / width), -1.0f + (10.0f / height), GameLevel);
            Game_New(this->GameLevel);
        }
        else if (btn1.IsDown &&
                 xy.x() > (btn1.vert[0] * width) + width &&
                 xy.x() < (btn1.vert[2] * width) + width &&
                 xy.y() >(-btn1.vert[5] * height) + height &&
                 xy.y() < (-btn1.vert[1] * height) + height)
        {
            this->GameLevel = 2;
            control.Add(0 - ((GameLevel * 40.0f / 2.0f) / width), -1.0f + (10.0f / height), GameLevel);
            Game_New(this->GameLevel);
        }
        else if (btn2.IsDown &&
                 xy.x() > (btn2.vert[0] * width) + width &&
                 xy.x() < (btn2.vert[2] * width) + width &&
                 xy.y() >(-btn2.vert[5] * height) + height &&
                 xy.y() < (-btn2.vert[1] * height) + height)
        {
            this->GameLevel = 3;
            control.Add(0 - ((GameLevel * 40.0f / 2.0f) / width), -1.0f + (10.0f / height), GameLevel);
            Game_New(this->GameLevel);
        }
    }
    else if (!Anim && GameLevel > 0)
    {
        if (xy.x() <= 30 || xy.x() >= 770 || xy.y() <= (double)705 - ((height - 120) / mapH) || xy.y() >= 705)
        {
            double x = 0 - ((GameLevel * 40.0 / 2.0) / width);
            double y = -1.0 + (10.0 / height);

            if (xy.x() > x * width + width && xy.x() < (x + 0.1 * GameLevel) * width + width
                    && xy.y() < -y * height + height && xy.y() > (-y - 0.1) * height + height)
            {
                int lenght = (x * width + width) - xy.x();
                int f = -lenght / 40;

                control.EditColor(f, AllColrors[colorNumb][0], AllColrors[colorNumb][1], AllColrors[colorNumb][2]);

                for (int i = 0; i < 3; ++i)
                {
                    mainColors[f][i] = AllColrors[colorNumb][i];
                }
            }
        }
    }

    btn.IsDown = false;
    btn1.IsDown = false;
    btn2.IsDown = false;
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (GameLevel == 0)
    {
        auto xy = event->pos();

        int width = this->width()/2;
        int height = this->height()/2;

        if (xy.x() > (btn.vert[0] * width) + width &&
                xy.x() < (btn.vert[2] * width) + width &&
                xy.y() >(-btn.vert[5] * height) + height &&
                xy.y() < (-btn.vert[1] * height) + height)
            btn.IsHover = true;
        else if (xy.x() > (btn1.vert[0] * width) + width &&
                 xy.x() < (btn1.vert[2] * width) + width &&
                 xy.y() >(-btn1.vert[5] * height) + height &&
                 xy.y() < (-btn1.vert[1] * height) + height)
            btn1.IsHover = true;
        else if (xy.x() > (btn2.vert[0] * width) + width &&
                 xy.x() < (btn2.vert[2] * width) + width &&
                 xy.y() >(-btn2.vert[5] * height) + height &&
                 xy.y() < (-btn2.vert[1] * height) + height)
            btn2.IsHover = true;
        else
        {
            btn2.IsHover = false;
            btn1.IsHover = false;
            btn.IsHover = false;
        }
    }
};

