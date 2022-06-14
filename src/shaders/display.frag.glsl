#version 450

precision highp float;

uniform vec3 albedo;
uniform vec2 screenSize;

vec3 lightPos = vec3(2.0, 4.0, 4.0);

in vec3 worldPos;
in vec3 vUv;

layout (binding = 0) uniform sampler2D gAlbedo;
layout (binding = 1) uniform sampler2D gSpecular;
layout (binding = 2) uniform sampler2D gNormal;

out vec4 outColor;

void main()
{
  vec2 texCoord = gl_FragCoord.xy / screenSize;// + 0.5;

  vec3 albedo = texture(gAlbedo, texCoord).xyz;
  vec4 pos = texture(gSpecular, texCoord);
  vec3 normal = texture(gNormal, texCoord).xyz;

  vec3 lightDir = normalize(lightPos - pos.xyz);

  float depth = pos.w;
  float dotNL = dot(normal, lightDir);

  outColor = vec4(dotNL * albedo, 1.0);
  //outColor = vec4(albedo, 1.0);
  //outColor = vec4(pos.xyz, 1.0);
  //outColor = vec4(normal, 1.0);
  //outColor = vec4(vec3(gl_FragCoord.z/gl_FragCoord.w), 1.0);
  //outColor = vec4(vec3(depth), 1.0);
  //outColor = vec4(vec3(texture(gSpecular, texCoord).w), 1.0);
}