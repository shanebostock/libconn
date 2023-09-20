#ifndef SHMEMORY_H
#define SHMEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define KEY 1234565


typedef struct packet {
	int done;
	int seq;
	char msgbuf[64];
} packet_s;

size_t get_shm_size(){
	return sizeof(packet_s)*10;
} 
//fix the variable name from shmsize to function call

void handle_error(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

key_t get_key(){
	return KEY;
}

int get_id_new(){
	int shmid;
	key_t key = get_key();
	if((shmid = shmget(key,get_shm_size(),IPC_CREAT | 0666)) <0){
 		handle_error("shmget");
 	}
 	return shmid;
}

int get_id(){
	int shmid;
	key_t key = get_key();
	if((shmid = shmget(key,get_shm_size(),0666)) <0){
 		handle_error("shmget");
 	}
 	return shmid;
}

packet_s* create_shared_memory(){

	int shmid = get_id_new();
	packet_s *shm;
	
    if ((shm = shmat(shmid, NULL, 0)) == (packet_s *) -1) {
        handle_error("shmat");
    }
    return shm;
}

packet_s* get_shared_memory(){

	int shmid = get_id();
	packet_s *shm;
	
    if ((shm = shmat(shmid, NULL, 0)) == (packet_s *) -1) {
        handle_error("shmat");
    }
    return shm;
}

// int get_fd(){
// 	int fd;
// 	if(fd = open("/dev/null", O_RDWR) == -1){
// 		handle_error("open");
// 	}
// }


// packet_s* create_shared_memory(size_t size) {
//   // Our memory buffer will be readable and writable:
//   int protection = PROT_READ | PROT_WRITE;

//   // The buffer will be shared (meaning other processes can access it), but
//   // anonymous (meaning third-party processes cannot obtain an address for it),
//   // so only this process and its children will be able to use it:
//   int visibility = MAP_SHARED | MAP_ANONYMOUS;

//   // The remaining parameters to `mmap()` are not important for this use case,
//   // but the manpage for `mmap` explains their purpose.
//   int fd = get_fd();
//   return (packet_s*)mmap(NULL, size, protection, visibility, fd, 0);
// }


#endif /* SHMEMORY_H */