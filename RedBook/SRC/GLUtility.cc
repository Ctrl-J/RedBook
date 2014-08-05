#include <Precompiled.h>
#include <GLUtility.h>

void GLUtility::SetLogFilename( std::wstring filename )
{
    logFilename = filename;
}
    
bool GLUtility::IsStringEmpty( std::wstring input )
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

void GLUtility::LogMessage( LogLevel outputLogLevel, std::wstring message)
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
        MessageBox( NULL, stringStream.str().c_str(), message.c_str(), MB_OK );
    }
    else
    {
        std::wofstream file( logFilename, std::ios::app);

        file << stringStream.str() << L": " << message << std::endl;

        file.close();
    }
}

void GLUtility::LogGLMessage( LogLevel outputLogLevel, GLuint ErrorStatus)
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
    
    std::wstring titleMessage = stringStream.str();

    stringStream.str( L"" );
    stringStream.clear();
        
    stringStream << "GL error code: " << ErrorStatus;

    if( IsStringEmpty( logFilename ) )
    {
        MessageBox( NULL, titleMessage.c_str(), stringStream.str().c_str(), MB_OK );
    }
    else
    {
        std::wofstream file( logFilename, std::ios::app );

        file << titleMessage << L": " << stringStream.str() << std::endl;

        file.close();
    }
       

}
