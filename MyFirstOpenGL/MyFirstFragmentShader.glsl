#version 440 core

uniform vec2 windowSize;

out vec4 fragColor;

void main() {

         if(gl_FragCoord.y > (windowSize.y * 0.5)) {
            fragColor = vec4(1.0, 1.0, 0.0, 1.0);
        } else {
            fragColor = vec4(1.0, 0.5, 0.0, 1.0);    
        }  

}
