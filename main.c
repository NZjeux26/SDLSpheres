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
    const int total = 25;
    float angleX = 0;
    float angleY = 0;
    float angleZ = 0.0;
    Vector3 globe[total + 1][total + 1];
    //create SDL window
    createWindow();

    //takes in a 3d vector along with the total points wanted and the desired radius.
    for (int i = 0; i < total + 1; i++){
        float lat = map(i, 0,total, 0, M_PI);
            for (int j=0; j < total + 1; j++){
                float lon = map(j,0,total, 0, M_PI * 2);
                float x = r * sin(lat) * cos(lon);
                float y = r * sin(lat) * sin(lon);
                float z = r * cos(lat);
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
        //3d rotations, see https://en.wikipedia.org/wiki/Rotation_matrix "basic 3d rotations"
        float roX[3][3] = {{1,0,0}, {0,cos(angleX), -sin(angleX)}, {0, sin(angleX), cos(angleX)}};
        float roY[3][3] = {{cos(angleY), 0, sin(angleY)}, {0,1,0}, {-sin(angleY), 0, cos(angleY)}};
        float roZ[3][3] = {{cos(angleZ), -sin(angleZ), 0},{sin(angleZ), cos(angleZ), 0}, {0,0,1}};

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        //applies the rotation angles with a matmul with the point in the vector
        for (int i = 0; i < total; i++){
            for (int j=0; j < total + 1; j++){
                Vector3 rotated = globe[i][j];
                Vector3 rotated2 = globe[i + 1][j];
                Vector3 rotated3 = globe[i][j+1];
                Vector3 rotated4 = globe[i + 1][j + 1];
                //applies rotations angles to projection of the vector
                matmul(roY, &rotated);
                matmul(roX, &rotated);
                matmul(roZ, &rotated);

                matmul(roY, &rotated2);
                matmul(roX, &rotated2);
                matmul(roZ, &rotated2);

                matmul(roY, &rotated3);
                matmul(roX, &rotated3);
                matmul(roZ, &rotated3);

                matmul(roY, &rotated4);
                matmul(roX, &rotated4);
                matmul(roZ, &rotated4);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
               // SDL_RenderDrawPoint(renderer, rotated.x + DISPLAY_WIDTH / 2, rotated.y + DISPLAY_LENGTH / 2);
                SDL_RenderDrawLine(renderer, rotated.x + DISPLAY_WIDTH / 2, rotated.y + DISPLAY_LENGTH / 2, rotated2.x + DISPLAY_WIDTH / 2, rotated2.y + DISPLAY_LENGTH / 2);
                SDL_RenderDrawLine(renderer, rotated.x + DISPLAY_WIDTH / 2, rotated.y + DISPLAY_LENGTH / 2, rotated3.x + DISPLAY_WIDTH / 2, rotated3.y + DISPLAY_LENGTH / 2);
                SDL_RenderDrawLine(renderer, rotated2.x + DISPLAY_WIDTH / 2, rotated2.y + DISPLAY_LENGTH / 2, rotated4.x + DISPLAY_WIDTH / 2, rotated4.y + DISPLAY_LENGTH / 2);
                SDL_RenderDrawLine(renderer, rotated3.x + DISPLAY_WIDTH / 2, rotated3.y + DISPLAY_LENGTH / 2, rotated4.x + DISPLAY_WIDTH / 2, rotated4.y + DISPLAY_LENGTH / 2);  

            }
        }
        //update the angles so the projection moves
        angleX += 0.003;
        angleZ += 0.004;
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
        "3D Sphere X86",
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