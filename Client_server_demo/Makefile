sudo_ftp_server: server_main.o server_response.o check_file_exist.o send_data_to_client.o sudo_ftp_client
	gcc -o sudo_ftp_server server_main.o server_response.o check_file_exist.o send_data_to_client.o
sudo_ftp_client: socket_header.h client_main.o get_doc_from_server.o client_send_request.o
	gcc -o sudo_ftp_client client_main.o get_doc_from_server.o client_send_request.o
server_main.o: server_main.c socket_header.h
	gcc -g -c server_main.c -l socket_header.h
server_response.o: server_response.c socket_header.h
	gcc -g -c server_response.c -l socket_header.h
check_file_exist.o: check_file_exist.c socket_header.h
	gcc -g -c check_file_exist.c -l socket_header.h
send_data_to_client.o: send_data_to_client.c socket_header.h
	gcc -g -c send_data_to_client.c -l socket_header.h
client_main.o: client_main.c socket_header.h
	gcc -g -c client_main.c -l socket_header.h
get_doc_from_server.o: get_doc_from_server.c socket_header.h
	gcc -g -c get_doc_from_server.c -l socket_header.h
client_send_request: client_send_request.c socket_header.h
	gcc -g -c client_send_request.c -l socket_header.h
