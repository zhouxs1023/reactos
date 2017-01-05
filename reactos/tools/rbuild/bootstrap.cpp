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

Bootstrap::Bootstrap ( const Project& project_,
                       const Module* module_,
                       const XMLElement& bootstrapNode )
	: project(project_),
	  module(module_),
	  node(bootstrapNode)
{
	Initialize();
}

Bootstrap::~Bootstrap ()
{
}

bool
Bootstrap::IsSupportedModuleType ( ModuleType type )
{
	switch ( type )
	{
		case Kernel:
		case KernelModeDLL:
		case KeyboardLayout:
		case NativeDLL:
		case NativeCUI:
		case Win32DLL:
		case Win32OCX:
		case Win32CUI:
		case Win32SCR:
		case Win32GUI:
		case KernelModeDriver:
		case BootSector:
		case BootLoader:
		case BootProgram:
		case Cabinet:
			return true;
		case BuildTool:
		case StaticLibrary:
		case HostStaticLibrary:
		case ObjectLibrary:
		case Iso:
		case LiveIso:
		case Test:
		case RpcServer:
		case RpcClient:
		case RpcProxy:
		case Alias:
		case IdlHeader:
		case IdlInterface:
		case MessageHeader:
		case EmbeddedTypeLib:
		case ElfExecutable:
			return false;
		case TypeDontCare:
			break;
	}
	throw InvalidOperationException ( __FILE__,
	                                  __LINE__ );
}

string
Bootstrap::ReplaceVariable ( const string& name,
                             const string& value,
                             string path )
{
	size_t i = path.find ( name );
	if ( i != string::npos )
		return path.replace ( i, name.length (), value );
	else
		return path;
}

void
Bootstrap::Initialize ()
{
	if ( !IsSupportedModuleType ( module->type ) )
	{
		throw XMLInvalidBuildFileException (
			node.location,
			"<bootstrap> is not applicable for this module type." );
	}

	const XMLAttribute* att = node.GetAttribute ( "installbase", false );
	if ( att != NULL )
		base = ReplaceVariable ( "$(CDOUTPUT)", Environment::GetCdOutputPath (), att->value );
	else
		base = "";

	att = node.GetAttribute ( "nameoncd", false );
	if ( att != NULL )
		nameoncd = att->value;
	else
		nameoncd = module->output->name;
}

void
Bootstrap::ProcessXML()
{
}