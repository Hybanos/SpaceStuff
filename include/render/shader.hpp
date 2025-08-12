#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <unordered_map>

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GL/gl.h"

class Shader {
    private:
        std::string vs;
        std::string fs;

        std::vector<std::string> layout_types; 
        std::vector<int> layout_to_buffer; 
        std::vector<int> buffer_to_layout; 

        GLuint program_id;

        void compile();
        void get_layout();
    public:
        Shader(const char * vs, const char * fs); 

        void use();
        int buff_count();
        int buff_to_loc(int buff);
        int loc_to_buff(int loc);
        int loc_to_type(int loc);
        int loc_to_type_size(int loc);

        void set_mat4(std::string name, glm::mat4 &data) {
            use();
            glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &data[0][0]);
        }
        
        void set_mat3(std::string name, glm::mat3 &data) {
            use();
            glUniformMatrix3fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &data[0][0]);
        }

        void set_vec3(std::string name, glm::vec3 &data) {
            use();
            glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &data[0]);
        }

        void set_vec2(std::string name, glm::vec2 &data) {
            use();
            glUniform2fv(glGetUniformLocation(program_id, name.c_str()), 1, &data[0]);
        }

        void set_int(std::string name, int &data) {
            use();
            glUniform1i(glGetUniformLocation(program_id, name.c_str()), data);
        }
};

static std::unordered_map<std::string, int> glsl_types = {
    {"bool", GL_BOOL},
    {"int", GL_INT},
    {"uint", GL_UNSIGNED_INT},
    {"float", GL_FLOAT},
    {"double", GL_DOUBLE},

    {"bvec2", GL_BOOL},
    {"bvec3", GL_BOOL},
    {"bvec4", GL_BOOL},
    {"ivec2", GL_INT},
    {"ivec3", GL_INT},
    {"ivec4", GL_INT},
    {"uvec2", GL_UNSIGNED_INT},
    {"uvec3", GL_UNSIGNED_INT},
    {"uvec4", GL_UNSIGNED_INT},
    {"vec2", GL_FLOAT},
    {"vec3", GL_FLOAT},
    {"vec4", GL_FLOAT},
    {"dvec2", GL_DOUBLE},
    {"dvec3", GL_DOUBLE},
    {"dvec4", GL_DOUBLE},

    {"mat2", GL_FLOAT},
    {"mat2x2", GL_FLOAT},
    {"mat3",GL_FLOAT },
    {"mat3x3", GL_FLOAT},
    {"mat4", GL_FLOAT},
    {"mat4x4", GL_FLOAT},

    {"mat2x3", GL_FLOAT},
    {"mat2x4", GL_FLOAT},
    {"mat3x2", GL_FLOAT},
    {"mat3x4", GL_FLOAT},
    {"mat4x2", GL_FLOAT},
    {"mat4x3", GL_FLOAT},

    {"dmat2", GL_DOUBLE},
    {"dmat2x2", GL_DOUBLE},
    {"dmat3", GL_DOUBLE},
    {"dmat3x3", GL_DOUBLE},
    {"dmat4", GL_DOUBLE},
    {"dmat4x4", GL_DOUBLE},

    {"dmat2x3", GL_DOUBLE},
    {"dmat2x4", GL_DOUBLE},
    {"dmat3x2", GL_DOUBLE},
    {"dmat3x4", GL_DOUBLE},
    {"dmat4x2", GL_DOUBLE},
    {"dmat4x3", GL_DOUBLE},
};

static std::unordered_map<std::string, int> glsl_types_size = {
    {"bool", 1},
    {"int", 1},
    {"uint", 1},
    {"float", 1},
    {"double", 1},

    {"bvec2", 2},
    {"bvec3", 3},
    {"bvec4", 4},
    {"ivec2", 2},
    {"ivec3", 3},
    {"ivec4", 4},
    {"uvec2", 2},
    {"uvec3", 3},
    {"uvec4", 4},
    {"vec2", 2},
    {"vec3", 3},
    {"vec4", 4},
    {"dvec2", 2},
    {"dvec3", 3},
    {"dvec4", 4},

    {"mat2", 4},
    {"mat2x2", 4},
    {"mat3", 9},
    {"mat3x3", 9},
    {"mat4", 16},
    {"mat4x4", 16},

    {"mat2x3", 6},
    {"mat2x4", 8},
    {"mat3x2", 6},
    {"mat3x4", 12},
    {"mat4x2", 8},
    {"mat4x3", 12},

    {"dmat2", 4},
    {"dmat2x2", 4},
    {"dmat3", 9},
    {"dmat3x3", 9},
    {"dmat4", 16},
    {"dmat4x4", 16},

    {"dmat2x3", 6},
    {"dmat2x4", 8},
    {"dmat3x2", 6},
    {"dmat3x4", 12},
    {"dmat4x2", 8},
    {"dmat4x3", 12},
};