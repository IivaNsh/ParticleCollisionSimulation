#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Vector2.hpp"

class Rectangle{
public:
    Vector2f m_position;
    Vector2f m_dimentions;
    float left, right, top, bottom;

    Rectangle(Vector2f position, Vector2f dimentions): m_position(position), m_dimentions(dimentions){
        left = m_position.x - m_dimentions.x / 2.0f;
        right = m_position.x + m_dimentions.x / 2.0f;
        top = m_position.y + m_dimentions.y / 2.0f;
        bottom = m_position.y - m_dimentions.y / 2.0f;
    }
};

#endif