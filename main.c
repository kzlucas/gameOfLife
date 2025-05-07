#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grid.h"


#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

void GetMatrixSize();
void AllocateMemory();
enum SDL_AppResult InitSDL();


static const int CELL_SIZE = 14;
static const Vector2Int CANVAS_SIZE = { 1920, 1080};
static const unsigned long FPS = 12;
static int showGrid = 0;
static int stopAnimation = 0;

Uint64 currentFrameTime = 0;
Uint64 lastFrameTime = 0;
double deltaTime = 0;
double ellapsedSinceLastFrame = 0;

 /* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;



/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    GetMatrixSize();
    AllocateMemory();
    currentFrameTime = SDL_GetPerformanceCounter();
    RandomizeNodeStates();
    return InitSDL();
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    // Limit FPS
    lastFrameTime = currentFrameTime;
    currentFrameTime = SDL_GetPerformanceCounter();
    deltaTime = (double)((currentFrameTime - lastFrameTime) * 1000 / (double)SDL_GetPerformanceFrequency());
    ellapsedSinceLastFrame += deltaTime;
    if ((ellapsedSinceLastFrame / 1000) < 1.0 / FPS) {
        return SDL_APP_CONTINUE;
    }
    ellapsedSinceLastFrame = 0;

    // Init vars
    SDL_FRect rect;

    SDL_SetRenderDrawColor(renderer, 0, 78, 33, SDL_ALPHA_OPAQUE);  /* dark gray, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    // draw cells grid
    Vector2Int cursor = { 0,0 };
    while (cursor.x < CANVAS_SIZE.x)
    {
        while (cursor.y < CANVAS_SIZE.y)
        {
            Vector2Int matrixCoords = { 0, 0 };
            matrixCoords.x = cursor.x / CELL_SIZE;
            matrixCoords.y = cursor.y / CELL_SIZE;
            int nodeState = GetNode(matrixCoords);


            SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);  /* green, full alpha */
            rect.x = (float)cursor.x;
            rect.y = (float)cursor.y;
            rect.w = (float)CELL_SIZE;
            rect.h = (float)CELL_SIZE;

            if (nodeState == 1)
            {
                SDL_RenderFillRect(renderer, &rect);
            }
            else if(showGrid == 1)
            {
                SDL_RenderRect(renderer, &rect);
            }

            cursor.y += CELL_SIZE;
        }
        cursor.y = 0;
        cursor.x += CELL_SIZE;
    }

    if (stopAnimation == 0)
    {
        GetNodesNextStates();
    }

    SDL_RenderPresent(renderer);  /* put it all on the screen! */
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}



/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    // On click, stop anim
    if ((*event).type == 1025) {
        stopAnimation = stopAnimation == 0 ? 1 : 0;
    }
    // On specebar, randomize
    if ((*event).type == 769) {
        RandomizeNodeStates();
    }

    //printf("%d", (*event).type);

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}



/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    FreeMem();

    /* SDL will clean up the window/renderer for us. */
}



void GetMatrixSize()
{
    // Calc nodes matrix size
    static Vector2Int CellsMatrixSize = { 0, 0 };
    CellsMatrixSize.x = CANVAS_SIZE.x / CELL_SIZE;
    CellsMatrixSize.y = CANVAS_SIZE.y / CELL_SIZE;
    printf("Cells matrix: %d x %d\n", CellsMatrixSize.x, CellsMatrixSize.y);
    SetMatrixSize(CellsMatrixSize);
}



enum SDL_AppResult InitSDL()
{

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("renderer/gameOfLife", CANVAS_SIZE.x, CANVAS_SIZE.y, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}