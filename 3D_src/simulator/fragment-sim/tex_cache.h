#ifndef TEX_CACHE_H
#define TEX_CACHE_H

struct T_info
{
    float        u,v;
	unsigned int Tid;
};


struct T_FIFO
{
    int           full,empty;
	int           R_ptr,W_ptr;
	unsigned int  counter;
	struct T_info list[14];
};


struct TAG
{
    int S_tag,T_tag;
};


struct cache
{
    struct T_FIFO *fifo;
	unsigned int  *cache;
	unsigned int  TEX_hit_data;
	int           penalty_count,transfer_count;
	int           state;//0 -> IDLE , 1 -> MISS
	int           valid[4];
	struct TAG    tag[4];
	//--------------------
	unsigned int  proc_thread;
	unsigned int  TEX_miss_data[14];//14 threads
	int           TEX_miss_flag[14];
};


#endif
