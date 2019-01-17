#version 330 core

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords; // Coordonnées de texture du sommet

uniform sampler2D fSampler;
uniform vec3 uLighSource;
out vec3 fFragColor;

void main() {
     //Ambient
     float ambientStrength = 0.5;
     vec3 ambient = ambientStrength * vec3(1,1,1);
     
     //Diffuse
     vec3 norm = normalize(vNormal_vs);
     vec3 lightDir = normalize(vec3(0,0,0) - vPosition_vs);
     //     lightDir = lightDir + uLighSource;
     float diff = max(dot(norm, lightDir), 0.0);
     vec3 diffuse = diff * vec3(1,1,1);
     

     // specular
     float specularS = 0.5;
    vec3 viewDir = normalize(vec3(0,0,0) - vPosition_vs);
    vec3 reflectDir = reflect(-lightDir, vNormal_vs);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = vec3(1,1,1) * (spec * specularS);
    
    vec3 res =  (specular + ambient + diffuse*0.5) * vec3(texture(fSampler,vTexCoords));
    fFragColor = res;
};
