#version 450

precision highp float;

uniform vec3 albedo;

out vec4 outFragColor;

in vec3 worldPos;
in vec3 vNormalWS;
in vec3 vviewDir;

void main()
{
  vec3 normal = normalize(vNormalWS);
  //temporary
  vec3 lightPos = vec3(2.0, 4.0, 4.0);

  //incoming direction of light
  vec3 lightDir = normalize(lightPos - worldPos);
  float dotNL = dot(normal, lightDir);

  float depth = gl_FragCoord.z / 2.0;// + 0.5;

  //outFragColor = vec4(normal, 1.0);
  outFragColor = vec4(vec3(depth), 1.0);
}