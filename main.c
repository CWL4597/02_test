#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "pos/console.h"
#include "p/start_mplayer.h"
#define   music   "./lrc/简单爱.lrc"
typedef  struct lrc{
	int m_time;            //歌词时间
	char lrc_buf[200];   //歌词内容    改数据域
	
	struct lrc *front;
	struct lrc *next;
}LRC;

void print_red(LRC *pb,int y)
{
	cusor_moveto(12,y);
	set_fg_color(COLOR_RED);
	printf("%s\n",pb->lrc_buf);
}


void print_blue(LRC *pb,int y)
{
	cusor_moveto(12,y);
	set_fg_color(COLOR_BLUE);
	printf("%s\n",pb->lrc_buf);
	
}

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
//pb为正在唱的节点，返回值为 打印五个节点歌词的首节点的地址

void link_print(LRC *head,char flag)
{
	LRC *pb = head;
	if(head == NULL)
		return;
	if(flag == 1)      //正序
	while(pb->next != head)
	{
		printf("%02d:%02d %s\n",(pb->m_time)/60,(pb->m_time)%60,pb->lrc_buf);
		pb = pb->next;
	}	
	else
	{
		pb = pb->front;
		while(pb != head)
		{
			printf("%02d:%02d %s\n",(pb->m_time)/60,(pb->m_time)%60,pb->lrc_buf);
			pb = pb->front;
		}	
	}	
	printf("%02d:%02d %s\n",(pb->m_time)/60,(pb->m_time)%60,pb->lrc_buf);
}


LRC * find_start(LRC *head,LRC *pb)
{
	if(pb==head)
	{
		return head;
	}
	else if(pb->front==head)
	{
		return pb->front;
	}
	else if(pb->next==head)
	{
		return pb->front->front->front->front;
	}
	else if(pb->next->next==head)
	{
		return pb->front->front->front;
	}
	else 
	{
		return pb->front->front;
	}
		
	return head;
}

void lrc_print(LRC *start ,LRC *pb)
{
	int i;
	for(i=0;i<5;i++)
	{
		if(start == pb)
		{
			
			print_red(start,11+i);
		}
		else
		{
			print_blue(start,11+i);
		}
		start = start->next;
	}
}
//从start指向的节点往后打五个节点的歌词，碰到pb正在唱的节点的时候高亮


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

char *src_fd(char *lrc_buf)
{
	FILE *fp=NULL;
	unsigned int length;
	fp=fopen(music,"r+");
	if(fp==NULL)
		return 0;
	fseek(fp,0,SEEK_END);
	length=ftell(fp);
	lrc_buf=malloc (sizeof(length)); 
	rewind(fp);
	fread(lrc_buf,1,length,fp);
	mplayer_play("./song/简单爱.mp3");  
	fclose(fp);
	return lrc_buf;
}

int main()
{
	char *q[100];int len;
	char *str;
	int i=0,j,k;
	int min,sec,s=0;
	LRC *head=NULL,*p,*pnew,*p1;
	int time_buf[50];
	
	char *lrc_buf=NULL;
	lrc_buf=src_fd(lrc_buf);  //文件
	
	len=msg_deal(lrc_buf,q,"\r\n");
	for(k=4;k<len;k++)       //处理第5行到最后一行
	{
		int time[5];
		LRC *pn;
		str=q[k];
		puts(str);
		i = 0;
		while(*str == '['){
			time[i] = atoi(str+1)*60+atoi(str+4);
			i++;
			str+=10;
		}
		for(j=0;j<i;j++)
		{
			malloc(212);
			 //pnew = malloc(sizeof(LRC));   //申请节点
			// pnew->m_time=time[j];     //节点的时间成员赋值
			// strcpy(pnew->lrc_buf,str);    //节点的歌词成员赋值
			// head = link_insert(head,pnew);	   //插入链表 
		}		
	}
	// link_print(head,1);		
	return 0;
	for(k=4;k<len;k++)       //处理第5行到最后一行
	{
		str=q[k];
		puts(str);
		#if 1
		i=0;
		while(*str=='[')
		{
			*(str+1+2) = 0;
			*(str+4+2) = 0;
			time_buf[i++]=atoi(str+1)*60+atoi(str+4);
			printf("%d\n",time_buf[i-1]);
			str=str+10;                     
		}
		//printf("i=%d\n",i);
		#endif
		for(j=0;j<i;j++)
		{
			pnew = malloc(sizeof(LRC));   //申请节点
			pnew->m_time=time_buf[j];     //节点的时间成员赋值
			strcpy(pnew->lrc_buf,str);    //节点的歌词成员赋值
			head = link_insert(head,pnew);	   //插入链表 
		}
	}	
    //link_print(head,1);
	return 0;
	clear_screen(); 
	while(1)                          
	{
		usleep(1000*1000);  
		set_fg_color(COLOR_RED);
		cusor_moveto(30,5);  //光标跳转到 y行 x列
		cusor_get_pos;      //保存光标位置
		cusor_hide();       //隐藏光标 
		printf("%02d:%02d      \r",s/60,s%60);
		fflush(stdout);
		p=link_find(head,s);         //从链表里面找s这个时间是否存在
		set_fg_color(COLOR_BLUE);
		if(p !=NULL) //找到节点显示
		{
			p1=find_start(head,p); //p1五首歌的头结点
			clear_screen();
			lrc_print(p1,p);  
		}
		s++;
		
		if(s==260)
			break;
	}	
	cusor_set_pos();      //恢复光标位置
	clear_screen(); 
	return 0;
}