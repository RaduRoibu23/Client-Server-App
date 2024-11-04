#ifndef REQUESTS_H
#define REQUESTS_H

#include <string>
using namespace std;


// computes and returns a POST request string (cookies can be NULL if not needed)
string compute_post_request(const char *host, const char *url, const char *content_type,
                                 const string &body_data, const string &cookie, const string &token);

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
string compute_get_request(const char *host, const char *url, const string &cookie, const string &token);

string compute_delete_request(const char *host, const char *url, const string &cookie, const string &token);

#endif // REQUESTS_H
