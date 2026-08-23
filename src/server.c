#include "../inc/define.h"



/* TODO: Add structure to the define/header file. */
typedef struct {
    int id;
    int socket;
} client_struct_t;

client_struct_t server_sock_clients_data[MAX_CLIENTS];
/* TODO: Create one structure for configuration server data. Where add server_sock_client_count, socket ID and other. */
int server_sock_client_count = 0;
int server_sock_id_count = 1;
/* TODO: Update the variable name with server_sock_mutex_lock */
pthread_mutex_t server_sock_lock;

/* TODO: If function which cannot used for different file then the function should static. */
void server_sock_send_list (int socket)
{
   /* TODO: Macro should contain the file name.
    * Ex: BUFF_SIZE -> SERVER_SOCK_BUFF_SIZE */
    char avlbl_clients[BUFF_SIZE] = "Available Clients:";

    pthread_mutex_lock (&server_sock_lock);
    for (int i = 0; i < server_sock_client_count; i++)
    {
       /* TODO: How you can optimise below logic.  */
        char temp[CLIENT_LIST_SIZE] = {0};

        sprintf (temp, " %d", server_sock_clients_data[i].id);
        /* TODO: What's the puprose of strcat? */
        strcat (avlbl_clients, temp);
    }
    pthread_mutex_unlock (&server_sock_lock);
    send (socket, &avlbl_clients, strlen(avlbl_clients), 0);
}

void server_sock_remove_client (int socket)
{
    int idx = -1;

    pthread_mutex_lock (&server_sock_lock);
    /* TODO: Optimise the below logic and handlr using one loop.  */
    for (int i = 0; i < server_sock_client_count; i++)
    {
        if (server_sock_clients_data[i].socket == socket)
        {
            idx = i;
            break;
        }
    }

    if (idx != -1)
    {
        for (int i = idx; i < server_sock_client_count; i++)
        {
            server_sock_clients_data[i] = server_sock_clients_data[i+1];
        }
        server_sock_client_count -= 1 ;
    }
    pthread_mutex_unlock (&server_sock_lock);
}

void* server_sock_client_handler (void *arg)
{
    int n = 0;
    char buff[BUFF_SIZE];
    int data_id = 0;
    int data_socket = 0;

    client_struct_t data = {0};
    memcpy(&data, arg, sizeof(data));

    /* TODO: Why this two times copy data? */
    data_id = data.id;
    data_socket = data.socket;

    while (1)
    {
        char temp[RCV_MSG_SIZE] = {0};
        char msg[RCV_MSG_SIZE] = {0};
        /* TODO: Why below need inside while-1 */
        char *err1 = "Invalid Client ID, check list!";
        char *err2 = "Send correct msg format:\n<client_id>:<message>";
        int send_id = 0, send_sock = -1;

        n = recv (data_socket, &buff, sizeof(buff), 0);
        if (n <= 0)
        {
            printf("Error : Client %d Disconnected!\n", data_id);
            server_sock_remove_client (data_socket);
            close (data_socket);
            break;
        }

        buff[n] = '\0';

        if (strcmp (buff, "list") == 0)
        {
            server_sock_send_list (data_socket);
            continue;
        }

        if ( sscanf(buff, "%d:%[^\n]", &send_id, msg) == 2)
        {
            pthread_mutex_lock (&server_sock_lock);
            for (int i = 0; i < server_sock_client_count; i++)
            {
                if (server_sock_clients_data[i].id == send_id)
                {
                    send_sock = server_sock_clients_data[i].socket;
                    break;
                }
            }
            pthread_mutex_unlock (&server_sock_lock);

            if (send_sock != -1)
            {
                char out[BUFF_SIZE];
                sprintf(out, "From Client %d : %s", data_id, msg);

                if ( send(send_sock, out, strlen(out), 0) > 0)
                {
                    printf ("Sent msg from client %d to client %d\n", data_id, send_id);
                }
            }
            else
            {
                send (data.socket, err1, strlen(err1), 0);
            }
        }
        else
        {
            send (data.socket, err2, strlen(err2), 0);
        }
    }
    pthread_exit(NULL);
}

int server_process_function (int argc, char *argv[])
{
    int server_fd = 0 ;
    struct sockaddr_in server;

    if (argc != 2)
    {
        printf("Input @cmdline format: ./a.out port_number\n ");
        return 1;
    }

    pthread_mutex_init (&server_sock_lock, NULL);

    server_fd = socket (AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1)
    {
        printf("Socket creation failed!\n");
        return 1;
    }
    else
    {
        printf("Socket Creation Successfull!\n");
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));;
    server.sin_addr.s_addr = INADDR_ANY;

    if ( bind (server_fd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror ("bind error");
        return 1;
    }

    if ( listen (server_fd, BACKLOG) == -1)
    {
        perror ("listen error");
        return 1;
    }

    while (1)
    {
        int id = 0, client_fd = 0;
        pthread_t client_thread;

        /* TODO: If we don't want to wait and stay on blocking mode. How we should handlr? */
        client_fd = accept ( server_fd, NULL, NULL);
        if (client_fd <= 0)
        {
            printf ("Failed to accept!\n");
            continue;
        }

        pthread_mutex_lock (&server_sock_lock);

        id = server_sock_id_count++;
        server_sock_clients_data[server_sock_client_count].id = id;
        server_sock_clients_data[server_sock_client_count].socket = client_fd;

        server_sock_client_count++;

        send (client_fd, &id, sizeof(id), 0);
        printf ("Client %d Connected!\n", id);

        pthread_mutex_unlock (&server_sock_lock);

        pthread_create(&client_thread, NULL, server_sock_client_handler, &server_sock_clients_data[server_sock_client_count-1]);
        pthread_detach(client_thread);
    }

    close(server_fd);
    return 0;
}

