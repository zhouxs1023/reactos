<?xml version="1.0"?>
<!DOCTYPE module SYSTEM "../../../tools/rbuild/project.dtd">
<module name="fontview" type="win32gui" installbase="system32" installname="fontview.exe">
	<include base="fontview">.</include>
	<library>gdi32</library>
	<library>user32</library>
	<library>shell32</library>
	<file>fontview.c</file>
	<file>display.c</file>
	<file>fontview.rc</file>
</module>