attribute vec4 vertex;
attribute vec4 vcolor;
uniform mat4 view;
uniform mat4 model;
uniform mat4 projective;
varying vec4 color;

void main(void)
{
    gl_Position = projective * model * view * vertex;
    color = vcolor;
}
