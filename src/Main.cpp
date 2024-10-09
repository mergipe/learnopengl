#include "SDL_events.h"
#include "SDL_video.h"
#include <iostream>
#include <glad/glad.h>
#include <SDL.h>

void setSDLGLAttribute(SDL_GLattr attribute, int value)
{
    if (SDL_GL_SetAttribute(attribute, value) != 0) {
        std::cerr << "Error setting OpenGL window attribute: " << SDL_GetError() << "\n";
    }
}

void processEvents(bool& exit)
{
    SDL_Event event{};
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            exit = true;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                glViewport(0, 0, event.window.data1, event.window.data2);
            }
            break;
        default:
            break;
        }
    }
}

void handleEvents() {}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << "\n";
        std::abort();
    }
    setSDLGLAttribute(SDL_GL_DOUBLEBUFFER, 1);
    setSDLGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    setSDLGLAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    setSDLGLAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_Window* const window{
        SDL_CreateWindow("LearnOpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                         SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE)};
    if (!window) {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << "\n";
        std::abort();
    }
    const SDL_GLContext context{SDL_GL_CreateContext(window)};
    if (!context) {
        std::cerr << "Error creating SDL OpenGL context: " << SDL_GetError() << "\n";
        std::abort();
    }
    if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
        std::cerr << "Failed to initialize GLAD\n";
        std::abort();
    }
    glViewport(0, 0, 800, 600);
    bool exit{false};
    while (!exit) {
        processEvents(exit);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
        SDL_Delay(1);
    }
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
