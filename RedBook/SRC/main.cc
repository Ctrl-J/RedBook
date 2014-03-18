#include <Precompiled.h>
#include <Application.h>

//
//  Not much to the main function. Window and context creation are handled within Application::Initialize,
//  and the main loop is controlled within Application::Run, any memory and/or context cleanup that isn't
//  automatically taken care of will occur within Application::Shutdown.
// 
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd_line, INT cmd_show )
{
    std::shared_ptr<Application> application = std::make_shared<Application>( L"Redbook Learning" );
    if( application == nullptr )
    {
        MessageBox( NULL, L"Error allocating application", L"UH OH", MB_OK | MB_ICONERROR );
        return -1;
    }

    if( application->Initialize() == false )
    {
        MessageBox( NULL, L"Error initializing application", L"UH OH", MB_OK | MB_ICONERROR );
        return -2;
    }

    application->Run();

    application->Shutdown();
    
    return 0;
}