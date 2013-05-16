#define SERVER "yiwebServer 0.1"
#define PROTOCOL "HTTP/1.0"
#define RFC1123FMT "%a, %d %b %Y %H:%M%s GMT"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  send_headers
 *  Description:  send header
 * =====================================================================================
 */
	void
send_headers ( FILE *f, int status, char* title, char* extra, time_t date )
{
	time_t now;
	char timebuf[128];
	fprintf(f, "%s %d %s\r\n", PROTOCOL, status, title);
	now = time(NULL);
	strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&now));	
	fprintf(f, "Date: %s\r\n", timebuf);
	fprintf(f, "Server: %s\r\n", SERVER);
	if(extra)
		fprintf(f, "%s \r\n", extra);
	if(date != -1)
	{
		strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&date));
		fprintf(f, "Last-Modified: %s\r\n", timebuf);
	}
	fprintf(f, "Connection: close\r\n");
	fprintf(f, "\r\n");	
}		/* -----  end of function send_headers  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  send_error
 *  Description:  send errors
 * =====================================================================================
 */
	void
send_error ( FILE* f, int status, char* title, char* extra, char* text )
{
	send_headers(f, status, title, extra, -1);
	fprintf(f, "<html><head><title>%d %s</title></head>\r\n", status, title);
	fprintf(f, "<body><h4>%d %s </h4>\r\n", status, title);
	fprintf(f, "%s\r\n", text);
	fprintf(f, "</body></html>\r\n");

}		/* -----  end of function send_error  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  send_file
 *  Description:  
 * =====================================================================================
 */
	void
send_file ( FILE* f, char* path, struct stat *statbuf )
{
	char data[4096];
	int n;
	
	FILE* file = fopen(path, "r");
	if(!file)
	{
		send_error(f, 403, "Forbidden", NULL, "Access Denied." );
	}else
	{
		send_headers(f, 200, "OK", NULL, statbuf->st_mtime);
		
		while ((n = fread(data, 1, sizeof(data), file)) > 0)
			fwrite(data, 1, n, f);
		fclose(file);
	}
}		/* -----  end of function send_file  ----- */












