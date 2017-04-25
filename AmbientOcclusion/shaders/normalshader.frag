#version 440
in fData
{
    vec3 normal;
}frag;
out vec4 fColor;

void main()
{
    fColor = vec4(frag.normal,1.0f);
}
