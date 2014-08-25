#include <Precompiled.h>
#include <GLUtility.h>

PFNGLATTACHSHADERPROC               glAttachShader = NULL;
PFNGLBINDBUFFERPROC                 glBindBuffer = NULL;
PFNGLBINDBUFFERBASEPROC             glBindBufferBase = NULL;
PFNGLBINDBUFFERRANGEPROC            glBindBufferRange = NULL;
PFNGLBINDVERTEXARRAYPROC            glBindVertexArray = NULL;
PFNGLBUFFERDATAPROC                 glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC              glBufferSubData = NULL;
PFNGLCOMPILESHADERPROC              glCompileShader = NULL;
PFNGLCREATEPROGRAMPROC              glCreateProgram = NULL;
PFNGLCREATESHADERPROC               glCreateShader = NULL;
PFNGLDELETESHADERPROC               glDeleteShader = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray = NULL;
PFNGLGENBUFFERSPROC                 glGenBuffers = NULL;
PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays = NULL;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC    glGetActiveUniformBlockIV = NULL;
PFNGLGETACTIVEUNIFORMSIVPROC        glGetActiveUniformsIV = NULL;
PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIVPROC               glGetProgramIV = NULL;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog = NULL;
PFNGLGETSHADERIVPROC                glGetShaderIV = NULL;
PFNGLGETUNIFORMBLOCKINDEXPROC       glGetUniformBlockIndex = NULL;
PFNGLGETUNIFORMINDICESPROC          glGetUniformIndices = NULL;
PFNGLLINKPROGRAMPROC                glLinkProgram = NULL;
PFNGLSHADERSOURCEPROC               glShaderSource = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC        glUniformBlockBinding = NULL;
PFNGLUSEPROGRAMPROC                 glUseProgram = NULL;
PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB = NULL;
PFNWGLCREATECONTEXTATTRIBSARBPROC   wglCreateContextAttribsARB = NULL;
PFNWGLSWAPINTERVALEXTPROC           wglSwapIntervalEXT = NULL;

GLUtility *GLUtility::instance = nullptr;

GLUtility::GLUtility()
{
    logFilename = L"";
    instance = nullptr;
}

GLUtility::~GLUtility()
{
    if( instance != nullptr )
    {
        delete instance;
        instance = nullptr;
    }
}

GLUtility *GLUtility::Instance(const std::wstring &setLogFilename)
{
    if( instance == nullptr )
    {
        instance = new GLUtility();
        instance->logFilename = setLogFilename;
    }

    if( instance->logFilename.compare( setLogFilename.c_str() ) != 0 )
    {
        instance->logFilename = setLogFilename;
    }

    return instance;
}

    
bool GLUtility::IsStringEmpty( const std::wstring &input )
{
    if( input.find_first_not_of( L" \t" ) == std::wstring::npos )
    {
        return true;
    }

    return false;
}

std::wstring GLUtility::GetTimeString( void )
{
    std::wstringstream stringStream;
    std::time_t timeValue = std::time( nullptr );
    auto getLocalTime = std::localtime( &timeValue );
    stringStream << (getLocalTime->tm_year + 1900) << L"-" << std::setw(2) << std::setfill(static_cast<wchar_t>('0')) << (getLocalTime->tm_mon + 1) << L"-" << getLocalTime->tm_mday << L" ("
        << getLocalTime->tm_hour << L":" << getLocalTime->tm_min << L":" << getLocalTime->tm_sec << L")";

    return stringStream.str();
}

void GLUtility::LogMessage( LogLevel outputLogLevel, const std::wstring &message)
{
    std::wstring timeString;
    timeString = GetTimeString();

    std::wstringstream stringStream;
    stringStream << L"Log Message - ";
    switch( outputLogLevel )
    {
    case GLUtility::LOG_CRITICAL:
        stringStream << L"CRITICAL [" << timeString << L"]";
        break;
    case GLUtility::LOG_ERROR:
        stringStream << L"ERROR [" << timeString << L"]";
        break;
    case GLUtility::LOG_WARNING:
        stringStream << L"WARNING [" << timeString << L"]";
        break;
    case GLUtility::LOG_INFO:
    default:
        stringStream << L"INFO [" << timeString << L"]";
        break;
    }
    
    if( IsStringEmpty( logFilename ) )
    {
        MessageBox( NULL, message.c_str(), stringStream.str().c_str(), MB_OK );
    }
    else
    {
        std::wofstream file( logFilename, std::ios::app);

        file << stringStream.str() << L": " << message << std::endl;

        file.close();
    }
}

void GLUtility::LogGLMessage( LogLevel outputLogLevel, const std::wstring &optionalMessage)
{
    GLenum error = glGetError();
    bool noError = false;
    bool emptyMessage = false;

    if( error == GL_NO_ERROR )
    {
        return;
    }
    if( IsStringEmpty( optionalMessage ) == true )
    {
        emptyMessage = true;
    }

    std::wstring timeString;
    timeString = GetTimeString();

    std::wstringstream stringStream;
    stringStream << L"GL ";
    switch( outputLogLevel )
    {
    case GLUtility::LOG_CRITICAL:
        stringStream << L"CRITICAL [" << timeString << L"]";
        break;
    case GLUtility::LOG_ERROR:
        stringStream << L"ERROR [" << timeString << L"]";
        break;
    case GLUtility::LOG_WARNING:
        stringStream << L"WARNING [" << timeString << L"]";
        break;
    case GLUtility::LOG_INFO:
    default:
        stringStream << L"INFO [" << timeString << L"]";
        break;
    }

    std::wstring errorString = L"GL error: " + getGLStringFromCode( error );
    if( emptyMessage == false )
    {
        errorString += L"\n" + optionalMessage;
    }

    if( IsStringEmpty( logFilename ) )
    {
        MessageBox( NULL, errorString.c_str(), stringStream.str().c_str(), MB_OK );
    }
    else
    {
        std::wofstream file( logFilename, std::ios::app );

        file << stringStream.str() << L" - " + errorString << std::endl;

        file.close();
    }
}

std::wstring GLUtility::getGLStringFromCode( GLenum errorCode )
{
    std::wstring output;
    switch( errorCode )
    {
    case GL_INVALID_ENUM:
        output = L"Invalid Enum";
        break;
    case GL_INVALID_VALUE:
        output = L"Invalid Value";
        break;
    case GL_INVALID_OPERATION:
        output = L"Invalid Operation";
        break;
    case GL_STACK_OVERFLOW:
        output = L"Stack Overflow";
        break;
    case GL_STACK_UNDERFLOW:
        output = L"Stack Underflow";
        break;
    case GL_OUT_OF_MEMORY:
        output = L"Out of Memory";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        output = L"Invalid Framebuffer Operation";
        break;
    case GL_TABLE_TOO_LARGE:
        output = L"Table Too Large";
        break;
    default:
        output = L"Undefined Error (check code)";
        break;
    }

    return output;
}

size_t GLUtility::TypeSize( GLenum type )
{
    size_t output;

    switch( type )
    {
    case GL_FLOAT:
        output = sizeof(GLfloat);
        break;
    case GL_FLOAT_VEC2:
        output = sizeof( GLfloat ) * 2;
        break;
    case GL_FLOAT_VEC3:
        output = sizeof( GLfloat ) * 3;
        break;
    case GL_FLOAT_VEC4:
        output = sizeof( GLfloat ) * 4;
        break;
    case GL_INT:
        output = sizeof( GLint );
        break;
    case GL_INT_VEC2:
        output = sizeof( GLint ) * 2;
        break;
    case GL_INT_VEC3:
        output = sizeof( GLint ) * 3;
        break;
    case GL_INT_VEC4:
        output = sizeof( GLint ) * 4;
        break;
    case GL_UNSIGNED_INT:
        output = sizeof( GLuint );
        break;
    case GL_UNSIGNED_INT_VEC2:
        output = sizeof( GLuint ) * 2;
        break;
    case GL_UNSIGNED_INT_VEC3:
        output = sizeof( GLuint ) * 3;
        break;
    case GL_UNSIGNED_INT_VEC4:
        output = sizeof( GLuint ) * 4;
        break;
    case GL_BOOL:
        output = sizeof( GLboolean );
        break;
    case GL_BOOL_VEC2:
        output = sizeof( GLboolean ) * 2;
        break;
    case GL_BOOL_VEC3:
        output = sizeof( GLboolean ) * 3;
        break;
    case GL_BOOL_VEC4:
        output = sizeof( GLboolean ) * 4;
        break;
    case GL_FLOAT_MAT2:
        output = sizeof( GLfloat ) * 4;
        break;
    case GL_FLOAT_MAT2x3:
        output = sizeof( GLfloat ) * 6;
        break;
    case GL_FLOAT_MAT2x4:
        output = sizeof( GLfloat ) * 8;
        break;
    case GL_FLOAT_MAT3x2:
        output = sizeof( GLfloat ) * 6;
        break;
    case GL_FLOAT_MAT3:
        output = sizeof( GLfloat ) * 9;
        break;
    case GL_FLOAT_MAT3x4:
        output = sizeof( GLfloat ) * 12;
        break;
    case GL_FLOAT_MAT4x2:
        output = sizeof( GLfloat ) * 8;
        break;
    case GL_FLOAT_MAT4x3:
        output = sizeof( GLfloat ) * 12;
        break;
    case GL_FLOAT_MAT4:
        output = sizeof( GLfloat ) * 16;
        break;
    default:
        GLUtility::Instance()->LogMessage( GLUtility::LogLevel::LOG_ERROR, L"Error finding GLsize." );
        exit( EXIT_FAILURE );
        break;
    }

    return output;
}

bool GLUtility::InitializeGLExtensions( void )
{
    glAttachShader = ( PFNGLATTACHSHADERPROC )wglGetProcAddress( "glAttachShader" );
    if( glAttachShader == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glAttachShader extension address" );
        return false;
    }
    
    glBindBuffer = ( PFNGLBINDBUFFERPROC )wglGetProcAddress( "glBindBuffer" );
    if( glBindBuffer == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glBindBuffer extension address" );
        return false;
    }

    glBindBufferBase = ( PFNGLBINDBUFFERBASEPROC )wglGetProcAddress( "glBindBufferBase" );
    if( glBindBufferBase == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glBindBufferBase extension address" );
        return false;
    }

    glBindBufferRange = ( PFNGLBINDBUFFERRANGEPROC )wglGetProcAddress( "glBindBufferRange" );
    if( glBindBufferRange == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glBindBufferRange extension address" );
        return false;
    }
    
    
    glBindVertexArray = ( PFNGLBINDVERTEXARRAYPROC )wglGetProcAddress( "glBindVertexArray" );
    if( glBindVertexArray == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glBindVertexArray extension address" );
        return false;
    }

    glBufferData = ( PFNGLBUFFERDATAPROC )wglGetProcAddress( "glBufferData" );
    if( glBufferData == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glBufferData extension address" );
        return false;
    }

    glBufferSubData = ( PFNGLBUFFERSUBDATAPROC )wglGetProcAddress( "glBufferSubData" );
    if( glBufferSubData == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glBufferSubData extension address" );
        return false;
    }
    
    glCreateProgram = ( PFNGLCREATEPROGRAMPROC )wglGetProcAddress( "glCreateProgram" );
    if( glCreateProgram == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glCreateProgram extension address" );
        return false;
    }
    
    glCompileShader = ( PFNGLCOMPILESHADERPROC )wglGetProcAddress( "glCompileShader" );
    if( glCompileShader == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glCompileShader extension address" );
        return false;
    }
    
    glCreateShader = ( PFNGLCREATESHADERPROC )wglGetProcAddress( "glCreateShader" );
    if( glCreateShader == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glCreateShader extension address" );
        return false;
    }
    
    glDeleteShader = ( PFNGLDELETESHADERPROC )wglGetProcAddress( "glDeleteShader" );
    if( glDeleteShader == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glDeleteShader extension address" );
        return false;
    }
    
    glEnableVertexAttribArray = ( PFNGLENABLEVERTEXATTRIBARRAYPROC )wglGetProcAddress( "glEnableVertexAttribArray" );
    if( glEnableVertexAttribArray == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glEnableVertexAttribArray extension address" );
        return false;
    }
    
    glGenBuffers = ( PFNGLGENBUFFERSPROC )wglGetProcAddress( "glGenBuffers" );
    if( glGenBuffers == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glGenBuffers extension address" );
        return false;
    }
    
    glGenVertexArrays = ( PFNGLGENVERTEXARRAYSPROC )wglGetProcAddress( "glGenVertexArrays" );
    if( glGenVertexArrays == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glGenVertexArrays extension address" );
        return false;
    }

    glGetActiveUniformBlockIV = ( PFNGLGETACTIVEUNIFORMBLOCKIVPROC )wglGetProcAddress( "glGetActiveUniformBlockiv" );
    if( glGetActiveUniformBlockIV == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glGetActiveUniformBlockIV extension address" );
        return false;
    }
    
    glGetActiveUniformsIV = ( PFNGLGETACTIVEUNIFORMSIVPROC )wglGetProcAddress( "glGetActiveUniformsiv" );
    if( glGetActiveUniformsIV == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glEnableVertexAttribArray extension address" );
        return false;
    }
    
    glGetProgramInfoLog = ( PFNGLGETPROGRAMINFOLOGPROC )wglGetProcAddress( "glGetProgramInfoLog" );
    if( glGetProgramInfoLog == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glGetProgramInfoLog extension address" );
        return false;
    }
    
    glGetProgramIV = ( PFNGLGETPROGRAMIVPROC )wglGetProcAddress( "glGetProgramiv" );
    if( glGetProgramIV == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glGetProgramIV extension address" );
        return false;
    }
    
    glGetShaderInfoLog = ( PFNGLGETSHADERINFOLOGPROC )wglGetProcAddress( "glGetShaderInfoLog" );
    if( glGetShaderInfoLog == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glGetShaderInfoLog extension address" );
        return false;
    }
    
    glGetShaderIV = ( PFNGLGETSHADERIVPROC )wglGetProcAddress( "glGetShaderiv" );
    if( glGetProgramIV == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glGetShaderIV extension address" );
        return false;
    }
    
    glGetUniformBlockIndex = ( PFNGLGETUNIFORMBLOCKINDEXPROC )wglGetProcAddress( "glGetUniformBlockIndex" );
    if( glGetUniformBlockIndex == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glGetUniformBlockIndex extension address" );
        return false;
    }
    
    glGetUniformIndices = ( PFNGLGETUNIFORMINDICESPROC )wglGetProcAddress( "glGetUniformIndices" );
    if( glGetUniformIndices == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glGetUniformIndices extension address" );
        return false;
    }
    
    glLinkProgram = ( PFNGLLINKPROGRAMPROC )wglGetProcAddress( "glLinkProgram" );
    if( glLinkProgram == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glLinkProgram extension address" );
        return false;
    }
     
    glShaderSource = ( PFNGLSHADERSOURCEPROC )wglGetProcAddress( "glShaderSource" );
    if( glShaderSource == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glShaderSource extension address" );
        return false;
    }

    glUniformBlockBinding = ( PFNGLUNIFORMBLOCKBINDINGPROC )wglGetProcAddress( "glUniformBlockBinding" );
    if( glUniformBlockBinding == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glUniformBlockBinding extension address" );
        return false;
    }

    glUseProgram = ( PFNGLUSEPROGRAMPROC )wglGetProcAddress( "glUseProgram" );
    if( glUseProgram == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glUseProgram extension address" );
        return false;
    }
    
    glVertexAttribPointer = ( PFNGLVERTEXATTRIBPOINTERPROC )wglGetProcAddress( "glVertexAttribPointer" );
    if( glVertexAttribPointer == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glVertexAttribPointer extension address" );
        return false;
    }

    wglChoosePixelFormatARB = ( PFNWGLCHOOSEPIXELFORMATARBPROC )wglGetProcAddress( "wglChoosePixelFormatARB" );
    if( wglChoosePixelFormatARB == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glChoosePixelFormatARB extension address" );
        return false;
    }

    wglCreateContextAttribsARB = ( PFNWGLCREATECONTEXTATTRIBSARBPROC )wglGetProcAddress( "wglCreateContextAttribsARB" );
    if( wglCreateContextAttribsARB == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glCreateContextAttribsARB extension address" );
        return false;
    }
    
    wglSwapIntervalEXT = ( PFNWGLSWAPINTERVALEXTPROC )wglGetProcAddress( "wglSwapIntervalEXT" );
    if( wglSwapIntervalEXT == NULL )
    {
        LogMessage( LogLevel::LOG_CRITICAL, L"Error loading glSwapIntervalEXT extension address" );
        return false;
    }
}