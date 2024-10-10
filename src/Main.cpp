#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include "Shader.h"

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

void run(SDL_Window* window)
{
    Shader shader{"/home/gustavo/workspaces/learnopengl/src/shader.vert",
                  "/home/gustavo/workspaces/learnopengl/src/shader.frag"};
    float vertices[] = {
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
    };
    unsigned int vaos[1]{};
    glGenVertexArrays(1, vaos);
    unsigned int vbos[1]{};
    glGenBuffers(1, vbos);
    glBindVertexArray(vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    bool exit{false};
    while (!exit) {
        processEvents(exit);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        glBindVertexArray(vaos[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        SDL_GL_SwapWindow(window);
        SDL_Delay(1);
    }
    glDeleteVertexArrays(2, vaos);
    glDeleteBuffers(2, vbos);
}

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
    run(window);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
