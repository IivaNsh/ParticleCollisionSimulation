#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include <linmath.h>

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>

#include <iostream>

// #include "Vector2.hpp"

class Particle {
public:

    vec2 position;
    vec2 last_position;
    vec2 acceleration;
    float radius;
    vec3 color;



    Particle(vec2 pos, float radius): radius(radius) {
        vec2_dup(position, pos);
    }
    Particle(vec2 pos, vec2 velocity, float radius): radius(radius) {
        vec2_dup(position, pos);
        set_velocity(velocity, 1.0);
    }
    Particle(vec2 pos, vec2 velocity, float radius, vec3 col): radius(radius) {
        vec2_dup(position, pos);
        set_velocity(velocity, 1.0);
        vec3_dup(color, col);
    }
    
    void get_velocity(vec2 res) { vec2_sub(res, position, last_position); }
    void set_velocity(vec2 velocity, float dt) { vec2 temp; vec2_scale(temp, velocity, dt); vec2_sub(last_position, position, temp); }
    

    void update(float dt){
        vec2 t;
        vec2_dup(t, position);

        vec2 acc;
        vec2 l_pos;
        vec2_dup(l_pos, last_position);
        vec2 pos;
        vec2_scale(pos, position, 2.0f);
        vec2_scale(acc, acceleration, dt*dt);
        vec2_sub(pos, pos, l_pos);
        vec2_add(position, pos, acc);

        vec2_dup(last_position, t);
        // last_position = t;
        // position = (position * 2.0f - last_position) + acceleration * dt*dt;
        /*last_position = (last_position + position) * 0.5f - acceleration * dt*dt;*/
    }

    void draw(){
        int num_segments = 30;
        float theta = 3.1415926 * 2 / float(num_segments);
        float tangetial_factor = std::tanf(theta); //calculate the tangential factor 

        float radial_factor = std::cosf(theta); //calculate the radial factor 

        float x = radius; //we start at angle = 0 

        float y = 0;

        glColor4f(color[0], color[1], color[2], 1.0);
        glBegin(GL_POLYGON);
        for (int i = 0; i < num_segments; i++)
        {
            glVertex2f(x + position[0], y + position[1]); //output vertex 

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
