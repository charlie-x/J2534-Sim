========================================================================
       DYNAMIC LINK LIBRARY : j2534_sim
========================================================================

Project to log J2534 versions 02.02 and 04.04 as well as a simple 02.02 to 04.04 conveter


This is old code, and i hacked it along as i needed it for different J2534 / hacking tools, sometimes while working on a dyno on a freezing, hot or raining day.
but it works with the drewtech tools i usually use
maybe posting it will make me clean it up or rewrite it :)


Build it and add a registry entry as below


Setup for Drewtech mongoose/cardaq/scandaq

FunctionLibrary should be the full path of the dll
LogsDirectory is for DrewTech
ConfigApplication is for Drewtech


// for 0202 (mongoose style)
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SOFTWARE\PassThruSupport\Vendor2\Devices\Device3]
"DeviceId"=dword:00000000
"Name"="Charliex Simulator"
"ProtocolsSupported"="CAN,ISO14230,ISO15765,ISO9141"
"ConfigApplication"="C:\\WINDOWS\\system32\\MongConf.exe"
"FunctionLibrary"="j2534_0202.dll"

// for 0404 (mongoose style)
Windows Registry Editor Version 5.00

[HKEY_LOCAL_MACHINE\SOFTWARE\PassThruSupport.04.04\charliex - simulator]
"LogsDirectory"="C:\\CharlieX\\Logs"
"UART_ECHO_BYTE_PS"=dword:00000001
"DeviceId"=dword:00000000
"Vendor"="Charliex"
"Name"="Simulator ISO/CAN"
"ConfigApplication"="C:\\WINDOWS\\system32\\mongconf.exe"
"CAN"=dword:00000001
"ISO15765"=dword:00000001
"ISO9141"=dword:00000001
"ISO14230"=dword:00000001
"FunctionLibrary"="j2534_0404.dll"
"UART_ECHO_BYTE"=dword:00000001
