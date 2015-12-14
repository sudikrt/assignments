sudo_ftp_server: server_main.o s_ftp_rpc_server.o accept_client.o server_response.o check_file_exist.o send_data_to_client.o sudo_ftp_client
	gcc -o sudo_ftp_server server_main.o s_ftp_rpc_server.o accept_client.o server_response.o check_file_exist.o send_data_to_client.o
sudo_ftp_client: socket_header.h client_main.o s_ftp_rpc_client.o client_send_request.o get_doc_from_server.o
	gcc -o sudo_ftp_client client_main.o s_ftp_rpc_client.o client_send_request.o get_doc_from_server.o
s_ftp_rpc_server: s_ftp_rpc_server.c
	gcc -g -c s_ftp_rpc_server.c
accept_client.o: accept_client.c
	gcc -g -c accept_client.c
server_main.o: server_main.c socket_header.h
	gcc -g -c server_main.c
server_response.o: server_response.c socket_header.h
	gcc -g -c server_response.c
check_file_exist.o: check_file_exist.c socket_header.h
	gcc -g -c check_file_exist.c
send_data_to_client.o: send_data_to_client.c socket_header.h
	gcc -g -c send_data_to_client.c
client_main.o: client_main.c socket_header.h
	gcc -g -c client_main.c
client_send_request.o: client_send_request.c socket_header.h
	gcc -g -c client_send_request.c
get_doc_from_server.o: get_doc_from_server.c socket_header.h
	gcc -g -c get_doc_from_server.c
s_ftp_rpc_client.o: s_ftp_rpc_client.c socket_header.h 
	gcc -g -c s_ftp_rpc_client.c 
