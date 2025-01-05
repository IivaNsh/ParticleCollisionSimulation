#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <memory>


#include "Particle.hpp"
#include "Rectangle.hpp"
#include "GridHash.hpp"
#include <iostream>

class Solver{
private:
    std::unique_ptr<GridHash> grid_hash;
    
    int substeps = 40;

public:

    Solver(int N){
        grid_hash = std::make_unique<GridHash>(2.0f, 2.0f);
    }

    void add_particle(Particle particle){
        grid_hash->add_particle(particle);
    }

    void circle_collision(){
        // for(int x = 0; x < 10; x++){
        //     for(int y = 0; y < 10; y++){

        //         for(int i : grid_hash->query_xy(x,y)){
        //             for(int j : grid_hash->query_xy(x,y)){

        //                 if (i == j) continue;
                
        //                 Vector2f distance_vector = grid_hash->particles[j].position - grid_hash->particles[i].position;
        //                 float overlap_distance = distance_vector.absolute() - grid_hash->particles[i].radius - grid_hash->particles[j].radius;

        //                 if( overlap_distance < 0.0 ){
        //                     auto normal = distance_vector.normalized();
        //                     grid_hash->particles[i].position += normal * 0.25 * overlap_distance;
        //                     grid_hash->particles[j].position -= normal * 0.25 * overlap_distance;
        //                 }
        //             }
        //         }

        //     }    
        // }
        for(int i = 0; i < grid_hash->particles.size(); i++){
            for(int j : grid_hash->query(i)){

                if (i == j) continue;
        
                Vector2f distance_vector = grid_hash->particles[j].position - grid_hash->particles[i].position;
                float overlap_distance = distance_vector.absolute() - grid_hash->particles[i].radius - grid_hash->particles[j].radius;

                if( overlap_distance < 0.0 ){
                    auto normal = distance_vector.normalized();
                    grid_hash->particles[i].position += normal * 0.25 * overlap_distance;
                    grid_hash->particles[j].position -= normal * 0.25 * overlap_distance;
                }
            }
        }
    }

    void circle_bound_box_collision(Rectangle rectangle, float dt){
        float damping = .9f;
        for(int i = 0; i < grid_hash->particles.size(); i++){
            Vector2f vel = grid_hash->particles[i].get_velocity();
            Vector2f a = Vector2f(-vel.x * damping, vel.y);
            Vector2f b = Vector2f(vel.x, -vel.y * damping);
            if(grid_hash->particles[i].position.x - grid_hash->particles[i].radius < rectangle.left){
                grid_hash->particles[i].position = Vector2f(rectangle.left + grid_hash->particles[i].radius, grid_hash->particles[i].position.y);
                grid_hash->particles[i].set_velocity(a, dt);
            }
            else if(grid_hash->particles[i].position.x + grid_hash->particles[i].radius > rectangle.right){
                grid_hash->particles[i].position = Vector2f(rectangle.right - grid_hash->particles[i].radius, grid_hash->particles[i].position.y);
                grid_hash->particles[i].set_velocity(a, dt);
            }
            if(grid_hash->particles[i].position.y - grid_hash->particles[i].radius < rectangle.bottom){
                grid_hash->particles[i].position = Vector2f(grid_hash->particles[i].position.x, rectangle.bottom + grid_hash->particles[i].radius);
                grid_hash->particles[i].set_velocity(b, dt);
            }
            else if(grid_hash->particles[i].position.y + grid_hash->particles[i].radius > rectangle.top){
                grid_hash->particles[i].position = Vector2f(grid_hash->particles[i].position.x, rectangle.top - grid_hash->particles[i].radius);
                grid_hash->particles[i].set_velocity(b, dt);
            }
        }
    }

    void apply_gravity(){
        for(int i = 0; i < grid_hash->particles.size(); i++){
            grid_hash->particles[i].acceleration = Vector2f(0,-200);
        }
    }

    void update(float dt){

        for(int k = 0; k < substeps; k++){

            float sub_dt = dt / float(substeps);
            
            grid_hash->update();

            circle_collision();
            circle_bound_box_collision(Rectangle(Vector2f(0,0), Vector2f(2,2)), 1.0f);

            apply_gravity();
            
            for(int i = 0; i < grid_hash->particles.size(); i++){
                grid_hash->particles[i].update(sub_dt);
            }
        }
    }

    void draw(){
        for(int i = 0; i < grid_hash->particles.size(); i++){
            grid_hash->particles[i].draw();
        }
    }

    // std::vector<Particle>* get_particles_prt() { return &m_particles; }

};

#endif