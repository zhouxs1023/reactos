<?xml version="1.0"?>
<!DOCTYPE group SYSTEM "../../../tools/rbuild/project.dtd">
<group>
<module name="mingw_common" type="staticlibrary" isstartuplib="true" crt="static">
	<library>oldnames</library>
	<library>ntdll</library>
	<define name="_CRTBLD" />

	<!-- This is to prevent from using data imports directly -->
	<define name="_M_CEE_PURE" />

	<include base="ReactOS">include/reactos/mingw-w64</include>
	<library>kernel32</library>
	<file>_newmode.c</file>
	<file>atonexit.c</file>
	<file>binmode.c</file>
	<file>charmax.c</file>
	<file>cinitexe.c</file>
	<file>CRT_fp10.c</file>
	<file>CRT_fp8.c</file>
	<file>CRT_glob.c</file>
	<file>crt_handler.c</file>
	<file>dllentry.c</file>
	<file>gccmain.c</file>
	<file>gs_support.c</file>
	<file>merr.c</file>
	<file>mingw_helpers.c</file>
	<file>natstart.c</file>
	<file>pesect.c</file>
	<file>pseudo-reloc.c</file>
	<file>pseudo-reloc-list.c</file>
	<file>tlsmcrt.c</file>
	<file>tlssup.c</file>
	<file>tlsthrd.c</file>
	<file>txtmode.c</file>
	<file>wildcard.c</file>
	<file>xncommod.c</file>
	<file>xthdloc.c</file>
	<file>xtxtmode.c</file>
	<file>ofmt_stub.c</file>
	<file>cxa_pure_virtual.c</file>
</module>
<module name="mingw_main" type="staticlibrary" isstartuplib="true" crt="dll">
	<define name="_CRTBLD" />
	<include base="ReactOS">include/reactos/mingw-w64</include>
	<compilerflag compilerset="msc">/wd4733</compilerflag>
	<file>crt0_c.c</file>
	<file>crtexe.c</file>
	<file>dllargv.c</file>
</module>
<module name="mingw_wmain" type="staticlibrary" isstartuplib="true" unicode="yes" crt="static">
	<define name="_CRTBLD" />
	<define name="WPRFLAG" />
	<include base="ReactOS">include/reactos/mingw-w64</include>
	<compilerflag compilerset="msc">/wd4733</compilerflag>
	<file>crt0_w.c</file>
	<file>crtexe.c</file>
	<file>dllargv.c</file>
</module>
<module name="mingw_dllmain" type="staticlibrary" isstartuplib="true" crt="static">
	<define name="_CRTBLD" />
	<include base="ReactOS">include/reactos/mingw-w64</include>
	<file>crtdll.c</file>
	<file>dllargv.c</file>
</module>
<module name="oldnames" type="staticlibrary">
	<define name="_CRTBLD" />
	<importlibrary definition="moldname-msvcrt.def" dllname="msvcrt.dll" />
</module>
<module name="getopt" type="staticlibrary" crt="dll">
	<file>getopt.c</file>
</module>
</group>