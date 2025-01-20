#include <stdio.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <array>
#include <unordered_map>
#include <utility>
#include <memory>

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <glfw3.h>

#include <linmath.h>


#define GLT_MANUAL_VIEWPORT
#define GLT_IMPLEMENTATION
#include "gltext.h"


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include "Vector2.hpp"
#include "Particle.hpp"
#include "Solver.hpp"



static void glfw_error_callback(int error, const char* description){
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

float uniform_random(float a, float b){
  return a + (b-a)*(static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX));
}



int main(){

  // init glfw
  glfwSetErrorCallback(glfw_error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);
 
  const char* glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_SAMPLES, 8 );
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    
  GLFWwindow* window;
  window = glfwCreateWindow(1000, 1000, "Particles Collision Simulation", NULL, NULL);
  if (!window)
  {
      glfwTerminate();
      exit(EXIT_FAILURE);
  }
  
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);

  glfwSwapInterval(0);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();


  // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
  #ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
  #endif
    ImGui_ImplOpenGL3_Init(glsl_version);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // Initialize glText
  gltInit();

  




  // set seed
  std::srand(std::time(nullptr));

  float last_time = glfwGetTime();

  int N = 5000;
  
  Solver solver(N);
  int count = 0;
  // for(int i = 0; i < 100; i++){
  //   solver.add_particle(Particle(Vector2f(uniform_random(-1,1), uniform_random(-1,1)), Vector2f(uniform_random(-0.01,0.01), uniform_random(-0.01,0.01)), 0.02, 1.0, 1.0, 1.0));
  // }
  // imgui window state
  // bool show_demo_window = true;
  // bool show_another_window = false;
  // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  float delay = 0.05;
  float timer = 0;

  while (!glfwWindowShouldClose(window))
  {

      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
      // ImGui::ShowDemoWindow(&show_demo_window);
      ImGui::ShowMetricsWindow();
      // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
      // {
      //     static float f = 0.0f;
      //     static int counter = 0;

      //     ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

      //     ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
      //     ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
      //     ImGui::Checkbox("Another Window", &show_another_window);

      //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
      //     ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

      //     if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
      //         counter++;
      //     ImGui::SameLine();
      //     ImGui::Text("counter = %d", counter);

      //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      //     ImGui::End();
      // }

      // // 3. Show another simple window.
      // if (show_another_window)
      // {
      //     ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
      //     ImGui::Text("Hello from another window!");
      //     if (ImGui::Button("Close Me"))
      //         show_another_window = false;
      //     ImGui::End();
      // }

      // Rendering
      ImGui::Render();



      int width, height;
      glfwGetFramebufferSize(window, &width, &height);


      // gltViewport(width, height);
      glViewport(0, 0, width, float(height)*float(height)/float(width));
      glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);






      float new_time = glfwGetTime();
      float dt = (new_time - last_time);
      // float dt = 0.005;
      last_time = new_time;

      if(glfwGetTime()-timer > delay){
        if(count < N){
          for(int i = 0; i < 1; i++){
            vec2 p = {0,0};

            vec2 v = {std::cos(glfwGetTime()),std::sin(glfwGetTime())};
            vec2_scale(v, v, 0.0*dt);

            float a = glfwGetTime()/3.0;
            vec3 col = {std::sin(a + 0.0),
                        std::sin(a - 2.0*M_PI/3.0),
                        std::sin(a + 2.0*M_PI/3.0)};
            vec3_scale(col, col, 0.5);
            {
              vec3 t = {0.5, 0.5, 0.5};
              vec3_add(col, col, t);
            }

            // float r = 0.02;
            float r = uniform_random(0.006, 0.01);

            solver.add_particle( Particle(p, v, r, col) );
            
            count++;
          }
        }
        timer = glfwGetTime();
      }

      solver.update(dt);

      solver.draw();



      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


      glfwSwapBuffers(window);
      glfwPollEvents();
  }


  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  gltTerminate();

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);

  return 0;
}
