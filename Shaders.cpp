#include "Shaders.h"

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

                void main()
                {
	                gl_FragColor = vec4(1, 1, 1, 1);
                }
            )glsl"
        },
        nullptr
    };

    static GLuint mainShaderProgram;

    bool BuildShaders() noexcept
    {
        //Create and compile the vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource[0], nullptr);
        glCompileShader(vertexShader);

        //Create and compile the fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource[0], nullptr);
        glCompileShader(fragmentShader);

        //Link the vertex and fragment shader into a shader program
        mainShaderProgram = glCreateProgram();
        glAttachShader(mainShaderProgram, vertexShader);
        glAttachShader(mainShaderProgram, fragmentShader);
        glLinkProgram(mainShaderProgram);
        glUseProgram(mainShaderProgram);

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

            case Program::Light:
            {

            } break;

            default:
            {

            } break;
        }

        //Return failure
        return 0;
    }
};