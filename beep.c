#include <unistd.h>
#include <sys/ioctl.h>

#include "beep.h"

Note MumIsTheBestInTheWorld[]={
	//6.	  		  //_5		 //3		 //5	
	{LA,TIME+TIME/2}, {SOL,TIME/2},{MI,TIME},{SOL,TIME},
	//1^		   //6_		  //_5		//6-
	{DO*2,TIME},{LA,TIME/2},{SOL,TIME/2} ,{LA,2*TIME},
	// 3      //5_		//_6		   //5
	{MI,TIME},{SOL,TIME/2},{LA,TIME/2},{SOL,TIME},
	// 3	 	//1_		//_6,
	{MI,TIME},{DO,TIME/2},{LA/2,TIME/2},
	//5_		//_3		//2-		   //2.
	{SOL,TIME/2},{MI,TIME/2},{RE,TIME*2},{RE,TIME+TIME/2},
	//_3 	//5			//5_			//_6
	{MI,TIME/2},{SOL,TIME},{SOL,TIME/2},{LA,TIME/2},
	// 3		//2			//1-			//5.
	{MI,TIME},{RE,TIME},{DO,TIME*2},{SOL,TIME+TIME/2},
	//_3		//2_		//_1 		//6,_
	{MI,TIME/2},{RE,TIME/2},{DO,TIME/2},{LA/2,TIME/2},
	//_1		//5,--
	{DO,TIME/2},{SOL/2,TIME*3}
};

Note GreatlyLongNow[]={  	
	// 2		3			3		3.				_2				1
	{RE,TIME}, {MI,TIME},{MI,TIME},{MI,TIME+TIME/2},{RE,TIME/2},{DO,TIME},
	//6,		1			2		1--				2			3			3
	{LA/2,TIME},{DO,TIME},{RE,TIME},{DO,TIME*3},{RE,TIME},{MI,TIME},{MI,TIME},
	//3.				_5			3			3			2			3
	{MI,TIME+TIME/2},{SOL,TIME/2},{MI,TIME},{MI,TIME},{RE,TIME},{MI,TIME},
	//3--		5			6			6		  6.				_5
	{MI,TIME*3},{SOL,TIME},{LA,TIME},{LA,TIME},{LA,TIME+TIME/2},{SOL,TIME/2},
	// 3		3		5				6		5---			2			3
	{MI,TIME},{MI,TIME},{SOL,TIME},{LA,TIME},{SOL,TIME*3},{RE,TIME},{MI,TIME},
	// 3		2.				_3				3		  2			3
	{MI,TIME},{RE,TIME+TIME/2},{MI,TIME/2},{MI,TIME},{RE,TIME},{MI,TIME},
	//6,		1_			  _6,			  6,-
	{LA/2,TIME},{DO,TIME/2},{LA/2,TIME/2},{LA/2,TIME*2},
	//2_		_2			2_				_1			6,
	{RE,TIME/2},{RE,TIME/2},{RE,TIME/2},{DO,TIME/2},{LA/2,TIME},
	//2_		_2			2_				_1	  		6,
	{RE,TIME/2},{RE,TIME/2},{RE,TIME/2},{DO,TIME/2},{LA/2,TIME},
	// 2		3		1			2.					_3			5
	{RE,TIME},{MI,TIME},{DO,TIME},{RE,TIME+TIME/2},{MI,TIME/2},{SOL,TIME},
	//6_		_6				6_			_5			3
	{LA,TIME/2},{LA,TIME/2},{LA,TIME/2},{SOL,TIME/2},{MI,TIME},
	//2_		_2			2_				_1			6,
	{RE,TIME/2},{RE,TIME/2},{RE,TIME/2},{DO,TIME/2},{LA/2,TIME},
	//6,		5,.					  _6,			 6,--
	{LA/2,TIME},{SOL/2,TIME+TIME/2},{LA/2,TIME/2},{LA/2,TIME*3},
	//2_		_2			2_				_1			6,
	{RE,TIME/2},{RE,TIME/2},{RE,TIME/2},{DO,TIME/2},{LA/2,TIME},
	//2_		_2			2_				_1	  		6,
	{RE,TIME/2},{RE,TIME/2},{RE,TIME/2},{DO,TIME/2},{LA/2,TIME},
	// 2		3		1			2.					_3			5
	{RE,TIME},{MI,TIME},{DO,TIME},{RE,TIME+TIME/2},{MI,TIME/2},{SOL,TIME},
	//6_		_6				6_			_5			3
	{LA,TIME/2},{LA,TIME/2},{LA,TIME/2},{SOL,TIME/2},{MI,TIME},
	//2_		_2			2_				_1			6,
	{RE,TIME/2},{RE,TIME/2},{RE,TIME/2},{DO,TIME/2},{LA/2,TIME},
	//6,		5,.					  _6,			 6,--
	{LA/2,TIME},{SOL/2,TIME+TIME/2},{LA/2,TIME/2},{LA/2,TIME*3}
};

Note FishBoat[]={ //3.				_5			6._					=1^			 6_
	{MI,TIME+TIME/2},{SOL,TIME/2},{LA,TIME/2+TIME/4},{DO*2,TIME/4},{LA,TIME/2},
	//_5			3 -.		2		  1.			 _3			 2._
	{SOL,TIME/2},{MI,TIME*3},{RE,TIME},{DO,TIME+TIME/2},{MI,TIME/2},{RE,TIME/2+TIME/4},
	//=3			2_			_1		 2--			3.				_5
	{MI,TIME/4},{RE,TIME/2},{DO,TIME/2},{RE,TIME*4},{MI,TIME+TIME/2},{SOL,TIME/2},
	// 2		1		6._					=1^			 	6_			_5
	{RE,TIME},{DO,TIME},{LA,TIME/2+TIME/4},{DO*2,TIME/4},{LA,TIME/2},{SOL,TIME/2},
	//6-		 5,.					_6,			1._					=3
	{LA,TIME*2},{SOL/2,TIME+TIME/2},{LA/2,TIME/2},{DO,TIME/2+TIME/4},{MI,TIME/4},
	//2_			_1		 5,--
	{RE,TIME/2},{DO,TIME/2},{SOL/2,TIME*4},
	//3.				_5			6._					=1^			6_
	{MI,TIME+TIME/2},{SOL,TIME/2},{LA,TIME/2+TIME/4},{DO*2,TIME/4},{LA,TIME/2},
	//_5			3-.			5_			_6			1^_				   _6
	{SOL,TIME/2},{MI,TIME*3},{SOL,TIME/2},{LA,TIME/2},{DO*2,TIME+TIME/2},{LA,TIME/2},
	//5._					=6			5_		  _3			2--
	{SOL,TIME/2+TIME/4},{LA,TIME/4},{SOL,TIME/2},{MI,TIME/2},{RE,TIME*4},
	//3.				_5			2._					=3			2_			_1
	{MI,TIME+TIME/2},{SOL,TIME/2},{RE,TIME/2+TIME/4},{MI,TIME/4},{RE,TIME/2},{DO,TIME/2},
	//6._				=1^				6_			_5			6-			1.
	{LA,TIME/2+TIME/4},{DO*2,TIME/4},{LA,TIME/2},{SOL,TIME/2},{LA,TIME*2},{DO,TIME+TIME/2},
	//_2	  	 3_			_5			    2_			_3			1--
	{RE,TIME/2},{MI,TIME/2},{SOL,TIME/2},{RE,TIME/2},{MI,TIME/2},{DO,TIME*4}
};

void *beep_thread_handler(void *arg)
{
	int i, div;

	while (1)
	{
		ioctl(*((int *)arg), BEEP_PRE, 255);

		for(i = 0; i < sizeof(MumIsTheBestInTheWorld)/sizeof(Note); i++)
		{
			div = (PCLK/256/4) / (MumIsTheBestInTheWorld[i].pitch);
			ioctl(*((int *)arg), BEEP_CNT, div);
			usleep(MumIsTheBestInTheWorld[i].dimation * 100); 
		}

		for(i = 0; i < sizeof(GreatlyLongNow)/sizeof(Note); i++)
		{
			div = (PCLK/256/4) / (GreatlyLongNow[i].pitch);
			ioctl(*((int *)arg), BEEP_CNT, div);
			usleep(GreatlyLongNow[i].dimation * 100); 
		}

		for(i = 0; i < sizeof(FishBoat)/sizeof(Note); i++)
		{
			div = (PCLK/256/4) / (FishBoat[i].pitch);
			ioctl(*((int *)arg), BEEP_CNT, div);
			usleep(FishBoat[i].dimation * 100); 
		}
	}
}
