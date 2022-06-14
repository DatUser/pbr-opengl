#version 450

precision highp float;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3/*4*/ in_normal;
layout (location = 2) in vec3/*4*/ in_color;

uniform mat4 modelView;
uniform mat4 localToProjection;

uniform vec2 zNearFar;

out float vDepth;
out vec3 worldPos;//position of pixel in world
out vec3/*4*/ vviewDir;
out vec3/*4*/ vNormalWS;
out vec3/*4*/ vAlbedo;
out vec3/*4*/ vSpecular;

void main()
{
  vec3 cameraPos = vec3(0.0, 0.0, 4.0);
  vec3 modelPos = vec3(0.0, 0.0, 0.0);

  vec4 viewPos = modelView * vec4(in_position, 1.0);
  gl_Position = localToProjection * viewPos;

  vDepth = (viewPos.z - zNearFar.x) / (zNearFar.x - zNearFar.y);

  worldPos = in_position.xyz;
  vAlbedo = in_color;//vec3(1.0, 0.0, 0.0);
  vNormalWS = normalize(in_normal);
  vviewDir = normalize(cameraPos - in_position.xyz);
}
