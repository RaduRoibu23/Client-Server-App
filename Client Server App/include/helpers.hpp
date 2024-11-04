#ifndef _HELPERS_
#define _HELPERS_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include <array>

using namespace std;

// shows the current error
void error(const char *msg);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(const char *host_ip, const int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, string &message);

// receives and returns the message from a server
string receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
string basic_extract_json_response(string &str);

// extracts and returns the cookie from a server response
string extract_cookie(string &response);

// extracts and returns the token from a server response
string extract_token(string &response);

// checks if a string is a number
bool isNumber(string &s);

// creates a JSON object for a register request
array<string, 2> user_info(const string &type);

// creates a JSON object for a add_book POST request
string add_book();

// extract the error from the server
string extract_server_error(const string &response);

// extract the book from the server
string get_book(string &response);

// extract all books from the server
void get_books(string &response);

// retrieves a book id from the client
string retrieve_book_id();

#endif /* _HELPERS_ */
