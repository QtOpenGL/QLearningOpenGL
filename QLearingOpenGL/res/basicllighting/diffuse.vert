attribute vec3 vertex;
attribute vec3 aNormal;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projective;

varying vec3 fragPos;
varying vec3 normal;

void main(void)
{
    fragPos = vec3(model * vec4(vertex, 1.0));
    normal = aNormal;
    gl_Position = projective * model * view * vec4(vertex, 1.0);
}
