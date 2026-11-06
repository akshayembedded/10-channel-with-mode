#ifndef CONFIG_H
#define CONFIG_H
/* Set each feature to 0 or 1, then rebuild all files. */
#define ENABLE_MODE_BUTTON 1
#define ENABLE_TIMER_ADC 1
#define ENABLE_EEPROM_BOOT_CYCLE 1
#define DEFAULT_BOOT_MODE 0 /* 0 = patterns, 1 = steady all-on */
#define EEPROM_WAIT_LIMIT 60000U
#if (DEFAULT_BOOT_MODE != 0) && (DEFAULT_BOOT_MODE != 1)
#error DEFAULT_BOOT_MODE must be 0 or 1
#endif
#endif
