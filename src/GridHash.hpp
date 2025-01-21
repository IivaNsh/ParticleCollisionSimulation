#ifndef GRID_HASH_H
#define GRID_HASH_H


#include <vector>
#include <unordered_map>
#include <cmath>

#include "Vector2.hpp"
#include "Particle.hpp"

class GridHash {
public:
  
  float W = 2;
  float H = 2;
  int NW = 60;
  int NH = 60;

  float cell_size_x, cell_size_y;

  std::vector<int> grid[3600];

  std::vector<Particle> particles;

  GridHash(float w, float h): W(w), H(h) {
    cell_size_x = W / float(NW);
    cell_size_y = H / float(NH);
  }

  void add_particle(Particle particle){
    particles.push_back(particle);
  }

  void clear(){
    for(int i = 0; i < NW * NH; i++){
      grid[i].clear();
    }
  }

  void update(){

    clear();

    for(int i = 0; i < particles.size(); i++){
      int ix = int(std::floor((particles[i].position[0]+1.0) / cell_size_x));
      int iy = int(std::floor((particles[i].position[1]+1.0) / cell_size_y));
      if(ix < 0) { ix = 0; }
      if(ix >= NW) { ix = NW-1; }
      if(iy < 0) { iy = 0; }
      if(iy >= NH) { iy = NH-1; }

      int id = ix + iy * NW;
      grid[id].push_back(i);
    }

  }

  std::vector<int> get(int ci, int cj){
    if(ci < 0 || ci >= NW || cj < 0 || cj >= NH) return {};

    return grid[ci + cj * NW];
  }

  std::vector<int> query(int i){
    int x = int(std::floor((particles[i].position[0]+1.0) / cell_size_x));
    int y = int(std::floor((particles[i].position[1]+1.0) / cell_size_y));
    if(x > 0) { x = 0; }
    if(x < NW) { x = NW-1; }
    if(y > 0) { y = 0; }
    if(y < NH) { y = NH-1; }

    int dx[9] = {-1,  0,  1, -1,  0,  1, -1,  0,  1};
    int dy[9] = {-1, -1, -1,  0,  0,  0, 1, 1, 1};
    
    std::vector<int> res;

    for(int j = 0; j < 9; j++){
        if(x + dx[j] >=0 && x + dx[j] < NW && y + dy[j] >= 0 && y + dy[j] < NH){
          int id = (x + dx[j]) + (y + dy[j]) * NW;
          res.insert( res.end(), grid[id].begin(), grid[id].end() );
        }
    }
    return res;
  }

  std::vector<int> query_xy(int x, int y){
    int id = x + y * NW;
    return grid[id];
  }

  void draw(){
    glColor4f(1, 1, 1, 0.2);
    glBegin(GL_LINES);
    for (int i = 0; i <= NW; i++)
    {
        glVertex2f(i * cell_size_x-1, -1);
        glVertex2f(i * cell_size_x-1, H-1);
    }
    for (int i = 0; i <= NH; i++)
    {
        glVertex2f(-1, i * cell_size_y-1);
        glVertex2f(W-1, i * cell_size_y-1);
    }
    glEnd();
  }

};





#endif