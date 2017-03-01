
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the J2534_SIM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// J2534_SIM_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

//have to use exports file since vc mangles __stdcall

#ifdef J2534_SIM_EXPORTS
#define J2534_SIM_API  __stdcall
//__declspec(dllexport) 
#else
#define J2534_SIM_API  __stdcall
//__declspec(dllimport) 
#endif


