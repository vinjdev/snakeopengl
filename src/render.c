#include "render.h"
#include "shader.h"
#include <cglm/affine-pre.h>

static float vertices[] = {
    -0.5f,-0.5f,
     0.5f,-0.5f,
     0.5f,0.5f,
    -0.5f,0.5f,
};

static unsigned int indices[] = {
    0,1,2,
    2,3,0
};

void render_init(Render* self) {
    glGenVertexArrays(1,&self->vao);   
    glGenBuffers(1,&self->vbo);
    glGenBuffers(1,&self->ebo);

    glBindVertexArray(self->vao);

    glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, 2*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
void render_quad(Render* self,Shader* shader,vec2 pos, vec2 size, float rotation) {
    mat4 model; glm_mat4_identity(model);
    glm_translate(model,(vec3){pos[0],pos[1],0.0f});
    glm_rotate_z(model,glm_rad(rotation),model);
    glm_scale(model,(vec3){size[0],size[1],1.0f});

    shader_setMat4(shader,"model",model);
    glBindVertexArray(self->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}
void render_destroy(Render* self) {
    glDeleteVertexArrays(1,&self->vao);
    glDeleteBuffers(1,&self->vbo);
    glDeleteBuffers(1,&self->ebo);
}
