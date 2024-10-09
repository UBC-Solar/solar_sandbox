#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define WINDOW_SIZE 5

uint8_t currentIdx = 0;
uint16_t sum = 0;
uint16_t last = 0;
uint16_t circularBuffer[WINDOW_SIZE] = {0};
uint8_t isFirstWindow = 1;

void slidingWindowAverage(uint16_t bits);

int main()
{

    // Add test cases here
    slidingWindowAverage(3);
    slidingWindowAverage(5);
    slidingWindowAverage(7);
    slidingWindowAverage(9);
    slidingWindowAverage(2);

    slidingWindowAverage(5);
    slidingWindowAverage(2);
    slidingWindowAverage(7);
    slidingWindowAverage(8);
    slidingWindowAverage(3);

    return 0;
}

void slidingWindowAverage(uint16_t bits)
{
    // index of 'stale' element to be removed
    uint8_t removeIdx = currentIdx % WINDOW_SIZE;
    sum -= circularBuffer[removeIdx];

    // add new element to the buffer
    circularBuffer[removeIdx] = bits;

    // add new element to the sum
    sum += bits;

    currentIdx++;

    // calculate and print the average
    if (isFirstWindow && currentIdx == WINDOW_SIZE)
    {
        printf("Window avg: %f\n", (float)sum / (float)WINDOW_SIZE);
        isFirstWindow = 0;
    }
    else if (!isFirstWindow)
    {
        printf("Window avg: %f\n", (float)sum / (float)WINDOW_SIZE);
    }
}
