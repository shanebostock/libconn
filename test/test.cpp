#include <connection.h>
#include <status.h>
#include <thread>
#include <functional>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct msg{
  char msg[1024];
  unsigned short freshness = 0;
} msg_s;

void* get_memory(int& shm_fd, const char* name){
	
    shm_fd = shm_open(name, O_CREAT|O_RDWR,S_IRWXU|S_IWUSR);
    if(shm_fd < 0){
        perror("shm_open");
    }
	ftruncate(shm_fd,3*sizeof(msg_s));

	void* pshm = mmap(NULL, sizeof(msg_s), PROT_WRITE, MAP_SHARED, shm_fd, 0);
	return pshm;

}
/*
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
*/
status_e default_argument_handler(conn_type_e type, conn_param_s &params){

	params.type = type;
	strcpy(params.port,PORT);
	strcpy(params.node,IP);	

	return STATUS_OK_E;
}

status_e conf_argument_handler(){

	return STATUS_FAIL_E;
}

void receiver(Connection &con){

	if(con.startconnection() !=STATUS_OK_E){
		printf("Receiver: Test Fail. Exiting. Check Logs.\n");
		return;
	}
	return;
	
}

void sender(Connection &con){
	if(con.startconnection() !=STATUS_OK_E){
		printf("Sender: Test Fail. Check Logs.\n");
		return;
	}
	return;
}

int main (int argc, char* argv[]){

	status_e rv;
	const char* name = "Shane";
	int shm_fd = 0;
	void* ptr = get_memory(shm_fd, name);
	const char* msg1 = "Hello";
	const char* msg2 = "my name is";
	const char* msg3 = "Computer";
	msg_s* pmsg = ptr;
	for (int i = 0; i<3; i++){
		msg_s msg;
		memset(msg.msg,msg1,sizeof(msg1));
		msg.freshness = 1;
		memset(pmsg,msg,sizeof(msg));
		psmg++;
	}
	shm_unlink(name);
	conn_param_s recv_params;
	conn_param_s send_params;

	rv = default_argument_handler(CONN_TYPE_SENDER_E, send_params);
	if(rv != STATUS_OK_E){
		exit(0);
	}
	rv = default_argument_handler(CONN_TYPE_RECEIVER_E,recv_params);
	if(rv != STATUS_OK_E){
		exit(0);
	}

	Connection RecvConn(recv_params);
	Connection SendConn(send_params);

	//std::thread t_processor(processor, )
	std::thread t_receiver(receiver, std::ref(RecvConn));
	std::thread t_sender(sender, std::ref(SendConn));

	t_receiver.join();
	t_sender.join();
	printf("Exiting.\n");

	return 0;
}