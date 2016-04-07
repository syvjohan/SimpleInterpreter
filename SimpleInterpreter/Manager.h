#pragma once

namespace Partitioning {
	class Lexical;
}

class Manager {
public:
	Manager();
	~Manager();

	void Initialize();

private: 
	Partitioning::Lexical *lexical;

	void PreInterpreting(const char *buffer);
	void Interpreting();
	void Input();
	bool IsCorrectMainFileType( const char *cStr );
	bool IsFileExist(const char *path);
	void CreateFilePath( char *buffer, const char *inputPath);
	void GetCurrentDirectoryOnWindows( char *buffer );
};
