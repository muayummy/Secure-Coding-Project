#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int b = 0;
char * f;

static int myCompare (const void * a, const void * b)
{
    return strcmp (*(const char **) a, *(const char **) b);
}

int main( int argc, char * argv[] )
{
    printf("Started server\n");
    if ( argc != 2 )
    {
        fprintf(stderr, "ERROR: Invalid arguments\n");
        fprintf(stderr, "USAGE: ./a.out <port number>\n" ); 
        return EXIT_FAILURE;
    }
    /* Create the std::list<T> ;ener socket as TCP socket */
    int sd = socket( PF_INET, SOCK_STREAM, 0 );
    /* here, the sd is a socket descriptor (part of the fd table) */

    if ( sd == -1 )
    { 
        perror( "socket() failed" );
        return EXIT_FAILURE;
    }


    /*socket structures */
    struct sockaddr_in server;
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = INADDR_ANY;  /* allow any IP address to connect */

    unsigned short port = atoi(argv[1]);

  /* htons() is host-to-network short for data marshalling */
  /* Internet is big endian; Intel is little endian */
    server.sin_port = htons( port );
    int len = sizeof( server );

    if ( bind( sd, (struct sockaddr *)&server, len ) == -1 )
    {
        perror( "bind() failed" );
        return EXIT_FAILURE;
    }


    /* identify this port as a listener port */
    /* the value 5 here means that the OS will
      maintain a queue of at most 5
       pending connections for the accept() call */
    if ( listen( sd, 5 ) == -1 )
    {
      perror( "listen() failed" );
      return EXIT_FAILURE;
    }

    printf( "Listening for TCP connections on port: %d\n", port );
    /*
    char * dicname = "storage";
    char cwdt[1024];
    getcwd(cwdt, sizeof(cwdt));
    char * cwd = malloc(strlen(cwdt) + strlen(dicname) + 2);
    strcpy(cwd, cwdt);
    strcat(cwd, "/");
    strcat(cwd, dicname);
    int ret;
    ret = chdir(cwd);
    if (ret != 0) 
    {
        printf("ERROR: <change current working directory failed>\n");
        fflush( stdout );
        return EXIT_FAILURE;
    }
*/
    struct sockaddr_in client;
    int fromlen = sizeof( client );

    int n;
    pid_t pid;
    char buffer[ BUFFER_SIZE ];

    while ( 1 )
    {
        int newsd = accept( sd, (struct sockaddr *)&client, (socklen_t *)&fromlen );

        printf( "Rcvd incoming TCP connection from: %s\n", inet_ntoa( (struct in_addr)client.sin_addr ) );

    /* Handle the accepted new client connection in a child process */
        pid = fork();

        if ( pid == -1 )
        {
            perror( "fork() failed" );
            return EXIT_FAILURE;
        }

        if ( pid > 0 )
        {
        /* parent simply closes the new client socket
          because the child process will handle that connection */
            close( newsd );
        }
        else /* pid == 0 */
        {
            do
            {

        /* recv() will block until we receive data (n > 0)
            or there's an error (n == -1)
             or the client closed the socket (n == 0) */
                n = recv( newsd, buffer, BUFFER_SIZE, 0 );
                if ( n == -1 )
                {
                    return EXIT_FAILURE;
                }
                else if ( n == 0 )
                {
                    printf( "[child %d] Rcvd 0 from recv(); closing socket...\n",  getpid() );
                }
                else /* n > 0 */
                {
                    //buffer[n] = '\0';
                    /*char * temp = buffer;
                    char * token;
                    const char s[2] = " ";
                    token = strtok(temp, s);*/
                    if ((send( newsd,buffer, strlen(buffer),0))== -1) {
                        fprintf(stderr, "Failure Sending Message\n");
                        close( newsd);
                        exit(1);
                    }
                    else {
                        printf("Message being sent: %s\n",buffer);
                    }
                    
                
                }
            }
            while ( n > 0 );
            
            printf("[child %d] Client disconnected\n" , getpid());
            close( newsd );
            exit(EXIT_SUCCESS);
        }
    }
    return EXIT_SUCCESS;
}
