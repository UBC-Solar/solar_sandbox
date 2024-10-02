/*
 * http.h
 *
 *  Created on: Oct 1, 2024
 *      Author: evan
 */

#ifndef INC_HTTP_H_
#define INC_HTTP_H_
#define JSON 1

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct jsonContents{
	char *key;
	char* content;
};
typedef struct jsonContents jsonField;

struct httpPost {
    char* endpoint;
    char* host;
    int contentType;
    jsonField* json;

    // ...
};

typedef struct httpPost postRequest;


char* postFormat(postRequest* p1);
uint16_t getContentLength(postRequest* p1);

#endif /* INC_HTTP_H_ */
