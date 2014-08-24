#pragma once
#include <Precompiled.h>

//Loaded gl extension methods
extern PFNGLATTACHSHADERPROC                glAttachShader;
extern PFNGLBINDBUFFERPROC                  glBindBuffer;
extern PFNGLBINDBUFFERBASEPROC              glBindBufferBase;
extern PFNGLBINDBUFFERRANGEPROC             glBindBufferRange;
extern PFNGLBINDVERTEXARRAYPROC             glBindVertexArray;
extern PFNGLBUFFERDATAPROC                  glBufferData;
extern PFNGLBUFFERSUBDATAPROC               glBufferSubData;
extern PFNGLCOMPILESHADERPROC               glCompileShader;
extern PFNGLCREATEPROGRAMPROC               glCreateProgram;
extern PFNGLCREATESHADERPROC                glCreateShader;
extern PFNGLDELETESHADERPROC                glDeleteShader;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC     glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC                  glGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC             glGenVertexArrays;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC     glGetActiveUniformBlockIV;
extern PFNGLGETACTIVEUNIFORMSIVPROC         glGetActiveUniformsIV;
extern PFNGLGETPROGRAMINFOLOGPROC           glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC                glGetProgramIV;
extern PFNGLGETSHADERINFOLOGPROC            glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC                 glGetShaderIV;
extern PFNGLGETUNIFORMBLOCKINDEXPROC        glGetUniformBlockIndex;
extern PFNGLGETUNIFORMINDICESPROC           glGetUniformIndices;
extern PFNGLLINKPROGRAMPROC                 glLinkProgram;
extern PFNGLSHADERSOURCEPROC                glShaderSource;
extern PFNGLUNIFORMBLOCKBINDINGPROC         glUniformBlockBinding;
extern PFNGLUSEPROGRAMPROC                  glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC         glVertexAttribPointer;
extern PFNWGLCHOOSEPIXELFORMATARBPROC       wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC    wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC            wglSwapIntervalEXT;


class GLUtility
{
public:
    static GLUtility &Instance(const std::wstring &setLogFilename = L"");
    ~GLUtility();

    enum LogLevel
    {
        LOG_CRITICAL,
        LOG_ERROR,
        LOG_WARNING,
        LOG_INFO
    };

    bool IsStringEmpty( const std::wstring &input );
    std::wstring GetTimeString( void );
    void LogMessage( LogLevel outputLogLevel, const std::wstring &message);
    void LogGLMessage( LogLevel outputLogLevel, const std::wstring &optionalMessage = L"" );
    bool InitializeGLExtensions( void );
    size_t TypeSize( GLenum type );

private:
    GLUtility();
    GLUtility( const GLUtility &other ) = delete;
    GLUtility &operator= ( const GLUtility &other ) = delete;

    std::wstring getGLStringFromCode( GLenum errorCode );
    
    std::wstring logFilename;
};