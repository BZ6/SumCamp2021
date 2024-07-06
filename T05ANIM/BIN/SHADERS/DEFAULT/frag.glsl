/* BZ6 */ 
#version 420

in vec4 DrawColor;   
in vec2 DrawTexCoord;
in vec3 DrawNormal;  
in vec3 DrawPos;  
   
//painting color
layout(location = 0) out vec4 OutColor;                   

uniform mat4 MatrWVP;
uniform float Time;
uniform vec3 Ka, Kd, Ks;
uniform float Ph;
 
layout(binding = 0) uniform sampler2D Texture0;
uniform bool IsTexture0;
                                                    
uniform vec3 CamLoc;
uniform vec3 CamDir;

vec3 Shade( vec3 P, vec3 N, vec3 L, vec3 LColor )
{
  vec3 V = normalize(P - CamLoc);
  vec3 color = vec3(0);

  // Reverse normal if need
  //N = faceforward(N, V, N);

  // Ambient
  color = min(Ka, vec3(0.1, 0.1, 0.1));

  // Duffuse
  float nl = dot(N, L);
  nl = max(nl, 0);
  vec3 diff = LColor * Kd * nl;
  if (IsTexture0)
    diff *= texture(Texture0, DrawTexCoord).rgb;
  color += diff;
 
  // Specular
  vec3 R = reflect(V, N);
  float rl = max(dot(R, L), 0);
  color += rl * LColor * Ks * pow(rl, Ph);

  return color;
}

void main( void )
{
  vec3 N = normalize(DrawNormal);
  vec3 L = normalize(vec3(5, 4, 3));  // light source
  vec3 S = Shade(DrawPos, N, L, vec3(1));
  //vec3 V = normalize(DrawPos - CamLoc);                
  //if (dot(V, CamDir) >= 0.99999)
  //  discard;  
  //else
  //  OutColor = vec4(texture(Texture0, DrawTexCoord).brg, 1);  
  OutColor = vec4(N, 1);  
}
