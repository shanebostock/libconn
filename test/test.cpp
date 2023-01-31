#include <connection.h>
#include <status.h>

int main (int argc, char* argv[]){
	conn_param_s params;
	params.type = CONN_TYPE_SERVER_E;

	Connection con(params);

	if(con.startconnection()!=STATUS_OK_E){
		printf("Test Fail. Exiting. Check Logs.\n");
		exit(0);
	}

	return 0;
}