#version 440 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
out VS_OUT {
    vec3 normal;
    vec3 tangent;
} vs_out;

uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main()
{
  vec4 temp =worldToView*modelToWorld*vec4(position, 1.0);
  gl_Position =temp;

  vec4 normalDirection = transpose(inverse(modelToWorld))*vec4(normal,0.0);
  vec4 tangentDirection = transpose(inverse(modelToWorld))*vec4(tangent,0.0);
  vs_out.normal = normalize(normalDirection).xyz;
  vs_out.tangent = normalize(tangentDirection).xyz;
}
