#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform float alpha;

void main()
{    
    color = vec4(spriteColor, alpha) * texture(image, TexCoords);
}