<module name="devcpux" type="win32dll" installbase="system32" installname="devcpux.dll" unicode="yes">
	<importlibrary definition="devcpux.spec" />
	<include base="devcpux">.</include>
	<include base="ReactOS">include/reactos/wine</include>
	<library>user32</library>
	<library>ntdll</library>
	<library>powrprof</library>
	<library>comctl32</library>
	<file>processor.c</file>
	<file>processor.rc</file>
</module>