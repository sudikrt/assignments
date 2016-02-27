sudo_ftp_server: server_main.o sftp_rpc_server_init.o sftp_rpc_server_start.o list.o thread_pool.o queue.o server_response_queue.o handler.o service_queue.o check_file_exist.o read_handler.o open_file_handler.o close_file_handler.o send_data_to_client.o open_file.o read_bytes.o read_chunks_handler.o sudo_ftp_client
	gcc -o sudo_ftp_server server_main.o sftp_rpc_server_init.o sftp_rpc_server_start.o list.o thread_pool.o queue.o server_response_queue.o handler.o service_queue.o check_file_exist.o read_handler.o open_file_handler.o close_file_handler.o send_data_to_client.o open_file.o read_bytes.o read_chunks_handler.o -lpthread 
sudo_ftp_client: socket_header.h client_main.o sftp_rpc_client.o client_request.o get_data_from_server.o get_doc_from_server.o handle_open_file_request.o handle_close_file_request.o handle_read_chunk_request.o 
	gcc -o sudo_ftp_client client_main.o sftp_rpc_client.o client_request.o get_data_from_server.o get_doc_from_server.o -lpthread handle_open_file_request.o handle_close_file_request.o handle_read_chunk_request.o
list.o: utils/list.c
	gcc -g -c utils/list.c
thread_pool.o: thread_pool.c
	gcc -g -c thread_pool.c -lpthread
read_bytes.o: read_bytes.c
	gcc -g -c read_bytes.c
read_chunks_handler.o: read_chunks_handler.c
	gcc -g -c read_chunks_handler.c
handle_read_chunk_request.o: handle_read_chunk_request.c
	gcc -g -c handle_read_chunk_request.c
handle_close_file_request.o: handle_close_file_request.c
	gcc -g -c handle_close_file_request.c
close_file_handler.o: close_file_handler.c
	gcc -g -c close_file_handler.c
open_file.o: open_file.c
	gcc -g -c open_file.c
handle_open_file_request.o: handle_open_file_request.c
	gcc -g -c handle_open_file_request.c
open_file_handler.o: open_file_handler.c
	gcc -g -c open_file_handler.c
queue.o: utils/queue.c
	gcc -g -c utils/queue.c -lpthread
handler.o: handler.c
	gcc -g -c handler.c
service_queue.o: service_queue.c
	gcc -g -c service_queue.c -lpthread
read_handler.o: read_handler.c
	gcc -g -c read_handler.c
get_data_from_server.o: get_data_from_server.c
	gcc -g -c get_data_from_server.c
sftp_rpc_server_init: sftp_rpc_server_init.c
	gcc -g -c sftp_rpc_server_init.c
sftp_rpc_server_start.o: sftp_rpc_server_start.c
	gcc -g -c sftp_rpc_server_start.c -lpthread
server_main.o: server_main.c
	gcc -g -c server_main.c -lpthread
server_response_queue.o: server_response_queue.c
	gcc -g -c server_response_queue.c -lpthread
check_file_exist.o: check_file_exist.c
	gcc -g -c check_file_exist.c
send_data_to_client.o: send_data_to_client.c socket_header.h
	gcc -g -c send_data_to_client.c
client_main.o: client_main.c socket_header.h
	gcc -g -c client_main.c
client_request.o: client_request.c socket_header.h
	gcc -g -c client_request.c -lpthread
get_doc_from_server.o: get_doc_from_server.c socket_header.h
	gcc -g -c get_doc_from_server.c
sftp_rpc_client.o: sftp_rpc_client.c socket_header.h 
	gcc -g -c sftp_rpc_client.c
clean: 
	rm -f *.o 
