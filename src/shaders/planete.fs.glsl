#version 330 core

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

uniform sampler2D fSampler;
uniform vec3 uLighSource;
uniform float rotationUV;
uniform float ambientStrength;
uniform float specularS;
out vec3 fFragColor;
//Ref : https://gist.github.com/ayamflow/c06bc0c8a64f985dd431bd0ac5b557cd
vec2 rotateUV(vec2 uv, float rotation)
{
    float mid = 0.5;
    return vec2(
        cos(rotation) * (uv.x - mid) + sin(rotation) * (uv.y - mid) + mid,
        cos(rotation) * (uv.y - mid) - sin(rotation) * (uv.x - mid) + mid
    );
}

void main() {
     //Ambient
     vec3 ambient = ambientStrength * vec3(1,1,1);
     
     //Diffuse
     vec3 norm = normalize(vNormal_vs );
     vec3 lightDir = normalize( vec3(0,0,0) - vPosition_vs  );
     float diff = max(dot(norm, lightDir), 0.0);
     vec3 diffuse = diff * vec3(1,1,1);
     

     // specular
    vec3 viewDir = normalize(vec3(0,0,0) - vPosition_vs);
    vec3 reflectDir = reflect(-vec3(0,0,0), vNormal_vs);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = vec3(1,1,1) * (spec * specularS);
    vec2 uv = rotateUV(vTexCoords,rotationUV);
    vec3 res =  (specular + ambient + diffuse) * vec3(texture(fSampler,uv));
    fFragColor = res;
};
