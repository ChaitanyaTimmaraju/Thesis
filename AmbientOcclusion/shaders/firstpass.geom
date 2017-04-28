
#version 440 core
#define highp
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in VS_OUT{
 vec3 eyePosition;
 vec3 highp normalPosition;

}gs_in[];


out fData
{
    vec3 point1,point2,point3;
}frag;
out highp vec4 interpolatedNormal;


vec3 GetNormal()
{
   vec3 a = vec3(gs_in[1].eyePosition) - vec3(gs_in[0].eyePosition);
   vec3 b = vec3(gs_in[2].eyePosition) - vec3(gs_in[0].eyePosition);
   return normalize(cross(a, b));
}

void main()
{

    frag.point1 = gs_in[0].eyePosition;
    frag.point2 = gs_in[1].eyePosition;
    frag.point3 = gs_in[2].eyePosition;

    interpolatedNormal.w = 1.0;

    for(int i=0;i<3;i++)
    {
        gl_Position = gl_in[i].gl_Position;
        interpolatedNormal.xyz = gs_in[i].normalPosition;
        EmitVertex();
    }
    EndPrimitive();
}
