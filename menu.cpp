#include <QPainter>
#include <iostream>
#include <string>
#include "menu.h"
#include "GL/gl.h"


typedef std::string string;

void TBtn::print_string(int x, int y, int h, int w, int fontSize, const char* text, int r, int g, int b, QPaintDevice* cc)
{    
    QPainter painter(cc);
    QPen penHLines(QColor(r,g,b), 1, Qt::DotLine, Qt::FlatCap, Qt::RoundJoin);
    painter.setPen(penHLines);
    QFont font = painter.font();
    font.setPixelSize(fontSize);
    painter.setFont(font);
    const QRect rectangle = QRect(x, y, w, h);
    QRect boundingRect;
    painter.drawText(rectangle, 0, text, &boundingRect);

    QPen pen = painter.pen();
    this->textWidth = boundingRect.adjusted(0, 0, -pen.width(), -pen.width()).width();



}

void TBtn::AddButton(string name, int x, int y, int width, int height)
{
    this->Width = width;
    this->Height = height;
    this->Name = name;

    this->vert[0] = this->vert[6] = x / 400.0;
    this->vert[1] = this->vert[3] = y / 400.0;
    this->vert[2] = this->vert[4] = (x + width) / 400.0;
    this->vert[5] = this->vert[7] = (y + height) / 400.0;
}

void TBtn::ShowButton(QPaintDevice* cc)
{
    glVertexPointer(2, GL_FLOAT, 0, this->vert);
    glEnableClientState(GL_VERTEX_ARRAY);
    if (this->IsDown)
        glColor3f(0.3f, 0.3f, 0.5f);
    else if (this->IsHover)
        glColor3f(0.7f, 0.7f, 0.9f);
    else
        glColor3f(0.5f, 0.5f, 0.7f);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glScalef(0.01f, -0.01f, 1.0f);

    print_string((vert[0] * cc->width()/2 + cc->width()/2) + (Width/2 - this->textWidth/2) - 2,
                (-vert[7] * (cc->height() / 2.0 - 3)) + (cc->height() / 2.0 - 3),
                Height,
                Width,
                40,
                this->Name.c_str(),
                255,
                255,
                255,
                cc);

    glLoadIdentity();
}
