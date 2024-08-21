#ifndef MODEL_H
#define MODEL_H

#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
namespace sablin{

unsigned int TextureFromFile(string filename, const string &directory, bool gamma = false);
unsigned int TextureHDRFromFile(string filename, const string &directory, bool gamma = false);

class Model 
{
private:
    void LoadModel(const string &path)
    {
        Assimp::Importer importer;
        // 从文件加载
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // 检查错误
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // 设定目录路径
        directory_ = path.substr(0, path.find_last_of('/'));

        // 递归地处理Scene Node
        ProcessNode(scene->mRootNode, scene);
    }

    void ProcessNode(aiNode *node, const aiScene *scene)
    {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes_.push_back(ProcessMesh(mesh, scene));
        }

        for(unsigned int i = 0; i < node->mNumChildren; i++)
            ProcessNode(node->mChildren[i], scene);

    }

    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // 遍历每个顶点
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;

            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position_ = vector;

            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal_ = vector;
            }

            // texture coordinates
            if(mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.tex_coords_ = vec;

                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.tangent_ = vector;

                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.bitangent_ = vector;
            }
            else{
                vertex.tex_coords_ = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        // 遍历三角形face填充索引
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);        
        }

        // 处理材质
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    

        float normal_map_alpha = 0.0f;
        float ao_map_alpha = 0.0f;
        // 1. albedo map
        vector<Texture> albedoMaps = LoadMaterialTextures(material, aiTextureType_BASE_COLOR, "albedoMap");
        textures.insert(textures.end(), albedoMaps.begin(), albedoMaps.end());
        // 2. normal map
        vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "normalMap");
        if(!normalMaps.empty()) normal_map_alpha = 1.0f;
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 3. metallic map
        std::vector<Texture> metallicMaps = LoadMaterialTextures(material, aiTextureType_METALNESS, "metallicMap");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. roughness map
        std::vector<Texture> roughnessMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "roughnessMap");
        textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
        // 5. ao map
        std::vector<Texture> aoMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, "aoMap");
        if(!aoMaps.empty()) ao_map_alpha = 1.0f;
        textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
        
        return Mesh(vertices, indices, textures, normal_map_alpha, ao_map_alpha);
    }

    vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, string type_name)
    {
        vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            // 检查texture是否早已加载
            for(unsigned int j = 0; j < loaded_textures_.size(); j++)
            {
                if(std::strcmp(loaded_textures_[j].path_.data(), str.C_Str()) == 0)
                {
                    textures.push_back(loaded_textures_[j]);
                    skip = true;
                    break;
                }
            }

            // 需要加载texture
            if(!skip){   
                Texture texture;
                texture.id_ = TextureFromFile(str.C_Str(), directory_);
                texture.type_ = type_name;
                texture.path_ = str.C_Str();
                textures.push_back(texture);
                loaded_textures_.push_back(texture);
            }
        }
        return textures;
    }
public:
    vector<Texture> loaded_textures_;
    vector<Mesh>    meshes_;
    string directory_;
    bool gamma_;

    Model(string const &path, bool gamma = false) :
        gamma_(gamma)
    {
        LoadModel(path);
    }

    void Draw(Shader &shader)
    {
        for(Mesh& mesh: meshes_)
            shader.Draw(mesh);
    }
};

}
#endif
