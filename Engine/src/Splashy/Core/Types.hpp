#pragma once

#ifdef __linux__ 

#define uchar __u_char

#endif

#ifdef _WIN32

#define uchar unsigned char

#endif
