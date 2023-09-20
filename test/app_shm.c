#if __cplusplus
#include "SharedMemory.h"
#else
#include "shmemory.h"
#endif

int main() {
#if __cplusplus
	printf("I'm CPP!\n");
	
	SharedMemory<packet_s> Shm(10);
	packet_s* shmem = Shm.create_shared_memory();
	packet_s* reader = shmem;
	packet_s* writer = shmem;
	packet_s writedata;
	packet_s readdata;
	const packet_s* write = &writedata;
	const packet_s* read = &readdata;  
	
	int i=1;

	char* msg = "hello";
	do{
		writedata.done = 0;
		writedata.seq = i;	
		memcpy((void*)writedata.msgbuf,(void*)msg, sizeof(msg));

		
		if (writer->done == 0){
			writer++;
		}
		else{
			memcpy((void*)writer, (void*)write, sizeof(packet_s));
			writer++;
			i++;
		}

		if(i%10==0){
			writer = shmem;
		}
		//else {writer++;}
	
		sleep(1);        	
	} while(i < 30);


#else
	printf("I'm C!\n");
  
	packet_s* shmem = create_shared_memory();
	packet_s* reader = shmem;
	packet_s* writer = shmem;
	packet_s writedata;
	packet_s readdata;
	const packet_s* write = &writedata;
	const packet_s* read = &readdata;  

	int i=1;
	int loop_counter = 0;
	char* msg = "hello";
	do{
		writedata.done = 0;
		writedata.seq = i;	
		memcpy((void*)writedata.msgbuf,(void*)msg, sizeof(msg));

		
		if (writer->done == 0){
			//continue;
		}
		else{
			memcpy((void*)writer, (void*)write, sizeof(packet_s));
		}

		if(i%10==0){
			writer = shmem;
		}
		else {writer++;}

		i++;
	
		sleep(1);
		loop_counter++;        	
	} while(loop_counter < 30);

#endif

  
	return 0;
}