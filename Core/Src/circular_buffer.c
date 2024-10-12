/*
 * circular_buffer.c
 *
 *  Created on: Oct 1, 2024
 *      Author: diego
 */

#include "circular_buffer.h"

#define MAX_LEN 20

// Declare a circular buffer with a fixed-size array
static uint8_t buffer[MAX_LEN];

circ_bbuf_t circ_bbuf = {
    .pBuffer = buffer,
    .head = 0,
    .tail = 0,
    .num_entries = 0,
    .maxlen = MAX_LEN
};

bool circ_bbuf_empty(circ_bbuf_t *p_circ_bbuf_t) {
    return (p_circ_bbuf_t->num_entries == 0);
}

bool circ_bbuf_full(circ_bbuf_t *p_circ_bbuf_t) {
    return (p_circ_bbuf_t->num_entries == p_circ_bbuf_t->maxlen);
}


int circ_bbuf_push(circ_bbuf_t *pBuffer, uint8_t data) {
    if (pBuffer->num_entries >= pBuffer->maxlen) {
        // Buffer is full; overwrite the oldest entry
        pBuffer->tail = (pBuffer->tail + 1) % pBuffer->maxlen;
    } else {
        pBuffer->num_entries++;
    }

    // Insert the data and move the head
    pBuffer->pBuffer[pBuffer->head] = data;
    pBuffer->head = (pBuffer->head + 1) % pBuffer->maxlen;

    return 0;  // Success
}

int circ_bbuf_pop(circ_bbuf_t *pBuffer, uint8_t *data) {
    if (circ_bbuf_empty(pBuffer)) {
        return -1;  // Buffer is empty
    }

    // Retrieve data from the tail and move the tail forward
    *data = pBuffer->pBuffer[pBuffer->tail];
    pBuffer->tail = (pBuffer->tail + 1) % pBuffer->maxlen;
    pBuffer->num_entries--;

    return 0;  // Success
}


