#version 450

precision highp float;

uniform vec3 albedo;
uniform vec2 screenSize;

out vec4 outFragColor;

in vec3 worldPos;
in vec3 vUv;

layout (binding = 0) uniform sampler2D gAlbedo;
layout (binding = 1) uniform sampler2D gSpecular;
layout (binding = 2) uniform sampler2D gNormal;

out vec4 outColor;

void main()
{
  vec2 texCoord = gl_FragCoord.xy / screenSize;// + 0.5;

  //outColor = vec4(texture(gAlbedo, texCoord).xyz, 1.0);
  //outColor = vec4(texture(gSpecular, texCoord).xyz, 1.0);
  outColor = vec4(texture(gNormal, texCoord).xyz, 1.0);
  //outColor = vec4(vec3(gl_FragCoord.z/gl_FragCoord.w), 1.0);
}