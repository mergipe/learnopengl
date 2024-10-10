#include "SDL_events.h"
#include "SDL_video.h"
#include <iostream>
#include <glad/glad.h>
#include <SDL.h>

const char* vertexShaderSource{"#version 330 core\n"
                               "layout (location = 0) in vec3 aPos;\n"
                               "void main ()\n"
                               "{\n"
                               "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                               "}\0"};

const char* fragmentShaderSource{"#version 330 core\n"
                                 "out vec4 FragColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                 "}\0"};

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

void checkShaderCompilation(unsigned int shader)
{
    int success{};
    char infoLog[512]{};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << "\n";
    }
}

void checkShaderProgramLinking(unsigned int shaderProgram)
{
    int success{};
    char infoLog[512]{};
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
    }
}

void setup(unsigned int& vao, unsigned int& vbo, unsigned int& ebo, unsigned int& shaderProgram)
{
    unsigned int vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkShaderCompilation(vertexShader);
    unsigned int fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    checkShaderCompilation(fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkShaderProgramLinking(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    float vertices[]{
        0.5f,  0.5f,  0.0f, // top right
        0.5f,  -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f  // top left
    };
    unsigned int indices[]{
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void run(SDL_Window* window)
{
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    unsigned int shaderProgram{glCreateProgram()};
    setup(vao, vbo, ebo, shaderProgram);
    bool exit{false};
    while (!exit) {
        processEvents(exit);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(window);
        SDL_Delay(1);
    }
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderProgram);
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
