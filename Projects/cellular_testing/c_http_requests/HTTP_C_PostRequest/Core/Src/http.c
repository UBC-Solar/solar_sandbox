/*
 * http.c
 *
 *  Created on: Oct 1, 2024
 *      Author: evan
 */

#include "http.h"



char* postFormat(postRequest *p1) {
	char *url = p1->endpoint;
	char *content_type = "application/json";

	uint16_t content_length = getContentLength(p1);

	int request_length = snprintf(NULL, 0, "POST %s HTTP/1.1\r\n"
			"Host: httpbin.org\r\n"
			"Content-Type: %s\r\n"
			"Content-Length: %i\r\n"
			"\r\n"
			"{\"%s\":\"%s\"}\r\n\r\n", url, content_type, content_length,
			p1->json->key, p1->json->content);

	char *httpRequest = malloc(request_length + 1);
	if (httpRequest == NULL) {
		return NULL;
	}

	sprintf(httpRequest, "POST %s HTTP/1.1\r\n"
			"Host: httpbin.org\r\n"
			"Content-Type: %s\r\n"
			"Content-Length: %i\r\n"
			"\r\n"
			"{\"%s\":\"%s\"}\r\n\r\n", url, content_type, content_length,
			p1->json->key, p1->json->content);


	return httpRequest;

}

uint16_t getContentLength(postRequest *p1) {
	char *content = p1->json->content;
	char *key = p1->json->key;

	uint16_t length = 6 + strlen(content) + strlen(key);

	return length;
}
