varying vec3 normal;
varying vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(void)
{
    // ambient
    float ambientStrenth = 0.1;
    vec3 ambient = ambientStrenth * lightColor;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = min((ambient + diffuse), vec3(1, 1, 1)) * objectColor;

    gl_FragColor = vec4(result, 1);
//    gl_FragColor = vec4(fragPos.z);
}
