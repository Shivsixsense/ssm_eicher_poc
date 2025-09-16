#include "storage.h"
#include "system_logging.h"
#include "cmsis_os.h"
#include "stm32g0xx_ll_gpio.h"
#include "main.h"

/******************************************************************************
 * Marco Defs
 *****************************************************************************/

/* Debug Log tag */
#define TAG "FLS "

#define _USE_STRFUNC 2 /* 0:Disable or 1-2:Enable */

/******************************************************************************
 * Type Defs
 *****************************************************************************/

/******************************************************************************
 * Global variables
 *****************************************************************************/

/* SPI handle for FLS */
extern SPI_HandleTypeDef hspi2;

/******************************************************************************
 * Local variables
 *****************************************************************************/

W25QXX_HandleTypeDef w25qxx;
struct lfs_info infofilex;

/******************************************************************************
 * Local Function Prototypes
 *****************************************************************************/

static lfs_soff_t get_reverse_relative_pos(lfs_t *lfs, lfs_file_t *file, char token);

/******************************************************************************
 * Global Function Definations
 *****************************************************************************/

/**
 * @brief initialize storage driver
 */
int init_storage(void)
{
    int retval = 0;
    //    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8 , GPIO_PIN_SET);

    if (w25qxx_init(&w25qxx, &hspi2, GPIOB, GPIO_PIN_12) == W25QXX_Ok)
    {
    	w25qxx_chip_erase(&w25qxx);
        retval = w25qxx_littlefs_init(&w25qxx);
    }

    return retval;
}

int file_is_file_exist(const char *fileName)
{
    return lfs_stat(&littlefs, fileName, &infofilex);
}

int file_delete_file(const char *fileName)
{
    return lfs_remove(&littlefs, fileName);
}

int file_create_new_file(const char *fileName)
{
    lfs_file_t file;
    int retval = 0;

    retval = lfs_file_open(&littlefs, &file, fileName, (LFS_O_RDWR | LFS_O_CREAT));
    retval = lfs_file_sync(&littlefs, &file);
    retval = lfs_file_close(&littlefs, &file);

    return retval;
}

int file_write_to_file(const char *fileName, const void *buffer, uint32_t size)
{
    lfs_file_t file;
    int retval = 0;

    retval = lfs_file_open(&littlefs, &file, fileName, (LFS_O_RDWR | LFS_O_CREAT));

    retval = lfs_file_size(&littlefs, &file);

    retval = lfs_file_seek(&littlefs, &file, retval, LFS_SEEK_CUR);

    retval = lfs_file_write(&littlefs, &file, buffer, size);

    retval = lfs_file_sync(&littlefs, &file);

    retval = lfs_file_close(&littlefs, &file);

    return retval;
}

uint32_t file_get_file_size(const char *fileName)
{
    lfs_stat(&littlefs, fileName, &infofilex);
    return infofilex.size;
}

int lfs_ls(lfs_t *lfs, const char *path)
{
    lfs_dir_t dir;
    int err = lfs_dir_open(lfs, &dir, path);
    if (err)
    {
        return err;
    }

    struct lfs_info info;
    while (true)
    {
        int res = lfs_dir_read(lfs, &dir, &info);
        if (res < 0)
        {
            return res;
        }

        if (res == 0)
        {
            break;
        }

        switch (info.type)
        {
        case LFS_TYPE_REG:
            print_logln("reg ");
            break;
        case LFS_TYPE_DIR:
            print_logln("dir ");
            break;
        default:
            print_logln("?   ");
            break;
        }

        static const char *prefixes[] = {"", "K", "M", "G"};
        for (int i = sizeof(prefixes) / sizeof(prefixes[0]) - 1; i >= 0; i--)
        {
            if (info.size >= (1 << 10 * i) - 1)
            {
                print_logln("%*u%sB ", 4 - (i != 0), info.size >> 10 * i, prefixes[i]);
                break;
            }
        }

        print_logln("%s\n", info.name);
    }

    err = lfs_dir_close(lfs, &dir);
    if (err)
    {
        return err;
    }

    return 0;
}

int file_read_line(const char *fileName, char *outstr, uint16_t maxStrLen)
{
    lfs_file_t file;
    int retval = 0;
    uint16_t readBytes = 0;

    if (lfs_file_open(&littlefs, &file, fileName, LFS_O_RDONLY) == LFS_ERR_OK)
    {
        do
        {
            retval = lfs_file_read(&littlefs, &file, outstr, 1);
            readBytes += retval;

            if (*outstr == '\n')
            {
                break;
            }
            outstr++;

        } while ((retval > 0) && (readBytes < maxStrLen));

        retval = lfs_file_close(&littlefs, &file);
    }

    return readBytes;
}

char *Ltf_gets(char *buff, int len, lfs_file_t *lsf_file, lfs_t *lt_fs)
{
    int n = 0;
    char c, *p = buff;
    unsigned char s[2];

    while (n < len - 1)
    { /* Read characters until buffer gets filled */

        lfs_file_read(lt_fs, lsf_file, s, 1);
        c = s[0];
        if (_USE_STRFUNC == 2 && c == '\r')
            continue; /* Strip '\r' */
        *p++ = c;
        n++;
        if (c == '\n')
            break; /* Break on EOL */
    }
    *p = 0;
    return n ? buff : NULL; /* When no data read (eof or error), return with error. */
}

int storage_save_record(const char *fileName, const char *recordData)
{
    lfs_file_t file;
    int retval = 0;

    if ((retval = lfs_file_open(&littlefs, &file, fileName, (LFS_O_RDWR | LFS_O_CREAT))) != LFS_ERR_OK)
    {
        return retval;
    }

    retval = lfs_file_seek(&littlefs, &file, retval, LFS_SEEK_END);

    retval = lfs_file_write(&littlefs, &file, "+", 1);
    retval = lfs_file_write(&littlefs, &file, recordData, strlen(recordData));

    retval = lfs_file_sync(&littlefs, &file);

    retval = lfs_file_close(&littlefs, &file);

    return retval;
}

int storage_get_record(const char *fileName, char *outRecordData)
{
    lfs_file_t file;
    int retval = 0;
    lfs_soff_t relPos = 0;

    if ((retval = lfs_file_open(&littlefs, &file, fileName, LFS_O_RDWR)) != LFS_ERR_OK)
    {
        return retval;
    }

    if((retval = lfs_file_seek(&littlefs, &file, (0), LFS_SEEK_END)) == 0)
    {
        return -1;
    }

    relPos = get_reverse_relative_pos(&littlefs, &file, '+');

    relPos = retval - relPos;

    do
    {
        retval = lfs_file_read(&littlefs, &file, outRecordData, 1);

        if (*outRecordData == '+')
        {
            *outRecordData = '\0';
            break;
        }
        outRecordData++;

    } while (retval > 0);

    retval = lfs_file_sync(&littlefs, &file);

    retval = lfs_file_rewind(&littlefs, &file);

    retval = lfs_file_truncate(&littlefs, &file, relPos);

    retval = lfs_file_close(&littlefs, &file);

    return retval;
}

lfs_size_t storage_is_record_available(const char *fileName)
{
    lfs_stat(&littlefs, fileName, &infofilex);
    return infofilex.size;
}

/******************************************************************************
 * Local Function Definations
 *****************************************************************************/

static lfs_soff_t get_reverse_relative_pos(lfs_t *lfs, lfs_file_t *file, char token)
{
    char tmp = 0;
    int retval = 0;
    lfs_soff_t relPos = 0;

    do
    {
        retval = lfs_file_read(lfs, file, &tmp, 1);
        relPos++;
        if (tmp == token)
        {
            break;
        }
        retval = lfs_file_seek(lfs, file, (-2), LFS_SEEK_CUR);

    } while (retval >= 0);

    return relPos;
}
