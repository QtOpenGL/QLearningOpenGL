attribute vec4 vertex;
attribute vec2 texCoord;
uniform mat4 view;
uniform mat4 model;
uniform mat4 projective;
varying vec2 texc;

void main(void)
{
    gl_Position = projective * model * view * vertex;
    texc = texCoord;
}
