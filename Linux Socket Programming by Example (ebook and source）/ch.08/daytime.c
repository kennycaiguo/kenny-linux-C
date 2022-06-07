/* daytime.c:
 * 
 * Example daytime client, modified
 * to accept different port numbers:
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/*
 * This function reports the error and
 * exits back to the shell :
 */
static void
bail(const char *on_what) {
    if ( errno != 0 ) {
        fputs(strerror(errno),stderr);
        fputs(": ",stderr);
    }
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}

int
main(int argc,char **argv) {
    int z;
    char *srvr_addr = NULL;
    char *srvr_port = "9099";
    struct sockaddr_in adr_srvr;/* AF_INET */
    int len_inet;               /* length  */
    int s;                       /* Socket */
    char dtbuf[128];     /* Date/Time info */

    /*
     * Use a server address from the command
     * line, if one has been provided.
     * Otherwise, this program will default
     * to using the arbitrary address
     * 127.0.0.1 :
     */
    if ( argc >= 2 ) {
        /* Addr on cmdline: */
        srvr_addr = argv[1];
    } else {
        /* Use default address: */
        srvr_addr = "127.0.0.1";
    }

    /*
     * If the port number is given, use it:
     */
    if ( argc >= 3 )
        srvr_port = argv[2];

    /*
     * Create a TDP/IP socket to use :
     */
    s = socket(PF_INET,SOCK_STREAM,0);
    if ( s == -1 )
        bail("socket()");

    /*
     * Create a server socket address:
     */
    memset(&adr_srvr,0,sizeof adr_srvr);
    adr_srvr.sin_family = AF_INET;
    adr_srvr.sin_port = htons(atoi(srvr_port));
    adr_srvr.sin_addr.s_addr =
        inet_addr(srvr_addr);
    if ( adr_srvr.sin_addr.s_addr == INADDR_NONE )
        bail("bad address.");

    /*
     * Connect to the server:
     */
    len_inet = sizeof adr_srvr;

    z = connect(s,&adr_srvr,len_inet);
    if ( z == -1 )
        bail("connect(2)");

    /*
     * Read the date/time info:
     */
    z = read(s,&dtbuf,sizeof dtbuf-1);
    if ( z == -1 )
        bail("read(2)");

    /*
     * Report the Date & Time :
     */
    dtbuf[z] = 0;   /* NUL terminate string */

    printf("Date & Time is: %s\n",dtbuf);

    /*
     * Close the socket and exit:
     */
    close(s);
    putchar('\n');

    return 0;
}
