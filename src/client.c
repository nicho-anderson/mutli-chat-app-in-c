#include "../inc/define.h"

int client_sock_socket;
bool client_sock_flag = false;

void *client_sock_receiver(void *arg)
{
    char buffer[BUFF_SIZE] = {0};

    while(1)
    {
       /* TODO: Why do you need n variable? */
        int n = 0;
        pthread_testcancel();
        n = recv (client_sock_socket, buffer, BUFF_SIZE - 1, 0);
        if (n <= 0)
        {
           /* TODO: Is it control way to termiant one thread and not informed to other thread? */
            client_sock_flag = true;
            pthread_exit (NULL);
        }
        buffer[n] = '\0';
        printf("\n%s\n", buffer);
       /* TODO: Why this print here? */
        printf("\n");
    }
}

void *client_sock_sender (void *arg)
{
    char buffer[BUFF_SIZE] = {0};

    while(1)
    {

        fgets(buffer, BUFF_SIZE, stdin);
        /* TODO: What's the diff between strcspn and strlen? and why you used strcspn? */
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp (buffer, "exit") == 0)
        {
           /* TODO: Is it control way to termiant one thread and not informed to other thread? */
            client_sock_flag = true;
            pthread_exit (NULL);
        }

        if ( send (client_sock_socket, buffer, strlen(buffer), 0) == -1 )
        {
            perror ("Send Error");
            /* TODO: Why you need continue here? */
            continue;
        }
    }
}

/* TODO: Find a way to handlr all the logic without any thread. */
int client_process_function (int argc, char *argv[])
{
    struct sockaddr_in client;
    int my_id;
    pthread_t send_thread, recv_thread;

    if ( argc != 3)
    {
       /* TODO: mentioned the ID address of server. */
        printf ("Input @cmdline : ./a.out port_number ip_address\n");
       /* TODO: When you return from anywhere because of the is error, you need to return -1 value. */
        return 1;
    }

    client_sock_socket = socket(AF_INET, SOCK_STREAM, 0);
    /* TODO: Add error check */

    client.sin_family = AF_INET;
    client.sin_port = htons(atoi(argv[1]));
    inet_aton (argv[2], &client.sin_addr);

    if (connect (client_sock_socket, (struct sockaddr *)&client, sizeof(client)) == -1)
    {
       /* TODO: Add error reason. */
        perror ("Connect Error");
        return 1;
    }

    /* TODO: Explain why you did this. */
    if ( recv (client_sock_socket, &my_id, sizeof(my_id), 0) <= 0 )
    {
       /* TODO: Always explain the error, why it has failed with reason in logs. */
        printf ("Returned from main!\n");
        return 1;
    }

    printf (">>>>>>>>>>>>>> Connected to the server! <<<<<<<<<<<<<<\n");
    printf ("My ID: %d\n", my_id);

    printf ("\nEnter:\nlist --> to see the available clients\nexit --> to exit from program\n<client_id>:<Msg> --> to send msgs to particular client\n\n");

    pthread_create(&recv_thread, NULL, client_sock_receiver, NULL);
    pthread_create(&send_thread, NULL, client_sock_sender, NULL);

    /* TODO: Is this correct way to do while loop? How you can optimise this. Also you must need to add sleep to take time for other process start/complete that works. */
    while (1)
    {
        if (client_sock_flag)
        {
            break;
        }
    }

    close (client_sock_socket);
    return 0;
}

