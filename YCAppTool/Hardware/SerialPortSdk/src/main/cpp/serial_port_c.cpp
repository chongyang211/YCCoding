////
//// Created by 杨充 on 2024/4/10.
////

#include "serial_port_c.h"


#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


SerialPort::SerialPort() {

}

SerialPort::SerialPort(const char *path) {
    SerialPort::path = path;
}

speed_t SerialPort::getBaudrate(int baudrate) {
    switch (baudrate) {
        case 0:
            return B0;
        case 50:
            return B50;
        case 75:
            return B75;
        case 110:
            return B110;
        case 134:
            return B134;
        case 150:
            return B150;
        case 200:
            return B200;
        case 300:
            return B300;
        case 600:
            return B600;
        case 1200:
            return B1200;
        case 1800:
            return B1800;
        case 2400:
            return B2400;
        case 4800:
            return B4800;
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        case 230400:
            return B230400;
        case 460800:
            return B460800;
        case 500000:
            return B500000;
        case 576000:
            return B576000;
        case 921600:
            return B921600;
        case 1000000:
            return B1000000;
        case 1152000:
            return B1152000;
        case 1500000:
            return B1500000;
        case 2000000:
            return B2000000;
        case 2500000:
            return B2500000;
        case 3000000:
            return B3000000;
        case 3500000:
            return B3500000;
        case 4000000:
            return B4000000;
        default:
            return -1;
    }
}

//设置波特率
//串口传输速率，用来衡量数据传输的快慢，即单位时间内载波参数变化的次数
//比如每秒钟传送240个字符，而每个字符格式包含10位（1个起始位，1个停止位，8个数据位），这时的波特率为240Bd，比特率为10位*240个/秒=2400bps。
//波特率与距离成反比，波特率越大传输距离相应的就越短。
int SerialPort::setSpeed(int fd, int speed) {
    speed_t b_speed;
    struct termios cfg;
    //获取串口波特率
    b_speed = getBaudrate(speed);
    //如果你直接使用操作系统提供的串口通信函数（如open、tcgetattr等），可以使用termios.h头文件中的函数来获取串口的波特率设置。
    //tcgetattr函数的第一个参数是文件描述符，第二个参数是用于存储属性的结构体。函数返回值为0表示成功获取属性。
    if (tcgetattr(fd, &cfg)) {
        LOGD("tcgetattr 调用方法失败，无法获取串口属性!");
        close(fd);
        return FALSE;
    }
    // 获取波特率
    // 使用cfgetospeed函数从属性结构体中获取波特率。cfgetospeed函数的参数是指向属性结构体的指针，返回值是当前的波特率。
    speed_t baudrate = cfgetospeed(&cfg);
    LOGD("当前串口波特率：%i", baudrate);
    // 设置串口属性
    // 这块其实跟谷歌官方demo中的是一样的

    // 配置为原始模式
    // 使用cfmakeraw函数将属性结构体配置为原始模式。
    // cfmakeraw函数的参数是指向属性结构体的指针，它会将属性设置为典型的原始模式配置，包括禁用输入/输出处理、禁用软件流控制等。
    cfmakeraw(&cfg);

    //使用cfsetispeed函数来设置串口的输入波特率。
    int setISpeed = cfsetispeed(&cfg, b_speed);
    LOGE("设置输入波特率是：%i", b_speed);
    if (setISpeed == -1) {
        LOGE("无法设置输入波特率");
        close(fd);
        return FALSE;
    }

    //可以使用cfsetospeed函数来设置串口的输出波特率
    int setOSpeed = cfsetospeed(&cfg, b_speed);
    LOGE("设置输出波特率是：%i", b_speed);
    if (setOSpeed == -1) {
        LOGE("无法设置输出波特率");
        close(fd);
        return FALSE;
    }

    // 使用tcsetattr函数将修改后的属性设置回串口。
    // tcsetattr函数的第一个参数是文件描述符，第二个参数是操作标志，第三个参数是属性结构体。函数返回值为0表示成功设置属性。
    if (tcsetattr(fd, TCSANOW, &cfg)) {
        LOGE("无法设置串口属性");
        close(fd);
        return FALSE;
    }
    LOGD("串口配置成功");
    return TRUE;
}

int SerialPort::setParity(int fd, int databits, int stopbits, char parity) {
    struct termios options;
    if (tcgetattr(fd, &options) != 0) {
        LOGE("setParity设置奇偶校验方法异常");
        return FALSE;
    }
    options.c_cflag &= ~CSIZE;
    switch (databits)                                           /* Set data bits */
    {
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            LOGE("Unsupported data size!");
            return FALSE;
    }
    switch (parity) {
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB;                         /* Clear parity enable */
            options.c_iflag &= ~INPCK;                          /* Enable parity checking */
            break;
        case 'o':
        case 'O':
            options.c_cflag |= (PARODD | PARENB);               /* Set odd checking */
            options.c_iflag |= INPCK;                           /* Disnable parity checking */
            break;
        case 'e':
        case 'E':
            options.c_cflag |= PARENB;                          /* Enable parity */
            options.c_cflag &= ~PARODD;                         /* Transformation even checking */
            options.c_iflag |= INPCK;                           /* Disnable parity checking */
            break;
        case 'S':
        case 's':  /*as no parity*/
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
        default:
            LOGE("Unsupported parity!");
            return FALSE;
    }
    /* 设置停止位*/
    switch (stopbits) {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
        default:
            LOGE("Unsupported stop bits!");
            return FALSE;
    }
    /* Set input parity option */
    if (parity != 'n')
        options.c_iflag |= INPCK;
    tcflush(fd, TCIFLUSH);
    options.c_cc[VTIME] = 150;                                  /* Set timeout to 15 seconds */
    options.c_cc[VMIN] = 0;                                     /* Update the options and do it NOW */
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        LOGE("The method tcsetattr exception!");
        return FALSE;
    }
    return TRUE;
}

//打开串口文件，并且设置参数
//设置串口波特率、数据位、停止位、校验位主要操作的就是termios 结构体，对应的头文件是termios.h。
int SerialPort::openSerialPort(SerialPortConfig config) {
    isClose = false;
    //flags默认一般是0
    int flags = 0;
    //打开/dev/tty*节点。这个是核心api
    //open函数的第一个参数是文件名，第二个参数是打开模式和选项的组合。
    //O_RDONLY：以只读方式打开文件
    //O_WRONLY：以只写方式打开文件
    //O_RDWR：以读写方式打开文件
    //O_APPEND：写入数据时添加到文件末尾
    //O_CREATE：如果文件不存在则产生该文件，使用该标志需要设置访问权限位mode_t
    //O_EXCL：指定该标志，并且指定了O_CREATE标志，如果打开的文件存在则会产生一个错误
    //O_TRUNC：如果文件存在并且成功以写或者只写方式打开，则清除文件所有内容，使得文件长度变为0
    //O_NOCTTY：如果打开的是一个终端设备，这个程序不会成为对应这个端口的控制终端，如果没有该标志，任何一个输入，例如键盘中止信号等，都将影响进程。
    //O_NONBLOCK：该标志与早期使用的O_NDELAY标志作用差不多。程序不关心DCD信号线的状态，如果指定该标志，进程将一直在休眠状态，直到DCD信号线为0。
    fd = open(path, O_RDWR | flags);
    if (fd < 0) {
        LOGE("读取端口文件异常，文件路径是：%s !", path);
        return FALSE;
    }
    //设置参数
    if (!setSpeed(fd, config.baudrate)) {
        LOGE("设置波特率异常!");
        return FALSE;
    }
    //设置奇偶校验
    if (!setParity(fd, config.databits, config.stopbits, config.parity)) {
        LOGE("Set Parity Error!");
        return FALSE;
    }
    LOGD("打开串口通信成功");
    return TRUE;
}

//串口读数据
int SerialPort::readData(BYTE *data, int size) {
    int ret, retval;
    fd_set rfds;
    ret = 0;
    if (isClose) return 0;
    for (int i = 0; i < size; i++) {
        data[i] = static_cast<char>(0xFF);
    }
    FD_ZERO(&rfds);     //清空集合
    FD_SET(fd, &rfds);  //把要检测的句柄fd加入到集合里
    // TODO Async operation. Thread blocking.
    if (FD_ISSET(fd, &rfds)) {
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        retval = select(fd + 1, &rfds, NULL, NULL, NULL);
        if (retval == -1) {
            LOGE("Select error!");
        } else if (retval) {
            //LOGD("This device has data!");
            //在C++中，可以使用read函数来从文件描述符（包括串口设备）读取数据。read函数是unistd.h头文件中的一部分，用于从文件描述符中读取数据。
            //read函数的第一个参数是文件描述符，第二个参数是用于存储读取数据的缓冲区，第三个参数是要读取的最大字节数。函数返回值是实际读取的字节数。
            ret = static_cast<int>(read(fd, data, static_cast<size_t>(size)));
        } else {
            LOGE("Select timeout!");
        }
    }
    if (isClose) close(fd);
    return ret;
}

int SerialPort::writeData(BYTE *data, int len) {
    int result;
    //在C++中，可以使用write函数来进行串口通信。
    //write函数是unistd.h头文件中的一部分，用于向文件描述符（包括串口设备）写入数据。
    //write函数的第一个参数是文件描述符，第二个参数是要写入的数据缓冲区，第三个参数是要写入的数据字节数。函数返回值是实际写入的字节数。
    result = static_cast<int>(write(fd, data, static_cast<size_t>(len)));
    if (result == -1) {
        LOGE("writeData 写数据失败");
    }
    return TRUE;
}

int SerialPort::setMode(int mode) {
    struct termios options;
    if (tcgetattr(fd, &options) != 0) {
        LOGE("The method tcgetattr exception!");
        return FALSE;
    }
    if (mode != 0) {
        if (mode == 1) {
            options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);   //input
            options.c_oflag &= ~OPOST;                            //out put
        } else if (mode == 2) {
            options.c_lflag |= (ICANON | ECHO | ECHOE | ISIG);    //input
            options.c_oflag |= OPOST;                             //out put
        }
        if (tcsetattr(fd, TCSANOW, &options) != 0) {
            LOGE("The method tcsetattr exception!");
            return FALSE;
        }
    }
    return TRUE;
}

void SerialPort::closePort() {
    isClose = true;
    int close_status = close(fd);
    if (close_status == -1) {
        LOGD("关闭串口通信设备失败!");
    } else {
        LOGD("关闭串口通信设备成功!");
    }
}