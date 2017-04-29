#version 440

in fData
{
    vec3 point1,point2,point3;
}frag;
in  vec4 interpolatedNormal;;

layout (location=0) out vec4 vertex1;
layout (location=1) out vec4 vertex2;
layout (location=2) out vec4 vertex3;
layout (location=3) out vec4 normal;

void main()
{


    vec3 temp = frag.point1;
   vertex1 = vec4(float(temp.x),float(temp.y),float(temp.z),1.0);

   temp = (frag.point1)-(frag.point2);
   vertex2 = vec4(float(temp.x),float(temp.y),float(temp.z),1.0);

   temp = (frag.point1)-(frag.point3);
   vertex3 = vec4(float(temp.x),float(temp.y),float(temp.z),1.0);

   normal =interpolatedNormal ;
}
