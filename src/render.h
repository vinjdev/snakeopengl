#ifndef RENDER_H
#define RENDER_H

#include <cglm/cglm.h>
#include "shader.h"

typedef struct {
    unsigned int vao,vbo,ebo;
} Render;

void render_init(Render* self);
void render_quad(Render* self,Shader* shader,vec2 pos, vec2 size,float rotation);
void render_destroy(Render* self);

#endif // !RENDER_H
