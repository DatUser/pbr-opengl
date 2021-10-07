#version 450

precision highp float;

in vec3 in_position;
in vec3 in_normal;

uniform mat4 localToProjection;

out vec3 worldPos;//position of pixel in world
out vec3 vviewDir;
out vec3 vNormalWS;

void
main()
{
  vec3 cameraPos = vec3(0.0, 0.0, 4.0);
  vec3 modelPos = vec3(0.0, 0.0, 0.0);

  vec4 positionLocal = vec4(in_position, 1.0);
  gl_Position = localToProjection * positionLocal;

  worldPos = in_position.xyz;
  vNormalWS = normalize(in_normal);//normalize(worldPos - modelPos);
  vviewDir = normalize(cameraPos - in_position.xyz);
}
