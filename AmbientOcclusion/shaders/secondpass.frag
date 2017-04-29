#version 440

in vec3 f_eyeSpacePositions;
out vec4 color;

const int sampleSize=25;
uniform float EPSILON;
uniform int textureToDisplay;
uniform sampler2D firstPassSamplers[4];
uniform int screenWidth;
uniform int screenHeight;
uniform vec3 fRandom_Vectors[sampleSize];
uniform mat4 worldToView;


bool rayTriangleIntersection(vec3 rayDirection,vec2 texCoordsOfTriangle)
{
    //Specifying vertices in counter clock-wise direction
    vec3 vertex1 = texture(firstPassSamplers[0],texCoordsOfTriangle).xyz;
    vec3 vertex2 = vertex1-texture(firstPassSamplers[1],texCoordsOfTriangle).xyz;
    vec3 vertex3 = vertex1-texture(firstPassSamplers[2],texCoordsOfTriangle).xyz;
    vec3 normal = texture(firstPassSamplers[3],texCoordsOfTriangle).xyz;
    //inside-out test

    float point1 = dot(normalize(cross(vertex2-vertex1,rayDirection-vertex1)),normal);
    float point2 = dot(normalize(cross(vertex3-vertex2,rayDirection-vertex2)),normal);
    float point3 = dot(normalize(cross(vertex1-vertex3,rayDirection-vertex3)),normal);

    if(point1<EPSILON||point2<EPSILON||point3<EPSILON)
        return false;

    return true;
}

void debug()
{
     vec2 textureCoords;
     vec4 samplePosition = worldToView *vec4( f_eyeSpacePositions,1.0);
     textureCoords = samplePosition.xy/samplePosition.w;
     textureCoords = (textureCoords + 1.0)/2.0;
     vec3 vertex1 = texture(firstPassSamplers[0],textureCoords).xyz;
     switch(textureToDisplay)
     {
      case 0: color.xyz = vertex1;break;
      case 1: color.xyz =vertex1-texture(firstPassSamplers[1],textureCoords).xyz;break;
      case 2: color.xyz = vertex1-texture(firstPassSamplers[2],textureCoords).xyz;break;
      case 3: color.xyz = texture(firstPassSamplers[textureToDisplay],textureCoords).xyz;break;
      case 4: vec3 vertex2 =vertex1-texture(firstPassSamplers[1],textureCoords).xyz;
                    vec3 vertex3 = vertex1-texture(firstPassSamplers[2],textureCoords).xyz;
                    color.xyz = normalize(cross(vertex2-vertex1,vertex3-vertex1));break;
     }
     color.w = 1.0;


}

void main(void)
{
    if(textureToDisplay<5)
    {
        debug();
    }else
    {
        vec4 randomVectorDirection;
        vec2 textureCoords;
        float ambientOcclusionFactor=0.0f;
        const float MAX_STEP_SIZE = 15.0;

        for(int currentSample=0;currentSample<sampleSize;currentSample++)
         {
            randomVectorDirection.w = 1.0;

            //for(float stepSize=1.0; stepSize<MAX_STEP_SIZE; stepSize++)
            //{
                //project them to find the texture coords
                randomVectorDirection.xyz = f_eyeSpacePositions.xyz;
                vec4 samplePosition = worldToView * randomVectorDirection;
                textureCoords = samplePosition.xy/samplePosition.w;
                textureCoords = (textureCoords + 1.0)/2.0;
                if(rayTriangleIntersection(randomVectorDirection.xyz,textureCoords))
                {
                   ambientOcclusionFactor+= 1.0;
                   //break;
                }
            //}
         }
             ambientOcclusionFactor /=sampleSize;
             color= vec4(ambientOcclusionFactor);
             color.w=1.0;
    }

}
