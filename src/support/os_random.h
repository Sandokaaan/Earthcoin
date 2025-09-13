// os_random.h
#pragma once
#include <cstddef>

bool GetOSRand(unsigned char *out, size_t outlen); // true on success

// os_random.cpp
#include "os_random.h"
#include <errno.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
bool GetOSRand(unsigned char *out, size_t outlen) {
    return BCryptGenRandom(nullptr, out, (ULONG)outlen, BCRYPT_USE_SYSTEM_PREFERRED_RNG) == 0;
}
#else
#include <unistd.h>
#include <fcntl.h>
#if defined(__linux__)
#include <sys/random.h>
#endif

bool GetOSRand(unsigned char *out, size_t outlen) {
#if defined(__linux__)
    ssize_t r = getrandom(out, outlen, 0);
    if (r == (ssize_t)outlen) return true;
    if (r >= 0) {
        size_t got = (size_t)r;
        while (got < outlen) {
            ssize_t s = getrandom(out + got, outlen - got, 0);
            if (s < 0) {
                if (errno == EINTR) continue;
                break;
            }
            got += (size_t)s;
        }
        return got == outlen;
    }
    // fallthrough to /dev/urandom if getrandom not supported
#endif

    int fd = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
    if (fd < 0) return false;
    size_t have = 0;
    while (have < outlen) {
        ssize_t n = read(fd, out + have, outlen - have);
        if (n <= 0) {
            if (errno == EINTR) continue;
            close(fd);
            return false;
        }
        have += (size_t)n;
    }
    close(fd);
    return true;
}
#endif
