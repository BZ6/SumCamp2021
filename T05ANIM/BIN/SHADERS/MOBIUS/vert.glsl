/* BZ6 */

#version 430
                            
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;
 
// global var (World * View * Proj)
uniform mat4 MatrWVP;
uniform float Time;
 
// out params (varying)
out vec4 DrawColor;         
out vec3 DrawNormal; 
out vec3 DrawPos;
 
 
void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = InColor;

  DrawPos = InPosition;     
  DrawNormal = InNormal;  
   //vec4(sin(TInColor;   ime) * InPosition.x, cos(Time) * InPosition.y, InPosition.z, 1);
}

