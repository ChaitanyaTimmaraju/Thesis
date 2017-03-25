#version 410
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main()
{
  vec4 temp =modelToWorld*vec4(position, 1.0);
  gl_Position =temp;

  vec4 normalDirection = normalize(modelToWorld*vec4(normal,0.0));

  vs_out.normal = normalize(modelToWorld*vec4(normal,0.0)).xyz;
}
