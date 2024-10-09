#include "shader.h"
#include <stdio.h>

static char* readFromFile(const char* path) {
    FILE* file = fopen(path,"r");
    if (!file) {
        fprintf(stderr, "cannot read file\n");
        exit(1);
    }
    fseek(file,0,SEEK_END);
    int len = ftell(file);
    fseek(file,0,SEEK_SET);
    char* buffer = (char*) malloc(len+1);
    buffer[len] = '\0';
    if(!buffer) {
        fclose(file);
        fprintf(stderr, "Shader file is empty\n");
        exit(1);
    }
    fread(buffer,1,len,file);
    fclose(file);

    return buffer;
}

static int compileShader(const char* shaderSource,GLenum type) {
    int shader = glCreateShader(type);
    glShaderSource(shader,1,&shaderSource,NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if(!success) {
        glGetShaderInfoLog(shader,512,NULL,infoLog);
        fprintf(stderr,"Shader compile error:\n%s\n",infoLog);
        exit(1);
    }
    return shader;
}

Shader* shader_create(const char* vs_path, const char* fs_path) {
    Shader* self = malloc(sizeof(Shader));

    char* v = readFromFile(vs_path);
    char* f = readFromFile(fs_path);

    if (!v || !f) {
        fprintf(stderr,"failed to load shaders\n");
        exit(1);
    }
    unsigned int vertex = compileShader(v,GL_VERTEX_SHADER);
    unsigned int fragment = compileShader(f,GL_FRAGMENT_SHADER);
    free(v); free(f);

    self->handle = glCreateProgram();
    glAttachShader(self->handle,vertex);
    glAttachShader(self->handle,fragment);

    glLinkProgram(self->handle);

    int success;
    char infoLog[512];
    glGetProgramiv(self->handle,GL_LINK_STATUS,&success);
    if(!success) {
        glGetProgramInfoLog(self->handle,512,NULL,infoLog);
        fprintf(stderr,"Failed to link shader program:\n%s\n",infoLog);
        exit(1);
    }
    glDeleteShader(vertex); glDeleteShader(fragment);

    return self;
}
void shader_use(Shader *self) {
    glUseProgram(self->handle);
}
void shader_destroy(Shader *self) {
    glDeleteProgram(self->handle);
}
void shader_setBool(Shader* self,const char* name,int value) {
    glUniform1i(glGetUniformLocation(self->handle,name),value);
}
void shader_setInt(Shader* self,const char* name,int value) {
    glUniform1i(glGetUniformLocation(self->handle,name),value);
}
void shader_setFloat(Shader* self,const char* name,float value) {
    glUniform1i(glGetUniformLocation(self->handle,name),value);
}

void shader_setvec4(Shader* self, const char* name,vec4 color) {
    glUniform4f(glGetUniformLocation(self->handle,name),color[0],color[1],color[2],color[3]);
}

void shader_setMat4(Shader* self,const char* name,mat4 mat) {
    glUniformMatrix4fv(glGetUniformLocation(self->handle,name),1,GL_FALSE,(float *) mat);
}
