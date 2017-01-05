<?xml version="1.0"?>
<!DOCTYPE group SYSTEM "../../tools/rbuild/project.dtd">
<group xmlns:xi="http://www.w3.org/2001/XInclude">
	<module name="halmps" type="kernelmodedll" entrypoint="HalInitSystem@8">
		<importlibrary base="hal" definition="../hal.pspec" />
		<bootstrap installbase="$(CDOUTPUT)/system32" />
		<include>include</include>
		<include base="ntoskrnl">include</include>
		<define name="CONFIG_SMP" />
		<define name="_NTHALDLL_" />
		<define name="_NTHAL_" />
		<library>hal_generic</library>
		<library>hal_generic_pcat</library>
		<library>hal_generic_mp</library>
		<library>ntoskrnl</library>
		<library>libcntpr</library>
		<directory name="mp">
			<if property="ARCH" value="i386">
				<file>mps.S</file>
				<file>mpsboot.asm</file>
				<file>mpsirql.c</file>
			</if>
		</directory>
	</module>
</group>