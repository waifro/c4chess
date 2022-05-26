#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_BOOT_FILE "./config_boot.ini"

FILE *CFG_BootFile_Init(void);
FILE *CFG_BootFile_LoadConfig(void);
char *CFG_BootFile_ReadLine(FILE *fd, char *set);
int CFG_BootFile_Config(char *buffer, char *set);

#endif // CONFIG_H
