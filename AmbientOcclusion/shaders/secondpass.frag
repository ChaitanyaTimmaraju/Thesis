#version 440
//Current Eye-space position
in vec3 f_eyeSpacePositions;
out vec4 color;

const int sampleSize=25;
//Default EPSILON value is 0.0f
uniform float EPSILON;
//Used in the debugging for displaying of first pass textures
uniform int displayOption;
//All the textures stored in eye space positions in firstpass.
uniform sampler2D firstPassSamplers[4];
uniform int screenWidth;
uniform int screenHeight;
uniform vec3 fRandom_Vectors[sampleSize];
//Projection matrix which is used to find the texture co-ordinates.
uniform mat4 worldToView;

bool rayTriangleIntersectionGeometricApproach(vec3 origin,vec3 rayDirection,vec2 textureCoords)
{
    vec3 vertex1 = texture(firstPassSamplers[0],textureCoords).xyz;
    vec3 vertex2 = vertex1-texture(firstPassSamplers[1],textureCoords).xyz;
    vec3 vertex3 = vertex1-texture(firstPassSamplers[2],textureCoords).xyz;
    vec3 normal = texture(firstPassSamplers[3],textureCoords).xyz;
    /*
       Plane equation = Ax+By+Cz+D =0
       (A,B,C) = Normal
       x,y,z = coords of any point on the line
       Point on triangle P = Origin+t*RayDirection
       t=- N(A,B,C).O+D/N(A,B,C).R
      */
    float D = dot(vertex1,normal);
    float denominator = dot(normal,rayDirection);
    if(denominator==0.0)
        return false;

    float t =(D-dot(normal,origin))/denominator;
    if(t>EPSILON)
        return true;

    return false;

}
//This is for displaying textures of the firstPass.
void debugOptions()
{
     vec2 textureCoords;
     vec4 projectedPoint = worldToView *vec4( f_eyeSpacePositions,1.0);
     textureCoords = projectedPoint.xy/projectedPoint.w;
     textureCoords = (textureCoords + 1.0)/2.0;
     vec3 vertex1 = texture(firstPassSamplers[0],textureCoords).xyz;
     switch(displayOption)
     {
      case 0: color.xyz = vertex1;break;
      case 1: color.xyz =vertex1-texture(firstPassSamplers[1],textureCoords).xyz;break;
      case 2: color.xyz = vertex1-texture(firstPassSamplers[2],textureCoords).xyz;break;
      case 3: color.xyz = texture(firstPassSamplers[displayOption],textureCoords).xyz;break;
      case 4: vec3 vertex2 =vertex1-texture(firstPassSamplers[1],textureCoords).xyz;
                    vec3 vertex3 = vertex1-texture(firstPassSamplers[2],textureCoords).xyz;
                    color.xyz = normalize(cross(vertex2-vertex1,vertex3-vertex1));break;
      case 5: /*Check intersection for current eye-space point and the triangle.
                            1) white = hit
                            2)black = miss
                    */
                    if(rayTriangleIntersectionGeometricApproach(f_eyeSpacePositions,vec3(1.0,1.0,1.0),textureCoords))
                        color.xyz = vec3(1.0);
                    else
                        color.xyz = vec3(0.0);
                    break;
     }
     color.w = 1.0;
}

void main(void)
{
    if(displayOption<6)
    {
        debugOptions();
    }else
    {
        //Actual ambient occlusion rendering.
        vec4 randomVectorDirection;
        vec2 textureCoords;
        float ambientOcclusionFactor=0.0f;
        const float MAX_STEP_SIZE = 5.0f;       
        for(int currentSample=0;currentSample<(sampleSize);++currentSample)
        {
            randomVectorDirection.xyz = f_eyeSpacePositions+(fRandom_Vectors[currentSample])  ;
            randomVectorDirection.w = 1.0;
            vec4 projectedPoint = worldToView * randomVectorDirection;
            textureCoords = projectedPoint.xy/projectedPoint.w;
            textureCoords = (textureCoords + 1.0)/2.0;
            if(rayTriangleIntersectionGeometricApproach(f_eyeSpacePositions,fRandom_Vectors[currentSample],textureCoords))
              {
                        ambientOcclusionFactor+=1.0;
            }
           }
       ambientOcclusionFactor /=sampleSize;
       color= vec4( ambientOcclusionFactor);
       color.w=1.0;
    }

}


/*
bool rayTriangleIntersection(vec3 pointOnTheTriangle,vec2 texCoordsOfTriangle)
{

     // Ref: http://www.cs.cornell.edu/courses/cs465/2003fa/homeworks/raytri.pdf


    //Specifying vertices in counter clock-wise direction
    vec3 vertex1 = texture(firstPassSamplers[0],texCoordsOfTriangle).xyz;
    vec3 vertex2 = vertex1-texture(firstPassSamplers[1],texCoordsOfTriangle).xyz;
    vec3 vertex3 = vertex1-texture(firstPassSamplers[2],texCoordsOfTriangle).xyz;
    vec3 normal = cross(vertex2-vertex1,vertex3-vertex1);

    //inside-out test
    float point1 = dot((cross(vertex2-vertex1,pointOnTheTriangle-vertex1)),normal);
    float point2 = dot((cross(vertex3-vertex2,pointOnTheTriangle-vertex2)),normal);
    float point3 = dot((cross(vertex1-vertex3,pointOnTheTriangle-vertex3)),normal);

    if(point1<EPSILON||point2<EPSILON||point3<EPSILON)
        return false;

    return true;
}
*/
