#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 LightDirection;
    vec3 EyeDirection;
    vec4 ShadowCoord;
} fs_in;

uniform sampler2D tex;
uniform sampler2DShadow shadowMap;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main() {
    vec3 lightColor = vec3(1.0);
    float lightPower = 1.0;

    vec3 diffuseColor = texture(tex, fs_in.TexCoords).rgb;
    vec3 ambientColor = vec3(0.3) * diffuseColor;
    vec3 specularColor = vec3(0.0);

    vec3 n = normalize(fs_in.Normal);
    vec3 l = normalize(fs_in.LightDirection);

    vec3 E = normalize(fs_in.EyeDirection);
    vec3 R = reflect(-l, n);

    float cosTheta = clamp(dot(n, l), 0, 1);
    float cosAlpha = clamp(dot(E, R), 0, 1);

    float visibility = 1.0;
    float bias = 0.005*tan(acos(cosTheta));
	bias = clamp(bias, 0,0.01);

    for (int i = 0; i < 4; ++i) {
        int index = int(16.0*random(floor(fs_in.FragPos.xyz*1000.0), i))%16;
        visibility -= 0.2 * (1.0 - texture(shadowMap, vec3(fs_in.ShadowCoord.xy + poissonDisk[index]/700.0, (fs_in.ShadowCoord.z - bias) / fs_in.ShadowCoord.w) ));
    }

    FragColor = 
        vec4(ambientColor +
        visibility * diffuseColor * lightColor * lightPower * cosTheta +
        visibility * specularColor * lightColor * lightPower * pow(cosAlpha, 5), 1.0);
}