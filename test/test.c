#include <connection.h>
#include <status.h>
#include <pthread.h>


#if 0
#include <functional.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
void print_input_error_message(){
    	
		printf("Arguments must supplied on the command line.\n");
        printf("Argument 1: \tcan be either <server> or <client>.\n");
        printf("Argument 2: \tis the port of the server to which you want your client to connect.\n");
        printf("\t\tif argument is 0 default port <%s> will be used.\n", PORT);
        printf("\t\tfor servers use 0.");
        printf("Argument 3: \tis the IPV4 address of the server you to which you want your client to connect.\n");
        printf("\t\tif argument is 0 default IP <%s> will be used.\n", IP);
        printf("\t\tfor servers use 0.");
        printf("\nTest Fail. Exiting. Check Logs.\n");	

}
status_e cmd_argument_handler(int argc, char* argv[], conn_param_s &params){

	if(argc >= 4){
        // read params from command line
        if(strcmp("server", argv[1]) == 0 || strcmp("SERVER", argv[1]) == 0){
            params.type = CONN_TYPE_SERVER_E;

        }
        else if (strcmp("client", argv[1]) == 0 || strcmp("CLIENT", argv[1]) == 0){           
            params.type = CONN_TYPE_CLIENT_E;
        }
        else {
        	print_input_error_message();
        	return STATUS_FAIL_E;
        }    
        
    	if(strcmp("0",argv[2]) == 0){
    		strcpy(params.port,PORT);
    	}
    	else{
    		strcpy(params.port,argv[2]);	
    	}
        
    	if(strcmp("0",argv[3]) == 0){
    		strcpy(params.node,IP);
    	}
    	else{
    		strcpy(params.node,argv[3]);	
    	}

        return STATUS_OK_E;
    }

    else{
    	print_input_error_message();
        return STATUS_FAIL_E;
    }
}

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


	test_libconn();
	
	printf("Exiting Nice.\n");

	return 0;
}