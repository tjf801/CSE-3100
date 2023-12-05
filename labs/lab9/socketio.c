/***** commonly used functions to send/receive data to/from sockets *****/

/* use a loop to send a buffer of n bytes
 *
 * it is fine if n is 0.
 *
 * Return values:
 * 0:   success
 * -1:  error
 * */
int send_all(int sid, void * buf, size_t n)
{
    size_t sent = 0;
    while (sent < n) {
        // ignore SIGPIPE signal. We handle error ourselves.
        ssize_t r = send(sid, (char *)buf + sent, n - sent, MSG_NOSIGNAL);
        if (r < 0) {
            if (errno == EINTR) // if it was interrupted, try again
                continue;
            perror("send()");   // may do this outside.
            return -1;
        }
        sent += r;  
    }
    return 0;
}

/* send a string. Terminating NUL is not sent.
 *
 * Return values:
 * 0:   success
 * -1:  error
 * */
int send_str(int sid, char *s)
{
    return send_all(sid, s, strlen(s));
}

/* use a loop to receive a buffer of n bytes
 *
 * Return values:
 * 0:   success
 * -1:  error
 * */
int recv_all(int sid, char * buf, size_t n)
{
    size_t received = 0;
    while (received < n) {
	ssize_t r = recv(sid, buf + received, n - received, 0);
	if (r <= 0) {
            if (r < 0) {
                if (errno == EINTR) // if it was interrupted, try again
                    continue;
                perror("recv()");
            }
	    return -1;
        }
	received += r;	
    }
    return 0;
}

/* Receive at leat one line from the socket. Each line ends
 * with a new line character. A NUL is placed at the 
 * end of all lines.
 *
 * More than one line, e.g, 10 or even 100 lines, may be received. 
 * The buffer has to be large enough. The caller has to find
 * individual lines.
 *
 * If the buffer cannot hold a complete line at the end, the function 
 * returns -1, even if some lines have been placed in the buffer. 
 * For example, the socket has received 3 lines. recv() would try
 * to place all 3 lines in the buffer. However, the buffer
 * is too small. Only the first part of line 3 is placed in buffer. 
 * The function returns -1 in this case.
 *
 * If the protocol has other requirements, write another function.
 *
 * Return values:
 * 0:   one or more lines has been placed in the buffer
 * -1:  error
 * */
int recv_lines(int sid, char * buf, size_t buf_size)
{
    if (buf_size <= 1)
        return -1;

    buf_size --; // save one byte for NUL

    size_t received = 0;
    while (received < buf_size) {
        int r = recv(sid, buf + received, buf_size - received, 0);
        if (r <= 0) {
            if (r < 0) {
                if (errno == EINTR) // if it was interrupted, try again
                    continue;
                perror("recv()");
            }
            return -1;
        }
        received += r;

        // check if the last charcter is a new line
        if (received && buf[received-1] == '\n') {
            buf[received] = 0; // a byte is saved before the loop
            return 0;
        }
    }
    // buffer is full, but does not have a complete line (at the end)
    return -1;
}
