


#include "shmemory.h"


int main() {

  
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
  


#if 0
  int pid = fork();
  printf("starting addresses:\n reader %p\n, writer %p\n, shmem %p\n", reader, writer, shmem);
  if (pid == 0) {
    int i=1;
    while(1){
		
    	writedata.seq = i;
		memcpy((void*)writer, (void*)write, sizeof(packet_s));
		if(i%3==0){
			writer = shmem;
		}
		else writer++;
		i++;
	printf("addresses:\n reader %p\n, writer %p\n, shmem %p\n", reader, writer, shmem);		
		sleep(1);        	
    }
    

  } else {
    
    while(1){
		sleep(1);
		memcpy((void*)read, (void*)reader, sizeof(packet_s));
		printf("After 1 s, parent read: %d\n", readdata.seq);
		if(readdata.seq%3==0){
			reader = shmem;
		}
		else reader++;
    	
  	}	
  
}
#endif
	return 0;
}