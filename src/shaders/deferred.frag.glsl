#version 450

precision highp float;

uniform vec3 albedo;

layout (location = 0) out vec3 gAlbedo;
layout (location = 1) out vec3 gNormal;
//layout (location = 2) out vec3 gDepth;

in vec3 worldPos;
in vec3 vNormalWS;
in vec3 vviewDir;
in vec3 color;

void main()
{
  //Stores albedo/normal/depth into textures
  gAlbedo = color;
  gNormal = normalize(vNormalWS);
  //gDepth = vec3(1.0);
}