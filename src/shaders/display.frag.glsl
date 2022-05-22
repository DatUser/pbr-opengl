#version 450

precision highp float;

uniform vec3 albedo;

out vec4 outFragColor;

in vec3 worldPos;
in vec3 vUv;

//layout (binding = 0) uniform sampler2D gAlbedo;
//layout (binding = 1) uniform sampler2D gNormal;

out vec4 outColor;

void main()
{
  vec2 texCoord = gl_FragCoord.xy / 2.0 + 0.5;

  //outColor = vec4(vec3(1.0), 1.0);
  outColor = vec4(vec3(vUv.x), 1.0);
  //outColor = vec4(vec3(texCoord.x), 1.0);
  //outColor = vec4(texture(gNormal, texCoord).xyz, 1.0);
}