#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <memory>
#include <algorithm>
#include <random>

#include "Particle.hpp"
#include "Rectangle.hpp"
#include "GridHash.hpp"
#include <iostream>

class Solver{
private:
    std::unique_ptr<GridHash> grid_hash;
    
    int substeps = 16;

public:

    Solver(int N){
        grid_hash = std::make_unique<GridHash>(2.0f, 2.0f);
    }

    void add_particle(Particle particle){
        grid_hash->add_particle(particle);
    }
    
    void particle_vs_particle(int i, int j){
        if (i == j) return;
        
        vec2 distance_vector;
        vec2_sub(distance_vector, grid_hash->particles[j].position, grid_hash->particles[i].position);
        float distance = vec2_len(distance_vector);
        float overlap_distance = distance - grid_hash->particles[i].radius - grid_hash->particles[j].radius;

        if( overlap_distance < 0.0 ){
            vec2 normal;
            vec2_norm(normal, distance_vector);

            vec2_scale(normal, normal, 0.5 * overlap_distance);
            vec2_add(grid_hash->particles[i].position, grid_hash->particles[i].position, normal);
            vec2_sub(grid_hash->particles[j].position, grid_hash->particles[j].position, normal);
            // grid_hash->particles[i].position += normal * 0.25 * overlap_distance;
            // grid_hash->particles[j].position -= normal * 0.25 * overlap_distance;
        }
    }

    void circle_collision(){
        for(int ci = 0; ci < grid_hash->NW; ci++){
            for(int cj = 0; cj < grid_hash->NH; cj++){
                cell_vs_cell(ci,cj, ci,cj);
                cell_vs_cell(ci,cj, ci-1,cj);
                cell_vs_cell(ci,cj, ci+1,cj);
                cell_vs_cell(ci,cj, ci,cj-1);
                cell_vs_cell(ci,cj, ci,cj+1);
                cell_vs_cell(ci,cj, ci-1,cj-1);
                cell_vs_cell(ci,cj, ci+1,cj+1);
                cell_vs_cell(ci,cj, ci+1,cj-1);
                cell_vs_cell(ci,cj, ci-1,cj+1);
            }
        }


        // for(int i = 0; i < grid_hash->particles.size(); i++){
        //     for(int j = 0; j < grid_hash->particles.size(); j++){

        //         particle_vs_particle(i,j);
        //     }
        // }
    }

    void cell_vs_cell(int ci1, int cj1, int ci2, int cj2){
        for(int i : grid_hash->get(ci1, cj1)){
            for(int j : grid_hash->get(ci2, cj2)){
                // std::cout<<"cvc collisions\n";
                particle_vs_particle(i,j);
            }
        }
    }

    void circle_bound_box_collision(Rectangle rectangle, float dt){
        float damping = 0.1f;
        for(int i = 0; i < grid_hash->particles.size(); i++){
            vec2 vel;
            grid_hash->particles[i].get_velocity(vel);
            vec2 a = {-vel[0] * damping, vel[1]};
            vec2 b = {vel[0], -vel[1] * damping};
            if(grid_hash->particles[i].position[0] - grid_hash->particles[i].radius < rectangle.left){
                grid_hash->particles[i].position[0] = rectangle.left + grid_hash->particles[i].radius;
                grid_hash->particles[i].set_velocity(a, dt);
            }
            else if(grid_hash->particles[i].position[0] + grid_hash->particles[i].radius > rectangle.right){
                grid_hash->particles[i].position[0] = rectangle.right - grid_hash->particles[i].radius;
                grid_hash->particles[i].set_velocity(a, dt);
            }
            if(grid_hash->particles[i].position[1] - grid_hash->particles[i].radius < rectangle.bottom){
                grid_hash->particles[i].position[1] = rectangle.bottom + grid_hash->particles[i].radius;
                grid_hash->particles[i].set_velocity(b, dt);
            }
            else if(grid_hash->particles[i].position[1] + grid_hash->particles[i].radius > rectangle.top){
                grid_hash->particles[i].position[1] = rectangle.top - grid_hash->particles[i].radius;
                grid_hash->particles[i].set_velocity(b, dt);
            }
        }
    }

    void apply_gravity(){
        for(int i = 0; i < grid_hash->particles.size(); i++){
            vec2 acc = {0,-1};
            vec2_dup(grid_hash->particles[i].acceleration, acc);
        }
    }

    void update(float dt){

        for(int k = 0; k < substeps; k++){

            float sub_dt = dt / float(substeps);

            grid_hash->update();

            circle_collision();

            apply_gravity();

            for(int i = 0; i < grid_hash->particles.size(); i++){
                grid_hash->particles[i].update(sub_dt);
            }

            vec2 p = {0,0}; vec2 dim = {2,2};
            circle_bound_box_collision(Rectangle(p, dim), 1.0f);
        }
    }

    void draw(){
        // std::vector<Particle> bunch;
        // std::sample(grid_hash->particles.begin(),grid_hash->particles.end(), std::back_inserter(bunch), 1000, std::mt19937 {std::random_device{}()});
        for(int i = 0; i < grid_hash->particles.size(); i++){
            grid_hash->particles[i].draw();
        }

        grid_hash->draw();
    }

    // std::vector<Particle>* get_particles_prt() { return &m_particles; }

};

#endif
