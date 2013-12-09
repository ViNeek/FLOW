//
//  fwUtilities.cpp
//  FLOW
//
//  Created by Nick Vitsas on 9/11/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

/*
    C:\Users\Nick\Documents\Visual Studio 2010\Projects\FLOW>protoc.exe --proto_path=. --cpp_out=. PRIME.proto
*/
#include "fwUtilities.h"

namespace Utilities {

void SetStdOutToNewConsole()
{
#ifdef _WIN32
    // allocate a console for this app
    AllocConsole();

    // redirect unbuffered STDOUT to the console
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
    FILE *fp = _fdopen( fileDescriptor, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );

    consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
    fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
    fp = _fdopen( fileDescriptor, "w" );
    *stderr = *fp;

    setvbuf( stderr, NULL, _IONBF, 0 );

    SetConsoleTitle(L"Debug Output");

    // give the console window a bigger buffer size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if ( GetConsoleScreenBufferInfo(consoleHandle, &csbi) )
    {
        COORD bufferSize;
        bufferSize.X = csbi.dwSize.X;
        bufferSize.Y = 30000;
        SetConsoleScreenBufferSize(consoleHandle, bufferSize);
    }

#endif
}

}