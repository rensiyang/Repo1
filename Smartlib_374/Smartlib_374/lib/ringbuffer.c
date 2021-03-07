/*************************************************************************
	> File Name: ringbuffer.c
	> Author: 
	> Mail: 
	> Created Time: Tue 18 Apr 2017 11:11:39 AM CST
 ************************************************************************/

#include <stdio.h>
#include "ringbuffer.h"

void Ring_Buffer_Init(ring_buffer_t *buffer, char *buf, ring_buffer_size_t buffer_size)
{
    buffer->tail_index = 0;
    buffer->head_index = 0;
    
    if(buffer_size > 0)
    {
	if(NULL != buf)
	{
		buffer->buffer = buf;
	}
	else
	{
		buffer->buffer = malloc(buffer_size);
	}
    	buffer->RING_BUF_SIZE = buffer_size;
    	buffer->RING_BUF_MASK = buffer_size-1;	
    }
}

void Ring_Buffer_Free(ring_buffer_t *buffer)
{ 
    free(buffer->buffer);
}

void Ring_Buffer_Queue(ring_buffer_t *buffer, char data)
{
    if(Ring_Buffer_is_Full(buffer)){
        /*is going to overwrite the oldest byte*/
        /*Increase tail index*/
        buffer->tail_index = ( (buffer->tail_index + 1) & (buffer->RING_BUF_MASK) );
    }
    /*place data in buffer*/
    buffer->buffer[buffer->head_index] = data;
    buffer->head_index = ( (buffer->head_index + 1) & (buffer->RING_BUF_MASK) );
}
void Ring_Buffer_Queue_Arr(ring_buffer_t *buffer, const char *data, ring_buffer_size_t size)
{
    ring_buffer_size_t i = 0;
    for(i = 0; i < size; i++){
        Ring_Buffer_Queue(buffer,data[i]);
    }
}

ring_buffer_size_t Ring_Buffer_Dequeue(ring_buffer_t *buffer, char *data)
{
    if(Ring_Buffer_is_Empty(buffer)){
        return 0;
    }
    *data = buffer->buffer[buffer->tail_index];
    buffer->tail_index = ( (buffer->tail_index + 1) & (buffer->RING_BUF_MASK) );
    return 1;
}
ring_buffer_size_t Ring_Buffer_Dequeue_Arr(ring_buffer_t *buffer, char *data, ring_buffer_size_t len)
{
    if(Ring_Buffer_is_Empty(buffer)){
        /*No items*/
        return 0;
    }

    char *data_ptr = data;
    ring_buffer_size_t cnt = 0;
    while((cnt < len) && Ring_Buffer_Dequeue(buffer,data_ptr)){
        cnt++;
        data_ptr++;
        if(Ring_Buffer_is_Empty(buffer)){
            /*No items*/
            break;
        }
    }
    return cnt;
}
ring_buffer_size_t Ring_Buffer_Peek(ring_buffer_t *buffer, char *data, ring_buffer_size_t index)
{
    if(index >= Ring_Buffer_num_Items(buffer)){
        return 0;
    }

    /*add index to pointer*/
    ring_buffer_size_t data_index = ( (buffer->tail_index+index) & (buffer->RING_BUF_MASK) );
    *data = buffer->buffer[data_index];

    return 1;
}

ring_buffer_size_t Ring_Buffer_Empty_Bytes_Cnt(ring_buffer_t *buffer)
{
	return (ring_buffer_size_t)(buffer->RING_BUF_SIZE) - Ring_Buffer_num_Items(buffer);
}

inline ring_buffer_size_t Ring_Buffer_is_Empty(ring_buffer_t *buffer)
{
    return (buffer->head_index == buffer->tail_index);
}

inline ring_buffer_size_t Ring_Buffer_is_Full(ring_buffer_t *buffer)
{
    return ( (buffer->head_index - buffer->tail_index) & (buffer->RING_BUF_MASK) ) == (buffer->RING_BUF_MASK);
}

inline ring_buffer_size_t Ring_Buffer_num_Items(ring_buffer_t *buffer)
{
    return ( (buffer->head_index - buffer->tail_index) & (buffer->RING_BUF_MASK) );
}



