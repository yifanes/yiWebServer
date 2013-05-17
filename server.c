/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  main process server
 *
 *        Version:  1.0
 *        Created:  2013年05月15日 21时58分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:	zhangxy
 *   Organization:  
 *
 * =====================================================================================
 */



#include	<stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h> //for sockaddr_in
#include	<sys/stat.h>
#include	<time.h>
#include	<string.h>

#include	"send.h"
#include	"process.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  main function
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	int sock;
	FILE* conf;
	char buf_conf[4096];
	char* port_str;
	char* path;
	char* pos;
	int PORT;

/* ----------------------- start parse the http.conf ------------------------------- */
conf = fopen("http.conf", "rb");
if(conf == NULL)
{
	printf("Can't open the http.conf file\r\n");
	exit(1);
}

if(!fgets(buf_conf, sizeof(buf_conf), conf))
	return -1;
	
port_str	 =	strtok(buf_conf, " ");
path		 =	strtok(NULL, " ");
pos			 =	strchr(path, '\n');

if(pos)
	path[pos-path] = 0;

PORT = atoi(port_str);
fclose(conf);

/* ------------------------ end -------------------------------------------------- */

/* ----------------------use socket to implement the http protocol---------------- */

struct sockaddr_in server_addr;

sock = socket(AF_INET, SOCK_STREAM, 0);

server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
server_addr.sin_port = htons(PORT);
bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));

listen(sock, 50);
printf("HTTP server listening on port %d at %s \n", PORT, inet_ntoa(server_addr.sin_addr));


/* ---------------------------end------------------------------------------------- */

	
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */


