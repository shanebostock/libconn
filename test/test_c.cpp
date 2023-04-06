#include <connection.h>
#include <status.h>
#include <thread>
#include <functional>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef struct msg{
  char msg[1024];
  unsigned short freshness = 0;
} msg_s;

void* get_memory(){

    int shm_fd;
    char name[6] = "Shane";
    char* pchar = name;
    shm_fd = shm_open(pchar, O_CREAT|O_RDWR,S_IRWXU|S_IWUSR);
    if(shm_fd < 0){
        perror("shm_open");
    }
	ftruncate(shm_fd,3*sizeof(msg_s));

	void* pshm = mmap(NULL, sizeof(msg_s), PROT_WRITE, MAP_SHARED, shm_fd, 0);
	return pshm;

}

int main(void){

    void* ptr = get_memory();


    return 0;
}