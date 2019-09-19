#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "pos/console.h"
#include "p/start_mplayer.h"
typedef  struct lrc{
	int m_time;            //歌词时间
	char lrc_buf[200];   //歌词内容    改数据域
	
	struct lrc *front;
	struct lrc *next;
}LRC;

LRC *link_free(LRC *head) 
{
	LRC *ps = head;
	if(head == NULL)
		return NULL;
	while(ps->next != head){
		ps =  ps->next;
		free(ps->front);
	}
	free(ps);
	return NULL;
}
void link_print(int line,LRC *head,char flag)
{
	LRC *pb = head;
	if(head == NULL)
		return;
	if(flag == 1)
	while(pb->next != head){
		printf("%d:%d %s\n",line,pb->m_time,pb->lrc_buf);
		pb = pb->next;
	}	
	else{
		pb = pb->front;
		while(pb != head){
			printf("%d:%d %s\n",line,pb->m_time,pb->lrc_buf);
			pb = pb->front;
		}	
	}	
	printf("%d:%d %s\n",line,pb->m_time,pb->lrc_buf);
}
LRC *link_find(LRC *head,int num)
{
	LRC *pb = head;
	if(head == NULL)
		return NULL;
	while(pb->next != head)
	{
		if(pb->m_time==num)
			break;
		pb = pb->next;
	}	
	if(pb->m_time==num)
		return pb;
	else
		return NULL;
}

//创建并插入
LRC *link_insert(LRC *head,LRC *pnew)
{
	LRC *pb = head;
	if(head == NULL)
	{
		head = pnew;
		head->next = head;
		head->front = head;
	}
	else if(pnew != NULL)
	{
		pb = head->front;
		pnew->next = pb->next;
		pb->next = pnew;
		pnew->front = pb;
		pnew->next->front = pnew;
	}
	return head;
}

void swap(LRC *pf,LRC *pb)
{
	LRC temp;
	temp = *pf;
	*pf = *pb;
	*pb = temp;
	//将指向交换
	temp.next = pf->next;
	pf->next = pb->next;
	pb->next = temp.next;
	
	temp.front = pf->front;
	pf->front = pb->front;
	pb->front = temp.front;
}
/*LRC *link_order(LRC *head)
{
	LRC *pf,*pb;
	pf = head;
	if(head == NULL)
		return NULL;
	while(pf->next != head)
	{
		pb = pf->next;
		while(pb != head)
		{
			if(pf->m_time > pb->m_time)
				swap(pf,pb);
				
			pb = pb->next;
		}
		pf = pf->next;
	}	
	return head;
}*/

int msg_deal(char *msg_src,char *msg_done[],char *str)
{
	int i=0;
	msg_done[i]=strtok(msg_src,str);
	while(msg_done[i]!=NULL)
	{
		i++;
		msg_done[i]=strtok(NULL,str);
	}
	return i;
}

int main()
{
	char *q[100];int len;
	char *str;
	int time_buf[5];
	int i=0,j,k;
	int min,sec,s=0;
	LRC *head=NULL,*p,*pnew;
	
	char lrc_buf[]="[ti:简单爱]\r\n\
[ar:周杰伦]\r\n\
[al:范特西]\r\n\
[by:大脸猫]\r\n\
[00:04.41]周杰伦 Chow, Jay Jou\r\n\
[00:07.41]aaaaaaaaaaa\r\n\
[00:10.56]简单爱(台视Star blue蓝星主题曲)\r\n\
[00:15.56]bbbbbbbbb\r\n\
[00:18.48]词：徐若瑄 曲：周杰伦\r\n\
[00:24.51]cccccccccccc\r\n\
[00:26.44]说不上为什么 我变得很主动\r\n\
[00:31.37]若爱上一个人 什么都会值得去做\r\n\
[02:04.94][00:36.09]我想大声宣布 对你依依不舍\r\n\
[02:09.97][00:41.26]连隔壁邻居都猜到我现在的感受\r\n\
[02:14.94][00:46.17]河边的风 在吹着头发 飘动\r\n\
[02:19.80][00:51.25]牵着你的手 一阵莫名感动\r\n\
[02:24.61][00:55.86]我想带你 回我的外婆家\r\n\
[02:28.32][00:59.79]一起看着日落 一直到我们都睡着\r\n\
[03:34.64][02:34.71][01:05.83]我想就这样牵着你的手不放开\r\n\
[03:39.68][02:39.34][01:10.71]爱能不能够永远单纯没有悲哀\r\n\
[03:44.27][02:43.90][01:15.44]我想 带你骑单车\r\n\
[03:46.74][02:46.60][01:18.05]我想 和你看棒球\r\n\
[03:49.77][02:49.58][01:20.71]想这样没担忧\r\n\
[03:51.61][02:51.59][01:22.69]唱着歌 一直走☆\r\n\
[03:54.38][02:54.35][01:25.57]我想就这样牵着你的手不放开\r\n\
[03:59.19][02:59.01][01:30.41]爱可不可以简简单单没有伤害\r\n\
[04:03.77][03:03.73][01:35.04]你 靠着我的肩膀\r\n\
[04:06.33][03:06.26][01:37.49]你 在我胸口睡着\r\n\
[04:09.13][03:09.34][01:40.57]像这样的生活\r\n\
[04:11.36][03:11.26][01:42.66]我爱你 你爱我★\r\n\
[03:13.76][01:44.97]想～～～ 简！简！单！单！ 爱～～～\r\n\
[03:23.61][01:54.30]想～～～ 简！简！单！单！ 爱～～～";

	len=msg_deal(lrc_buf,q,"\r\n");
	for(k=4;k<len;k++)       //处理第5行到最后一行
	{
		str=q[k];
		i=0;
		while(*str=='[')
		{
			time_buf[i++]=atoi(str+1)*60+atoi(str+4);
			str=str+10;                     
		}
		for(j=0;j<i;j++)
		{
			pnew = malloc(sizeof(LRC));   //申请节点
			pnew->m_time=time_buf[j];     //节点的时间成员赋值
			strcpy(pnew->lrc_buf,str);    //节点的歌词成员赋值
			head = link_insert(head,pnew);	   //插入链表 
		}	
	}	
    //link_print(__LINE__,head,1);
	//return 0;
	
	mplayer_play("song/简单爱.mp3");  
	set_fg_color(COLOR_BLUE);
	clear_screen(); 
	cusor_moveto(0,0);
	while(1)                          
	{
		usleep(1000*1000);  
		cusor_moveto(8,6);  //光标跳转到 y行 x列
		cusor_get_pos;    //保存光标位置
		cusor_set_pos;
		printf("%02d:%02d                                  \r",s/60,s%60);
		
		fflush(stdout);
		p=link_find(head,s);         //从链表里面找s这个时间是否存在

		if(p !=NULL) //找到节点显示
		{
			cusor_moveto(5,10);
			cusor_get_pos;
		printf("%s\n",p->lrc_buf);
		printf("                                                    \r");
		//printf("%02d:%02d:%s\n",(p->m_time)/60,(p->m_time)%60,p->lrc_buf);
		}
		s++;
		
		if(s==260)
			break;
	}	     
	return 0;
}