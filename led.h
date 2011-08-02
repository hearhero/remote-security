#ifndef _LED_H_
#define _LED_H_

#define LED_MAGIC 'k'

#define LED_ON _IO(LED_MAGIC, 1)
#define LED_OFF _IO(LED_MAGIC, 2)

void *led_thread_handler(void *arg);

#endif
