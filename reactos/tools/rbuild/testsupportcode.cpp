/*
 * Copyright (C) 2005 Casper S. Hornstrup
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include "pch.h"
#include <assert.h>

#include "rbuild.h"

using std::string;
using std::vector;

static std::string
GetFilename ( const std::string& filename )
{
	size_t index = filename.find_last_of ( cSep );
	if ( index == string::npos )
		return filename;
	else
		return filename.substr ( index + 1, filename.length () - index );
}

TestSupportCode::TestSupportCode ( const Project& project )
	: project ( project )
{
}

TestSupportCode::~TestSupportCode ()
{
}

bool
TestSupportCode::IsTestModule ( const Module& module )
{
	return module.type == Test;
}

void
TestSupportCode::GenerateTestSupportCode ( bool verbose )
{
	for( std::map<std::string, Module*>::const_iterator p = project.modules.begin(); p != project.modules.end(); ++ p )
	{
		if ( IsTestModule ( *p->second ) )
		{
			GenerateTestSupportCodeForModule ( *p->second,
			                                   verbose );
		}
	}
}

void
TestSupportCode::GenerateTestSupportCodeForModule ( Module& module,
                                                    bool verbose )
{
	if ( verbose )
	{
		printf ( "\nGenerating test support code for %s",
		         module.name.c_str () );
	}

	WriteHooksFile ( module );
	WriteStubsFile ( module );
	WriteStartupFile ( module );
}

string
TestSupportCode::GetHooksFilename ( Module& module )
{
	return NormalizeFilename ( Environment::GetIntermediatePath () + sSep + module.output->relative_path + sSep + "_hooks.c" );
}

char*
TestSupportCode::WriteStubbedSymbolToHooksFile ( char* buffer,
                                                 const StubbedComponent& component,
                                                 const StubbedSymbol& symbol )
{
	buffer = buffer + sprintf ( buffer,
	                            "  {\"%s\", \"%s\", NULL, NULL, NULL},\n",
	                            component.name.c_str (),
	                            symbol.newname.c_str () );
	return buffer;
}

char*
TestSupportCode::WriteStubbedComponentToHooksFile ( char* buffer,
                                                    const StubbedComponent& component )
{
	for ( size_t i = 0; i < component.symbols.size () ; i++ )
		buffer = WriteStubbedSymbolToHooksFile ( buffer,
		                                         component,
		                                         *component.symbols[i] );
	return buffer;
}

void
TestSupportCode::WriteHooksFile ( Module& module )
{
	char* buf;
	char* s;

	buf = (char*) malloc ( 50*1024 );
	if ( buf == NULL )
		throw OutOfMemoryException ();

	s = buf;
	s = s + sprintf ( s, "/* This file is automatically generated. */\n" );
	s = s + sprintf ( s, "#include <windows.h>\n" );
	s = s + sprintf ( s, "#include \"regtests.h\"\n" );
	s = s + sprintf ( s, "\n" );
	s = s + sprintf ( s, "_API_DESCRIPTION ExternalDependencies[] =\n" );
	s = s + sprintf ( s, "{\n" );

	int symbolCount = 0;
	for ( size_t i = 0; i < module.stubbedComponents.size () ; i++ )
	{
		s = WriteStubbedComponentToHooksFile ( s,
		                                       *module.stubbedComponents[i] );
		symbolCount += module.stubbedComponents[i]->symbols.size ();
	}

	s = s + sprintf ( s, "};\n" );
	s = s + sprintf ( s, "\n" );
	s = s + sprintf ( s, "#define ExternalDependencyCount %d\n", symbolCount );
	s = s + sprintf ( s, "ULONG MaxExternalDependency = ExternalDependencyCount - 1;\n" );
	s = s + sprintf ( s, "\n" );

	FileSupportCode::WriteIfChanged ( buf, GetHooksFilename ( module ) );

	free ( buf );
}

string
TestSupportCode::GetStubsFilename ( Module& module )
{
	return NormalizeFilename ( Environment::GetIntermediatePath () + sSep + module.output->relative_path + sSep + "_stubs.S" );
}

string
GetLinkerSymbol ( const StubbedSymbol& symbol )
{
	if (symbol.symbol[0] == '@')
		return symbol.symbol;
	else
		return "_" + symbol.symbol;
}

string
GetLinkerImportSymbol ( const StubbedSymbol& symbol )
{
	if (symbol.symbol[0] == '@')
		return "__imp_" + symbol.symbol;
	else
		return "__imp__" + symbol.symbol;
}

string
GetIndirectCallTargetSymbol ( const StubbedSymbol& symbol )
{
	return GetLinkerSymbol ( symbol ) + "_";
}

char*
TestSupportCode::WriteStubbedSymbolToStubsFile ( char* buffer,
                                                 const StubbedComponent& component,
                                                 const StubbedSymbol& symbol,
                                                 int stubIndex )
{
	string linkerSymbol = GetLinkerSymbol ( symbol );
	string linkerImportSymbol = GetLinkerImportSymbol ( symbol );
	string indirectCallTargetSymbol = GetIndirectCallTargetSymbol ( symbol );
	buffer = buffer + sprintf ( buffer,
	                            ".globl %s\n",
	                            linkerSymbol.c_str () );
	buffer = buffer + sprintf ( buffer,
	                            ".globl %s\n",
	                            linkerImportSymbol.c_str () );
	buffer = buffer + sprintf ( buffer,
	                            "%s:\n",
	                            linkerSymbol.c_str () );
	buffer = buffer + sprintf ( buffer,
	                            "%s:\n",
	                            linkerImportSymbol.c_str () );
	buffer = buffer + sprintf ( buffer,
	                            "  .long %s\n",
	                            indirectCallTargetSymbol.c_str () );
	buffer = buffer + sprintf ( buffer,
	                            "%s:\n",
	                            indirectCallTargetSymbol.c_str () );
	buffer = buffer + sprintf ( buffer,
	                            "  pushl $%d\n",
	                            stubIndex );
	buffer = buffer + sprintf ( buffer,
	                            "  jmp passthrough\n" );
	buffer = buffer + sprintf ( buffer, "\n" );
	return buffer;
}

char*
TestSupportCode::WriteStubbedComponentToStubsFile ( char* buffer,
                                                    const StubbedComponent& component,
                                                    int* stubIndex )
{
	for ( size_t i = 0; i < component.symbols.size () ; i++ )
		buffer = WriteStubbedSymbolToStubsFile ( buffer,
		                                         component,
		                                         *component.symbols[i],
		                                         (*stubIndex)++ );
	return buffer;
}

void
TestSupportCode::WriteStubsFile ( Module& module )
{
	char* buf;
	char* s;

	buf = (char*) malloc ( 512*1024 );
	if ( buf == NULL )
		throw OutOfMemoryException ();

	s = buf;
	s = s + sprintf ( s, "/* This file is automatically generated. */\n" );
	s = s + sprintf ( s, "passthrough:\n" );
	s = s + sprintf ( s, "  call _FrameworkGetHook@4\n" );
	s = s + sprintf ( s, "  test %%eax, %%eax\n" );
	s = s + sprintf ( s, "  je .return\n" );
	s = s + sprintf ( s, "  jmp *%%eax\n" );
	s = s + sprintf ( s, ".return:\n" );
	s = s + sprintf ( s, "  /* This will most likely corrupt the stack */\n" );
	s = s + sprintf ( s, "  ret\n" );
	s = s + sprintf ( s, "\n" );

	int stubIndex = 0;
	for ( size_t i = 0; i < module.stubbedComponents.size () ; i++ )
	{
		s = WriteStubbedComponentToStubsFile ( s,
		                                       *module.stubbedComponents[i],
		                                       &stubIndex );
	}

	FileSupportCode::WriteIfChanged ( buf, GetStubsFilename ( module ) );

	free ( buf );
}

string
TestSupportCode::GetStartupFilename ( Module& module )
{
	return NormalizeFilename ( Environment::GetIntermediatePath () + sSep + module.output->relative_path + sSep + "_startup.c" );
}

bool
TestSupportCode::IsUnknownCharacter ( char ch )
{
	if ( ch >= 'a' && ch <= 'z' )
		return false;
	if ( ch >= 'A' && ch <= 'Z' )
		return false;
	if ( ch >= '0' && ch <= '9' )
		return false;
	return true;
}

string
TestSupportCode::GetTestDispatcherName ( string filename )
{
	string filenamePart = ReplaceExtension ( GetFilename ( filename ), "" );
	if ( filenamePart.length () > 0 )
		filenamePart[0] = toupper ( filenamePart[0] );
	for ( size_t i = 1; i < filenamePart.length (); i++ )
	{
		if ( IsUnknownCharacter ( filenamePart[i] ) )
			filenamePart[i] = '_';
		else
			filenamePart[i] = tolower ( filenamePart[i] );
	}
	return filenamePart + "Test";
}

bool
TestSupportCode::IsTestFile ( string& filename ) const
{
	if ( stricmp ( GetFilename ( filename ).c_str (), "setup.c" ) == 0 )
		return false;
	return true;
}

void
TestSupportCode::GetSourceFilenames ( string_list& list,
                                      Module& module ) const
{
	size_t i;

	const vector<CompilationUnit*>& compilationUnits = module.non_if_data.compilationUnits;
	for ( i = 0; i < compilationUnits.size (); i++ )
	{
		const FileLocation& sourceFileLocation = compilationUnits[i]->GetFilename ();
		string filename = sourceFileLocation.relative_path + sSep + sourceFileLocation.name;
		if ( !compilationUnits[i]->IsGeneratedFile () && IsTestFile ( filename ) )
			list.push_back ( filename );
	}
}

char*
TestSupportCode::WriteTestDispatcherPrototypesToStartupFile ( char* buffer,
                                                              Module& module )
{
	string_list files;
	GetSourceFilenames ( files,
	                     module );
	for ( size_t i = 0; i < files.size (); i++ )
	{
		buffer = buffer + sprintf ( buffer,
	                                    "extern void %s(int Command, char *Buffer);\n",
		                            GetTestDispatcherName ( files[i] ).c_str () );
	}
	buffer = buffer + sprintf ( buffer, "\n" );
	return buffer;
}

char*
TestSupportCode::WriteRegisterTestsFunctionToStartupFile ( char* buffer,
                                                           Module& module )
{
	buffer = buffer + sprintf ( buffer,
	                            "extern void AddTest(TestRoutine Routine);\n" );
	buffer = buffer + sprintf ( buffer,
	                            "\n" );

	buffer = buffer + sprintf ( buffer,
	                            "void\n" );
	buffer = buffer + sprintf ( buffer,
	                            "RegisterTests()\n" );
	buffer = buffer + sprintf ( buffer,
	                            "{\n" );

	string_list files;
	GetSourceFilenames ( files,
	                     module );
	for ( size_t i = 0; i < files.size (); i++ )
	{
		buffer = buffer + sprintf ( buffer,
		                            "AddTest((TestRoutine)%s);\n",
		                            GetTestDispatcherName ( files[i]).c_str () );
	}
	buffer = buffer + sprintf ( buffer,
	                            "}\n" );
	buffer = buffer + sprintf ( buffer, "\n" );
	return buffer;
}

void
TestSupportCode::WriteStartupFile ( Module& module )
{
	char* buf;
	char* s;

	buf = (char*) malloc ( 50*1024 );
	if ( buf == NULL )
		throw OutOfMemoryException ();

	s = buf;
	s = s + sprintf ( s, "/* This file is automatically generated. */\n" );
	s = s + sprintf ( s, "\n" );
	s = s + sprintf ( s, "#include <windows.h>\n" );
	s = s + sprintf ( s, "#include \"regtests.h\"\n" );
	s = s + sprintf ( s, "\n" );
	s = WriteTestDispatcherPrototypesToStartupFile ( s,
	                                                 module );
	s = WriteRegisterTestsFunctionToStartupFile ( s,
	                                              module );
	s = s + sprintf ( s, "\n" );
	s = s + sprintf ( s, "void\n" );
	s = s + sprintf ( s, "ConsoleWrite(char *Buffer)\n" );
	s = s + sprintf ( s, "{\n" );
	s = s + sprintf ( s, "  printf(Buffer);\n" );
	s = s + sprintf ( s, "}\n" );
	s = s + sprintf ( s, "\n" );
	s = s + sprintf ( s, "int\n" );
	s = s + sprintf ( s, "WINAPI\n" );
	s = s + sprintf ( s, "WinMain(HINSTANCE hInstance,\n" );
	s = s + sprintf ( s, "  HINSTANCE hPrevInstance,\n" );
	s = s + sprintf ( s, "  LPSTR lpszCmdParam,\n" );
	s = s + sprintf ( s, "  int nCmdShow)\n" );
	s = s + sprintf ( s, "{\n" );
	s = s + sprintf ( s, "  _SetPriorityClass(_GetCurrentProcess(), HIGH_PRIORITY_CLASS);\n" );
	s = s + sprintf ( s, "  _SetThreadPriority(_GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);\n" );
	s = s + sprintf ( s, "  InitializeTests();\n" );
	s = s + sprintf ( s, "  RegisterTests();\n" );
	s = s + sprintf ( s, "  SetupOnce();\n" );
	s = s + sprintf ( s, "  PerformTests(ConsoleWrite, NULL);\n" );
	s = s + sprintf ( s, "  _ExitProcess(0);\n" );
	s = s + sprintf ( s, "  return 0;\n" );
	s = s + sprintf ( s, "}\n" );
	s = s + sprintf ( s, "\n" );

	FileSupportCode::WriteIfChanged ( buf, GetStartupFilename ( module ) );

	free ( buf );
}