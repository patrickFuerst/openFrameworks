#version 410 core                                                                               
                                                                                                
layout (location = 0) in vec3 Position_VS_in;                                                   
layout (location = 3) in vec2 TexCoord_VS_in;
layout (location = 2) in vec3 Normal_VS_in;                                                     
                                                                                                
uniform mat4 gWorld;                                                                            
                                                                                                
out vec3 WorldPos_CS_in;                                                                        
out vec2 TexCoord_CS_in;                                                                        
out vec3 Normal_CS_in;                                                                          
                                                                                                
void main()                                                                                     
{                                                                                               
    WorldPos_CS_in =  Position_VS_in;
    TexCoord_CS_in = TexCoord_VS_in;                                                            
    Normal_CS_in   = normalize( Normal_VS_in );
}