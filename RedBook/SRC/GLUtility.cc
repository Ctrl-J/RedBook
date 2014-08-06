#include <Precompiled.h>
#include <GLUtility.h>

GLUtility::GLUtility()
{
    logFilename = L"";
}

GLUtility::~GLUtility()
{
}

GLUtility &GLUtility::Instance(const std::wstring &setLogFilename)
{
    static GLUtility utilityInstance;
    utilityInstance.logFilename = setLogFilename;
    return utilityInstance;
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

void GLUtility::LogGLMessage( LogLevel outputLogLevel, GLenum ErrorStatus)
{
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

    std::wstring errorString = L"GL error: " + getGLStringFromCode( ErrorStatus );

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