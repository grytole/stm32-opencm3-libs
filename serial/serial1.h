#ifndef __SERIAL1_H__
#define __SERIAL1_H__

#include <stdint.h>

void serial1_setup(uint32_t baudrate);

uint32_t serial1_read(uint8_t *data, uint32_t size);
uint32_t serial1_write(uint8_t *data, uint32_t size);
uint32_t serial1_printf(const char *format, ...);

void serial1_rx_buffer_status(uint32_t *bytes_used, uint32_t *bytes_free);
void serial1_tx_buffer_status(uint32_t *bytes_used, uint32_t *bytes_free);

#endif /* __SERIAL1_H__ */
