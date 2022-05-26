#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_BOOT_FILE "./config_boot.ini"

FILE *CFG_BootFile_Init(void);
FILE *CFG_BootFile_LoadConfig(void);

int CFG_BootFile_ReadLine(FILE *fd, char *buffer);

int CFG_BootFile_ReadSet(char *buffer, char *dest);
int CFG_BootFile_ReadValue(char *buffer, char *dest);

int CFG_BootFile_ConfigRule(char *set, char *value);

#endif // CONFIG_H
