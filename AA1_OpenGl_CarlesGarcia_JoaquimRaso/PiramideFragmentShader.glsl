#version 440 core

uniform float time;

out vec4 fragColor;

float timeValue;

void main()
{

timeValue = mod(time, 6.0);
    if(timeValue <= 2.0f)    {
    fragColor = vec4(1.0, 0.0, 0.0,1.0); // Rojo
      }
       if(timeValue <= 4.0f && timeValue >2.0f)
      {
      fragColor = vec4(0.0, 1.0, 0.0,1.0); // Verde
      }
       if(timeValue <= 6.0f && timeValue >4.0f)
      {
      fragColor = vec4(0.0, 0.0, 1.0,1.0); // Azul
      }
}    
      




