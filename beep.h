#ifndef __BEEP_H__
#define __BEEP_H__

#define BIG_D
#define PCLK (202800000/4)

#define BEEP_MAGIC 'k'

#define BEEP_ON _IO(BEEP_MAGIC, 1)
#define BEEP_OFF _IO(BEEP_MAGIC, 2)
#define BEEP_CNT _IO(BEEP_MAGIC, 3)
#define BEEP_PRE _IO(BEEP_MAGIC, 4)
#define BEEP_DEF _IO(BEEP_MAGIC, 5)

typedef struct
{
	int pitch; 
	int dimation;
}Note;
// 1			2		3		 4 			5	  		6       7
// C 			D	 	E	 	 F	 	 	G	 		A	 	B
//261.6256	293.6648   329.6276 349.2282   391.9954		440		493.8833

//C大调
#ifdef BIG_C
#define DO 262
#define RE 294
#define MI 330
#define FA 349
#define SOL 392
#define LA  440
#define SI  494
#define TIME 6000
#endif
 
 //D大调
#ifdef BIG_D
#define DO 293
#define RE 330
#define MI 370
#define FA 349
#define SOL 440
#define LA  494
#define SI  554
#define TIME 6000
#endif

void *beep_thread_handler(void *arg);

#endif
