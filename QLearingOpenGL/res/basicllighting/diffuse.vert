attribute vec4 vertex;
attribute vec3 aNormal;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projective;

varying vec3 fragPos;
varying vec3 normal;

void main(void)
{
    fragPos = vec3(model * vertex);
    normal = aNormal;
    gl_Position = projective * model * view * vertex;
}
