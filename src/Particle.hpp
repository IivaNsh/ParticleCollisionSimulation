#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include <linmath.h>

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>


#include "Vector2.hpp"

class Particle {
public:

    Vector2f position;
    Vector2f last_position;
    Vector2f acceleration;
    float radius;
    float color[3];

    Particle(Vector2f position, float radius): position(position), radius(radius) {}
    Particle(Vector2f position, Vector2f velocity, float radius): position(position), radius(radius) { set_velocity(velocity, 1.0); }
    Particle(Vector2f position, Vector2f velocity, float radius, float r, float g, float b): position(position), radius(radius) { set_velocity(velocity, 1.0); set_color(r,g,b); }
    
    Vector2f get_velocity() { return position - last_position; }
    void set_velocity(Vector2f velocity, float dt) { last_position = position - velocity * dt; }
    

    void set_color(float r, float g, float b){
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    void update(float dt){
        Vector2f t = position;
        position = (position * 2.0f - last_position) + acceleration * dt*dt;
        last_position = t;
    }

    void draw(){
        int num_segments = 20;
        float theta = 3.1415926 * 2 / float(num_segments);
        float tangetial_factor = std::tanf(theta); //calculate the tangential factor 

        float radial_factor = std::cosf(theta); //calculate the radial factor 

        float x = radius; //we start at angle = 0 

        float y = 0;

        glColor4f(color[0], color[1], color[2], 1.0);
        glBegin(GL_POLYGON);
        for (int i = 0; i < num_segments; i++)
        {
            glVertex2f(x + position.x, y + position.y); //output vertex 

            //calculate the tangential vector 
            //remember, the radial vector is (x, y) 
            //to get the tangential vector we flip those coordinates and negate one of them 

            float tx = -y;
            float ty = x;

            //add the tangential vector 

            x += tx * tangetial_factor;
            y += ty * tangetial_factor;

            //correct using the radial factor

            x *= radial_factor;
            y *= radial_factor;
        }
        glEnd();
    }


};

#endif
