<?xml version="1.0"?>
<!DOCTYPE module SYSTEM "../../../tools/rbuild/project.dtd">
<module name="kbdfc" type="keyboardlayout" entrypoint="0" installbase="system32" installname="kbdfc.dll">
	<importlibrary definition="kbdfc.spec" />
	<include base="ntoskrnl">include</include>
	<file>kbdfc.c</file>
	<file>kbdfc.rc</file>
</module>