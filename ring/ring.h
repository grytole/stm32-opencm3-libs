#ifndef __RING_H__
#define __RING_H__

#include <stdint.h>

typedef struct {
  uint8_t *data;
  uint32_t size;
  uint32_t head;
  uint32_t tail;
} Ring;

#define ring_size(ring) ((ring)->size - 1)
#define ring_data(ring) ((ring)->data)
#define ring_empty(ring) ((ring)->head == (ring)->tail)

void ring_init(Ring *ring, uint8_t *buffer, uint32_t size);
bool ring_push(Ring *ring, uint8_t data);
bool ring_pop(Ring *ring, uint8_t *data);
bool ring_full(Ring *ring);

#endif /* __RING_H__ */

