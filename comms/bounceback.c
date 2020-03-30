
#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int set_interface_attribs (int fd, int speed, int parity) {
    // from https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
    struct termios tty;
    if (tcgetattr (fd, &tty) != 0)
    {
            // error_message ("error %d from tcgetattr", errno);
            return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
        return -1;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: ./bounceback string bytes_to_send (can be -1 for all) \n");
        return 1;
    }

    char *message = argv[1];
    int bytes_to_send = atoi(argv[2]);
    if (bytes_to_send == -1)
            bytes_to_send = strlen(message);
    char *portname = "/dev/ttyS4"; // this may vary per machine
    int fd = open(portname, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "error %d opening %s: %s\n", errno, portname, strerror(errno));
        return 1;
    }

    set_interface_attribs(fd, 9600, 0);  // 9600 baud, 1 start bit, 8 data bits, 1 stop bit

    int sent = write(fd, message, bytes_to_send);
    printf("sent %d bytes\n", sent);
    if (sent != bytes_to_send) {
            return 1;
    }

    char buf [100];
    printf("waiting for response...\n");
    int n = read (fd, buf, sizeof(buf));
    if (n < 0) {
        fprintf(stderr, "error %d: %s\n", errno, strerror (errno));
        return 1;
    }
    printf("read %d bytes\n", n);
    int i;
    for (i = 0; i < n; i++)
        printf("char: %c, as int: %d\n", buf[i], (int) buf[i]);

    close(fd);
    return 0;
}

