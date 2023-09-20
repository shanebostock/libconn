
#include "shmemory.h"

int main(int argc, char const *argv[])
{
	packet_s* shmem = get_shared_memory();
	packet_s* reader = shmem;
	packet_s* writer = shmem;
	packet_s writedata;
	packet_s readdata;
	const packet_s* write = &writedata;
	const packet_s* read = &readdata;  

	

	int i=1;
	int loop_counter = 0;
	do{
		
		sleep(1);
		memcpy((void*)read, (void*)reader, sizeof(packet_s));
		reader->done = 1;
		printf("After 1 s, client read: %d, %s\n", readdata.seq, readdata.msgbuf);
		if(readdata.seq%10==0){
			reader = shmem;
		}
		else reader++;

		loop_counter++;        	
	} while(loop_counter < 30);

	return 0;
}