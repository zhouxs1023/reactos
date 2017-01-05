<?xml version="1.0"?>
<!DOCTYPE group SYSTEM "../../tools/rbuild/project.dtd">
<group>
<module name="csr" type="nativecui" installbase="system32" installname="csr.exe">
	<include base="csr">.</include>
	<include base="ReactOS">include/reactos/subsys</include>
	<library>nt</library>
	<library>ntdll</library>
	<library>csrsrv</library>
	<file>main.c</file>
</module>
<directory name="csrsrv">
	<xi:include href="csrsrv/csrsrv.rbuild" />
</directory>
</group>