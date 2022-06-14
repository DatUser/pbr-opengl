#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
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
#include "cube.hh"

#define WIDTH 1024
#define HEIGHT 1024

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

GLuint VBO[3];
GLuint VAO[3];//1: sphere | 2: cube | 3: quad (for screen in frag)
GLuint EBO;

GLuint gBuffer;

GLuint idTexAlbedo;
GLuint idTexSpecular;
GLuint idTexNormal;

program::program* instance;
program::program* displayShader;


void display()
{
  //Render
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();

  //---------------------------
  //Pass information to gbuffer
  //---------------------------
  glBindFramebuffer(GL_FRAMEBUFFER, gBuffer); TEST_OPENGL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);TEST_OPENGL_ERROR();
  //resize(WIDTH, HEIGHT);


  //Clear color attachments
  static const float clearColor[] = { 0.0f , 1.0f, 0.0f, 1.0f };
  glClearTexImage(idTexAlbedo, 0, GL_RGBA/*8*/, GL_FLOAT/*UNSIGNED_BYTE*/, clearColor); TEST_OPENGL_ERROR();
  glClearTexImage(idTexSpecular, 0, GL_RGBA/*8*/, GL_FLOAT/*UNSIGNED_BYTE*/, clearColor); TEST_OPENGL_ERROR();
  glClearTexImage(idTexNormal, 0, GL_RGBA/*12*/, GL_FLOAT/*UNSIGNED_BYTE*/, clearColor); TEST_OPENGL_ERROR();

  //Clear depth buffer (with 1.0 since we use GL_LESS)
  glClearDepth(1.0); TEST_OPENGL_ERROR();

  instance->use();

  //Set vertices reading order in triangle Counter-Clockwise
  glFrontFace(GL_CCW); TEST_OPENGL_ERROR();

  //Sphere draw
  glBindVertexArray(VAO[0]);TEST_OPENGL_ERROR();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);TEST_OPENGL_ERROR();

  //Cube draw
  glBindVertexArray(VAO[1]); TEST_OPENGL_ERROR();
  glDrawArrays(GL_TRIANGLES, 0, 30/*36*/); TEST_OPENGL_ERROR();

  glBindFramebuffer(GL_FRAMEBUFFER, 0); TEST_OPENGL_ERROR();
  //---------------------------
  //Pass information to gbuffer
  //---------------------------
  glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  displayShader->use();


  glBindVertexArray(VAO[2]);TEST_OPENGL_ERROR();
  glActiveTexture(GL_TEXTURE0); TEST_OPENGL_ERROR();
  glBindTexture(GL_TEXTURE_2D, idTexAlbedo); TEST_OPENGL_ERROR();
  glActiveTexture(GL_TEXTURE1); TEST_OPENGL_ERROR();
  glBindTexture(GL_TEXTURE_2D, idTexSpecular); TEST_OPENGL_ERROR();
  glActiveTexture(GL_TEXTURE2); TEST_OPENGL_ERROR();
  glBindTexture(GL_TEXTURE_2D, idTexNormal); TEST_OPENGL_ERROR();

  glDrawArrays(GL_TRIANGLES, 0, 6);TEST_OPENGL_ERROR();

  glutSwapBuffers(); TEST_OPENGL_ERROR();
}

void resize(int width, int height)
{
  glViewport(0, 0, width, height);TEST_OPENGL_ERROR();
  displayShader->set_vec2("screenSize", glm::vec2(WIDTH, HEIGHT));
}

bool initGlut(int &argc, char* argv[])
{
  glutInit(&argc, argv);TEST_OPENGL_ERROR();
  glutInitContextVersion(4, 5);TEST_OPENGL_ERROR();
  glutInitContextProfile(GLUT_CORE_PROFILE|GLUT_DEBUG);TEST_OPENGL_ERROR();
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);TEST_OPENGL_ERROR();
  glutInitWindowSize(WIDTH, HEIGHT)/*(1024, 1024)*/;TEST_OPENGL_ERROR();
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
  //Enable Depth Test
  glEnable(GL_DEPTH_TEST);TEST_OPENGL_ERROR();
  glDepthFunc(GL_LESS); TEST_OPENGL_ERROR();

  glEnable(GL_CULL_FACE); TEST_OPENGL_ERROR();
  //Enable Depth Write
  //glDepthMask(GL_TRUE); TEST_OPENGL_ERROR();
}

void init_vbo(program::program* instance)
{if (!instance) return;
  Point3 center(0, 0, 0);
  Sphere s(center, 0.3);

  auto data = s.generate_vertices(500, 500);
  vertices = data.first;
  indices = data.second;

  glGenVertexArrays(2, VAO); TEST_OPENGL_ERROR();
  glGenBuffers(2, VBO); TEST_OPENGL_ERROR();
  glGenBuffers(1, &EBO); TEST_OPENGL_ERROR();

  glBindVertexArray(VAO[0]); TEST_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); TEST_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);   TEST_OPENGL_ERROR();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); TEST_OPENGL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW); TEST_OPENGL_ERROR();

  // Vertex pos
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
      9 * sizeof(float), (void*)0); TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(0); TEST_OPENGL_ERROR();

  // Vertex normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
      (void*)(3 * sizeof(float)));TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(1); TEST_OPENGL_ERROR();

  // Vertex color
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
      (void*)(6 * sizeof(float)));TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(2); TEST_OPENGL_ERROR();

  //--------------
  //Ground
  //--------------
  Cube c(center, 2.f);
  c.generate();
  std::vector<float> vertices_cube = c.get_vertices();
  //Binding buffer for cube
  glBindVertexArray(VAO[1]); TEST_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); TEST_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, vertices_cube.size() * sizeof(float), &vertices_cube[0], GL_STATIC_DRAW); TEST_OPENGL_ERROR();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0); TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(0); TEST_OPENGL_ERROR();

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (3 * sizeof(float))); TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(1); TEST_OPENGL_ERROR();

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) (6 * sizeof(float))); TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(2); TEST_OPENGL_ERROR();

  glBindVertexArray(0);



}

//displayShader quad
void init_quad()
{
  //only needs to contain uvs
  std::vector<float> quad = {
    -1.0, 1.0, 0.0, //Up left pos
    0.0, 1.0, 0.0,  //UV
    -1.0, -1.0, 0.0, //Down left
    0.0, 0.0, 0.0,  //UV
    1.0, -1.0, 0.0, //Down right
    1.0, 0.0, 0.0,  //UV
    1.0, -1.0, 0.0, //Down right
    1.0, 0.0, 0.0,  //UV
    1.0, 1.0, 0.0,  //Up right
    1.0, 1.0, 0.0,  //UV
    -1.0, 1.0, 0.0, //Up left
    0.0, 1.0, 0.0  //UV
  };

  glGenVertexArrays(1, &VAO[2]); TEST_OPENGL_ERROR();
  glGenBuffers(1, &VBO[2]); TEST_OPENGL_ERROR();

  glBindVertexArray(VAO[2]); TEST_OPENGL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, VBO[2]); TEST_OPENGL_ERROR();
  glBufferData(GL_ARRAY_BUFFER, quad.size() * sizeof(float), quad.data(), GL_STATIC_DRAW); TEST_OPENGL_ERROR();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0); TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(0); TEST_OPENGL_ERROR();

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float))); TEST_OPENGL_ERROR();
  glEnableVertexAttribArray(1); TEST_OPENGL_ERROR();

  glBindBuffer(GL_ARRAY_BUFFER, 0); TEST_OPENGL_ERROR();
  glBindVertexArray(0); TEST_OPENGL_ERROR();
}

void init_uniform(program::program* instance)
{
  glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 8.0f);
  glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);

  glm::vec2 zNearFar(0.1f, 12.0f);
  glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);

  glm::mat4 modelView = view * model;
  glm::mat4 locToProj = proj * modelView;

  instance->set_vec2("zNearFar", zNearFar);
  instance->set_matrix4("modelView", modelView);

  GLint projection_location =
    glGetUniformLocation(instance->get_id(), "localToProjection");TEST_OPENGL_ERROR();
  glUniformMatrix4fv(projection_location, 1, GL_FALSE, &locToProj[0][0]);
  TEST_OPENGL_ERROR();
}
//@idgBuffer id framebuffer object
//@idTex id texture
//@idRBO id render buffer(for depth map)
void generate_gBuffer(GLuint* idgBuffer, GLuint* idRBO)
{
  glGenFramebuffers(1, idgBuffer); TEST_OPENGL_ERROR();
  glBindFramebuffer(GL_FRAMEBUFFER, *idgBuffer); TEST_OPENGL_ERROR();

  //Albedo buffer
  glGenTextures(1, &idTexAlbedo); TEST_OPENGL_ERROR();
  glBindTexture(GL_TEXTURE_2D, idTexAlbedo); TEST_OPENGL_ERROR();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL); TEST_OPENGL_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); TEST_OPENGL_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); TEST_OPENGL_ERROR();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, idTexAlbedo, 0); TEST_OPENGL_ERROR();

  //Pos buffer
  glGenTextures(1, &idTexSpecular); TEST_OPENGL_ERROR();
  glBindTexture(GL_TEXTURE_2D, idTexSpecular); TEST_OPENGL_ERROR();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL); TEST_OPENGL_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); TEST_OPENGL_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); TEST_OPENGL_ERROR();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, idTexSpecular, 0); TEST_OPENGL_ERROR();

  //Normal buffer
  glGenTextures(1, &idTexNormal); TEST_OPENGL_ERROR();
  glBindTexture(GL_TEXTURE_2D, idTexNormal); TEST_OPENGL_ERROR();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA12, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL); TEST_OPENGL_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); TEST_OPENGL_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); TEST_OPENGL_ERROR();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, idTexNormal, 0); TEST_OPENGL_ERROR();

  GLuint attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
  glDrawBuffers(3, attachments); TEST_OPENGL_ERROR();

  //Render buffer for depth
  glGenRenderbuffers(1, idRBO); TEST_OPENGL_ERROR();
  glBindRenderbuffer(GL_RENDERBUFFER, *idRBO); TEST_OPENGL_ERROR();
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT); TEST_OPENGL_ERROR();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *idRBO); TEST_OPENGL_ERROR();

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Incomplete gBuffer" << std::endl;

  glBindFramebuffer(GL_FRAMEBUFFER, 0); TEST_OPENGL_ERROR();
}

int main(int argc, char** argv)
{

  initGlut(argc, argv);

  GLenum err = glewInit();
  if (err != GLEW_OK)
    std::cout << glewGetErrorString(err) << std::endl;

  init();

  //std::string file_v = "../src/shaders/pbr.vert.glsl";
  //std::string file_f("../src/shaders/pbr.frag.glsl");
  //std::string file_f = "../src/shaders/color.frag.glsl";

  std::vector<std::pair<GLenum, std::string>> vShaders = {
    { GL_VERTEX_SHADER, "../src/shaders/deferred.vert.glsl" },
    //{ GL_FRAGMENT_SHADER, "../src/shaders/color.frag.glsl" }
    //{ GL_FRAGMENT_SHADER, "../src/shaders/normal.frag.glsl" }
    { GL_FRAGMENT_SHADER, "../src/shaders/deferred.frag.glsl" }
    //{ GL_FRAGMENT_SHADER, "../src/shaders/display.frag.glsl" }
  };

  //-----------------------
  //Create buffer shader
  //-----------------------
  instance = program::program::make_program(vShaders);

  instance->use();

  //Create frambuffer object


  init_vbo(instance);
  init_uniform(instance);

  GLuint idRBO;
  generate_gBuffer(&gBuffer, &idRBO);

  //-----------------------
  //Create display shader
  //-----------------------

  vShaders.clear();
  vShaders.push_back({ GL_VERTEX_SHADER, "../src/shaders/display.vert.glsl" });
  vShaders.push_back({ GL_FRAGMENT_SHADER, "../src/shaders/display.frag.glsl" });
  displayShader = program::program::make_program(vShaders);
  displayShader->use();

  init_quad();

  glutMainLoop();

  delete instance;
  delete displayShader;

  return 0;
}
