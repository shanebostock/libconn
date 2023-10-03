#include <connection.h>
#include <status.h>
#include <pthread.h>


#if 0
#include <functional.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

void print_input_error_message(){
    	
		printf("Arguments must supplied on the command line.\n");
        printf("Argument 1: \t MY PORT. \n");
        printf("Argument 2: \t SERVER PORT. \n");
        printf("Port assignment must be greater than 1024. \n");
        // printf("\t\tif argument is 0 default port <%s> will be used.\n", PORT);
        // printf("\t\tfor servers use 0.");
        // printf("Argument 3: \tis the IPV4 address of the server you to which you want your client to connect.\n");
        // printf("\t\tif argument is 0 default IP <%s> will be used.\n", IP);
        // printf("\t\tfor servers use 0.");
        printf("\nTest Fail. Exiting. Check Logs.\n");
        exit(0);	

}

status_e cmd_argument_handler(int argc, char* argv[], conn_params_s *params){

	if(argc >= 3){
        // read params from command line
  
        
    	if(atoi(argv[1]) > 1024){
    		strcpy(params->myport,argv[1]);
    	}
    	else{
    		print_input_error_message();
    	}
    
    	if(atoi(argv[2]) > 1024){
    	    strcpy(params->serverport,argv[2]);
    	}
    	else{
    		print_input_error_message();
    	}

        return STATUS_OK_E;
    }

    else{
    	print_input_error_message();
        return STATUS_FAIL_E;
    }
}

#if 0
status_e conf_argument_handler(){

	return STATUS_FAIL_E;
}

status_e start(Connection &con, conn_param_s &params){

	return con.startconnection(params);
}

#endif 

void test_libconn(){

	status_e rv = linker_test_func(-4);
	if(rv == STATUS_OK_E){
		printf("Status: OK\n");
	}
}

int main (int argc, char* argv[]){
	
	pthread_t t_listener, t_talker;
	int32_t t_create_rv;
	
	
	

	conn_params_s params;
	cmd_argument_handler(argc, argv, &params);
	

	t_create_rv = pthread_create(&t_listener, NULL, (void*)listener, &params);

	if(!t_create_rv){
		printf("Listener thread created.\n");
	}

	t_create_rv = pthread_create(&t_talker, NULL, (void*)talker, &params);

	if(!t_create_rv){
		printf("Talker thread created.\n");
	}
	
    /* main thread waits here */
	do{
		sleep(5);
	} while (1);
	#if 1
	int32_t t_join_rv;
	int32_t t_rv;
	int32_t *p_rv = &t_rv;
	t_join_rv = pthread_join(t_listener, (void*)p_rv);
	if(!t_join_rv){
		printf("Listener thread joined.\n");
	}
	
	t_join_rv = pthread_join(t_talker, (void*)p_rv);
	if(!t_join_rv){
		printf("Talker thread joined.\n");
	}
	#endif



	
	printf("Exiting Nice.\n");

	return 0;
}