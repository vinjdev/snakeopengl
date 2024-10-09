#include "../lib/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "shader.h"
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

static void framebuffer_size_callback(GLFWwindow* window,int width,int height) {
    (void) window;
    glViewport(0,0,width,height);
}

int main() {

    if(!glfwInit()) {
        fprintf(stderr,"Error: starting glfw\n");
        return -1;
    } 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH,SCREEN_HEIGHT,"SNAKE",NULL,NULL);
    if (!window) {
        fprintf(stderr,"failed to open glfw window\n");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr,"Failed to load opengl functions\n");
    }
    glfwSwapInterval(1);

    Shader* shader = shader_create("res/vertex.glsl","res/fragment.glsl");
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader->handle);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    shader_destroy(shader);
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
