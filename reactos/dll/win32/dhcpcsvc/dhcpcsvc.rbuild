<module name="dhcpcsvc" type="win32dll" baseaddress="${BASEADDRESS_DHCPCSVC}" installbase="system32" installname="dhcpcsvc.dll">
	<importlibrary definition="dhcpcsvc.spec" />
	<include base="dhcpcsvc">include</include>
	<library>ntdll</library>
	<library>msvcrt</library>
	<library>ws2_32</library>
	<library>iphlpapi</library>
	<library>advapi32</library>
	<library>oldnames</library>
	
	<!-- FIXME: workarounds until we have a proper oldnames library -->
    <define name="tzset">_tzset</define>
    
	<directory name="dhcp">
		<file>adapter.c</file>
		<file>alloc.c</file>
		<file>api.c</file>
		<file>compat.c</file>
		<file>dhclient.c</file>
		<file>dispatch.c</file>
		<file>hash.c</file>
		<file>options.c</file>
		<file>pipe.c</file>
		<file>socket.c</file>
		<file>tables.c</file>
		<file>util.c</file>
	</directory>
	<directory name="include">
		<pch>rosdhcp.h</pch>
	</directory>
	<file>dhcpcsvc.c</file>
	<file>dhcpcsvc.rc</file>
</module>