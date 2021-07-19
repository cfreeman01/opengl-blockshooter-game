#version 330 core

#if __VERSION__ < 130
#define TEXTURE2D texture2D
#else
#define TEXTURE2D texture
#endif

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D bossTexture;
uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1.0) * TEXTURE2D(bossTexture, texCoord);
}