#pragma once

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/ShaderProgram.hpp"

/**
 * Simple single color material.
 * This material uses no lights.
 * This material demonstrates how to render a single color material, and how to cache all attributes, uniforms and pass in a single MVP matrix to the shader.
 */
class HUDMaterial
{
    public:
		HUDMaterial(glm::vec3 pColor = glm::vec3(1,0,0));
        virtual ~HUDMaterial();

        void render();

        //in rgb values
        void setDiffuseColor (glm::vec3 pDiffuseColor);
    private:
        //all the static properties are shared between instances of ColorMaterial
        //note that they are all PRIVATE, we do not expose this static info to the outside world
		static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        //in this example we cache all identifiers for uniforms & attributes
        static GLint _uMVPMatrix;
        static GLint _uDiffuseColor;

        static GLint _aVertex ;
        static GLint _aNormal;
        static GLint _aUV ;
        static GLint _aTangent;
        static GLint _aBitangent;

        //this one is unique per instance of color material
        glm::vec3 _diffuseColor;
};