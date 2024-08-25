#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>

#include "Mesh.h"
#include "Camera.h"

namespace sablin{

extern unsigned int TextureHDRFromFile(string filename, const string &directory, bool gamma);

struct PointLight{
    glm::vec3 position_;
    glm::vec3 color_;
    float linear_;
    float quadratic_;
    float radius_;
};

struct DirLight{
    glm::vec3 position_;
    glm::vec3 color_;
};

class Shader
{
protected:
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        }
    }
public:
    unsigned int ID;
public:
    Shader(const std::string vertexPath, const std::string fragmentPath, const std::string geometryPath = "")
    {
        // 1. 将Shader File读入string
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			
            if(!geometryPath.empty())
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();

        // 2. 编译VS
        unsigned int vertex;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        
        //3. 编译FS
        // fragment Shader
        unsigned int fragment;
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        //4. 编译GS
        unsigned int geometry;
        if(!geometryPath.empty())
        {
            const char * gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }

        // 5. 创建Shader程序并将VS,FS附加上去
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if(!geometryPath.empty())
            glAttachShader(ID, geometry);

        // 6. 链接Shader程序(将VS,PS确定下来并检查有效性)
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // 7. 清除VS、FS与GS，其在链接后即可清除
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(!geometryPath.empty())
            glDeleteShader(geometry);

    }

    // 激活（使用）此Shader
    void use() 
    { 
        glUseProgram(ID); 
    }

    // 各个传参函数
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) 
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    virtual void Draw(Mesh& mesh) 
    {
        setFloat("normalMapAlpha", mesh.normal_map_alpha_);
        setFloat("aoMapAlpha", mesh.ao_map_alpha_);
        for(unsigned int i = 0; i < mesh.textures_.size(); i++)
        {
            // 告知着色器的采样器要从哪个纹理单元采样
            setInt(mesh.textures_[i].type_, i);
            // 激活纹理单元
            glActiveTexture(GL_TEXTURE0 + i);
            // 将纹理绑定至此纹理单元
            glBindTexture(GL_TEXTURE_2D, mesh.textures_[i].id_);
        }
        
        // 绘制Mesh
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.indices_.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // 恢复默认纹理单元激活
        glActiveTexture(GL_TEXTURE0);
    }
};

class IBL{
private:
    Shader backgroundShader;
    unsigned int envCubemap;
public:
    unsigned int irradianceMap;
    unsigned int prefilterMap;
    unsigned int brdfLUTTexture;

    IBL(const string& dir, const string& env_map):
        backgroundShader(dir + "Shaders/background.vs", dir + "Shaders/background.fs")
    {
        // 创建Framebuffer与RenderBuffer(与深度模板有关)
        unsigned int captureFBO;
        unsigned int captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        // 加载HDR environment map
        unsigned int hdrTexture = TextureHDRFromFile(env_map, dir, false);

        // 创建并绑定要渲染至的environment cubemap
        glGenTextures(1, &envCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // set up projection and view matrices for capturing data onto the 6 cubemap face directions
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        // 将HDR texture转为Cubemap
        Shader hdrToCubemapShader(dir + "Shaders/cubemap.vs", dir + "Shaders/hdr_to_cubemap.fs");
        hdrToCubemapShader.use();
        hdrToCubemapShader.setInt("hdrTexture", 0);
        hdrToCubemapShader.setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);

        glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            hdrToCubemapShader.setMat4("view", captureViews[i]);
            // 将envCubemap每一面绑定为Shader的输出
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Mesh::RenderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 为避免点状走样，为cubemap生成mipmap
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        // 创建irradiance cubemap
        glGenTextures(1, &irradianceMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 将framebuffer与renderbuffer尺寸设为与irradiance cubemap相同
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

        // solve diffuse integral by convolution to create an irradiance cubemap.
        Shader irradianceShader(dir + "Shaders/cubemap.vs", dir + "Shaders/irradiance_convolution.fs");
        irradianceShader.use();
        irradianceShader.setInt("environmentMap", 0);
        irradianceShader.setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            irradianceShader.setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            Mesh::RenderCube();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 创建pre-filter cubemap
        glGenTextures(1, &prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        // run a quasi monte-carlo simulation on the environment lighting to create a prefilter cubemap.
        Shader prefilterShader(dir + "Shaders/cubemap.vs", dir + "Shaders/prefilter.fs");
        prefilterShader.use();
        prefilterShader.setInt("environmentMap", 0);
        prefilterShader.setMat4("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        unsigned int maxMipLevels = 5;
        for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
        {
            // reisze framebuffer according to mip-level size.
            unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            glViewport(0, 0, mipWidth, mipHeight);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            prefilterShader.setFloat("roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                prefilterShader.setMat4("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                Mesh::RenderCube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // generate a 2D LUT from the BRDF equations used.
        glGenTextures(1, &brdfLUTTexture);

        // pre-allocate enough memory for the LUT texture.
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
        // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // resize framebuffer object
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

        // render screen-space quad with BRDF shader.
        glViewport(0, 0, 512, 512);
        Shader brdfShader(dir + "Shaders/brdf.vs", dir + "Shaders/brdf.fs");
        brdfShader.use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Mesh::RenderQuad();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void SetupBackground(const glm::mat4& projection){
        backgroundShader.use();
        backgroundShader.setInt("environmentMap", 0);
        backgroundShader.setMat4("projection", projection);
    }

    void RenderBackground(const glm::mat4& view){
        backgroundShader.use();
        backgroundShader.setMat4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        Mesh::RenderCube();
    }
};

class ShadowShader: public Shader{
public:
    static const unsigned int DepthMapResolution = 4096;
    std::vector<float> shadowCascadeLevels;
    glm::vec3 lightDir;
    Camera& camera;
    int framebuffer_width;
    int framebuffer_height;
    unsigned int shadowFBO;
    unsigned int shadowDepthMap;
    unsigned int matricesUBO;

    ShadowShader(const string& dir, Camera& cam, glm::vec3 lightDirection, int width, int height):
        Shader(dir + "Shaders/shadow_depth.vs", dir + "Shaders/shadow_depth.fs", dir + "Shaders/shadow_depth.gs"),
        shadowCascadeLevels{cam.far_plane_ / 50.0f, cam.far_plane_ / 25.0f, cam.far_plane_ / 10.0f, cam.far_plane_ / 2.0f},
        lightDir(glm::normalize(lightDirection)),
        camera(cam),
        framebuffer_width(width),
        framebuffer_height(height)
    {
        glGenFramebuffers(1, &shadowFBO);

        glGenTextures(1, &shadowDepthMap);
        glBindTexture(GL_TEXTURE_2D_ARRAY, shadowDepthMap);
        glTexImage3D(
            GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, DepthMapResolution, DepthMapResolution, int(shadowCascadeLevels.size()) + 1,
            0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowDepthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
            throw 0;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 创建UBO容纳16个矩阵
        glGenBuffers(1, &matricesUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 16, nullptr, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void BeforeDraw(){
        use();
        // 以矩阵填充UBO
        const auto lightMatrices = GetLightSpaceMatrices();
        glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
        for (size_t i = 0; i < lightMatrices.size(); ++i)
        {
            glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &lightMatrices[i]);
        }

        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glViewport(0, 0, DepthMapResolution, DepthMapResolution);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);  // peter panning
    }

    virtual void Draw(Mesh& mesh) override{
        use();
        // 绘制Mesh
        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh.indices_.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void AfterDraw(){
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, framebuffer_width, framebuffer_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    std::vector<glm::mat4> GetLightSpaceMatrices()
    {
        std::vector<glm::mat4> ret;
        for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
        {
            if (i == 0)
            {
                ret.push_back(GetLightSpaceMatrix(camera.near_plane_, shadowCascadeLevels[i]));
            }
            else if (i < shadowCascadeLevels.size())
            {
                ret.push_back(GetLightSpaceMatrix(shadowCascadeLevels[i - 1], shadowCascadeLevels[i]));
            }
            else
            {
                ret.push_back(GetLightSpaceMatrix(shadowCascadeLevels[i - 1], camera.far_plane_));
            }
        }
        return ret;
    }


    glm::mat4 GetLightSpaceMatrix(const float nearPlane, const float farPlane)
    {
        const auto proj = glm::perspective(
            glm::radians(camera.zoom_), (float)framebuffer_width/(float)framebuffer_height, nearPlane,
            farPlane);
        const auto corners = GetFrustumCornersWorldSpace(proj, camera.GetViewMatrix());

        glm::vec3 center = glm::vec3(0, 0, 0);
        for (const auto& v : corners)
        {
            center += glm::vec3(v);
        }
        center /= corners.size();

        const auto lightView = glm::lookAt(center + lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));
        
        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::lowest();
        for (const auto& v : corners)
        {
            const auto trf = lightView * v;
            minX = std::min(minX, trf.x);
            maxX = std::max(maxX, trf.x);
            minY = std::min(minY, trf.y);
            maxY = std::max(maxY, trf.y);
            minZ = std::min(minZ, trf.z);
            maxZ = std::max(maxZ, trf.z);
        }


        minZ = -minZ;
        maxZ = -maxZ;
        float a = glm::dot(camera.front_, lightDir);
        float d = glm::degrees(acosf(a));
        if(d > 90.0f){
            float zMult = 1.0f + 0.8f * a;
            if (maxZ < 0) maxZ /= zMult; else maxZ *= zMult;
        }else{
            float zMult = 1.0f + 1.0f * a;
            if (maxZ < 0) maxZ /= zMult; else maxZ *= zMult;
        }

        float xMult = 1.2f;
        if (minX < 0) minX *= xMult; else minX /= xMult;
        if (maxX < 0) maxX /= xMult; else maxX *= xMult;

        float yMult = 1.2f;
        if (minY < 0) minY *= yMult; else minY /= yMult;
        if (maxY < 0) maxY /= yMult; else maxY *= yMult;

        const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

        return lightProjection * lightView;
    }

    std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
    {
        const auto proinv = glm::inverse(proj);
        const auto viewinv = glm::inverse(view);

        std::vector<glm::vec4> frustumCorners;
        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    glm::vec4 pt = viewinv * proinv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                    frustumCorners.push_back(pt / pt.w);
                }
            }
        }

        return frustumCorners;
    }

    std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& projview)
    {
        const auto inv = glm::inverse(projview);

        std::vector<glm::vec4> frustumCorners;
        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                    frustumCorners.push_back(pt / pt.w);
                }
            }
        }

        return frustumCorners;
    }
};

class SSAOShader{
public:
    unsigned int ssaoFBO;
    unsigned int ssaoBlurFBO;
    unsigned int ssaoColorBuffer;
    unsigned int ssaoColorBufferBlur;
    Shader ssaoShader;
    Shader ssaoBlurShader;
    std::vector<glm::vec3> ssaoKernel;
    unsigned int noiseTexture;
    
    SSAOShader(string dir, int frame_width, int frame_height):
        ssaoShader(dir + "Shaders/ssao.vs", dir + "Shaders/ssao.fs"),
        ssaoBlurShader(dir + "Shaders/ssao.vs", dir + "Shaders/ssao_blur.fs")
    {
        glGenFramebuffers(1, &ssaoFBO);  
        glGenFramebuffers(1, &ssaoBlurFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        // SSAO color buffer
        glGenTextures(1, &ssaoColorBuffer);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frame_width, frame_height, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SSAO Framebuffer not complete!" << std::endl;
        // SSAO blur buffer
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
        glGenTextures(1, &ssaoColorBufferBlur);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frame_width, frame_height, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // generate sample kernel
        std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
        std::default_random_engine generator;
        for (unsigned int i = 0; i < 64; ++i)
        {
            glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
            sample = glm::normalize(sample);
            sample *= randomFloats(generator);
            float scale = float(i) / 64.0f;

            scale = lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            ssaoKernel.push_back(sample);
        }

        // generate noise texture
        std::vector<glm::vec3> ssaoNoise;
        for (unsigned int i = 0; i < 16; i++)
        {
            glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
            ssaoNoise.push_back(noise);
        }
        glGenTextures(1, &noiseTexture);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        ssaoShader.use();
        ssaoShader.setInt("gPosition", 0);
        ssaoShader.setInt("gNormal", 1);
        ssaoShader.setInt("texNoise", 2);
        ssaoBlurShader.use();
        ssaoBlurShader.setInt("ssaoInput", 0);
    }

    void DrawSSAO(glm::mat4 projection, glm::mat4 view, Gbuffer& gBuffer){
        // 1. generate SSAO texture
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        ssaoShader.use();
        // Send kernel + rotation 
        for (unsigned int i = 0; i < 32; ++i)
            ssaoShader.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
        ssaoShader.setMat4("projection", projection);
        ssaoShader.setMat4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        Mesh::RenderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. blur SSAO texture to remove noise
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        ssaoBlurShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        Mesh::RenderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};

class PBRShader{
private:
    Shader pbrShader;
public:
    unsigned int pbrColor;
    unsigned int pbrDepth;
    unsigned int pbrBuffer;

    PBRShader(const string& dir, int width, int height):
        pbrShader(dir + "Shaders/pbr.vs", dir + "Shaders/pbr.fs"){
        glGenFramebuffers(1, &pbrBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, pbrBuffer);

        glGenTextures(1, &pbrColor);
        glBindTexture(GL_TEXTURE_2D, pbrColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pbrColor, 0);

        // 创建深度缓冲
        glGenTextures(1, &pbrDepth);
        glBindTexture(GL_TEXTURE_2D, pbrDepth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        // attach depth texture as FBO's depth buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pbrDepth, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 检查是否成功
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        pbrShader.use();
        pbrShader.setInt("irradianceMap", 0);
        pbrShader.setInt("prefilterMap", 1);
        pbrShader.setInt("brdfLUT", 2);
        pbrShader.setInt("positionMap", 3);
        pbrShader.setInt("albedoMap", 4);
        pbrShader.setInt("normalMap", 5);
        pbrShader.setInt("metalRoughAoMap", 6);
        pbrShader.setInt("shadowMap", 7);
        pbrShader.setInt("ssao", 8);
    }

    void SetIBLMaps(IBL& ibl){
        pbrShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ibl.irradianceMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ibl.prefilterMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ibl.brdfLUTTexture);
    }

    void SetGbufferMaps(Gbuffer& gBuffer){
        pbrShader.use();
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gAlbedo);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gMetalRoughAo);
    }

    void SetPointLights(std::vector<PointLight>& lights){
        pbrShader.use();
        for (unsigned int i = 0; i < lights.size(); ++i)
        {
            pbrShader.setVec3("pointLights[" + std::to_string(i) + "].Position", lights[i].position_);
            pbrShader.setVec3("pointLights[" + std::to_string(i) + "].Color", lights[i].color_);
            pbrShader.setFloat("pointLights[" + std::to_string(i) + "].Linear", lights[i].linear_);
            pbrShader.setFloat("pointLights[" + std::to_string(i) + "].Quadratic", lights[i].quadratic_);

            const float constant = 1.0f;
            float linear = lights[i].linear_;
            float quadratic = lights[i].quadratic_;

            const float maxBrightness = std::fmaxf(std::fmaxf(lights[i].color_.r, lights[i].color_.g), lights[i].color_.b);
            float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
            pbrShader.setFloat("pointLights[" + std::to_string(i) + "].Radius", radius);
        }
    }

    void SetDirLights(std::vector<DirLight>& lights){
        pbrShader.use();
        for (unsigned int i = 0; i < lights.size(); ++i)
        {
            pbrShader.setVec3("dirLights[" + std::to_string(i) + "].Position", lights[i].position_);
            pbrShader.setVec3("dirLights[" + std::to_string(i) + "].Color", lights[i].color_);
        }
    }

    void SetShadowInfo(ShadowShader& shadowShader){
        pbrShader.use();
        pbrShader.setVec3("lightDir", shadowShader.lightDir);
        pbrShader.setMat4("view", shadowShader.camera.GetViewMatrix());
        pbrShader.setFloat("farPlane", shadowShader.camera.far_plane_);
        pbrShader.setInt("cascadeCount", shadowShader.shadowCascadeLevels.size());
        for (size_t i = 0; i < shadowShader.shadowCascadeLevels.size(); ++i)
        {
            pbrShader.setFloat("cascadePlaneDistances[" + std::to_string(i) + "]", shadowShader.shadowCascadeLevels[i]);
        }
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D_ARRAY, shadowShader.shadowDepthMap);
    }

    void SetSSAOInfo(SSAOShader& ssaoShader){
        pbrShader.use();
        glActiveTexture(GL_TEXTURE8);
        glBindTexture(GL_TEXTURE_2D, ssaoShader.ssaoColorBufferBlur);
    }

    void DeferredRender(const glm::vec3& campos){
        pbrShader.use();
        pbrShader.setVec3("camPos", campos);
        Mesh::RenderQuad();
    }

};

class PostProcessShader{
public:
    Shader postProcessShader;
    PostProcessShader(string dir):
        postProcessShader(dir + "Shaders/postprocess.vs", dir + "Shaders/postprocess.fs"){
        postProcessShader.use();
        postProcessShader.setInt("colorMap", 0);
        postProcessShader.setInt("normalMap", 1);
        postProcessShader.setInt("positionMap", 2);
        postProcessShader.setInt("depthMap", 3);
    }

    void Draw(PBRShader& pbrShader, Gbuffer& gBuffer){
        postProcessShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pbrShader.pbrColor); 
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, pbrShader.pbrDepth);
        Mesh::RenderQuad();
    }

};


}

#endif
