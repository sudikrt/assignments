sudo_ftp_server: server_main.o sftp_rpc_server_init.o sftp_rpc_server_start.o server_response_queue.o check_file_exist.o send_data_to_client.o sudo_ftp_client
	gcc -o sudo_ftp_server server_main.o sftp_rpc_server_init.o sftp_rpc_server_start.o server_response_queue.o check_file_exist.o send_data_to_client.o -lpthread 
sudo_ftp_client: socket_header.h client_main.o sftp_rpc_client.o client_request.o get_data_from_server.o get_doc_from_server.o
	gcc -o sudo_ftp_client client_main.o sftp_rpc_client.o client_request.o get_data_from_server.o get_doc_from_server.o
get_data_from_server.o: get_data_from_server.c
	gcc -g -c get_data_from_server.c
sftp_rpc_server_init: sftp_rpc_server_init.c
	gcc -g -c sftp_rpc_server_init.c
sftp_rpc_server_start.o: sftp_rpc_server_start.c
	gcc -g -c sftp_rpc_server_start.c -lpthread
server_main.o: server_main.c socket_header.h
	gcc -g -c server_main.c -lpthread
server_response_queue.o: server_response_queue.c socket_header.h
	gcc -g -c server_response_queue.c
check_file_exist.o: check_file_exist.c socket_header.h
	gcc -g -c check_file_exist.c
send_data_to_client.o: send_data_to_client.c socket_header.h
	gcc -g -c send_data_to_client.c
client_main.o: client_main.c socket_header.h
	gcc -g -c client_main.c
client_request.o: client_request.c socket_header.h
	gcc -g -c client_request.c
get_doc_from_server.o: get_doc_from_server.c socket_header.h
	gcc -g -c get_doc_from_server.c
sftp_rpc_client.o: sftp_rpc_client.c socket_header.h 
	gcc -g -c sftp_rpc_client.c 
