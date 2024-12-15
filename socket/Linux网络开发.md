### Linux网络开发

#### 一、Linux应用层和内核层的桥梁----系统调用

##### 1.什么是系统调用

系统调用由操作系统提供，由应用程序调用

* 如何把一段字符串输出到屏幕-------系统调用
  user progress -----> c library -----> kernel
    write() -------->   kibc write()---->sys_write()

```c
#include <stdio.h>
#include <stdlib.h>

void main() {
    printf("hello world\n");
}

sunao@sunao-virtual-machine:~/code/linux_net$ strace ./printf 
execve("./printf", ["./printf"], [/* 67 vars */]) = 0
brk(NULL)                               = 0xbed000
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
open("/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=95299, ...}) = 0
mmap(NULL, 95299, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fad1a9b3000
close(3)                                = 0
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
open("/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0`\t\2\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=1868984, ...}) = 0
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fad1a9b2000
mmap(NULL, 3971488, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fad1a3dc000
mprotect(0x7fad1a59c000, 2097152, PROT_NONE) = 0
mmap(0x7fad1a79c000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1c0000) = 0x7fad1a79c000
mmap(0x7fad1a7a2000, 14752, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fad1a7a2000
close(3)                                = 0
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fad1a9b1000
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fad1a9b0000
arch_prctl(ARCH_SET_FS, 0x7fad1a9b1700) = 0
mprotect(0x7fad1a79c000, 16384, PROT_READ) = 0
mprotect(0x600000, 4096, PROT_READ)     = 0
mprotect(0x7fad1a9cb000, 4096, PROT_READ) = 0
munmap(0x7fad1a9b3000, 95299)           = 0
fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 20), ...}) = 0
brk(NULL)                               = 0xbed000
brk(0xc0e000)                           = 0xc0e000
write(1, "hello world\n", 12hello world
)           = 12
exit_group(12)                          = ?
+++ exited with 12 +++
```

* **磁盘**：
  - 磁盘是计算机的长期存储设备，负责存储操作系统、应用程序及用户数据。磁盘上的数据不会因为断电而丢失。在题目中，`hello_world` 程序存储在磁盘上。
2. **DDR（动态随机存取存储器）**：
   - DDR 是计算机的主要内存，负责临时存储正在运行的程序和数据。相比磁盘，DDR 具有更快的读取和写入速度，但断电后数据会丢失。在运行程序时，程序从磁盘加载到 DDR 中执行。

3. **CACHE（高速缓存）**：
   - CACHE 是位于 CPU 和 DDR 之间的高速存储器，用于缓存经常访问的数据和指令，以减少 CPU 直接访问内存的延迟。CACHE 的访问速度远高于 DDR，但容量较小。

4. **CPU（中央处理器）**：
   - CPU 是计算机的核心处理单元，负责执行程序中的指令并进行各种计算操作。CPU 通过指令控制其他硬件组件，完成程序运行。

1. **加载程序到内存**：
   - 当用户或操作系统请求运行 `hello_world` 程序时，磁盘控制器会从磁盘中读取该程序的二进制文件，并通过总线将其传输到 DDR（内存）中。

2. **缓存数据到 CACHE**：
   - CPU 开始执行 `hello_world` 程序时，会首先从 DDR 读取程序指令。如果程序中存在经常访问的指令或数据，CPU 会将这些内容缓存到 CACHE 中以提高访问速度。

3. **执行程序**：
   - CPU 从 CACHE（或 DDR）中读取 `hello_world` 程序的指令并执行。程序中的 `printf("hello world!");` 指令会让 CPU 通过总线控制显示器（图中的显示器）将字符串 "hello world!" 显示出来。

4. **完成执行**：
   - 当 `hello_world` 程序执行完毕后，CPU 会通知操作系统清理相关资源，如释放占用的内存（DDR），以便其他程序使用。

##### 2.Linux系统调用的方式

* C库提供的包装函数，比如：socket/listen/accept/read/write等
* long syscall(long number, ...);

##### 3.如何给linux内核添加一个自己的系统调用

###### 3.1 修改系统调用表

###### 3.2 添加系统调用函数的声明

###### 3.3 实现系统调用函数

###### 3.4 应用程序使用系统调用

#### 二、Linux网络相关系统调用的内核实现

##### 1.Socket

建立一个通信端口（socket - create an endpoint for communication）

```c
/**函数以及头文件**/
 #include <sys/types.h>          /* See NOTES */
 #include <sys/socket.h>

/*creates an endpoint for communication and returns a descriptor*/
int socket(int domain, int type, int protocol);

//for example
int sfd = sockrt(AF_INET, SOCK_STREAM, 0);
if(sfd == -1) {
    perror("socket");
    return -1;
}

/*内核实现*/
SYSCALL_DEFINE3(socket, int, family, int, type, int, protocol)
{
	int retval;
	struct socket *sock;
	int flags;

	/* Check the SOCK_* constants for consistency.  */
	BUILD_BUG_ON(SOCK_CLOEXEC != O_CLOEXEC);
	BUILD_BUG_ON((SOCK_MAX | SOCK_TYPE_MASK) != SOCK_TYPE_MASK);
	BUILD_BUG_ON(SOCK_CLOEXEC & SOCK_TYPE_MASK);
	BUILD_BUG_ON(SOCK_NONBLOCK & SOCK_TYPE_MASK);

	flags = type & ~SOCK_TYPE_MASK;
	if (flags & ~(SOCK_CLOEXEC | SOCK_NONBLOCK))
		return -EINVAL;
	type &= SOCK_TYPE_MASK;

	if (SOCK_NONBLOCK != O_NONBLOCK && (flags & SOCK_NONBLOCK))
		flags = (flags & ~SOCK_NONBLOCK) | O_NONBLOCK;

	retval = sock_create(family, type, protocol, &sock);
	if (retval < 0)
		goto out;

	retval = sock_map_fd(sock, flags & (O_CLOEXEC | O_NONBLOCK));
	if (retval < 0)
		goto out_release;

out:
	/* It may be already another descriptor 8) Not kernel problem. */
	return retval;

out_release:
	sock_release(sock);
	return retval;
}

/*部分函数解释*/
pf = rcu_dereference(net_families[family]);
static const struct net_proto_family inet_family_ops = {
	.family = PF_INET,
	.create = inet_create,
	.owner	= THIS_MODULE,
};//family协议族
```

#### 三、网络通信

##### 1.网络协议

###### 1.1 什么是计算机网络

* 计算机网络是指将地理位置不同的具有独立功能的多台计算机及其外部设备，通过有形或无形的通信线路连接起来，在网络操作系统、网络管理软件及网络通信协议的管理和协调下，实现资源共享和信息传递的计算机系统。

###### 1.2 什么是网络协议

* 网络协议是一种特殊的软件，是计算机网络实现其功能的最基本的机制。网络协议的本质就是规则，即各种硬件和软件必须遵守的共同规则。网络协议不是一套单独的软件，他融合于所涉及网络通信的软件甚至硬件之中，因此可以说协议在网络中无处不在。

###### 1.3 什么是协议栈？

* 为了减少网络设计的复杂性，绝大多数网络采用分层设计的方法。所谓分层设计，就是按照信息流动的过程将整个网络的整体功能分解为一个个的功能层，不同的机器上的同等功能层之间采用相同的协议，同一机器上相邻功能层之间通过接口信息进行信息传递。各层的协议和接口统称为协议栈。
* ISO（国际标准化组织）/OSI（开放系统互连）网络协议模型：
  * 应用层：业务逻辑
  * 表示层：数据的表示形式
  * 会话层：建立、管理和终止通信过程
  * 传输层：源到目的地的点对点传输
  * 网络层：路径选择、数据通道、错误检测等通信路径
  * 物理层：在数据和电平信号之间的转换

###### 1.4TCP/IP协议栈

* 传输层：TCP、UDP
* 网络层：IP、ICMP、IGMP
* 链路层：ARP、PARP

###### 1.5 消息包和数据流

* 应用层：HTTP请求=用户数据包
* 传输层：TCP头+用户数据包=TCP包
* 网络层：IP头+TCP头=IP包
* 链路层：以太网包头+IP包+以太网尾=以太网帧
* 物理层：以太网帧->电平信号
  * 发送数据流：消息自协议栈顶层向底层流动，逐层打包。
  * 接受数据流：消息自协议栈底层向顶层流动，逐层解析。

###### 1.6 IP地址

* IP地址（互联网协议地址）是一种地址格式，为互联网上的每个网络和主机分配一个逻辑地址，其目的是为了消除物理地址的差异性。IP地址的计算机内部用一个网络字节序的32位无符号整型表示。通常习惯将其表示为点分十进制整数字符串的形式。

* ```c
  点分十进制整数字符串：1.2.3.4
  内存布局：|0x01|0x02|0x03|0x04
  		低地址--------->高地址
  网络字节序就是大端字节序，高位在低地址，低位在高地址。
  ```

* 一台计算机的IP地址=网络地址+主机地址

  ```c
  A级地址：以0为首8位网络地址+24位主机地址
  B级地址：以10为首16位网络地址+16位主机地址
  C级地址：以110为首24位网络地址+8位主机地址
  D级地址：以1110为首的32为多(组)播地址
  ```

* 主机IP地址 & 子网掩码 = 网络地址
* 主机IP地址 & ~子网掩码 = 主机地址

##### 2.套接字

######  2.1 什么是套接字

* Socket套接字，一个由系统内核负责维护，通过文件描述符访问的对象，可用于同一台机器或不同机器中的进程之间的通信。

* 进程表项

  ```c
  文件描述符表
  0：文件描述符标志|*->标准输入文件表项->键盘
  1：文件描述符标志|*->标准输出文件表项->显示器
  2：文件描述符标志|*->套接字对象->网卡
      应用程序					应用程序
         v						 v
  磁盘文件的文件描述符 		    表述网络的文件描述符
         v						 v
      文件对象				   套接字对象
         v						 v
      文件系统				   网络协议栈
         v						 v	
      磁盘设备					网络设备
  套接字也可以被视为是围绕表示网络文件的文件描述符的一套函数库。调用其中的函数就可以访问网络上的数据，实现不同主机间的通信功能。
  ```

###### 2.2 绑定和连接

```c
套接字就是系统内核内存中的一块数据-----------逻辑对象
    								      |绑定（bind）
包含了IP地址和端口号等参数的网络设备----------物理对象
互联网
  |<-网络地址：192.168.182.0
子网络
  |<-主机地址：0.0.0.48
计算机
  |<-端口号：80
 应用
通过IP地址端口号就可以唯一定位互联网上的一个通信引用
主机A
应用程序
   |
逻辑对象（套接字）
   |绑定（bind）			     连接（connection）
物理对象（IP地址和端口号）--------物理对象（IP地址和端口号）
    								|绑定（bind）
   								 逻辑对象（套接字）
    								 |
    							   应用程序
    								主机B
```

##### 3.常用函数

