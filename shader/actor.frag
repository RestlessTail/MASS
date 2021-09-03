#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture1;
uniform float brightness;
uniform float transparency;

void main()
{    
    color = texture(texture1, TexCoords) * vec4(brightness, brightness, brightness, 1.0f) * vec4(1.0f, 1.0f, 1.0f, transparency);
}