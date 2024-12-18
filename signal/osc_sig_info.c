#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define PROC_PID_PATH "/proc/osc_process_pid"

typedef struct get_output_form_portid {
    int portid;
    int sw_id;
    int port_id;
}get_output_form_portid;

typedef struct osc_port_info_s {
    int port;
    char state[64];
}osc_port_info_t;

get_output_form_portid output_info[] = {
    {0x8001, 2, 0},
    {0x8002, 2, 2},
    {0x8003, 0, 0},
    {0x8004, 0, 1},
    {0x8005, 0, 2},
    {0x8006, 0, 3},
    {0x8007, 0, 4},
    {0x8008, 0, 5},
    {0x8009, 0, 9},
    {0x800a, 1, 0},
    {0x800b, 1, 8},
    {0x800c, 1, 9},
};

const char *sw_port_info[] = {"disabled  ", "listening ", "learning  ", "forwarding", "blocking  "};
const char *sw_port_info1[] = {"NM1   ", "NM2   ", "SLOT:1", "SLOT:2", "SLOT:3", "SLOT:4", "SLOT:5", "SLOT:6", "SLOT:7", "SLOT:8", "SFP1  ", "SFP2  "};

osc_port_info_t port_info[] = {
    {0x8001, "port:0x8001(Description: NM1   )  state:forwarding"},
    {0x8002, "port:0x8002(Description: NM2   )  state:forwarding"},
    {0x8003, "port:0x8003(Description: SLOT:1)  state:forwarding"},
    {0x8004, "port:0x8004(Description: SLOT:2)  state:forwarding"},
    {0x8005, "port:0x8005(Description: SLOT:3)  state:forwarding"},
    {0x8006, "port:0x8006(Description: SLOT:4)  state:forwarding"},
    {0x8007, "port:0x8007(Description: SLOT:5)  state:forwarding"},
    {0x8008, "port:0x8008(Description: SLOT:6)  state:forwarding"},
    {0x8009, "port:0x8009(Description: SLOT:7)  state:forwarding"},
    {0x800a, "port:0x800a(Description: SLOT:8)  state:forwarding"},
    {0x800b, "port:0x800b(Description: SFP1  )  state:forwarding"},
    {0x800c, "port:0x800c(Description: SFP2  )  state:forwarding"},
};

void osc_create_command(int portid, int state, char *output) {
    if((output == NULL) || (state > 4)){
        return;
    }
    // 生成字符串
    sprintf(output, "port:0x%04x(Description: %s)  state:%s",portid, sw_port_info1[(portid & 0xf) - 1], sw_port_info[state]);
}

void update_port_state(osc_port_info_t port_info[], int size, unsigned short port, const char *new_state) {
    for (int i = 0; i < size; i++) {
        if (port_info[i].port == port) {
            // 找到对应的端口，更新状态字符串
            strncpy(port_info[i].state, new_state, sizeof(port_info[i].state) - 1);
            port_info[i].state[sizeof(port_info[i].state) - 1] = '\0';
            return;
        }
    }
}

void write_port_info_to_file(const char *filename, osc_port_info_t port_info[], int size) {
    //打开文件，如果文件不存在则创建，写入模式
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Unable to open file");
        return;
    }

    // 遍历端口信息数组并写入文件
    for (int i = 0; i < size; i++) {
        fprintf(file, "%s\n", port_info[i].state);
    }

    // 关闭文件
    fclose(file);
}

// 信号处理函数
void handle_signal(int signo, siginfo_t *info, void *context) {
    if (signo == SIGUSR1) {
        printf("Received SIGUSR1 from kernel! Data: 0x%x\n", info->si_int);
        //system("echo 'User function executed!'");
        //const char *command = "./qdctloscmainmb1130a 3 1 0 8 1";
        const char *command = "./qdctloscmainmb1130a 0 1 8 4 369";
            // 调用 system 执行命令
        system(command);

        const char *osc_info_cmd;
        osc_create_command((info->si_int >> 8), (info->si_int & 0xff), osc_info_cmd);

        update_port_state(port_info, sizeof(port_info)/ sizeof(port_info[0]), info->si_int >> 8, osc_info_cmd);

        write_port_info_to_file("osc_info.txt", port_info, sizeof(port_info) / sizeof(port_info[0]));
    }
}

// 信号处理函数
void handle_signal_test() {
    //if (signo == SIGUSR1) {
        //printf("Received SIGUSR1 from kernel! Data: 0x%x\n", info->si_int);
        //system("echo 'User function executed!'");
	//const char *command = "./qdctloscmainmb1130a 3 1 0 8 1";
	//const char *command = "./qdctloscmainmb1130a 0 1 8 4 369";
        // 调用 system 执行命令
    //system(command);
    int si = 0x800a04;
    const char osc_info_cmd[64];
    osc_create_command((si >> 8), (si & 0xff), osc_info_cmd);

    update_port_state(port_info, sizeof(port_info)/ sizeof(port_info[0]), si >> 8, osc_info_cmd);

    write_port_info_to_file("osc_info.txt", port_info, sizeof(port_info) / sizeof(port_info[0]));
}

int main() {

#if 0
    // 挂起进程，等待信号
    while (1) {
        pause();
    }
#endif

    handle_signal_test();

    return 0;
}

