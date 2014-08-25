#pragma once
#include <Precompiled.h>
#include <ChapterBase.h>

class ChapterTwo : public ChapterBase
{
public:

    ChapterTwo( Application *appPointer );
    ~ChapterTwo( void );
    ChapterTwo( const ChapterTwo &other ) = delete;
    ChapterTwo &operator= ( const ChapterTwo &other ) = delete;

    void Initialize( void );
    void Draw( void );
    void Step( double time_step );
    void Shutdown( void );

private:
    void setupUniformBlock( void );
    void updateUniformBlock( const glm::vec3 &translation, const GLfloat &scale, const glm::vec4 &rotation, const bool &enabled );

    enum class vertex_array_object_identifiers
    {
        triangles = 0,
        num_arrays
    };

    enum class buffer_object_identifiers
    {
        array_buffer = 0,
        num_buffers
    };

    enum class attribute_object_identifiers
    {
        vertex_position = 0,
        num_attributes
    };

    enum class uniform_identifiers
    {
        rotation = 0,
        translation,
        scale,
        enabled,
        num_uniforms
    };

    std::vector<GLuint> vertex_array_objects;
    std::vector<GLuint> buffer_objects;

    const GLuint num_vertices;

    struct UniformBlockStructure
    {
        glm::vec3 translation;
        GLfloat scale;
        glm::vec4 rotation;
        bool enabled;
    };

    double scaleSpeed;
    std::shared_ptr<UniformBlockStructure> blockAttributes;
    glm::vec3 clearColor;
    glm::vec3 clearColorSpeed;

    GLuint shaderProgram;

    GLint numberOfActiveUniforms;

    GLint  uniformBlockIndex;
    GLuint uniformBlockObject;
    GLint  uniformBlockLocation;
    GLint  uniformBlockSize;
    GLint  uniformBindingIndex;
    
    GLuint uniformIndices[static_cast<int>(uniform_identifiers::num_uniforms)];
    GLint  uniformSizes[static_cast<int>(uniform_identifiers::num_uniforms)];
    GLint  uniformOffsets[static_cast<int>(uniform_identifiers::num_uniforms)];
    GLint  uniformTypes[static_cast<int>(uniform_identifiers::num_uniforms)];

    double commandDelay;
    double commandTimer;

    std::vector<GLfloat> bufferData;
};