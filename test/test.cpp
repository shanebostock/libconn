#include <connection.h>
#include <status.h>
#include <thread>
#include <functional>

status_e default_argument_handler(conn_type_e type, conn_param_s &params){

	params.type = type;
	strcpy(params.port,PORT);
	strcpy(params.node,IP);	

	return STATUS_OK_E;
}

status_e input_argument_handler(){

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

	std::thread t_receiver(receiver, std::ref(RecvConn));
	std::thread t_sender(sender, std::ref(SendConn));

	t_receiver.join();
	t_sender.join();
	printf("Exiting.\n");

	return 0;
}