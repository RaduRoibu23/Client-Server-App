#include "include/helpers.hpp"
#include "include/requests.hpp"
#include "include/json.hpp"
#include <iostream>
#include <regex>


using namespace std;
using namespace nlohmann;

constexpr auto HEADER_TERMINATOR = "\r\n\r\n";
constexpr auto CONTENT_LENGTH = "Content-Length: ";

constexpr auto BUFLEN = 4096;
void error(const char *msg)
{
  perror(msg);
  exit(0);
}

int open_connection(const char *host_ip, const int portno, int ip_type, int socket_type, int flag)
{
  struct sockaddr_in serv_addr;
  int sockfd = socket(ip_type, socket_type, flag);
  if (sockfd < 0)
    error("ERROR opening socket");

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = ip_type;
  serv_addr.sin_port = htons(portno);
  inet_aton(host_ip, &serv_addr.sin_addr);

  /* Connect the socket */
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  return sockfd;
}

void close_connection(int sockfd)
{
  close(sockfd);
}

void send_to_server(int sockfd, string &message)
{
  int bytes, sent = 0;
  int total = message.size();

  do
  {
    bytes = send(sockfd, message.data() + sent, total - sent, 0);
    if (bytes < 0)
    {
      error("ERROR writing message to socket");
    }

    if (bytes == 0)
    {
      break;
    }

    sent += bytes;
  } while (sent < total);
}

string receive_from_server(int sockfd)
{
    char response[BUFLEN];
    string buffer;
    int header_end = 0;
    int content_length = 0;

    int bytes = read(sockfd, response, BUFLEN);

if (bytes < 0) {
    cerr << "ERROR reading response from socket" << endl;
} else {
    while (bytes > 0) {
        buffer.append(response, bytes);

        header_end = buffer.find(HEADER_TERMINATOR, 0);

        if (header_end >= 0) {
            header_end += strlen(HEADER_TERMINATOR);

            int content_length_start = buffer.find(CONTENT_LENGTH, 0);

            if (content_length_start >= 0) {
                content_length_start += strlen(CONTENT_LENGTH);
                content_length = strtol(buffer.substr(content_length_start).c_str(), NULL, 10);
                break;
            }
        }

        bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0) {
            cerr << "ERROR reading response from socket" << endl;
            break;
        }
    }
}
    size_t total = content_length + header_end;
    
    while (buffer.length() < total) {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0) {
            cerr << "ERROR reading response from socket" << endl;
            break;
        }

        if (bytes == 0) {
            break;
        }

        buffer.append(response, bytes);
    }

    return buffer;
}
string basic_extract_json_response (string &str)
{
  return strstr(str.c_str(), "{\"");
}
string extract_cookie(string &response)
{
  smatch match;
  regex cookie_pattern("Set-Cookie: (.*?);");

  if (regex_search(response, match, cookie_pattern) && match.size() > 1) {
    return match.str(1);
  } else {
    return "";
  }
}

string extract_token(string &response)
{
    auto token_start = response.find("{");
    auto token_end = response.rfind("}");

    if (token_start != string::npos && token_end != string::npos) {
        string json_str = response.substr(token_start, token_end - token_start + 1);
        json j = json::parse(json_str);
        return j["token"];
    }

    return "";
}

bool isNumber(string &s)
{
  if (s.empty()) return false;

  for (char c : s) {
    if (!isdigit(c)) return false;
  }

  return true;
}

bool isInvalidCredentials(const string &username, const string &password)
{
  auto contains_space = [](const string &s) {
    if (s.empty()) {
      return true;
    }

    for (char c : s) {
      if (c == ' ') {
        return true;
      }
    }

    return false;
    };

  return contains_space(username) || contains_space(password);
}

array<string, 2> user_info(const string &type)
{
  array<string, 2> info;
  string username, password;

  if (type == "register" || type == "login")
  {
    cout << "username=";
    getline(cin, username);

    cout << "password=";
    getline(cin, password);

    if (isInvalidCredentials(username, password))
    {
      cout << "Eroare. NU trebuie sa ai spatii" << endl;
      return {};
    }
  }

  info[0] = username;
  info[1] = password;

  return info;
}

string add_book()
{
  json j;
  string title, author, genre, page_count, publisher, book;

  cout << "title=";
  getline(cin, title);

  cout << "author=";
  getline(cin, author);

  cout << "genre=";
  getline(cin, genre);

  cout << "publisher=";
  getline(cin, publisher);

  cout << "page_count=";
  getline(cin, page_count);
  if (!isNumber(page_count))
  {
    cout << "Nu este un numar. Eroare" << endl;
    return "";
  }

  j["title"] = title;
  j["author"] = author;
  j["genre"] = genre;
  j["page_count"] = page_count;
  j["publisher"] = publisher;

  book = j.dump();

  return book;
}
string extract_server_error(const string &response)
{
  size_t startPos = response.find('{');
  if (startPos == string::npos)
  {
    return "";
  }

  string jsonString = response.substr(startPos);
  json jsonData;

  try
  {
    jsonData = json::parse(jsonString);
  }
  catch (const json::parse_error &)
  {
    return "";
  }

  if (jsonData.contains("error"))
  {
    return jsonData["error"];
  }

  return "";
}

string get_book(string &response)
{
  auto start_pos = response.find_first_of('{');
  auto end_pos = response.find_last_of('}');

  if (start_pos != string::npos && end_pos != string::npos) {
    string json_str = response.substr(start_pos, end_pos - start_pos + 1);
    return json_str;
  }

  return "";
}

void get_books(string &response)
{
  auto start_pos = response.find('[');
  auto end_pos = response.rfind(']');

  if (start_pos != string::npos && end_pos != string::npos) {
    string json_str = response.substr(start_pos, end_pos - start_pos + 1);

    auto j = json::parse(json_str);

    if (j.is_array() && j.empty()) {
      cout << "Nu exista carti in biblioetca. Eroare" << endl;
      return;
    }

    cout << json_str << endl;
  }
}

string retrieve_book_id()
{
  string id;

  while (true)
  {
    cout << "id=";
    getline(cin, id);
    id.erase(remove_if(id.begin(), id.end(), ::isspace), id.end());
    if (isNumber(id))
    {
      break;
    }
    else
    {
      cout << "Invalid ID. Eroare" << endl;
      break;
    }
  }
  return id;
}
