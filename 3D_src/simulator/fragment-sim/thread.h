#ifndef THREAD_H
#define THREAD_H

/**** thread type ****/
#define NO_THREAD 0
#define SC_THREAD 1
#define VS_THREAD 2
#define FS_THREAD 3

/**** thread state ****/
#define READY   0
#define STALL   1

/**** stall flag ****/
#define TEX_MISS 0
#define OUT_FULL 1
#define DATA_DEP 2
#define WAIT_SC  3


struct THREAD_UNIT
{
    unsigned int Ttype;
	unsigned int PC;    	
	//----------------
	unsigned int Tstate;
	int          stall_flag;
	unsigned int RAW_count;
};


struct THREAD_CONTROL
{
    struct THREAD_UNIT *info;
	int                triangle[2][3];//2 triangle(Tid)
	int                tri_priority;  //0: triangle[0] ,1: triangle[1]
	int                VS_done[3];
	//--------------------------------
	int                tri_slot;
	int                VS_slot;
	int                SC_slot,FS_slot;
	unsigned int       NO_count,SC_count,VS_count,FS_count;
};



#endif
