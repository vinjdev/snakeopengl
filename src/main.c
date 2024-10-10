#include "../lib/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "shader.h"
#include "render.h"
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/vec2.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRID_SIZE 40
#define MAX_SNAKE_LENGTH SCREEN_WIDTH/GRID_SIZE * SCREEN_HEIGHT / GRID_SIZE
#define UPDATE_INTERVAL 0.15f

typedef struct {
    vec2 pos;
} Point;

Point snake[MAX_SNAKE_LENGTH];
Point food;
vec2 direction = {1,0};
int snake_length = 2;
int gameOver = 0;

static void framebuffer_size_callback(GLFWwindow* window,int width,int height) {
    (void) window;
    glViewport(0,0,width,height);
}

static void key_callback(GLFWwindow* window, int key,int scancode,int action,int mods) {
    (void) mods;
    (void) scancode;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window,1);
    }
    switch (key) {
        case GLFW_KEY_UP: 
        if (action == GLFW_PRESS && direction[1] != -1) glm_vec2_copy((vec2) {0,1},direction);
        break;
        case GLFW_KEY_DOWN: 
        if (action == GLFW_PRESS && direction[1] != 1) glm_vec2_copy((vec2) {0,-1},direction);
        break;
        case GLFW_KEY_LEFT: 
        if (action == GLFW_PRESS && direction[0] != 1) glm_vec2_copy((vec2) {-1,0},direction);
        break;
        case GLFW_KEY_RIGHT: 
        if (action == GLFW_PRESS && direction[0] != -1) glm_vec2_copy((vec2) {1,0},direction);
        break;
        case GLFW_KEY_SPACE:
        if (action == GLFW_PRESS) {
            gameOver = 0; 
            printf("Game not over..\n");
        }
    default: break;
    }
}

void draw_border(Render* render, Shader* shader) {
    shader_setvec4(shader, "uColor", (vec4) {1.0f, 1.0f, 1.0f, 0.3f});
    
    // Draw vertical grid lines
    for (int x = 0; x <= SCREEN_WIDTH; x += GRID_SIZE) {
        render_quad(render, shader, (vec2) {x, 0}, (vec2) {1.0f, SCREEN_HEIGHT}, 0.0f); // Vertical line (1-pixel wide)
    }

    // Draw horizontal grid lines
    for (int y = 0; y <= SCREEN_HEIGHT; y += GRID_SIZE) {
        render_quad(render, shader, (vec2) {0, y}, (vec2) {SCREEN_WIDTH, 1.0f}, 0.0f);  // Horizontal line (1-pixel tall)
    }
}

void generate_food() {
    food.pos[0] = (rand() % (SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE;
    food.pos[1] = (rand() % (SCREEN_HEIGHT / GRID_SIZE)) * GRID_SIZE;
}

void init_snake() {
    snake[0].pos[0] = (SCREEN_WIDTH / GRID_SIZE / 2) * GRID_SIZE;
    snake[0].pos[1] = (SCREEN_HEIGHT / GRID_SIZE / 2) * GRID_SIZE;

    snake[1].pos[0] = snake[0].pos[0] - GRID_SIZE;
    snake[1].pos[1] = snake[0].pos[1];
    
}

void update_snake() {
    if (!gameOver) {
        if (snake[0].pos[0] == food.pos[0] && snake[0].pos[1] == food.pos[1]) {
            if (snake_length < MAX_SNAKE_LENGTH) {
                snake_length++;
            }
            generate_food();
            for (int i = 0; i < snake_length; i++) {
                if (food.pos[0] == snake[i].pos[0] && food.pos[1] == snake[i].pos[1]) {
                    generate_food();
                }
            }
        }
        for (int i = snake_length - 1; i > 0; i--) {
            glm_vec2_copy(snake[i-1].pos,snake[i].pos);
        }
        snake[0].pos[0] += direction[0] * GRID_SIZE;
        snake[0].pos[1] += direction[1] * GRID_SIZE;
        
        //collision
        if (snake[0].pos[0] > SCREEN_WIDTH)  gameOver = 1; 
        if (snake[0].pos[0] < 0)             gameOver = 1;
        if (snake[0].pos[1] > SCREEN_HEIGHT) gameOver = 1;
        if (snake[0].pos[1] < 0)             gameOver = 1;

        for (int i = 1; i < snake_length - 1; i++) {
            if (snake[0].pos[0] == snake[i].pos[0] && snake[0].pos[1] == snake[i].pos[1]) {
                gameOver = 1;
            }
        }
        if (gameOver) {
            printf("game over...\n");
        }
    }
     
}

void render_food(Render* render, Shader* shader) {
    vec2 size = {GRID_SIZE-1, GRID_SIZE-1};
    vec2 offset = {1.0f,1.0f};
    vec2 pos = {food.pos[0]+offset[0],food.pos[1]+offset[1]};
    shader_setvec4(shader,"uColor",(vec4) {1.0f,0.0f,0.0f,1.0f});
    render_quad(render,shader,pos, size, 0.0f);
}

void render_snake(Render* render, Shader* shader) {
    vec2 size = {GRID_SIZE-1,GRID_SIZE-1};
    vec2 offset = {1.0f,1.0f};

    vec2 posHead = {snake[0].pos[0] + offset[0], snake[0].pos[1] + offset[1]};

    shader_setvec4(shader,"uColor",(vec4) {0.0f,1.0f,0.0f,1.0f});
    render_quad(render,shader,posHead,size,0.0f);

    for(int i = 1; i < snake_length; i++) {
        shader_setvec4(shader,"uColor",(vec4) {0.0f,1.0f-i*0.01f,0.0f,1.0f});
        vec2 pos = {snake[i].pos[0] + offset[0], snake[i].pos[1] + offset[1]};
        render_quad(render,shader,pos,size,0.0f);
    }
    
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
    glfwSetKeyCallback(window, key_callback);
    glfwSwapInterval(1);

    Shader* shader = shader_create("res/vertex.glsl","res/fragment.glsl");
    Render render;
    render_init(&render);

    shader_use(shader);
    mat4 proj,view; 
    glm_mat4_identity(proj);
    glm_mat4_identity(view);
    glm_ortho(0,SCREEN_WIDTH,0,SCREEN_HEIGHT,0.0f,1.0f,proj);
    shader_setMat4(shader,"proj",proj);
    shader_setMat4(shader,"view",view);

    generate_food();
    init_snake();

    double last_frame_time = glfwGetTime();
    double last_update_time = 0.0f;
    double dt = 0.0f;
    double FPS = 0.0f;

    while(!glfwWindowShouldClose(window)) {
        
        
        double current_time = glfwGetTime();
        dt = current_time - last_frame_time;
        last_frame_time = current_time;

        if (current_time - last_update_time >= UPDATE_INTERVAL) {
            update_snake();
            last_update_time = current_time;
        }
         
        FPS = 1.0f / dt; 

        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_use(shader);
        draw_border(&render,shader);
        
        render_food(&render,shader);
        render_snake(&render,shader);


        glfwSwapBuffers(window);
        glfwPollEvents();
        if (!gameOver) {
            printf("delta time: %.4f\tcurrent time: %.2f\tlast time: %.2f\n",dt*1000,current_time,last_update_time);
            printf("FPS: %.2f\n",FPS);
        }
    }
    
    shader_destroy(shader);
    render_destroy(&render);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
