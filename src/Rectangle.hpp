#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <linmath.h>
// #include "Vector2.hpp"

class Rectangle{
public:
    vec2 m_position;
    vec2 m_dimentions;
    float left, right, top, bottom;

    Rectangle(vec2 position, vec2 dimentions){
        vec2_dup(m_position,position);
        vec2_dup(m_dimentions,dimentions);
        
        left = m_position[0] - m_dimentions[0] / 2.0f;
        right = m_position[0] + m_dimentions[0] / 2.0f;
        top = m_position[1] + m_dimentions[1] / 2.0f;
        bottom = m_position[1] - m_dimentions[1] / 2.0f;
    }
};

#endif