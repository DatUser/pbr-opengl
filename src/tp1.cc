#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "program.hh"
#include "vector3.hh"
#include "sphere.hh"

#define TEST_OPENGL_ERROR()                                                             \
  do {                                                                                  \
    GLenum err = glGetError();                                                          \
    if (err != GL_NO_ERROR) std::cerr << "OpenGL ERROR: "                               \
                                      << gluErrorString(err)                            \
                                      << " file " << __FILE__                           \
                                      << " line " << __LINE__ << std::endl;             \
  } while(0)


std::vector<float> vertices;
std::vector<unsigned int> indices;

GLuint VBO;
GLuint VAO;
GLuint EBO;

void display()
{
  //glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
  //
  glBindVertexArray(VAO);TEST_OPENGL_ERROR();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);TEST_OPENGL_ERROR();
  glBindVertexArray(0);TEST_OPENGL_ERROR();

  glutSwapBuffers();
}

void resize(int width, int height)
{
  glViewport(0, 0, width, height);TEST_OPENGL_ERROR();
}

bool initGlut(int &argc, char* argv[])
{
  glutInit(&argc, argv);TEST_OPENGL_ERROR();
  glutInitContextVersion(4, 5);TEST_OPENGL_ERROR();
  glutInitContextProfile(GLUT_CORE_PROFILE|GLUT_DEBUG);TEST_OPENGL_ERROR();
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);TEST_OPENGL_ERROR();
  glutInitWindowSize(1024, 1024);TEST_OPENGL_ERROR();
  glutInitWindowPosition(10, 10);TEST_OPENGL_ERROR();
  glutCreateWindow ("Test OpenGL−POGL");TEST_OPENGL_ERROR();
  glutDisplayFunc(display);TEST_OPENGL_ERROR();
  glutReshapeFunc(resize);TEST_OPENGL_ERROR();
  return true;
}

/*bool initGlew()
{
  return (glewInit() ==GLEW_OK);
}*/

void init()
{
  glEnable(GL_DEPTH_TEST);TEST_OPENGL_ERROR();
  glDepthFunc(GL_ALWAYS);TEST_OPENGL_ERROR();

  glEnable(GL_CULL_FACE);TEST_OPENGL_ERROR();
  glCullFace(GL_FRONT);TEST_OPENGL_ERROR();
}

void init_vbo(program::program* instance)
{
  Point3 center(0, 0, 0);
  Sphere s(center, 1);

  auto data = s.generate_vertices(500, 500);
  vertices = data.first;
  indices = data.second;
  
  GLint vertex_location =
    glGetAttribLocation(instance->get_id(), "in_position");TEST_OPENGL_ERROR();
  GLint normal_location =
    glGetAttribLocation(instance->get_id(), "in_normal");TEST_OPENGL_ERROR();

  glGenVertexArrays(1, &VAO); TEST_OPENGL_ERROR();
  glGenBuffers(1, &VBO); TEST_OPENGL_ERROR();
  glGenBuffers(1, &EBO); TEST_OPENGL_ERROR();

  glBindVertexArray(VAO); TEST_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, VBO); TEST_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);   TEST_OPENGL_ERROR();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); TEST_OPENGL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
               &indices[0], GL_STATIC_DRAW); TEST_OPENGL_ERROR();

  // Vertex pos
  glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE,
      6 * sizeof(float), (void*)0); TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(vertex_location); TEST_OPENGL_ERROR();

  // Vertex normal
  glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
      (void*)(3 * sizeof(float)));TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(normal_location); TEST_OPENGL_ERROR();

  glBindVertexArray(0);

  glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f); 
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 4),
				glm::vec3(0,0,-1),
				glm::vec3(0,1,0));

  glm::mat4 locToProj = proj * view * model;

  GLint64 projection_location =
    glGetUniformLocation(instance->get_id(), "localToProjection");TEST_OPENGL_ERROR();
  glUniformMatrix4fv(projection_location, 1, GL_FALSE, &locToProj[0][0]);TEST_OPENGL_ERROR();
}

int main(int argc, char** argv)
{

  initGlut(argc, argv);

  GLenum err = glewInit();
  if (err != GLEW_OK)
    std::cout << glewGetErrorString(err) << std::endl;

  init();

  std::string file_v("../src/shaders/pbr.vert.glsl");
  std::string file_f("../src/shaders/pbr.frag.glsl");
  
  program::program* instance = program::program::make_program(file_v, file_f);
  init_vbo(instance);

  instance->use();

  glutMainLoop();

  delete instance;

  return 0;
}
