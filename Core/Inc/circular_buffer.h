/*
 * circular_buffer.h
 *
 *  Created on: Oct 1, 2024
 *      Author: diego
 */

#ifndef INC_CIRCULAR_BUFFER_H_
#define INC_CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float *pBuffer;
    int head, tail, num_entries;
    int maxlen;
} circ_bbuf_t;


int circ_bbuf_push(circ_bbuf_t *pBuffer, uint8_t data);
int circ_bbuf_pop(circ_bbuf_t *pBuffer, uint8_t *data);
bool circ_bbuf_empty(circ_bbuf_t *p_circ_bbuf_t);
bool circ_bbuf_full(circ_bbuf_t *p_circ_bbuf_t);

#endif /* INC_CIRCULAR_BUFFER_H_ */


