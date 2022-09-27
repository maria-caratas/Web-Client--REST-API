#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

int main(int argc, char *argv[]) {
    char host[] = "34.241.4.235";
    int port = 8080;
    int sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
    bool connected = false, exit = false,  access_to_library = false;
    char command[100], cookie[100], token[BUFLEN];
    char *user1;

    while (!exit) {
        // Get the next command    
        scanf("%s", command);
        
        // Exit command
        if(strcmp(command, "exit") == 0) {
            exit = true;
        } 

        // Open a new connection
        sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
        
        // Register command
        if (strcmp(command, "register") == 0) {
            // Get the credentials
            char *username = calloc(100, sizeof(char));
            char *password = calloc(100, sizeof(char));
            printf("username=");
            scanf("%s", username);
            printf("password=");
            scanf("%s", password);
            JSON_Value *value = json_value_init_object();
            JSON_Object *object = json_value_get_object(value);
            json_object_set_string(object, "username", username);
            json_object_set_string(object, "password", password);
            user1 = json_serialize_to_string(value);
            char *message = compute_post_request(host, "/api/v1/tema/auth/register", "application/json", user1, 1, NULL, 0, "is taken");
	        send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            if(strstr(response, "is taken") == NULL) {
                printf("You are now registered.\n");
            } else {
                printf("The username has already been used. Please try another one.\n");
            }
        } 

        // Authentication command
        if (strcmp(command, "login") == 0) {
            // Get the credentials
            connected = true;
            char *username = calloc(100, sizeof(char));
            char *password = calloc(100, sizeof(char));
            printf("username=");
            scanf("%s", username);
            printf("password=");
            scanf("%s", password);
            JSON_Value *value = json_value_init_object();
            JSON_Object *object = json_value_get_object(value);
            json_object_set_string(object, "username", username);
            json_object_set_string(object, "password", password);
            user1 = json_serialize_to_string(value);        
            char *message = compute_post_request(host, "/api/v1/tema/auth/login", "application/json", user1, 1, NULL, 0, NULL);
	        send_to_server(sockfd, message);
            char *response = receive_from_server(sockfd);
            char *p = strstr(response, "Set-Cookie: ");
            if (p == NULL) {
                printf("Login failed!\n");
                connected = false;
                access_to_library = false;
                continue;
            }
            strtok(p, ";");
            p = p +12;
            strcpy(cookie, p);
            if (cookie != NULL) {
                printf("Logged in\n");
            }
        }  

        // Logout command
        if (strcmp(command, "logout") == 0) {
            if (!connected) {
                printf("You have to be logged in first.\n");
            } else {
                char *message = compute_get_request(host, "/api/v1/tema/auth/logout", NULL, cookie, 1, token, "get");
	            send_to_server(sockfd, message);
                receive_from_server(sockfd);
                connected = false;
                access_to_library = false;
                printf("Logged out.\n");
            }
        }

        // Access command
        if (strncmp(command, "enter_library", 13) == 0) {
            if (!connected) {
                printf("Please login first.\n");
            } else {
                access_to_library = true;
                // Get the token
                char *message = compute_get_request(host, "/api/v1/tema/library/access", NULL, cookie, 1, token, "get");
	            send_to_server(sockfd, message);
                char *response = receive_from_server(sockfd);
                char *temp = strstr(response, "token");
                if (temp == NULL) {
                    printf("You don't have acces!\n");
                    access_to_library = false;
                } else {
                    temp = temp + 8;
                    memset(token, 0, BUFLEN);
                    strcpy(token, temp);
                    token[strlen(token) - 2] = '\0';
                    printf("Access allowed.\n");
                }
            }
        }
        
        // get_books command
        if (strcmp(command, "get_books") == 0) {
            if (!access_to_library) {
                printf("Enter the library first.\n");
            } else {
                char *message = compute_get_request(host, "/api/v1/tema/library/books", NULL, cookie, 1, token, "get");
	            send_to_server(sockfd, message);
                char *response = receive_from_server(sockfd);
                char *res = strstr(response, "[");
                printf("%s\n", res);
            }
        }

         // add_book command
        if (strcmp(command, "add_book") == 0) {
            		if (access_to_library) {
					char title[100], author[100], genre[100], publisher[100];
                    char *new;
					int page_count;
					printf("title=");
					scanf("%s", title);		
					printf("author=");
					scanf("%s", author);
					printf("genre=");
					scanf("%s", genre);
					printf("publisher=");
					scanf("%s", publisher);
					printf("page_count=");
                    scanf("%d", &page_count);
					char pages_string[BUFLEN];
					sprintf(pages_string, "%d", page_count);
					JSON_Value *value = json_value_init_object();
					JSON_Object *object = json_value_get_object(value);
					json_object_set_string(object, "title", title);
					json_object_set_string(object, "author", author);
					json_object_set_string(object, "genre", genre);
					json_object_set_string(object, "page_count", pages_string);
					json_object_set_string(object, "publisher", publisher);
					new = json_serialize_to_string(value);
                    char *message = compute_post_request(host, "/api/v1/tema/library/books", "application/json", new, 1, NULL, 0, token);
                    send_to_server(sockfd, message);
                    receive_from_server(sockfd);
					printf("The book was successfully added!\n");
				} else {
					printf("Enter the library first!\n");
				}
			}

        // get_book command
        if (strcmp(command, "get_book") == 0) {
            if (access_to_library) {
                char aux[BUFLEN];
                int id = 0;
                printf("id=");
                scanf("%d", &id);
                sprintf(aux, "%s/%d", "/api/v1/tema/library/books", id);
                char *message = compute_get_request(host, aux, NULL, cookie, 1, token, "get");
	            send_to_server(sockfd, message);
                char *response = receive_from_server(sockfd);
                char *temp = strstr(response, "No book was found!");
                if (temp != NULL) {
                    printf("No book.\n");
                } else {
                    char *res = strstr(response,"[");
                    printf("%s\n", res);
                }
            } else {
                printf("Enter the library first.\n");
            }

        }

        // delete_book command
        if (strcmp(command, "delete_book") == 0) {
            if (access_to_library) {
                char aux[BUFLEN];
                int id = 0;
                printf("id=");
                scanf("%d", &id);        
                sprintf(aux, "%s/%d", "/api/v1/tema/library/books",id);
                char *message = compute_get_request(host, aux, NULL, cookie, 1, token, "delete");
	            send_to_server(sockfd, message);
                char *response = receive_from_server(sockfd);
                char *temp = strstr(response, "No book was deleted!");
                if (temp != NULL) {
                    printf("No book.\n");
                    continue;
                } else {
                    printf("The book was succesfully deleted!\n");
                }
            } else {
                    printf("Enter the library first.\n");
            }
        }
    }
    return 0;
}
    