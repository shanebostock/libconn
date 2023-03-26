#include <connection.h>
#include <status.h>


status_e cmd_argument_handler(int argc, char* argv[], conn_param_s &params){

	if(argc >= 2){
        // read params from command line
        if(strcmp("server", argv[1]) == 0 || strcmp("SERVER", argv[1]) == 0){
            params.type = CONN_TYPE_SERVER_E;

        }
        else if (strcmp("client", argv[1]) == 0 || strcmp("CLIENT", argv[1]) == 0){
            
            params.type = CONN_TYPE_CLIENT_E;
            
            if(argc < 4){
            	return STATUS_FAIL_E;
            }
            if(strcmp("0",argv[2]) == 0){
            	strcpy(params.port,PORT);
            }
            else{
            	strcpy(params.port,argv[2]);	
            }
            
            strcpy(params.node,argv[3]);
        }
        else {
            return STATUS_FAIL_E; 
        }

        return STATUS_OK_E;
    }
    
    else{
    	printf("Test Fail. Exiting. Check Logs.\n");
		printf("Arguments must supplied on the command line.\n");
        printf("Argument 1: can be either <server> or <client>.\n");
        printf("Argument 2: (client only) is the port of the server to which you wish to connect.\n");
        printf("Argement 2: (client only) if argument is 0 default port <%s> will be used.\n", PORT);
        printf("Argument 3: (client only) is the IPV4 address of the server you to which you wish to connect.\n");
        return STATUS_FAIL_E;
    }
}

status_e default_argument_handler(conn_type_e type, conn_param_s &params){

	parms.type = type;
	strcpy(params.port,PORT);
	strcpy(params.node,IP);	

	return STATUS_OK_E;
}

status_e input_argument_handler(){

	return STATUS_FAIL_E;
}

int main (int argc, char* argv[]){

	conn_param_s ob_params;
	conn_param_s ib_params;
	conn_param_s cmd_params;

	if(argc >= 2){
		if(argument_handler(argc, argv, params) != STATUS_OK_E){
			exit(0);
		}
	}
	else{
		if(manual_handler(ob_params, ib_params) != STATUS_OK_E){
			exit(0);
		}
	}

	Connection IBcon(params);
	Connection OBcon(params);

	if(con.startconnection()!=STATUS_OK_E){
		printf("Test Fail. Exiting. Check Logs.\n");
		exit(0);
	}

	return 0;
}