#if __cplusplus
#include "SharedMemory.h"
#else
#include "shmemory.h"
#endif

int main() {
#if __cplusplus
	printf("I'm CPP!\n");
	
	SharedMemory<packet_s> Shm(10);
	packet_s* shmem = Shm.get_shared_memory();
	packet_s* reader = shmem;
	packet_s* writer = shmem;
	packet_s writedata;
	packet_s readdata;
	const packet_s* write = &writedata;
	const packet_s* read = &readdata;  
	
	int read_counter = 1;
	do{
		
		sleep(1);
		if(reader->done == 0) {
			memcpy((void*)read, (void*)reader, sizeof(packet_s));
			reader->done = 1;
			printf("Client read: %d, %s\n", readdata.seq, readdata.msgbuf);
			reader++;
			read_counter++;	
		}
		
		if(read_counter%10==0) {
			reader = shmem;
		}
		        	
	} while(read_counter < 30);


#else
	printf("I'm C!\n");
  
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

#endif

  
	return 0;
}
