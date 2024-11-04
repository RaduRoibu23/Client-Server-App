#include "include/helpers.hpp"
#include "include/requests.hpp"
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <iostream>
using namespace std;


// GET request generation
string compute_get_request(const char *host, const char *url, const string &cookie, const string &token) {
    string request;

    request.reserve(256);  // Optimizare pentru a evita realocările frecvente functioneaza si fara

    request.append("GET ").append(url).append(" HTTP/1.1\r\n")
           .append("Host: ").append(host).append("\r\n");

    if (!cookie.empty()) {
        request.append("Cookie: ").append(cookie).append("\r\n");
    }

    if (!token.empty()) {
        request.append("Authorization: Bearer ").append(token).append("\r\n");
    }

    request.append("Connection: close\r\n\r\n");

    return request;
}

// POST request generation
string compute_post_request(const char *host, const char *url, const char *content_type,
                                 const string &body_data, const string &cookie, const string &token) {
    string request;

    request.reserve(256 + body_data.length());

    request.append("POST ").append(url).append(" HTTP/1.1\r\n")
           .append("Host: ").append(host).append("\r\n")
           .append("Content-Type: ").append(content_type).append("\r\n")
           .append("Content-Length: ");

    ostringstream content_length_stream;
    content_length_stream << body_data.length();
    request.append(content_length_stream.str()).append("\r\n");

    if (!cookie.empty()) {
        request.append("Cookie: ").append(cookie).append("\r\n");
    }

    if (!token.empty()) {
        request.append("Authorization: Bearer ").append(token).append("\r\n");
    }

    request.append("Connection: close\r\n\r\n")
           .append(body_data).append("\r\n");

    return request;
}

// DELETE request generation
string compute_delete_request(const char *host, const char *url, const string &cookie, const string &token) {
    string request;

    request.reserve(256);

    request.append("DELETE ").append(url).append(" HTTP/1.1\r\n")
           .append("Host: ").append(host).append("\r\n");

    if (!cookie.empty()) {
        request.append("Cookie: ").append(cookie).append("\r\n");
    }
    //tip schema autentificare Bearer
    if (!token.empty()) {
        request.append("Authorization: Bearer ").append(token).append("\r\n");
    }

    request.append("Connection: close\r\n\r\n");

    return request;
}
