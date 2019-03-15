varying vec4 color;
uniform sampler2D texture;
varying vec2 texc;

void main(void)
{
//    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
    gl_FragColor = texture2D(texture, texc.st);
}
