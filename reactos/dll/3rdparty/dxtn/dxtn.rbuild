<module name="dxtn" type="win32dll" entrypoint="0" installbase="system32" installname="dxtn.dll" allowwarnings="true" crt="msvcrt">
	<importlibrary definition="dxtn.spec" />
	<include base="dxtn">.</include>
	<file>fxt1.c</file>
	<file>dxtn.c</file>
	<file>wrapper.c</file>
	<file>texstore.c</file>
</module>