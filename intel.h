#define INTEL_CORE_1GEN 1
#define INTEL_CORE_2GEN 2
#define INTEL_CORE_LGA2011 3

#define INTEL_VENDOR_ID 0x00008086

/* DataSheet:
 * http://www.intel.com/content/www/us/en/processors/core/CoreTechnicalResources.html */

#define INTEL_CORE_1GEN_BUSES 3
static const uint16_t intel_core_1gen_buses[INTEL_CORE_1GEN_BUSES] = {
    0xFF, 0x7F, 0x3F
};

#define INTEL_CORE_1GEN_DEVID 5
static const uint32_t intel_core_1gen_devid[] = {
    /* Core i7-900 and Extreme Edition, Desktop */
    0x2C01,

    /* Core i7-800 and i5-700 Desktop
     * Core i7-900 Mobile Processor Extreme Edition Series
     * Core i7-800, Core i7-700 Mobile */
    0x2C81,

    /* Core-i5-600, i3-500 desktop, Pentium 6000, Desktop */
    0x2D01,

    /* Core i7-900 and Extreme Edition, Desktop, 32 nm */
    0x2D81,

    /* Xeon Processor C5500/C3500 Series */
    0x2CC1
};

#define INTEL_CORE_2GEN_DEVID 8
static const uint32_t intel_core_2gen_devid[INTEL_CORE_2GEN_DEVID] = {
    /* 2nd Generation Core Processor Family DRAM Controller */
    0x01008086, 0x01048086,

    /* Xeon E3-1200 2nd Generation Core Processor Family DRAM Controller */
    0x01088086, 0x010c8086,

    /* 3rd Generation Core Processor Family DRAM Controller */
    0x01508086, 0x01548086, 0x01588086, 0x015c8086
};


/* PAM registers */

/* 40h-46h of the System Address Docoder (SAD). */
#define INTEL_CORE_1GEN_PAM(dev_bus) (1 << 31 | dev_bus << 16 | 0 << 11 | 1 << 8 | 0x40)

/* 80h-86h */
#define INTEL_CORE_2GEN_PAM 0x80000080
