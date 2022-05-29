
#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <err.h>

#define TEST_OPENGL_ERROR()                                                             \
  do {                                                                                  \
    GLenum err = glGetError();                                                          \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR: "                               \
                                      << gluErrorString(err)                            \
                                      << " file " << __FILE__                           \
                                      << " line " << __LINE__ << std::endl;             \
  } while(0)
namespace program {

class program
{
  public:
    program()
    : shaders(std::vector<GLuint>{})
    {
    }

    ~program()
    {
      /*glDetachShader(id, vertex_shader);
      glDetachShader(id, fragment_shader);
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);*/

      for (GLuint shader : shaders)
      {
        glDetachShader(id, shader);
        glDeleteShader(shader);
      }
    }

    static program *make_program(std::vector<std::pair<GLenum, std::string>>& shaders_src);
      /*std::string& vertex_shader_src,
	std::string& fragment_shader_src);*/

    char* get_log();

    bool is_ready();

    void use();

    void set_int(const std::string& name, int value);
    void set_float(const std::string& name, float value);
    void set_vec2(const std::string& name, const glm::vec2& value);
    void set_vec3(const std::string& name, const glm::vec3& value);
    void set_vec4(const std::string& name, const glm::vec4& value);
    void set_matrix4(const std::string& name, const glm::mat4& value);

    GLuint get_id() const { return id; }

  private:
    //static program* instance;
    void create_program();

    //Program id
    GLuint id;

    std::vector<GLuint> shaders;
};

std::string load_from_file(std::string& file);

GLuint load_and_compile(GLenum shader_type, std::string& file);
}
