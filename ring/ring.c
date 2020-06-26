#include <stdint.h>
#include "ring.h"

void ring_init(Ring *ring, uint8_t *buffer, uint32_t size)
{
  ring->data = buffer;
  ring->size = size;
  ring->head = 0;
  ring->tail = 0;
}

bool ring_push(Ring *ring, uint8_t data)
{
  bool success = false;
  if (((ring->tail + 1) % ring->size) != ring->head)
  {
    ring->data[ring->tail++] = data;
    ring->tail %= ring->size;
    success = true;
  }
  return (success);
}

bool ring_pop(Ring *ring, uint8_t *data)
{
  bool success = false;
  if (ring->head != ring->tail)
  {
    *data = ring->data[ring->head++];
    ring->head %= ring->size;
    success = true;
  }
  return (success);
}

bool ring_full(Ring *ring)
{
  if (ring->tail > ring->head)
  {
    return (1 == (ring->tail - ring->head));
  }
  else
  {
    return (1 == (ring->head - ring->tail));
  }
}

