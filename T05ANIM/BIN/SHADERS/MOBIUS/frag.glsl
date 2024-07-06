/* BZ6 */ 
#version 420

in vec4 DrawColor;   
in vec2 DrawTexCoord;
in vec3 DrawNormal;  
in vec3 DrawPos;  
   
//painting color
layout(location = 0) out vec4 OutColor;                   
        
void main( void )
{                                                      
  OutColor = vec4(1, 0.5, 0.47, 1);  
}
