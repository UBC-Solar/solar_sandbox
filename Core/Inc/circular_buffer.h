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
} circ_buf_t;

int circ_buf_enqueue(circ_buf_t *pBuffer, float data, uint8_t maxlen);
int circ_buf_empty(circ_buf_t *p_circ_bbuf_t);


#endif /* INC_CIRCULAR_BUFFER_H_ */


