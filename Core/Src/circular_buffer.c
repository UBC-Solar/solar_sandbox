/*
 * circular_buffer.c
 *
 *  Created on: Oct 1, 2024
 *      Author: diego
 */

#include "circular_buffer.h"

bool circ_buf_full(circ_buf_t *p_circ_bbuf_t, uint8_t maxlen) {
    return (p_circ_bbuf_t->num_entries == maxlen);
}

bool circ_buf_empty(circ_buf_t *p_circ_bbuf_t) {
    return (p_circ_bbuf_t->num_entries == 0);
}

int circ_buf_enqueue(circ_buf_t *pBuffer, float data, uint8_t maxlen) {
    if (circ_buf_full(pBuffer, maxlen)) {
        // Buffer is full; overwrite the oldest entry
        pBuffer->tail = (pBuffer->tail + 1) % maxlen;
    } else {
        pBuffer->num_entries++;
    }

    // Insert the data and move the head
    pBuffer->pBuffer[pBuffer->head] = data;
    pBuffer->head = (pBuffer->head + 1) % maxlen;

    return 0;  // Success
}


