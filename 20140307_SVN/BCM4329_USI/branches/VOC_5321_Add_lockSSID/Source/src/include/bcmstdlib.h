/*
 * prototypes for functions defined in bcmstdlib.c
 * Copyright (C) 2010, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: bcmstdlib.h,v 13.26.70.3.2.5.16.4 2009/09/14 09:21:45 Exp $:
 */

/*
 * bcmstdlib.h file should be used only to construct an OSL or alone without any OSL
 * It should not be used with any orbitarary OSL's as there could be a conflict
 * with some of the routines defined here.
*/

#ifndef	_BCMSTDLIB_H
#define	_BCMSTDLIB_H

#include <typedefs.h>
#include <bcmdefs.h>
#include <stdarg.h>

#ifndef INT_MAX
#define INT_MAX 2147483647 /* from limits.h */
#endif


/* For backwards compatibility, define "BWL_NO_INTERNAL_STDLIB_SUPPORT" to
 * exclude support for the BRCM stdlib APIs. This should be cleaned-up such
 * that platforms that require the BRCM stdlib API should simply define
 * "BWL_INTERNAL_STDLIB_SUPPORT". This would eliminate the need for the
 * following #ifndef check.
 */
#ifndef BWL_NO_INTERNAL_STDLIB_SUPPORT
	#define BWL_INTERNAL_STDLIB_SUPPORT
#endif

#ifdef BWL_INTERNAL_STDLIB_SUPPORT
/* This should be cleaned-up such that platforms that require the BRCM stdlib
 * API should simply define "BWL_INTERNAL_STDLIB_SUPPORT". This would eliminate
 * the need for the following #ifdef check.
 */
#if !defined(_WIN32)

typedef int FILE;
#define stdout ((FILE *)1)
#define stderr ((FILE *)2)

/* i/o functions */
extern int fputc(int c, FILE *stream);
extern void BCMROMFN(putc)(int c);
/* extern int putc(int c, FILE *stream); */
#define putchar(c) putc(c)
extern int fputs(const char *s, FILE *stream);
extern int puts(const char *s);
extern int getc(void);
extern bool BCMROMFN(keypressed)(void);

/* string functions */
#ifdef BCMSPACE
#define PRINTF_BUFLEN	512
#else
#define PRINTF_BUFLEN	256
#endif
extern int BCMROMFN(printf)(const char *fmt, ...);
extern int BCMROMFN(sprintf)(char *buf, const char *fmt, ...);

extern char *BCMROMFN(index)(const char *s, int c);

/* For EFI, use some of the common EFI Driver Library functions
 * to reduce final size and improve efficiency
 */
extern int BCMROMFN(strcmp)(const char *s1, const char *s2);
extern size_t BCMROMFN(strlen)(const char *s);
extern char *BCMROMFN(strcpy)(char *dest, const char *src);
extern char *BCMROMFN(strstr)(const char *s, const char *find);
extern char *BCMROMFN(strncpy)(char *dest, const char *src, size_t n);
extern char *BCMROMFN(strcat)(char *d, const char *s);

extern int BCMROMFN(strncmp)(const char *s1, const char *s2, size_t n);
extern char *BCMROMFN(strchr)(const char *str, int c);
extern char *BCMROMFN(strrchr)(const char *str, int c);
extern size_t BCMROMFN(strspn)(const char *s1, const char *s2);
extern size_t BCMROMFN(strcspn)(const char *s1, const char *s2);
extern unsigned long BCMROMFN(strtoul)(const char *cp, char **endp, int base);
#define strtol(nptr, endptr, base) ((long)strtoul((nptr), (endptr), (base)))
#define	atoi(s)	((int)(strtoul((s), NULL, 10)))

extern void *BCMROMFN(memmove)(void *dest, const void *src, size_t n);
extern void *BCMROMFN(memchr)(const void *s, int c, size_t n);

extern int BCMROMFN(vsprintf)(char *buf, const char *fmt, va_list ap);
/* mem functions */
/* For EFI, using EFIDriverLib versions */
/* Cannot use memmem in ROM because of character array initialization with "" in gcc */
extern void *BCMROMFN(memset)(void *dest, int c, size_t n);
/* Cannot use memcpy in ROM because of structure assignment in gcc */
extern void *BCMROMFN(memcpy)(void *dest, const void *src, size_t n);
extern int BCMROMFN(memcmp)(const void *s1, const void *s2, size_t n);

/* bcopy, bcmp, and bzero */
#define	bcopy(src, dst, len)	memcpy((dst), (src), (len))
#define	bcmp(b1, b2, len)	memcmp((b1), (b2), (len))
#define	bzero(b, len)		memset((b), '\0', (len))

extern unsigned long rand(void);

#endif 
#endif   /* BWL_INTERNAL_STDLIB_SUPPORT */

extern int BCMROMFN(snprintf)(char *str, size_t n, char const *fmt, ...);
extern int BCMROMFN(vsnprintf)(char *buf, size_t size, const char *fmt, va_list ap);

#endif 	/* _BCMSTDLIB_H */
