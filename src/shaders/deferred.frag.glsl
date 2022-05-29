#version 450

precision highp float;

uniform vec3 albedo;

layout (location = 0) out vec4 gAlbedo;
layout (location = 1) out vec4 gSpecular;
layout (location = 2) out vec4 gNormal;
//layout (location = 2) out vec3 gDepth;

in vec3 worldPos;
in vec3/*4*/ vNormalWS;
in vec3 vviewDir;
in vec3/*4*/ vAlbedo;
in vec3/*4*/ vSpecular;

void main()
{
  //Stores albedo/normal/depth into textures
  gAlbedo = vec4(vec3(1.0), 1.0);
  gSpecular = vec4(vSpecular, 1.0);
  gNormal = vec4(normalize(vNormalWS) * 0.5 + 0.5, 1.0);
  //gDepth = vec3(1.0);
}