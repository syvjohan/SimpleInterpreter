#include "Manager.h"
#include "Console.h"
#include "Lexical.h"
#include "HelpHeaders.h"
#include "ErrorHeaders.h"
#include "Defines.h"

#include <stdio.h>
#include <windows.h>

//*****
//Manager::Manager
//*****
Manager::Manager() {}

//*****
//Manager::~Manager
//*****
Manager::~Manager() {
	delete lexical;
}

//*****
//Manager::Initialize
//*****
void Manager::Initialize() {
	SetConsoleSize();
	ConsoleBackgroundAndTextColors( 15, 0 ); //Change text color to white.

	lexical = new Partitioning::Lexical();

	Input();
}

//*****
//Manager::PreInterpreting
//*****
void Manager::PreInterpreting(const char *path) {
	const size_t size = lexical->CalculateFileSize( path );
	lexical->fileSize = size;
	char *code = DBG_NEW char[size]; //deleted inside Lexical.cpp destructor.

	lexical->ReadFile( code, path );

	lexical->SetCode( code );
	code = lexical->RegisterAllIncludes();
	lexical->SetCode( code );

	lexical->RegisterAllStructs();

	lexical->RegisterAllSubroutines();

	//Need to be set again because of includes, struct and subroutines registration.
	lexical->SetCode( code );
}

//*****
//Manager::Interpreting
//*****
void Manager::Interpreting() {
	lexical->GetInstructions();
}

//*****
//Manager::Input
//*****
void Manager::Input() {
	printf( "Insert file path and press Enter to start interpretation.\n" );
	printf( "Search in folder project\\" );

	char buffer[PATHSIZE];
	fgets( buffer, PATHSIZE, stdin ); //read input from console(path).
	
	//DEBUG PURPOSE.
	memcpy(buffer, "testing.main.q\n", 15);
	buffer[15] = '\0';
	if ( IsCorrectMainFileType( buffer ) ) {
		char path[PATHSIZE * 5];
		CreateFilePath( path, (const char *)buffer ); //Get current directory and create a new path.

		if ( IsFileExist( path ) ) {
			PreInterpreting( path );
			Interpreting(); //Start interpreting.

			printf( "\n\n" );
			printf( "\nInterpretation finished!\n\n" );

			//Create a new object for further interpretations and clean up memory.
			delete lexical;
			Partitioning::Lexical *lexical = new Partitioning::Lexical();

			Input(); //Recursion.
		}
	}

	ConsoleBackgroundAndTextColors( 4, 0 ); //Change text color to red.
	printf( "\nCannot identify file. Observe that allowed file format is filename.main.q\n" );

	ConsoleBackgroundAndTextColors( 15, 0 ); //Change text color to white.
	printf( "\nPlease try again!\n\n" );
	Input(); //Recursion.

}

//*****
//Manager::IsCorrectMainFileType
//*****
bool Manager::IsCorrectMainFileType( const char *cStr ) {
	const char *findDot = strstr( cStr, "." );
	if ( findDot ) {
		int len = strlen( findDot );
		if ( len == 8 ) {
			if ( Global::HelpClass::StrCmp(findDot, ".main.q\n") ) {
				return true;
			}
		}
	} 

	return false;
}

//*****
//Manager::DoFileExist
//*****
bool Manager::IsFileExist( const char *path ) {
	FILE *file;
	file = fopen( path, "r" );
	fclose( file );

	if ( !file ) {
		return false;
	}
	return true;
}

//*****
//Manager::CreateFilePath
//*****
void Manager::CreateFilePath( char *buffer, const char *inputPath ) {
	GetCurrentDirectoryOnWindows( buffer );

	int len = strlen( buffer );
	for ( int i = len; i != 0; --i ) {
		if ( buffer[i] == '\\' ) {
			 buffer[i] = '\0';
			 break;
	    }
	}

	len = strlen( buffer );
	memcpy( buffer + len, "\\project\\", 11 );
	len += 9;

	int len2 = strlen( inputPath ) -1; //Remove '\n'
	memcpy( buffer + len, inputPath, len2 );	
	buffer[len + len2] = '\0';
}

//*****
//Manager::GetCurrentDirectoryOnWindows
//*****
void Manager::GetCurrentDirectoryOnWindows(char *buffer) {
	const unsigned long maxDir = PATHSIZE;
	GetCurrentDirectory( maxDir, buffer );
}
