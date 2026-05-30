$input v_color0

#include <bgfx_shader.sh>

void main()
{
    float size = 16.0;

    vec2 coord = floor(gl_FragCoord.xy / size);
    float checker = mod(coord.x + coord.y, 2.0);

    vec3 magenta = vec3(1.0, 0.0, 1.0);
    vec3 black   = vec3(0.0, 0.0, 0.0);

    vec3 color = mix(magenta, black, checker);

    gl_FragColor = vec4(color, 1.0);
}