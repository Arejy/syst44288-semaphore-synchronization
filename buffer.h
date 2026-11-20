#ifndef BUFFER_H //if not defined 
#define BUFFER_H //then create BUFFER_H

typedef int buffer_item; //initializing buffer_item
#define BUFFER_SIZE 5 //defining the buffer size to 5

//Synchronization functions
void init_buffer(void); //initializes buffer
int insert_item(buffer_item item); //insert to the buffer
int remove_item(buffer_item *item); //remove from the buffer

#endif