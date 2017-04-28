#version 440
in fData
{
    vec3 normal;
}frag;
out vec4 fColor;
uniform sampler2D tex;


void main()
{
    //fColor = vec4(frag.normal,1.0f);
    fColor = texture(tex,vec2(gl_FragCoord.xy/1024));
}
