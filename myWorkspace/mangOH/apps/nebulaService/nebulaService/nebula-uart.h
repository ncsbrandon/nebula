#ifndef NEBULA_UART_H
#define NEBULA_UART_H

#include "legato.h"

LE_SHARED int UARTOpen
(
    const char* port
);

LE_SHARED void UARTClose
(
    int fd
);

LE_SHARED le_result_t UARTWriteByte
(
    int fd,
    uint8_t data
);

LE_SHARED le_result_t UARTWriteBuffer
(
    int fd,
    char* data,
    uint8_t len
);

LE_SHARED le_result_t UARTReadBuffer
(
    int fd,
    char* data,
    uint8_t len
);

LE_SHARED le_result_t UARTWriteSentence
(
    int fd,
    char* data
);

LE_SHARED le_result_t UARTReadSentence
(
    int fd,
    char* data,
    uint8_t len
);

#endif // NEBULA_UART_H