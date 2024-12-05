#version 450 core
in vec2 fragUVs;
out vec4 color;
uniform sampler2D uTexture;
void main()
{
  color = texture(uTexture, fragUVs);
}