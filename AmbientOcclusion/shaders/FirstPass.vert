#version 410
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec4 vColor;
uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main()
{
  vec4 temp =vec4(position, 1.0);
  gl_Position =temp;

  //vec4 lightDirection = normalize(vec4(0.0,0.0,-4.0,1.0)-temp);
  //vec4 normalDirection = normalize(vec4(normal,0.0));
  //vColor = dot(normalDirection,lightDirection)*vec4(1.0);

  vColor = vec4(normal,1.0);


}
