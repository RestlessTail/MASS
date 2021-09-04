#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform float transparency;

void main()
{
    FragColor = texture(texture1, TexCoord) * vec4(1.0f, 1.0f, 1.0f, transparency);
    //FragColor = vec4(1.0f, 0.0f, 1.0f, 0.5f);
}