#ifndef OS_OS_H
#define OS_OS_H

#include <shared/types.h>

bool osInit(void);
void osEnd(void);

#if defined(PLATFORM_WIN32)
 #include <oslib_win32/os_win32.h>
#elif defined(PLATFORM_LINUX)
 #include <oslib_linux/os_linux.h>
#else
 #error Unsupported platform
#endif

#endif

