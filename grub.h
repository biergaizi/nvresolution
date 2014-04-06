#define GRUB
#include <grub/types.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/err.h>
#include <grub/dl.h>
#include <grub/normal.h>
#include <grub/i386/io.h>

/* GPLv3+ is allowed to use as the license of the program in GRUB by me, the author.*/
GRUB_MOD_LICENSE ("GPLv3+");

#define printf					grub_printf
#define malloc					grub_malloc
#define free					grub_free
#define strcmp					grub_strcmp
#define fprintf(stream, ...)			grub_printf(__VA_ARGS__)
#define strtol(x,y,z)				grub_strtoul(x,y,z)
#define atoi(x)					grub_strtoul(x,NULL,10)
#define assert(x)
#define memset					grub_memset
#define memcmp      grub_memcmp
#define memcpy      grub_memcpy
#define outl		grub_outl
#define outb		grub_outb
#define inl		grub_inl
#define inb		grub_inb
#define inw     grub_inw

static int status = 0;

#define exit(x) \
{ \
    status = x; \
    return; \
}

#define bool char
#define false 0
#define true 1

#define uint8_t unsigned char
#define int16_t signed short
#define uint16_t unsigned short
#define int32_t signed int
#define uint32_t unsigned int
