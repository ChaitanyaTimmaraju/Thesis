#version 440 core

in fData
{
    //These are eye-space positions which are passed through geometry shader.
    vec3 point1,point2,point3;
}frag;
in  vec4 interpolatedNormal;;

layout (location=0) out vec4 vertex1;
layout (location=1) out vec4 vertex2;
layout (location=2) out vec4 vertex3;
layout (location=3) out vec4 normal;

void main()
{
   vertex1 = vec4(frag.point1,1.0);
   vertex2 = vec4(frag.point2,1.0);
   vertex3 =vec4(frag.point3,1.0);
   normal =interpolatedNormal ;
}
