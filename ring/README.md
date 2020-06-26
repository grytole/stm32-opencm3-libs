```c
#include "ring.h"

uint8_t dataBuf[255];
Ring myRingBuf;

ring_init(&myRingBuf, &dataBuf, sizeof(dataBuf));

...

if (false == ring_push(&myRingBuf, 0x42)) {
  /* no space left - buffer is full */
}

uint8_t ch;
if (false == ring_pop(&myRingBuf, &ch)) {
  /* no data to pop - buffer is empty */
}

if (ring_empty(&myRingBuf)) {
  /* buffer is empty */
}

if (ring_full(&myRingBuf)) {
  /* buffer is full */
}

uint32_t maxBytesAvailable = ring_size(&myRingBuffer);
```
