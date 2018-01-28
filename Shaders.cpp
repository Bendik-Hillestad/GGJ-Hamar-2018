#include "Shaders.h"

#include <cstdio>
#include <vector>

namespace GGJ
{
    static constexpr GLchar const* vertexSource[]
    {
        {
            R"glsl(
                #version 330 core

                layout(location = 0) in vec2 vertexPosition;

                uniform mat4 view;
                uniform mat4 model;

                out vec2 texCoord;

                void main()
                {
                    gl_Position		= ((view*model) * vec4(vertexPosition, 0, 1));
    
                    texCoord 		= vertexPosition + 0.5;
                }
            )glsl"
        },
        nullptr
    };

    static constexpr GLchar const* fragmentSource[]
    {
        {
            R"glsl(
                #version 330 core

                in vec2 texCoord;

                uniform vec3 color;

                void main()
                {
                    gl_FragColor = vec4(color, 1);
                }
            )glsl"
        },
        {
            R"glsl(
                #version 330 core

                in vec2 texCoord;

                void main()
                {
                    gl_FragColor = vec4(0, 0, 0, 1);
                }
            )glsl"
        },
        {
            R"glsl(
                #version 330 core

                in vec2 texCoord;

                uniform	sampler2D occluderMap;
                uniform vec2      shadowRes;

                const float THRESHOLD = 0.75;

                void main()
                {
                    //Output distance from light source
                    float dist  = 1.0;
    
                    float theta = texCoord.x * 6.28318531;
                    float c     = cos(theta);
                    float s     = sin(theta);
    
                    float sqrSR = shadowRes.y * shadowRes.y;
    
                    for (float i = 0.0; i < shadowRes.y; i++)
                    {
                        for (float j = 1.0; j <= shadowRes.y; j++)
                        {
                            //Current distance from light source
                            float r = (i * j) / sqrSR;
            
                            //Get rectangular coordinates into occluder map
                            vec2 rectCoord = vec2(r * c, r * s) / 2.0 + 0.5;
            
                            //Sample occluder map
                            vec4 texel = texture2D(occluderMap, rectCoord);
            
                            //Update distance
                            dist = mix(dist, min(dist, r), step(THRESHOLD, texel.a));
                        }
                    }
    
                    gl_FragColor = vec4(vec3(dist), 1.0);
                }
            )glsl"
        },
        {
            R"glsl(
                #version 330 core

                in vec2 texCoord;

                uniform sampler2D 	shadowMap;
                uniform vec2		shadowRes;
                uniform vec3		lightColor;

                float sampleShadow(vec2 coord, float r)
                {
                    return step(r, texture2D(shadowMap, coord).r);
                }

                void main()
                {
                    //Direction and length to sample from
                    vec2  dir   = texCoord - vec2(0.5, 0.5);
                    float theta = atan(dir.y, -dir.x) + 3.14159265;
                    float r     = length(dir) * 2.0;
    
                    //The coordinate in our shadow map to look in
                    vec2 centerCoord = vec2(theta / 6.28318531, 0.0);
    
                    //Our blur amount, scaled by distance
                    float blur = (0.5 / shadowRes.x) * smoothstep(0.0, 1.0, r);
    
                    //Sum used in guassian blur
                    float sum = 0.0;
    
                    sum += sampleShadow(vec2(centerCoord.x - 4.0 * blur, centerCoord.y), r) * 0.05;
                    sum += sampleShadow(vec2(centerCoord.x - 3.0 * blur, centerCoord.y), r) * 0.09;
                    sum += sampleShadow(vec2(centerCoord.x - 2.0 * blur, centerCoord.y), r) * 0.12;
                    sum += sampleShadow(vec2(centerCoord.x - 1.0 * blur, centerCoord.y), r) * 0.15;
    
                    sum += sampleShadow(centerCoord, r) * 0.16;
    
                    sum += sampleShadow(vec2(centerCoord.x + 1.0 * blur, centerCoord.y), r) * 0.15;
                    sum += sampleShadow(vec2(centerCoord.x + 2.0 * blur, centerCoord.y), r) * 0.12;
                    sum += sampleShadow(vec2(centerCoord.x + 3.0 * blur, centerCoord.y), r) * 0.09;
                    sum += sampleShadow(vec2(centerCoord.x + 4.0 * blur, centerCoord.y), r) * 0.05;
    
                    //Scale our sum by distance
                    sum *= smoothstep(1.0, 0.0, r);
    
                    //Output light
                    gl_FragColor = vec4(lightColor * sum, 1.0);
                }
            )glsl"
        },
        {
            R"glsl(
                #version 330 core

                in vec2 texCoord;

                uniform sampler2D tex;

                void main()
                {
                    gl_FragColor = texture2D(tex, texCoord);
                }
            )glsl"
        },
        nullptr
    };

    static GLuint mainShaderProgram;
    static GLuint occluderShaderProgram;
    static GLuint shadowShaderProgram;
    static GLuint lightShaderProgram;
    static GLuint postShaderProgram;

    bool BuildShaders() noexcept
    {
        GLint result      = GL_FALSE;
        int infoLogLength = 0;

        //Create and compile the vertex shader
        GLuint mainVertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(mainVertexShader, 1, &vertexSource[0], nullptr);
        glCompileShader(mainVertexShader);

        //Check the shader for errors
        glGetShaderiv(mainVertexShader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(mainVertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0 && !result)
        {
            std::vector<char> log(infoLogLength + 1);
            glGetShaderInfoLog(mainVertexShader, infoLogLength, NULL, &log[0]);
            std::printf("%s\n", &log[0]);
            getchar();

            return false;
        }

        //Create and compile the fragment shader
        GLuint mainFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(mainFragmentShader, 1, &fragmentSource[0], nullptr);
        glCompileShader(mainFragmentShader);

        //Check the shader for errors
        glGetShaderiv(mainFragmentShader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(mainFragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0 && !result)
        {
            std::vector<char> log(infoLogLength + 1);
            glGetShaderInfoLog(mainFragmentShader, infoLogLength, NULL, &log[0]);
            std::printf("%s\n", &log[0]);
            getchar();

            return false;
        }

        //Link the vertex and fragment shader into a shader program
        mainShaderProgram = glCreateProgram();
        glAttachShader(mainShaderProgram, mainVertexShader);
        glAttachShader(mainShaderProgram, mainFragmentShader);
        glLinkProgram(mainShaderProgram);
        glUseProgram(mainShaderProgram);

        //Check the program for errors
        glGetProgramiv(mainShaderProgram, GL_LINK_STATUS, &result);
        glGetProgramiv(mainShaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0 && !result)
        {
            std::vector<char> log(infoLogLength + 1);
            glGetProgramInfoLog(mainShaderProgram, infoLogLength, NULL, &log[0]);
            std::printf("%s\n", &log[0]);
            getchar();

            return false;
        }

        //Create and compile the occluder fragment shader
        GLuint occluderFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(occluderFragmentShader, 1, &fragmentSource[1], nullptr);
        glCompileShader(occluderFragmentShader);

        //Check the shader for errors
        glGetShaderiv(occluderFragmentShader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(occluderFragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0 && !result)
        {
            std::vector<char> log(infoLogLength + 1);
            glGetShaderInfoLog(occluderFragmentShader, infoLogLength, NULL, &log[0]);
            std::printf("%s\n", &log[0]);
            getchar();

            return false;
        }

        //Link the vertex and fragment shader into a shader program
        occluderShaderProgram = glCreateProgram();
        glAttachShader(occluderShaderProgram, mainVertexShader);
        glAttachShader(occluderShaderProgram, occluderFragmentShader);
        glLinkProgram(occluderShaderProgram);
        glUseProgram(occluderShaderProgram);

        //Check the program for errors
        glGetProgramiv(occluderShaderProgram, GL_LINK_STATUS, &result);
        glGetProgramiv(occluderShaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0 && !result)
        {
            std::vector<char> log(infoLogLength + 1);
            glGetProgramInfoLog(occluderShaderProgram, infoLogLength, NULL, &log[0]);
            std::printf("%s\n", &log[0]);
            getchar();

            return false;
        }

        //Create and compile the shadow fragment shader
        GLuint shadowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shadowFragmentShader, 1, &fragmentSource[2], nullptr);
        glCompileShader(shadowFragmentShader);

        //Check the shader for errors
        glGetShaderiv(shadowFragmentShader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shadowFragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0 && !result)
        {
            std::vector<char> log(infoLogLength + 1);
            glGetShaderInfoLog(shadowFragmentShader, infoLogLength, NULL, &log[0]);
            std::printf("%s\n", &log[0]);
            getchar();

            return false;
        }

        //Link the vertex and fragment shader into a shader program
        shadowShaderProgram = glCreateProgram();
        glAttachShader(shadowShaderProgram, mainVertexShader);
        glAttachShader(shadowShaderProgram, shadowFragmentShader);
        glLinkProgram(shadowShaderProgram);
        glUseProgram(shadowShaderProgram);
        
        //Check the program for errors
        glGetProgramiv(shadowShaderProgram, GL_LINK_STATUS, &result);
        glGetProgramiv(shadowShaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0 && !result)
        {
            std::vector<char> log(infoLogLength + 1);
            glGetProgramInfoLog(shadowShaderProgram, infoLogLength, NULL, &log[0]);
            std::printf("%s\n", &log[0]);
            getchar();

            return false;
        }

        //Create and compile the light fragment shader
        GLuint lightFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(lightFragmentShader, 1, &fragmentSource[3], nullptr);
        glCompileShader(lightFragmentShader);

        //Check the shader for errors
        glGetShaderiv(lightFragmentShader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(lightFragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0 && !result)
        {
            std::vector<char> log(infoLogLength + 1);
            glGetShaderInfoLog(lightFragmentShader, infoLogLength, NULL, &log[0]);
            std::printf("%s\n", &log[0]);
            getchar();

            return false;
        }

        //Link the vertex and fragment shader into a shader program
        lightShaderProgram = glCreateProgram();
        glAttachShader(lightShaderProgram, mainVertexShader);
        glAttachShader(lightShaderProgram, lightFragmentShader);
        glLinkProgram(lightShaderProgram);
        glUseProgram(lightShaderProgram);

        //Check the program for errors
        glGetProgramiv(lightShaderProgram, GL_LINK_STATUS, &result);
        glGetProgramiv(lightShaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0 && !result)
        {
            std::vector<char> log(infoLogLength + 1);
            glGetProgramInfoLog(lightShaderProgram, infoLogLength, NULL, &log[0]);
            std::printf("%s\n", &log[0]);
            getchar();

            return false;
        }

        //Create and compile the post fragment shader
        GLuint postFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(postFragmentShader, 1, &fragmentSource[4], nullptr);
        glCompileShader(postFragmentShader);

        //Check the shader for errors
        glGetShaderiv(postFragmentShader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(postFragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0 && !result)
        {
            std::vector<char> log(infoLogLength + 1);
            glGetShaderInfoLog(postFragmentShader, infoLogLength, NULL, &log[0]);
            std::printf("%s\n", &log[0]);
            getchar();

            return false;
        }

        //Link the vertex and fragment shader into a shader program
        postShaderProgram = glCreateProgram();
        glAttachShader(postShaderProgram, mainVertexShader);
        glAttachShader(postShaderProgram, postFragmentShader);
        glLinkProgram(postShaderProgram);
        glUseProgram(postShaderProgram);

        //Check the program for errors
        glGetProgramiv(postShaderProgram, GL_LINK_STATUS, &result);
        glGetProgramiv(postShaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0 && !result)
        {
            std::vector<char> log(infoLogLength + 1);
            glGetProgramInfoLog(postShaderProgram, infoLogLength, NULL, &log[0]);
            std::printf("%s\n", &log[0]);
            getchar();

            return false;
        }

        //Return success
        return true;
    }

    GLuint UseProgram(Program program) noexcept
    {
        //Switch on the program
        switch (program)
        {
            case Program::Main:
            {
                if (mainShaderProgram != 0)
                {
                    glUseProgram(mainShaderProgram);

                    return mainShaderProgram;
                }
            } break;

            case Program::Occluder:
            {
                glUseProgram(occluderShaderProgram);

                return occluderShaderProgram;
            } break;

            case Program::Shadow:
            {
                glUseProgram(shadowShaderProgram);

                return shadowShaderProgram;
            } break;

            case Program::Light:
            {
                glUseProgram(lightShaderProgram);

                return lightShaderProgram;
            } break;

            case Program::Post:
            {
                glUseProgram(postShaderProgram);

                return postShaderProgram;
            } break;

            default:
            {

            } break;
        }

        //Return failure
        return 0;
    }
};