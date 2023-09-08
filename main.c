#include <stdio.h>
#include "include/SDL2/SDL.h"
#include "3Dfunc.h"
#include <math.h>

#define DISPLAY_WIDTH 1200
#define DISPLAY_LENGTH 800

void createWindow();
SDL_Window* window;
SDL_Renderer* renderer;

int main(int argc, char* argv[]) {
    const int r = 200;
    const int total = 50;
    float angleX = 0;
    float angleY = 0;
    float angleZ = 0.0;
    Vector3 globe[total][total];
    //create SDL window
    createWindow();

    
    for (int i = 0; i < total; i++){
        float lat = map(i, 0,total, -M_PI_2, M_PI_2);
            for (int j=0; j < total; j++){
                float lon = map(j,0,total, -M_PI, M_PI);
                float x = r * sin(lon) * cos(lat);
                float y = r * sin(lon) * sin(lat);
                float z = r * cos(lon);
                globe[i][j] = createVector3(x,y,z);
            }
    }
    //Main loop
    int is_running = 1;
  
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = 0;
            }
        }

        float roX[3][3] = {{1,0,0}, {0,cos(angleX), -sin(angleX)}, {0, sin(angleX), cos(angleX)}};
        float roY[3][3] = {{cos(angleY), 0, sin(angleY)}, {0,1,0}, {-sin(angleY), 0, cos(angleY)}};
        float roZ[3][3] = {{cos(angleZ), -sin(angleZ), 0},{sin(angleZ), cos(angleZ), 0}, {0,0,1}};

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        //calculates the lat then the long points for the vector.

        for (int i = 0; i < total; i++){
            for (int j=0; j < total; j++){
                Vector3 rotated = globe[i][j];
                //applies rotations angles to projection of the vector
                matmul(roY, &rotated);
                matmul(roX, &rotated);
                matmul(roZ, &rotated);

                //loat distance = 100.0;
                //projectPoint(&rotated, distance);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawPoint(renderer, rotated.x + DISPLAY_WIDTH / 2, rotated.y + DISPLAY_LENGTH / 2);
            }
        }
        angleX += 0.003;
        angleZ += 0.003;
        SDL_RenderPresent(renderer);
        SDL_Delay(16); //Cap the frame rate
    }
    //Clean up and quit
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void createWindow(){
     // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    // Create a window
    window = SDL_CreateWindow(
        "3D Sphere",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH, DISPLAY_LENGTH,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        SDL_Log("Failed to create ERROR: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}