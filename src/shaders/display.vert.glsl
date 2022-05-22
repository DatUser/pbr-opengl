#version 450

precision highp float;

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_uv;

uniform mat4 localToProjection;

out vec3 worldPos;//position of pixel in world
out vec3 vUv;

void main()
{
  vUv = in_uv;
  gl_Position = vec4(in_position, 1.0);
}
