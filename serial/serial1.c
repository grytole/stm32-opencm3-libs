#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "serial1.h"

#define BUFFER_SIZE (256)

uint8_t tx_buffer[BUFFER_SIZE] = {};
uint8_t rx_buffer[BUFFER_SIZE] = {};

typedef struct {
  uint8_t *data;
  uint32_t size;
  uint32_t head;
  uint32_t tail;
} ring_t;

ring_t tx_ring = {
  .data = tx_buffer,
  .size = sizeof(tx_buffer),
  .head = 0,
  .tail = 0
};

ring_t rx_ring = {
  .data = rx_buffer,
  .size = sizeof(rx_buffer),
  .head = 0,
  .tail = 0
};

static bool ring_push(ring_t *ring, uint8_t data);
static bool ring_pop(ring_t *ring, uint8_t *data);
static uint32_t ring_used(ring_t *ring);
static uint32_t ring_length(ring_t *ring);

static bool ring_push(ring_t *ring, uint8_t data)
{
  if (((ring->tail + 1) % ring->size) != ring->head)
  {
    ring->data[ring->tail++] = data;
    ring->tail %= ring->size;
    return true;
  }
  return false;
}

static bool ring_pop(ring_t *ring, uint8_t *data)
{
  if (ring->head != ring->tail)
  {
    *data = ring->data[ring->head++];
    ring->head %= ring->size;
    return true;
  }
  return false;
}

static uint32_t ring_used(ring_t *ring)
{
  if (ring->tail >= ring->head)
  {
    return (ring->tail - ring->head);
  }
  else
  {
    return (ring->size - (ring->head - ring->tail));
  }
}

static uint32_t ring_length(ring_t *ring)
{
  return ((1 < ring->size) ? (ring->size - 1) : (0));
}

void serial1_setup(uint32_t baudrate)
{
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_AFIO);
  rcc_periph_clock_enable(RCC_USART1);

  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

  usart_set_baudrate(USART1, baudrate);
  usart_set_databits(USART1, 8);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_stopbits(USART1, USART_STOPBITS_1);
  usart_set_mode(USART1, USART_MODE_TX_RX);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

  nvic_enable_irq(NVIC_USART1_IRQ);
  nvic_set_priority(NVIC_USART1_IRQ, 1);

  usart_enable_tx_interrupt(USART1);
  usart_enable_rx_interrupt(USART1);

  usart_enable(USART1);
}

void usart1_isr(void)
{
  uint8_t data = 0;

  if (true == usart_get_flag(USART1, USART_SR_TXE))
  {
    if (true == ring_pop(&tx_ring, &data))
    {
      usart_send(USART1, data);
    }
    else
    {
      usart_disable_tx_interrupt(USART1);
    }
  }

  if (true == usart_get_flag(USART1, USART_SR_RXNE))
  {
    data = (uint8_t)usart_recv(USART1);
    ring_push(&rx_ring, data);
  }
}

uint32_t serial1_read(uint8_t *data, uint32_t size)
{
  uint32_t i = 0;
  uint32_t read_num = 0;

  if ((NULL != data) && (0 < size))
  {
    for (i = 0; i < size; i++)
    {
      if (false == ring_pop(&rx_ring, &data[i]))
      {
        break;
      }
      read_num++;
    }
  }

  return read_num;
}

uint32_t serial1_write(uint8_t *data, uint32_t size)
{
  uint32_t i = 0;
  uint32_t write_num = 0;

  if ((NULL != data) && (0 < size))
  {
    for (i = 0; i < size; i++)
    {
      if (false == ring_push(&tx_ring, data[i]))
      {
        break;
      }
      write_num++;
    }

    usart_enable_tx_interrupt(USART1);
  }

  return write_num;
}

uint32_t serial1_printf(const char *format, ...)
{
  char str[256] = {};
  va_list args;
  uint32_t n = 0;

  va_start(args, format);
  (void)vsnprintf(str, sizeof(str), format, args);
  va_end(args);

  while ('\0' != str[n])
  {
    if (false == ring_push(&tx_ring, str[n]))
    {
      break;
    }
    n++;
  }

  if (0 < n)
  {
    usart_enable_tx_interrupt(USART1);
  }

  return n;
}

void serial1_rx_buffer_status(uint32_t *bytes_used, uint32_t *bytes_free)
{
  uint32_t used = ring_used(&rx_ring);
  uint32_t len = ring_length(&rx_ring);

  if (NULL != bytes_used)
  {
    *bytes_used = used;
  }

  if (NULL != bytes_free)
  {
    *bytes_free = len - used;
  }
}

void serial1_tx_buffer_status(uint32_t *bytes_used, uint32_t *bytes_free)
{
  uint32_t used = ring_used(&tx_ring);
  uint32_t len = ring_length(&tx_ring);

  if (NULL != bytes_used)
  {
    *bytes_used = used;
  }

  if (NULL != bytes_free)
  {
    *bytes_free = len - used;
  }
}
