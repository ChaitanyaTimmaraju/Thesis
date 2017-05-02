
#version 440 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in VS_OUT{
 vec3 eyePosition;
 vec3  normalVector;

}gs_in[];


out fData
{
    vec3 point1,point2,point3;
}frag;
out  vec4 interpolatedNormal;

void main()
{

    frag.point1 = gs_in[0].eyePosition;
    frag.point2 = gs_in[1].eyePosition;
    frag.point3 = gs_in[2].eyePosition;

    interpolatedNormal.w = 1.0;

    for(int i=0;i<3;i++)
    {
        gl_Position = gl_in[i].gl_Position;
        interpolatedNormal.xyz = gs_in[i].normalVector;
        EmitVertex();
    }
    EndPrimitive();
}
