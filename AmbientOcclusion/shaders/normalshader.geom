
#version 440 core
layout (points) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
    vec3 tangent;
} gs_in[];

out fData
{
    vec3 normal;
}frag;

const float MAGNITUDE = 0.1f;

void GenerateLine(int index,vec3 col)
{
    /*gl_Position = gl_in[index].gl_Position;
    frag.normal = col;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0f) * MAGNITUDE;
    frag.normal = vec3(1.0,0.5,0.2);
    EmitVertex();
    EndPrimitive();
    */
}

void main()
{
    /*GenerateLine(0,vec3(1.0)); // First vertex normal
    GenerateLine(1,vec3(1.0)); // Second vertex normal
    GenerateLine(2,vec3(1.0)); // Third vertex normal
    */


    gl_Position = gl_in[0].gl_Position;
    frag.normal = vec3(0.0,0.0,1.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(gs_in[0].normal, 0.0f) * MAGNITUDE;
    frag.normal = vec3(0.0,0.0,1.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[0].gl_Position;
    frag.normal = vec3(1.0,0.0,0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(gs_in[0].tangent, 0.0f) * MAGNITUDE;
    frag.normal = vec3(1.0,0.0,0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = gl_in[0].gl_Position;
    frag.normal = vec3(0.0,1.0,0.0);
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(cross(gs_in[0].normal,gs_in[0].tangent), 0.0f) * MAGNITUDE;
    frag.normal = vec3(0.0,1.0,0.0);
    EmitVertex();
    EndPrimitive();
}
