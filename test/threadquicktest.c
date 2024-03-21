#include <connection.h>
#include <status.h>
#include <pthread.h>

typedef struct thread_params{
  char name[MAXBUFLEN];
  char msg[MAXBUFLEN];
} thread_params;

int32_t thread1(thread_params* params){

  printf("%s\n %s\n", params->name, params->msg);
  sleep(5);
  return 0;
}

int32_t thread2(thread_params* params){
  printf("%s\n %s\n", params->name, params->msg);
  sleep (10);
  return 1;
}


int main (int argc, char* argv[]){
	
	pthread_t t_one, t_two;
	int32_t t_create_rv = 10;


	int32_t t_join_rv = 10;
	int32_t rv = 10;
	int32_t *p_rv = &rv;
	
	thread_params params1, params2;


	strcpy(params1.name,"thread1");
	strcpy(params1.msg, "hello");
	strcpy(params2.name,"thread2");
	strcpy(params2.msg, "goodbye");

	t_create_rv = pthread_create(&t_one, NULL, (void*)thread1, &params1);

	if(!t_create_rv){
		printf("thread1 thread created.\n");
	}

	t_create_rv = pthread_create(&t_two, NULL, (void*)thread2, &params2);

	if(!t_create_rv){
		printf("thread2 thread created.\n");
	}
	
	//do{
		t_join_rv = pthread_join(t_one, (void*)p_rv);
		printf("try to join thread1: %d\n", t_join_rv);

		t_join_rv = pthread_join(t_two, (void*)p_rv);
		printf("try to join thread2: %d\n", t_join_rv);
		sleep(5);
	//} while (rv !=1);

	printf("t_create_rv %d, t_join_rv %d, rv %d\n", t_create_rv, t_join_rv, rv);
	
	printf("Exiting Nice.\n");

	return 0;
}