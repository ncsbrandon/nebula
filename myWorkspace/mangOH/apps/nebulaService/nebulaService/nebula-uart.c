#include "legato.h"
#include "nebula-uart.h"

#include <pthread.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <stdio.h> //printf
#include <string.h>    //strlen

#define BUFFER_SIZE 100

int UARTOpen
(
    const char *port
)
{
    LE_INFO("Opening serial %s", port);
    int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        LE_INFO("Error [%d] opening uart %s...", fd, port);
        return LE_FAULT;
    }

    fcntl(fd, F_SETFL, 0); /* Reads will be blocking */
    struct termios options;
    tcgetattr(fd, &options);
    (void)cfsetispeed(&options, B115200); /* (void) is to stop warning in cygwin */
    (void)cfsetospeed(&options, B115200);
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;     /* 8 bits */
    options.c_cflag &= ~CSTOPB; /* 1 stop bit */
    options.c_cflag &= ~PARENB; /* no parity */
    options.c_cflag &= ~PARODD;
    options.c_cflag &= ~CRTSCTS; /* HW flow control off */
    options.c_lflag = 0;         /* RAW input */
    options.c_iflag = 0;         /* SW flow control off, no parity checks etc */
    options.c_oflag &= ~OPOST;   /* RAW output */
    options.c_cc[VTIME] = 10;    /* 1 sec */
    options.c_cc[VMIN] = BUFFER_SIZE;
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(fd, TCSAFLUSH, &options);

    LE_INFO("Opened serial %s", port);

    return fd;
}

void UARTClose
(
    int fd
)
{
    close(fd);
    LE_INFO("Closed serial");
}

le_result_t UARTWriteByte
(
    int fd,
    uint8_t data
)
{
    le_result_t result;

    const int writeResult = write(fd, &data, 1);
    if (writeResult < 0)
    {
        LE_ERROR("uart write failed with error %d", writeResult);
        result = LE_FAULT;
    }
    else
    {
        LE_DEBUG("uart write [%d]", writeResult);
        result = LE_OK;
    }

    return result;
}

le_result_t UARTWriteBuffer
(
    int fd,
    char* data,
    uint8_t len
)
{
    le_result_t result;

    const int writeResult = write(fd, data, len);
    if (writeResult < 0)
    {
        LE_ERROR("uart write failed with error %d", writeResult);
        result = LE_FAULT;
    }
    else
    {
        LE_DEBUG("uart write [%d]", writeResult);
        result = LE_OK;
    }

    return result;
}

le_result_t UARTReadBuffer
(
    int fd,
    char* data,
    uint8_t len
)
{
    le_result_t result;

    memset(data, 0, len);

    int readResult = read(fd, data, len-1);
    if (readResult < 0)
    {
        LE_ERROR("uart read failed with error %d", readResult);
        result = LE_FAULT;
    }
    else
    {
        LE_DEBUG("uart buffer [%d]", readResult);
        data[readResult]='\0';
        result = LE_OK;
    }

    return result;
}

le_result_t UARTWriteSentence
(
    int fd,
    char* data
)
{
    le_result_t result = UARTWriteBuffer(fd, data, strlen(data));
    if(result != LE_OK)
        return result;
    
    result = UARTWriteByte(fd, '\r');
    if(result != LE_OK)
        return result;

    result = UARTWriteByte(fd, '\n');
    if(result != LE_OK)
        return result;

    return LE_OK;
}

le_result_t UARTReadSentence
(
    int fd,
    char* data,
    uint8_t len
)
{
    le_result_t result;

    memset(data, 0, len);

    int n = 0;
    int i = 0;
    char buf = '\0';
    do {
        n = read(fd, &buf, 1);
        if(buf != '\r' && buf != '\n') {
            LE_DEBUG("uart read n[%d] i[%d] buf[%c]", n, i, buf);
            sprintf(data + i, "%c", buf);
            i += n;
        } else {
            LE_DEBUG("uart ignore n[%d] i[%d] buf[%c]", n, i, buf);
        }
    } while( buf != '\r' && n > 0);

    if (n < 0) {
        LE_ERROR("uart read failed with error [%d]", n);
        result = LE_FAULT;
    }
    else if (i == 0) {
        LE_ERROR("nothing to read");
        result = LE_FAULT;
    }
    else {
        LE_DEBUG("uart sentence [%d]", i);
        data[i]='\0';
        result = LE_OK;
    }

    return result;
}