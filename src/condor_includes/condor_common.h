#if defined(WIN32)

#define NOGDI
#define NOUSER
#define NOSOUND
#include <winsock2.h>
#include <windows.h>
#include "_condor_fix_nt.h"
#include <stdlib.h>

#else

#include "_condor_fix_types.h"
#include "condor_fix_stdio.h"
#include <stdlib.h>
#include "condor_fix_unistd.h"
#include "condor_fix_limits.h"
#include "condor_fix_string.h"
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#if !defined(SUNOS41)
#include <signal.h>
#endif

#endif // defined(WIN32)
