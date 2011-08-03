#ifndef __GPRS_H__
#define __GPRS_H__

#include <termios.h>
#include <stdbool.h>

bool termios_init(int fd, struct termios *tio, int baud);
bool gprs_init(int fd, const char *cmd[], int count);
bool message_send(int fd, char *cmd, int cmdlen, char *msg, int msglen);
void order_change(char *buf, int len);
void hex_to_asc(char *dst, char *src, int len);
bool utf8_to_unicode(char *inbuf, int inlen, char **outbuf, int *outlen);

#endif
