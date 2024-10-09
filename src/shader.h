#ifndef SHADER_H
#define SHADER_H

#include "../lib/glad/include/glad/glad.h"
#include <cglm/cglm.h>

typedef struct {
    unsigned int handle;
} Shader;

Shader* shader_create(const char* vs_path, const char* fs_path);
void shader_use(Shader *self);
void shader_destroy(Shader *self);
void shader_setBool(Shader* self,const char* name,int value);
void shader_setInt(Shader* self,const char* name,int value);
void shader_setFloat(Shader* self,const char* name,float value);
void shader_setMat4(Shader* self,const char* name,mat4 mat);

#endif // !SHADER_H
