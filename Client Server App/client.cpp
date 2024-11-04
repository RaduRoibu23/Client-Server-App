#include "include/json.hpp"
#include "include/requests.hpp"
#include "include/helpers.hpp"
#include <iostream>

using namespace std;
using namespace nlohmann;

const char* HOST = "34.246.184.49"; // IP-ul serverului dat in enunt
const int PORT = 8080; // Portul pe care asculta serverul
const char* REGISTER = "/api/v1/tema/auth/register"; // Ruta pentru inregistrare
const char* LOGIN = "/api/v1/tema/auth/login"; // Ruta pentru logare
const char* LOGOUT = "/api/v1/tema/auth/logout"; // Ruta pentru delogare
const char* ACCESS = "/api/v1/tema/library/access"; // Ruta pentru acces la biblioteca
const char* BOOKS = "/api/v1/tema/library/books"; // Ruta pentru carti
const char* CONTENT = "application/json";  // Tipul de continut

int main(void)
{
  /* Deschid conexiunea TCP cu serverul  */
  int socket_fd;

  string login_cookie; // Cookie-ul de login
  string token;      // Token-ul de acces la biblioteca

  string empty_cookie = ""; // Cookie-ul gol
  string empty_token = ""; // Token-ul gol

  while (1)
  {
    string comanda_input;
    getline(cin, comanda_input);
    // Stergerea spatiilor albe din comanda
    for (auto space = comanda_input.begin(); space != comanda_input.end(); ++space) {
        if (*space == ' ' || *space == '\t' || *space == '\n' || *space == '\r' || *space == '\f' || *space == '\v') {
            comanda_input.erase(space);
            --space;
        }
    }
    // Verificarea comenzii si executarea acesteia in functie de caz
    //Deschiderea && inchiderea conexiunii cu serverul  trebuie efectuate la fiecare comanda
    if (comanda_input == "register")
    {
        socket_fd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0); // Deschiderea conexiunii cu serverul
        array<string, 2> info = user_info("register");
        if (info[0].empty() || info[1].empty())
          continue;
        
        json j;  // Crearea unui obiect JSON
        // Adaugarea informatiilor in obiectul JSON
        j["username"] = info[0];
        j["password"] = info[1];

        string bd = j.dump();  // Serializarea obiectului JSON

        string mesaj = compute_post_request(HOST, REGISTER, CONTENT, bd, empty_cookie, empty_token);
        send_to_server(socket_fd, mesaj);

        string response = receive_from_server(socket_fd);
        string error = extract_server_error(response);

        if (error.empty())
        {
          cout << "Te-ai inregistrat cu succes." << endl;
        }
        else
        {
          cout <<"Eroare. "<< error << endl;
        }

        close_connection(socket_fd);
      } else if (comanda_input == "login") {
        socket_fd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        array<string, 2> info = user_info("login");
        json j;
        j["username"] = info[0];
        j["password"] = info[1];

        string bd = j.dump();

        string mesaj = compute_post_request(HOST, LOGIN, CONTENT, bd, empty_cookie, empty_token);
        send_to_server(socket_fd, mesaj);

        string response = receive_from_server(socket_fd);

        login_cookie = extract_cookie(response);
        string error = extract_server_error(response);

        if (error.empty())
          cout << "Utilizatorul a fost logat cu succes." << endl;
        else
          cout << ""<< endl;
        
        close_connection(socket_fd);

        
      } else if (comanda_input == "enter_library") {
        socket_fd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        if (login_cookie.empty()) {
          cout << "Trebuie sa te loghezi inainte. Eroare" << endl;
          continue;
          
        }
        string mesaj = compute_get_request(HOST, ACCESS, login_cookie, empty_token);
        send_to_server(socket_fd, mesaj);

        string response = receive_from_server(socket_fd);
        token = extract_token(response);
        string error = extract_server_error(response);

        if (token.empty())
          cout << "Nu ai acces la biblioteca. Eroare" << endl;
        else
          cout << "Succes. Utilizatorul are acces la biblioteca." << endl;
        close_connection(socket_fd);

        
    }else if (comanda_input == "get_books") {
        socket_fd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        if (token.empty())
        {
          cout << "Nu ai acces la biblioteca. Eroare" << endl;
          continue;
        }

        string mesaj = compute_get_request(HOST, BOOKS, login_cookie, token);
        send_to_server(socket_fd, mesaj);

        string response = receive_from_server(socket_fd);
        get_books(response);

        close_connection(socket_fd);
        
      }else if (comanda_input == "get_book") {
        socket_fd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
        if (token.empty())
        {
          cout << "Nu ai acces la biblioteca. Eroare" << endl;
          continue;          
        }

        cout << "id=";
        string id;
        getline(cin, id);

        string url = string(BOOKS) + "/" + id;

        string mesaj = compute_get_request(HOST, url.c_str(), login_cookie, token);
        send_to_server(socket_fd, mesaj);

        string response = receive_from_server(socket_fd);
        string error = extract_server_error(response);
        string book = get_book(response);

        if (error.empty())
        {
          cout << book << endl;
        }
        else
        {
          cout<< "Eroare. " << error << endl;
        }

        close_connection(socket_fd);

        
      }else if (comanda_input == "add_book") {

        socket_fd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
        if (token.empty())
        {
          cout << "Nu ai acces la biblioeteca. Eroare" << endl;
          continue;          
        }

        string bd = add_book();

        string mesaj = compute_post_request(HOST, BOOKS, CONTENT, bd, login_cookie, token);
        send_to_server(socket_fd, mesaj);

        string response = receive_from_server(socket_fd);
        string error = extract_server_error(response);
        if (error.empty())
          cout << "Carte adaugata cu succes" << endl;
        close_connection(socket_fd);

        
      }else if (comanda_input == "delete_book") {
        socket_fd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
        if (token.empty())
        {
          cout << "Nu ai acces la bilbioetca. Eroare" << endl;
          continue;
          
        }

        cout << "id=";
        string id;
        getline(cin, id);
        string url = string(BOOKS) + "/" + id;

        string mesaj = compute_delete_request(HOST, url.c_str(), login_cookie, token);
        send_to_server(socket_fd, mesaj);

        string response = receive_from_server(socket_fd);
        string error = extract_server_error(response);

        if (error.empty())
        {
          cout << "Cartea " << id << " stearsa cu succes"<< endl;
          }
        else
        {
          cout<< "Eroare. " << error << endl;
        }

        close_connection(socket_fd);

        
      }else if (comanda_input == "logout") {
        socket_fd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        if (login_cookie.empty())
        {
          cout << "Trebuie sa te loghezi inainte. Eroare" << endl;
          continue;
          
        }

        string mesaj = compute_get_request(HOST, LOGOUT, login_cookie, empty_token);
        send_to_server(socket_fd, mesaj);

        string response = receive_from_server(socket_fd);
        string error = extract_server_error(response);

        if (error.empty())
        {
          cout << "Succes. Te-ai delogat." << endl;
        }

        token.clear();
        login_cookie.clear();

        close_connection(socket_fd);

        
      }else if (comanda_input == "exit") {

        close_connection(socket_fd);
        return 0;

      }else {
        cout << "Comanda gresit. Verifica!. Eroare" << endl;
        
      }
    }
  return 0;
}