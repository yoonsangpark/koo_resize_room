/*
 *
 * Copyright (C) 2024 KOO Inc.
 *
 * Driver for the Koomon
 *
 * Licensed under the GPL-2 or later.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

#include <stdlib.h>
#include <sys/statvfs.h>

int has_mp4_extension(const char *filename) {
    const char *ext = strrchr(filename, '.'); // 마지막 '.' 찾기
    return (ext && strcmp(ext, ".mp4") == 0);
}

#define FILE_PATH_SIZE 1024
int delete_mp4_file(void) {

	const char *dir_path = "/mnt/sd/recordings";
	char filepath[FILE_PATH_SIZE];

	DIR *dir = opendir(dir_path);
	if (dir == NULL) {
		perror("opendir Error");
		return EXIT_FAILURE;
	}

	struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { 
            if (has_mp4_extension(entry->d_name)) {
				snprintf(filepath, sizeof(filepath), "%s/%s", dir_path, entry->d_name);
                printf("- %s\n", filepath);
				remove(filepath);
				break;
            }
        }
    }

	closedir(dir);	

	return 0;
}

int main(int argc, char** argv)
{

	//--------------------------------------------------
	struct statvfs stat;
	const char *path = "/mnt/sd";

	if (statvfs(path, &stat) != 0) {
		perror("statvfs Error");
		return EXIT_FAILURE;
	}

	unsigned long long total = (unsigned long long)stat.f_blocks * stat.f_frsize;
	unsigned long long free = (unsigned long long)stat.f_bfree * stat.f_frsize;
	unsigned long long available = (unsigned long long)stat.f_bavail * stat.f_frsize;

	printf("경로: %s\n", path);
	printf("총 디스크 크기: %llu 바이트 (%.2f GB)\n", total, total / (1024.0 * 1024 * 1024));
	printf("사용 가능한 공간: %llu 바이트 (%.2f GB)\n", available, available / (1024.0 * 1024 * 1024));
	printf("남은 공간: %llu 바이트 (%.2f GB)\n", free, free / (1024.0 * 1024 * 1024));	

	
	//--------------------------------------------------
	#define ROOM_LIMIT_SIZE 100 // 100MB

	unsigned long long available_size = available / (1024.0 * 1024);
	printf("available_size (MB): %llu MB\n", available_size);
	printf("ROOM_LIMIT_SIZE (MB): %lu MB\n", ROOM_LIMIT_SIZE);

	if (available_size > ROOM_LIMIT_SIZE) {
		delete_mp4_file();
	}

	return 0;
}
