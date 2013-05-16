
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  process
 *  Description:  
 * =====================================================================================
 */
	int
process ( FILE *f, int s, char* path )
{
	char buf[4096];
	char* path_local;
	char* method;
	char* protocol;
	struct stat statbuf;

	recv(s, buf, sizeof(buf), 0);
	printf("URL: %s", buf);
	
	method = strtok(buf, " ");
	path_local = strtok(NULL, " ");
	protocol = strtok(NULL, " ");
	fseek(f, 0, SEEK_CUR);
	
	//path/path_local
	strcat(path, path_local);
	
	if(strcmp(method, "GET") != 0)
		send_error(f, 501, "Not Supported", NULL, "Method is not supported.");
	else if( stat(path, &statbuf) < 0 )
		send_error(f, 404, "Not Found", NULL, "File Not Found.");
	else
		send_file(f, path, &statbuf);

	return 0;

}		/* -----  end of function process  ----- */
