# Web-Client--REST-API

This project simulates the interaction between a web client and a REST-API. 
The server commands are processed in a loop until receiving the "exit" command.
I used the parson library for handling the JSON REST API objects.

In the "client.c" file:
  
    -the variables "connected" and "access_to_library" are initialized to false. These variables will be
     used to verify if the user is logged in and has access to the library.
    -when the server receives the "register" command, a prompt will be provided for introducing the credentials.
     The program checks that the username is not already used.
    -for the "login" command, the user will be connected and the credentials will be saved.
    -for the "logout" command, the connection will be closed and the user will no longrer have access to library.
    -for the "enter_library" command, it will be verified that the user is logged in and a message will be shown.
    -for the "get_books" command, the books will be displayed on the screen
    -for the "add_book" command, it will be created a json object using the data received from the user.
    -for the "delete_book" command, the book with the specified id will be deleted.
    -for the "logout" command, the variables "connected" and "access_to_library" will be set on false and the
    client will be disconnected.
    
