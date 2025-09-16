/*
 * storage.h
 *
 *  Created on: Dec 1st, 2023
 *      Author: Das, Somen
 *
 * @brief: Handle Flash storage interfaces
 */
#pragma once
#include "system_types.h"
#include "w25qxx.h"
#include "w25qxx_littlefs.h"



/******************************************************************************
 * Global Function Declarations
 *****************************************************************************/

int init_storage(void);

int file_is_file_exist(const char *fileName);
int file_delete_file(const char *fileName);
int file_create_new_file(const char *fileName);
int file_write_to_file(const char *fileName, const void *buffer, uint32_t size);
uint32_t file_get_file_size(const char *fileName);
int lfs_ls(lfs_t *lfs, const char *path);
int file_read_line(const char *fileName, char *outstr, uint16_t maxStrLen);
char *Ltf_gets(char *buff, int len, lfs_file_t *lsf_file, lfs_t *lt_fs);

lfs_size_t storage_is_record_available(const char *fileName);
int storage_get_record(const char *fileName, char *outRecordData);
int storage_save_record(const char *fileName, const char *recordData);
