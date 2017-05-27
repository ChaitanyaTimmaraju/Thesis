#version 440 core
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
uniform vec3 mouseClickRay;
//Projection matrix which is used to find the texture co-ordinates.
uniform mat4 worldToView;

vec3 computeNormal(vec3 vertex1,vec3 vertex2, vec3 vertex3)
{
    vec3 a = vertex2 - vertex1;
    vec3 b = vertex3 - vertex1;
    return cross(a,b);
}

bool insideOutTest(vec3 vertex1, vec3 vertex2,vec3 vertex3, vec3 normal,vec3 pointOnTheTriangle)
{

     // Ref: http://www.cs.cornell.edu/courses/cs465/2003fa/homeworks/raytri.pdf
    //To check whether point lies inside the triangle or not.
    //inside-out test
    float point1 = dot((cross(vertex2-vertex1,pointOnTheTriangle-vertex1)),normal);
    float point2 = dot((cross(vertex3-vertex2,pointOnTheTriangle-vertex2)),normal);
    float point3 = dot((cross(vertex1-vertex3,pointOnTheTriangle-vertex3)),normal);

    if(point1<EPSILON||point2<EPSILON||point3<EPSILON)            
        return false;

    return true;
}

bool rayPlaneIntersectionGeometricApproach(vec3 origin,vec3 rayDirection,vec2 textureCoords)
{

    vec3 vertex1 = texture(firstPassSamplers[0],textureCoords).xyz;
    vec3 vertex2 = texture(firstPassSamplers[1],textureCoords).xyz;
    vec3 vertex3 = texture(firstPassSamplers[2],textureCoords).xyz;
    vec3 normal = texture(firstPassSamplers[3],textureCoords).xyz;
    /*
       Plane equation = Ax+By+Cz+D =0
       (A,B,C) = Normal
       x,y,z = coords of any point on the line
       Point on triangle P = Origin+t*RayDirection
       t=D-N.Origin/N.RayDir'n
      */
    //Using centroid as our point to compute distance from origin to plane
    float D = dot((vertex1+vertex2+vertex3)/3.0,normal);
    float denominator = dot(normal,rayDirection);
    if(denominator==0.0)
        return false;
    float t =(D-dot(normal,origin))/denominator;
    vec3 pointOnTriangle = origin + t*rayDirection;
    return insideOutTest( vertex1,  vertex2, vertex3,  normal, pointOnTriangle);
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
              case 0: color.xyz = vertex1;
                            break;
              case 1: color.xyz =texture(firstPassSamplers[1],textureCoords).xyz;
                            break;
              case 2: color.xyz = texture(firstPassSamplers[2],textureCoords).xyz;
                            break;
              case 3: color.xyz = texture(firstPassSamplers[displayOption],textureCoords).xyz;
                            break;
              case 4: vec3 vertex2 =texture(firstPassSamplers[1],textureCoords).xyz;
                            vec3 vertex3 = texture(firstPassSamplers[2],textureCoords).xyz;
                          color.xyz = normalize(cross(vertex2-vertex1,vertex3-vertex1));
                          /*  if((vertex1.x>=0.0 && vertex1.x<0.1)||(vertex2.x>=0.0 && vertex2.x<0.1)||(vertex3.x>=0.0 && vertex3.x<0.1))
                            {
                                if((vertex1.y>=0.0&&vertex1.y<0.1)||(vertex2.y>=0.0&&vertex2.y<0.1)||(vertex3.y>=0.0&&vertex3.y<0.1))
                                 if(vertex1.z>=-2.0&&vertex1.z<-1.5)
                                     if(rayPlaneIntersectionGeometricApproach(vec3(0.0,0.0,0.0),vec3(mouseClickRay.xy,-3.0),textureCoords))
                                            color.w =1.0;
                                         //   color.xyz = normalize(cross(vertex2-vertex1,vertex3-vertex1));
                           }*/
                            break;
              case 5: /*Check intersection for current eye-space point and the triangle.
                                    1) White = hit
                                    2)Green = miss
                            */
                           if(rayPlaneIntersectionGeometricApproach(vec3(0.0,0.0,0.0),vec3(mouseClickRay.xy,-3.0),textureCoords))
                                color.xyz = vec3(1.0);
                            else
                                color.xyz = vec3(0.15,0.51,0.34);
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
        vec4 randomSamplePosition;
        vec2 textureCoords;
        float ambientOcclusionFactor=0.0f;
        const float MAX_STEP_SIZE = 0.5;
        for(int currentSample=0;currentSample<(sampleSize);++currentSample)
        {
             for(float stepsize = 0.1;stepsize<MAX_STEP_SIZE;stepsize+=0.1 )
             {
                randomSamplePosition.xyz = f_eyeSpacePositions+(fRandom_Vectors[currentSample]*stepsize);
                randomSamplePosition.w = 1.0;
                randomSamplePosition = worldToView*randomSamplePosition;
                randomSamplePosition.xy/=randomSamplePosition.w;
                textureCoords = (randomSamplePosition.xy+1.0)/2.0;
                if(rayPlaneIntersectionGeometricApproach(f_eyeSpacePositions,(fRandom_Vectors[currentSample]*stepsize),textureCoords))
                {
                    ambientOcclusionFactor+=1.0;
                    break;
                }
             }
        }
       color.w=1.0;
       ambientOcclusionFactor/=sampleSize;
       color.xyz = vec3(1.0-ambientOcclusionFactor);
    }

}

