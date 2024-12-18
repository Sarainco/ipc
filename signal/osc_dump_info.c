#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DUMP_SIZE 51  // 每行显示的字节数
#define FILENAME "/root/osc_info.txt"

void dump_memory(FILE *file) {
    unsigned char buffer[DUMP_SIZE];
    size_t bytes_read;
    size_t offset = 0;

    while ((bytes_read = fread(buffer, 1, DUMP_SIZE, file)) > 0) {
        //printf("0x%08zx  ", offset);  // 打印内存地址偏移

        // 打印十六进制数据
        //for (size_t i = 0; i < bytes_read; i++) {
          //  printf("%02x ", buffer[i]);
        //}

        // 填充不足的十六进制位置
        //for (size_t i = bytes_read; i < DUMP_SIZE; i++) {
          //  printf("   ");
        //}

        printf(" |");

        // 打印可打印字符
        for (size_t i = 0; i < bytes_read; i++) {
            if (buffer[i] >= 32 && buffer[i] <= 126) {
                printf("%c", buffer[i]);
            } else {
                printf(".");
            }
        }
        // 填充剩余的字符区域（使其对齐）
        for (size_t i = bytes_read; i < DUMP_SIZE; i++) {
            printf(" ");
        }

        printf("|\n");

        offset += bytes_read;
    }
}

int main() {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    dump_memory(file);

    fclose(file);

    return 0;
}


