#ifndef _UNISTD_H
#define _UNISTD_H 1

/* This is intended as a drop-in replacement for unistd.h on Windows.
 * Please add functionality as needed.
 * https://stackoverflow.com/a/826027/1202830
 */
#include <direct.h> /* for _getcwd() and _chdir() */
//#include <getopt.h> /* getopt at: https://gist.github.com/ashelly/7776712 */
#include <io.h>
#include <process.h> /* for getpid() and the exec..() family */
#include <stdlib.h>
#include <memory>


#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <io.h>
typedef int mode_t;

/// @Note If STRICT_UGO_PERMISSIONS is not defined, then setting Read for any
///       of User, Group, or Other will set Read for User and setting Write
///       will set Write for User.  Otherwise, Read and Write for Group and
///       Other are ignored.
///
/// @Note For the POSIX modes that do not have a Windows equivalent, the modes
///       defined here use the POSIX values left shifted 16 bits.

static const mode_t S_ISUID = 0x08000000;        ///< does nothing
static const mode_t S_ISGID = 0x04000000;        ///< does nothing
static const mode_t S_ISVTX = 0x02000000;        ///< does nothing
static const mode_t S_IRUSR = mode_t(_S_IREAD);  ///< read by user
static const mode_t S_IWUSR = mode_t(_S_IWRITE); ///< write by user
static const mode_t S_IXUSR = 0x00400000;        ///< does nothing
#ifndef STRICT_UGO_PERMISSIONS
static const mode_t S_IRGRP = mode_t(_S_IREAD);  ///< read by *USER*
static const mode_t S_IWGRP = mode_t(_S_IWRITE); ///< write by *USER*
static const mode_t S_IXGRP = 0x00080000;        ///< does nothing
static const mode_t S_IROTH = mode_t(_S_IREAD);  ///< read by *USER*
static const mode_t S_IWOTH = mode_t(_S_IWRITE); ///< write by *USER*
static const mode_t S_IXOTH = 0x00010000;        ///< does nothing
static const mode_t S_IRWXU = S_IXUSR | S_IWUSR | S_IXUSR;
#else
static const mode_t S_IRGRP = 0x00200000; ///< does nothing
static const mode_t S_IWGRP = 0x00100000; ///< does nothing
static const mode_t S_IXGRP = 0x00080000; ///< does nothing
static const mode_t S_IROTH = 0x00040000; ///< does nothing
static const mode_t S_IWOTH = 0x00020000; ///< does nothing
static const mode_t S_IXOTH = 0x00010000; ///< does nothing
#endif
static const mode_t MS_MODE_MASK = 0x0000ffff; ///< low word

static inline int portchmod(const char *path, mode_t mode)
{
    int result = _chmod(path, (mode & MS_MODE_MASK));

    if (result != 0)
    {
        result = errno;
    }

    return (result);
}
#else
static inline int portchmod(const char *path, mode_t mode)
{
    int result = chmod(path, mode);

    if (result != 0)
    {
        result = errno;
    }

    return (result);
}
#endif

const char *portgetenv(const char *str);


#define srandom srand
#define random rand

/* Values for the second argument to access.
   These may be OR'd together.  */
#define R_OK 4 /* Test for read permission.  */
#define W_OK 2 /* Test for write permission.  */
// #define   X_OK    1       /* execute permission - unsupported in windows*/
#define F_OK 0 /* Test for existence.  */

#define access _access
#define dup2 _dup2
#define execve _execve
#define ftruncate _chsize
#define unlink _unlink
#define fileno _fileno
#define getcwd _getcwd
#define chdir _chdir
#define isatty _isatty
#define lseek _lseek
/* read, write, and close are NOT being #defined here, because while there are file handle specific versions for
 * Windows, they probably don't work for sockets. You need to look at your app and consider whether to call e.g.
 * closesocket(). */

#ifdef _WIN64
#    define ssize_t __int64
#else
#    define ssize_t long
#endif

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
/* should be in some equivalent to <sys/types.h> */
//typedef __int8 int8_t;
//typedef __int16 int16_t;
//typedef __int32 int32_t;
//typedef __int64 int64_t;
//typedef unsigned __int8 uint8_t;
//typedef unsigned __int16 uint16_t;
//typedef unsigned __int32 uint32_t;
//typedef unsigned __int64 uint64_t;

#endif /* unistd.h  */