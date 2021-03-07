/*************************************************************************
	> File Name: ringbuffer.h
	> Author: 
	> Mail: 
	> Created Time: Tue 18 Apr 2017 11:11:54 AM CST
 ************************************************************************/

#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#ifdef __cplusplus
	extern "C"
	{
#endif

//#include <stddef.h>
#include <sys/types.h>

//#define RING_BUF_SIZE       (1024*128)

//#if (RING_BUF_SIZE & (RING_BUF_SIZE - 1)) != 0
//#error "RING_BUF_SIZE must be a power of two"
//#endif

typedef unsigned int ring_buffer_size_t;

//#define RING_BUF_MASK       (RING_BUF_SIZE - 1)

typedef struct ring_buffer_t_t {
    /*buffer memory*/
    //char buffer[RING_BUF_SIZE];
    char *buffer;
    /*index of tail*/
    ring_buffer_size_t tail_index;
    /*index of head*/
    ring_buffer_size_t head_index;
    
    ring_buffer_size_t RING_BUF_SIZE;
    ring_buffer_size_t RING_BUF_MASK;
}ring_buffer_t;

void Ring_Buffer_Init(ring_buffer_t *buffer, char *buf, ring_buffer_size_t buffer_size);
void Ring_Buffer_Free(ring_buffer_t *buffer);
void Ring_Buffer_Queue(ring_buffer_t *buffer, char data);
void Ring_Buffer_Queue_Arr(ring_buffer_t *buffer, const char *data, 
                                ring_buffer_size_t size);
ring_buffer_size_t Ring_Buffer_Dequeue(ring_buffer_t *buffer, char *data);
ring_buffer_size_t Ring_Buffer_Dequeue_Arr(ring_buffer_t *buffer, char *data, ring_buffer_size_t len);
ring_buffer_size_t Ring_Buffer_Peek(ring_buffer_t *buffer, char *data, ring_buffer_size_t index);
ring_buffer_size_t Ring_Buffer_Empty_Bytes_Cnt(ring_buffer_t *buffer);

inline ring_buffer_size_t Ring_Buffer_is_Full(ring_buffer_t * buffer);
inline ring_buffer_size_t Ring_Buffer_is_Empty(ring_buffer_t * buffer);
inline ring_buffer_size_t Ring_Buffer_num_Items(ring_buffer_t * buffer);

#ifdef __cplusplus
	}
#endif

#endif
