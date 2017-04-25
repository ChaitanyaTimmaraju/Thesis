#version 440

in fData
{
    vec3 point1,point2,point3;
}frag;
in vec4 f_color;

layout (location=0) out vec4 vertex1;
layout (location=1) out vec4 vertex2;
layout (location=2) out vec4 vertex3;
layout (location=3) out vec4 normal;


vec3 GetNormal()
{
   vec3 a = frag.point2-frag.point1;
   vec3 b = frag.point3-frag.point1;
   return normalize(cross(a, b));
}

void main()
{

   vertex1 = vec4(frag.point1,1.0);
   vertex2 = vec4(frag.point2,1.0);
   vertex3 = vec4(frag.point3,1.0);
   normal = vec4(GetNormal(),1.0);

}
