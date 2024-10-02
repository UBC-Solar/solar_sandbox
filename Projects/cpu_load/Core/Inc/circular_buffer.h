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

typedef struct{
	uint8_t *const pBuffer;
	int head;
	int tail;
	const int maxlen;
	bool full;
}circ_bbuf_t;

int circ_bbuf_push(circ_bbuf_t *pBuffer, uint8_t data);
int circ_bbuf_pop(circ_bbuf_t *pBuffer, uint8_t *data);


#endif /* INC_CIRCULAR_BUFFER_H_ */
