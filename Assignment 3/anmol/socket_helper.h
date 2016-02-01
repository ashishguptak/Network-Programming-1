
int SEND(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, int tolen) {

  if(sendto(sockfd, msg, len, flags, to, tolen) <0) {
    perror("sending datagram message");
    exit(4);
  } 

  return 1;
}

int RECV(int sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from, int *fromlen) {

  int bytes_received = recvfrom(sockfd, buf, len, flags, from, fromlen);
  
  if (bytes_received < 0) {
    perror("Sending datagram message");
    exit(4);
  }

  return bytes_received;
}
