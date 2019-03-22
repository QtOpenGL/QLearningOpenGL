attribute vec3 vertex;
attribute vec3 aNormal;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projective;
uniform vec3 u_ViewPos;

varying vec3 fragPos;
varying vec3 normal;
varying vec3 viewPos;

void main(void)
{
    fragPos = vec3(model * vec4(vertex, 1.0));
    normal = aNormal;
    viewPos = u_ViewPos;
    gl_Position = projective * model * view * vec4(vertex, 1.0);
}
