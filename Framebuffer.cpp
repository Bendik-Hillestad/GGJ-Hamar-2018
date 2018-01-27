#include "Framebuffer.h"

namespace GGJ
{
    Framebuffer::Framebuffer() noexcept
    {}

    Framebuffer Framebuffer::CreateBuffer(GLuint width, GLuint height, GLint magFilter, GLint minFilter) noexcept
    {
        //Create the container for the framebuffer data
        Framebuffer buffer{};

        //Generate a texture object
        glGenTextures(1, &buffer.texture);

        //Bind our texture as a 2D texture
        glBindTexture(GL_TEXTURE_2D, buffer.texture);

        //Set sampling parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

        //Clamp UV coordinates between 0 and 1
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        //Initialise the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        //Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);

        //Save the width and height
        buffer.width  =  width;
        buffer.height = height;

        //Return the created framebuffer
        return buffer;
    }

    void Framebuffer::Bind(GLuint offset) noexcept
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + offset, this->texture, 0);
    }

    void Framebuffer::Dispose() noexcept
    {
        //Check that a texture is allocated
        if (this->texture)
        {
            //Release it
            glDeleteTextures(1, &this->texture);
            this->texture = 0;
        }

        //Just zero out width and height as well
        this->width  = 0;
        this->height = 0;
    }

    GLuint Framebuffer::GetWidth() const noexcept
    {
        return this->width;
    }

    GLuint Framebuffer::GetHeight() const noexcept
    {
        return this->height;
    }
};
