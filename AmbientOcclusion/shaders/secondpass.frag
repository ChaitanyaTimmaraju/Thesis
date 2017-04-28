#version 440
#define highp

in vec3 f_eyeSpacePositions;
out vec4 color;

const bool DEBUG = true;
const int sampleSize=25;

uniform sampler2D vertex1Sampler;
uniform sampler2D vertex2Sampler;
uniform sampler2D vertex3Sampler;
uniform sampler2D normalSampler;
uniform int screenWidth;
uniform int screenHeight;
uniform vec3 fRandom_Vectors[sampleSize];
uniform mat4 worldToView;


bool rayTriangleIntersection(vec3 rayDirection,vec2 texCoordsOfTriangle)
{
    //Specifying vertices in counter clock-wise direction
    vec3 vertex1 = texture(vertex1Sampler,texCoordsOfTriangle).xyz;
    vec3 vertex2 = texture(vertex2Sampler,texCoordsOfTriangle).xyz;
    vec3 vertex3 = texture(vertex3Sampler,texCoordsOfTriangle).xyz;
    vertex1 = vertex1*2.0 -1.0;
    vertex2 = vertex2*2.0 -1.0;
    vertex3 = vertex3*2.0 -1.0;

    //inside-out test
    vec3 normal = texture(normalSampler,texCoordsOfTriangle).xyz;
    float point1 = dot(normalize(cross(vertex2-vertex1,rayDirection-vertex1)),normal);
    float point2 = dot(normalize(cross(vertex3-vertex2,rayDirection-vertex2)),normal);
    float point3 = dot(normalize(cross(vertex1-vertex3,rayDirection-vertex3)),normal);

    if(point1<-1.0||point2<-1.0||point3<-1.0)
        return false;

    return true;
}


void debug()
{
    vec2 textureCoords;
   // textureCoords.x = gl_FragCoord.x/screenWidth;
  // textureCoords.y = gl_FragCoord.y/screenHeight;
   vec4 samplePosition = worldToView *vec4( f_eyeSpacePositions,1.0);
   textureCoords = samplePosition.xy/samplePosition.w;
   textureCoords = (textureCoords + 1.0)/2.0;
    //For finding normal
    vec3 vertex1 = texture(vertex1Sampler,textureCoords).xyz*2.0-1.0;
    vec3 vertex2 = texture(vertex2Sampler,textureCoords).xyz*2.0-1.0;
    vec3 vertex3 = texture(vertex3Sampler,textureCoords).xyz*2.0-1.0;

    if(rayTriangleIntersection(f_eyeSpacePositions.xyz,textureCoords))
          color.xyz = texture(normalSampler,textureCoords).xyz;
    else
        color.xyz = vec3(0.4,0.2,0.3);
    color.w = 1.0;


}







void main(void)
{
    if(DEBUG)
    {

        debug();


    }else
    {
        vec4 randomVectorDirection;
        vec2 textureCoords;
        float ambientOcclusionFactor=0.0f;
        const float MAX_STEP_SIZE = 5.0;

        for(int currentSample=0;currentSample<sampleSize;currentSample++)
         {
            randomVectorDirection.xyz = f_eyeSpacePositions.xyz+fRandom_Vectors[currentSample];
            randomVectorDirection.w = 1.0;

            for(float stepSize=1.0; stepSize<MAX_STEP_SIZE; stepSize++)
            {
                //project them to find the texture coords
                vec4 samplePosition = worldToView * randomVectorDirection*stepSize;
                textureCoords = samplePosition.xy/samplePosition.w;
                textureCoords = (textureCoords + 1.0)/2.0;
                if(rayTriangleIntersection(randomVectorDirection.xyz,textureCoords))
                {
                   ambientOcclusionFactor+= 1.0;
                   break;
                }
            }
         }
             ambientOcclusionFactor /=sampleSize;
             color= vec4(ambientOcclusionFactor);
             color.w=1.0;
    }

}
