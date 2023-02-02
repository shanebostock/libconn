#include <connection.h>
#include <status.h>


status_e argument_handler(int argc, char* argv[], conn_param_s &params){

    if(argc >= 2){
        
        if(strcmp("server", argv[1]) == 0 || strcmp("SERVER", argv[1]) == 0){
            params.type = CONN_TYPE_SERVER_E;
            
        }
        else if (strcmp("client", argv[1]) == 0 || strcmp("CLIENT", argv[1]) == 0){
            params.type = CONN_TYPE_CLIENT_E;
            if(argc < 4){
            	return STATUS_FAIL_E;
            }
            strcpy(params.port,argv[2]);
            strcpy(params.node,argv[3]);
        }
        else {
            return STATUS_FAIL_E; 
        }

        return STATUS_OK_E;
    }
    
    else{

        return STATUS_FAIL_E;
    }
}

int main (int argc, char* argv[]){

	conn_param_s params;

	if(argument_handler(argc, argv, params) != STATUS_OK_E){
		printf("Test Fail. Exiting. Check Logs.\n");
		printf("Arguments must supplied on the command line.\n");
        printf("Argument 1: can be either <server> or <client> \n");
        printf("Argument 2: (client only) is the port of the server to which you wish to connect. \n. ");
        printf("Argument 3: (client only) is the IPV4 address of the server you to which you wish to connect. \n");
		exit(0);
	}

	Connection con(params);

	if(con.startconnection()!=STATUS_OK_E){
		printf("Test Fail. Exiting. Check Logs.\n");
		exit(0);
	}

	return 0;
}