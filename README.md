# Client-Server-App

This project is a command-line client for managing a library system. It allows users to register, log in,
access the library,view books, add books, delete books, and log out.

## Key Features

### 1. User Authentication
- **Register**: Users can create a new account by providing a username and password.
- **Login**: Existing users can log in to access the library system.
- **Logout**: Users can securely log out of the system.

### 2. Library Access
- **Enter Library**: Authenticated users can gain access to the library.
- **Get Books**: Users can retrieve a list of all available books in the library.
- **Get Book**: Users can retrieve details of a specific book by its ID.
- **Add Book**: Users with access can add new books to the library.
- **Delete Book**: Users with access can delete books from the library by their ID.

## Efficiency

### 1. Modular Design
The project is organized into multiple files, each handling specific functionalities:
- `client.cpp`: Main entry point, handles user input and command execution.
- `helpers.cpp` and `helpers.hpp`: Contains helper functions for data manipulation and validation.
- `requests.cpp` and `requests.hpp`: Manages HTTP requests to the server.
- `include/json.hpp`: Handles JSON data serialization and deserialization.

### 2. JSON Handling
The project uses the `nlohmann/json` library for efficient JSON data handling. This library is:
- **Header-only**: Easy to integrate and use.
- **Efficient**: Provides fast serialization and deserialization of JSON data.
- **Robust**: Includes built-in error handling and data validation mechanisms.

### 3. HTTP Communication
The project uses custom functions to create and send HTTP requests:
- **Compute Requests**: Functions like `compute_post_request` and `compute_get_request` efficiently build HTTP requests.
- **Send and Receive**: Functions like `send_to_server` and `receive_from_server` handle communication with the server.

### 4. Error Handling
The project includes robust error handling mechanisms:
- **Extract Server Error**: Functions like `extract_server_error` parse server responses to identify and handle errors.
- **User Feedback**: Provides clear and concise feedback to users based on server responses.

### 5. Command Loop
The main command loop efficiently handles user input and executes commands:
- **Whitespace Removal**: Cleans user input to avoid errors.
- **Command Execution**: Executes commands based on user input and provides appropriate feedback.

## Conclusion

This project is designed to be efficient and user-friendly, with a clear modular structure, robust error handling, and efficient JSON and HTTP handling. It provides a comprehensive set of features for managing a library system through a command-line interface.
