#version 330

in vec4 vCol;
in vec2 TextureCoordinate;
in vec3 Normal;
in vec3 fragPos;


uniform vec4 lightColor;
uniform sampler2D ourTexture;
uniform vec3 lightPosition;
out vec4 color;

void main()
{

    float ambientStrength = 0.5;    
    vec4 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    vec4 result = ambient+diffuse;

//	color = vec4(vCol.x, vCol.y, vCol.z, 1.0);
    color = result*lightColor*texture(ourTexture,TextureCoordinate);
}
