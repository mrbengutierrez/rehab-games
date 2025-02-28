/*
This header file was generated when you ran setup. Once created, the setup
process won't overwrite it, so you can adjust the values by hand and
recompile if you need to.

To enable lots of debug output, add this line and re-run setup.py:
#define SYSV_IPC_DEBUG

To recreate this file, just delete it and re-run setup.py.

KEY_MIN, KEY_MAX and SEMAPHORE_VALUE_MAX are stored internally in longs, so
you should never #define them to anything larger than LONG_MAX regardless of
what your operating system is capable of.

*/

#define KEY_MIN		LONG_MIN
#ifndef _SEM_SEMUN_UNDEFINED
#define _SEM_SEMUN_UNDEFINED		
#endif
#define KEY_MAX		LONG_MAX
#ifndef PAGE_SIZE
#define PAGE_SIZE		4096
#endif
#define SYSV_IPC_VERSION		"1.0.0"
#define SEMAPHORE_VALUE_MAX		32767
