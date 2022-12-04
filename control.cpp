#include "control.h"
#include <GL/gl.h>

void Control::ShowControl(QPaintDevice* cc)
{
    glLoadIdentity();
    glPushMatrix();

    for (int i = 0; i < this->count; ++i)
    {
        glBegin(GL_TRIANGLE_STRIP);

        glColor4f(cubes[i].r - 0.2f, cubes[i].g - 0.2f, cubes[i].b - 0.2f, 0.85f); glVertex2f(cubes[i].x, cubes[i].y + (float)(40.0f / (cc->height()/2)));
        glColor4f(cubes[i].r - 0.3f, cubes[i].g - 0.3f, cubes[i].b - 0.3f, 0.85f); glVertex2f(cubes[i].x + (float)(40.0f / (cc->width()/2)), cubes[i].y + (float)(40.0f / (cc->height()/2))); glVertex2f(cubes[i].x, cubes[i].y);
        glColor4f(cubes[i].r - 0.6f, cubes[i].g - 0.6f, cubes[i].b - 0.6f, 0.85f); glVertex2f(cubes[i].x + (float)(40.0f / (cc->width()/2)), cubes[i].y);

        glEnd();
    }
    glPopMatrix();
}

void Control::Add(float x, float y, int count)
{
    this->count = count;
    this->cubes = new Control[count];

    for (int i = 0; i < count; ++i)
    {
        this->cubes[i].x = x + (i / 10.0f);
        this->cubes[i].y = y;

        this->cubes[i].r = 0.9f;
        this->cubes[i].g = 0.9f;
        this->cubes[i].b = 0.9f;
    }
}

void Control::EditColor(int numb, float r, float g, float b)
{
    this->cubes[numb].r = r;
    this->cubes[numb].g = g;
    this->cubes[numb].b = b;
}

void Control::Delete()
{
    if (cubes)
        delete cubes;
}

