/* Copyright (c) 2014 Tom Li
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Credits:
 *
 * 915Resolution - The source of all the resolution hacks
 * http://915resolution.mango-lang.org/
 *
 * DieBuche - The original work on hacking legacy NVIDIA/ATI VBIOS
 * http://www.insanelymac.com/forum/topic/211294-information-on-vesa-modes-in-atinvidia-bios/
 *
 * --- my nvesabios for hacking
 *     https://github.com/biergaizi/nvesabios
 *
 * Clover
 * http://sourceforge.net/projects/cloverefiboot/
 *
 * pene - Reverse Engineering on NVIDIA VBIOS
 * http://www.projectosx.com/forum/index.php?showtopic=2562&st=2840&p=22683&#entry22683
 *
 * Chameleon
 * http://forge.voodooprojects.org/p/chameleon
 *
 * dmazar - Providing the information about unlocking PAM on newer CPUs
 * http://www.insanelymac.com/forum/topic/284296-solved-resolution-hack-for-1st-gen-intel-hd-graphics/
 *
 * GNU GRUB
 * http://www.gnu.org/software/grub/
 *
 * VBIOS images submitted by Hackintosh users
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/mman.h>
#include <sys/io.h>

#define VBIOS_START 0xc0000
#define VBIOS_SIZE 0x10000
#define VBIOS_FILE "/dev/mem"

#define MECH_ONE_ADDR	0xCF8
#define MECH_ONE_DATA	0xCFC

#include "nvidia.h"
#include "intel.h"

#define VENDOR_ID_MASK 0x0000FFFF

typedef struct {
    int bios_fd;
    uint8_t *bios_ptr;

    char *nv_mode_table;
    uint32_t mode_table_size;

    unsigned int chipset_id;
    int chipset_type;

    bool locked;
    uint32_t pam_addr;
    uint8_t b1;
    uint8_t b2;
} vbios_map;


void detect_chipset_info(vbios_map *);
vbios_map *map_vbios(void);
vbios_map *open_vbios(void);
void close_vbios(vbios_map *);
void unlock_vbios(vbios_map *);
void relock_vbios(vbios_map *);
void search_and_replace(uint8_t *, const int, const uint8_t *, const int, const uint8_t *);
void set_mode(vbios_map *, int, int);
void show_help(char *name);


void detect_chipset_info(vbios_map *map)
{
    /* query chipset id */
    outl(0x80000000, MECH_ONE_ADDR);
    map->chipset_id = inl(MECH_ONE_DATA);

    int i;
    for (i = 0; i < INTEL_CORE_2GEN_DEVID; i++) {
        if (map->chipset_id == intel_core_2gen_devid[i]) {
            map->chipset_type = INTEL_CORE_2GEN;
            map->pam_addr = INTEL_CORE_2GEN_PAM;
            return;
        }
    }

    uint8_t bus;
    uint16_t dev_id;

    if (((map->chipset_id) & VENDOR_ID_MASK) == INTEL_VENDOR_ID) {
        for (i = 0; i < INTEL_CORE_1GEN_BUSES; i++) {
            bus = intel_core_1gen_buses[i];

            outl(1 << 31 | bus << 16 | 0 << 11 | 1 << 8 | 0, MECH_ONE_ADDR);
            dev_id = inw(MECH_ONE_DATA + 2);

            int j;
            for (j = 0; j < INTEL_CORE_1GEN_DEVID; j++) {
                if (dev_id == intel_core_1gen_devid[j]) {
                    map->chipset_type = INTEL_CORE_1GEN;
                    map->pam_addr = INTEL_CORE_1GEN_PAM(bus);
                    return;
                }
            }
        }
    }

    /* Core i7 Processor Family for the LGA-2011
     * http://www.intel.com/content/www/us/en/processors/core/core-i7-lga-2011-datasheet-vol-2.html
     * Bus 0, Dev 5, Func 0, DevId = 3C28 contains reg CPUBUSNO at 108h, bits 15:8 = Uncore bus
     * Bus "Uncore bus", Dev 12, Func 6, DevId = 3CF4 is System Address Decoder and PAM regs are at 40h-46h
     */

    /* TODO: moving these magic into intel.h, but it seems hard to do. */
    outl(1 << 31 | 0 << 16 | 5 << 11 | 0 << 8 | 0, MECH_ONE_ADDR);
    dev_id = inw(MECH_ONE_DATA + 2);
    if (dev_id == 0x3C28) {
        outl(1 << 31 | 0 << 16 | 5 << 11 | 0 << 8 | 0x108, MECH_ONE_ADDR);
        bus = inb(MECH_ONE_DATA + 1);

        outl(1 << 31 | bus << 16 | 12 << 11 | 6 << 8 | 0, MECH_ONE_ADDR);
        dev_id = inw(MECH_ONE_DATA + 2);
        if (dev_id == 0x3CF4) {
            map->chipset_type = INTEL_CORE_LGA2011;
            map->pam_addr = bus;
            return;
        }
    }

}

vbios_map *map_vbios(void)
{
    vbios_map *map = malloc(sizeof(vbios_map));
    memset(map, 0, sizeof(vbios_map));

    map->bios_fd = open(VBIOS_FILE, O_RDWR);
    if (map->bios_fd < 0) {
        fprintf(stderr, "Unable to open the BIOS file.\n");
        exit(1);
    }

    map->bios_ptr = mmap(0, VBIOS_SIZE, PROT_READ | PROT_WRITE,
                         MAP_SHARED, map->bios_fd, VBIOS_START);

    if (map->bios_ptr == MAP_FAILED) {
        fprintf(stderr, "Unable to mmap() the video BIOS.\n");
        close(map->bios_fd);
        exit(1);
    }
    return map;
}

vbios_map *open_vbios(void)
{
    vbios_map *map = map_vbios();

    /* get the permission in order to write registers */
    if (iopl(3) < 0) {
        fprintf(stderr, "Unable to obtain the proper IO permissions.\n");
        exit(1);
    }
    detect_chipset_info(map);

    uint16_t nv_data_table_offset = 0;
    uint16_t *nv_data_table = NULL;
    NV_VESA_TABLE *std_vesa = NULL;

    int i;
    for (i = 0; i < 512; i++) {
        if (memcmp(&map->bios_ptr[i], nv_string, 4) != 0) {
            continue;
        }

        int j;
        for (j = 0; i < 0x300; j++) {
            if (memcmp(&map->bios_ptr[j], nv_pattern, 4) != 0) {
                continue;
            }
            nv_data_table_offset = *((uint16_t *) (&map->bios_ptr[j + 4]));
            break;
        }

        nv_data_table = (uint16_t *) (map->bios_ptr + (nv_data_table_offset + OFFSET_TO_VESA_TABLE_INDEX));
        std_vesa = (NV_VESA_TABLE *) (map->bios_ptr + *nv_data_table);
        map->nv_mode_table = (char *) std_vesa + sizeof(NV_COMMON_TABLE_HEADER);

        if (!map->nv_mode_table) {
            fprintf(stderr, "Unable to locate the nv_mode_table.");
            close_vbios(map);
            return NULL;
        }
        map->mode_table_size = std_vesa->header.size;
        break;
    }
    return map;
}

void close_vbios(vbios_map *map)
{
    munmap(map->bios_ptr, VBIOS_SIZE);
    close(map->bios_fd);
    free(map);
}

void unlock_vbios(vbios_map *map)
{
    outl(map->pam_addr, MECH_ONE_ADDR);
    map->b1 = inb(MECH_ONE_DATA + 1);
    map->b2 = inb(MECH_ONE_DATA + 2);
    outl(map->pam_addr, MECH_ONE_ADDR);
    outb(0x33, MECH_ONE_DATA + 1);
    outb(0x33, MECH_ONE_DATA + 2);

    uint8_t *ptr = map->bios_ptr + 0xa123;
    uint8_t val_orig = *ptr;
    *ptr += 1;
    if (*ptr != val_orig) {
        *ptr = val_orig;
        map->locked = false;
    }
    else {
        fprintf(stderr, "Registers set, but still unable to modify VBIOS.\n");
        exit(1);
    }
}

void relock_vbios(vbios_map *map)
{
    outl(map->pam_addr, MECH_ONE_ADDR);
    outb(map->b1, MECH_ONE_DATA + 1);
    outb(map->b2, MECH_ONE_DATA + 2);
}


void search_and_replace(uint8_t *target, const int target_size, const uint8_t *search, const int search_size, const uint8_t *replace)
{
    int i;
    for (i = 0; i < target_size; i++) {
        if (memcmp(&target[i], search, search_size) == 0) {
            memcpy(&target[i], replace, search_size);
            /* once is enough. */
            return;
        }
    }
    fprintf(stderr, "Unable to find the default mode in order to replace it.\n");
    fprintf(stderr, "It probably means you replace the mode already.\n");
    exit(1);
}


void set_mode(vbios_map *map, int width, int height)
{
    unlock_vbios(map);

    int i;
    for (i = 0; i < NV_TOTAL_RESOLUTIONS; i++) {
        if (nv_resolution[i].height == height && nv_resolution[i].width == width) {
            break;
        }
    }
    if (i == NV_TOTAL_RESOLUTIONS) {
        fprintf(stderr, "The specified mode doesn't available in the database.\n");
        exit(1);
        return;
    }
    search_and_replace(map->bios_ptr, VBIOS_SIZE, nv_sample0, 17, nv_key0[i].matrix);
    search_and_replace(map->bios_ptr, VBIOS_SIZE, nv_sample1, 9,  nv_key1[i].matrix);
    search_and_replace(map->bios_ptr, VBIOS_SIZE, nv_sample2, 13, nv_key2[i].matrix);
    search_and_replace(map->bios_ptr, VBIOS_SIZE, nv_sample3, 5, nv_key3[i].matrix);

    /* What is it? */
    if (((*map->bios_ptr + 0x34) & 0x8F) == 0x80) {
        *(map->bios_ptr + 0x34) |= 0x01;
    }

    relock_vbios(map);
}

void show_help(char *name)
{
    printf("Usage: %s [width] [height]\n", name);
    return;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        show_help(argv[0]);
        return 0;
    }
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);

    vbios_map *map = open_vbios();
    set_mode(map, width, height);
    close_vbios(map);

    printf("Hacked resolution to %dx%d.\n", width, height);
    return 0;
}
