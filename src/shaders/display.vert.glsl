#version 450

precision highp float;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec3 in_color;

uniform mat4 localToProjection;

out vec3 worldPos;//position of pixel in world
out vec3 vviewDir;
out vec3 vNormalWS;
out vec3 color;

void main()
{
  gl_Position = vec4(in_position, 1.0);
}
