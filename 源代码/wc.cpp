#include<stdio.h>
#include<stdlib.h>
#include<io.h>

#define MAX  50
//最多可以禁用的单词数

//分别计算字符数，单词数，行数，(代码行，空行，注释行)
int countcn(FILE*);
int countwn(FILE*);
int countln(FILE*);
int* countal(FILE*);
int countwn2(FILE* f, FILE* f2);

struct charlink
{
	char c;
	charlink* next;
};
struct word
{
	charlink* mychar;
	int len;
};


int main(int argc,char** argv)
{
	char direct[MAX] = { 0 };//用于将输入的文件的绝对路径
	int sign[7]={0};//记录参数-c,-w,-l,-o,-a,-e
	char* result="result.txt";//记录输出文件
	char* stoplist="stoplist.txt";//记录停用单词
	char* source = "0";//输入的资源文件
	char* dir = "0";//输入文件夹
	int ln=0,cn=0,wn=0;//分别记录行数，字符数，单词数，本行单词数，代码行，空行，注释行
	int* three;
	int i=0,j=0;

	for(i=1;i<argc;i++)
		{
		    if( argv[i][0]=='-')//判断输入参数
				switch (argv[i][1])
			{
				case 'c':sign[0]=1;break;
				case 'w':sign[1]=1;break;
				case 'l':sign[2]=1;break;
				case 'o':sign[3]=1;break;
				case 'a':sign[4]=1;break;
				case 'e':sign[5]=1;break;
				case 's':sign[6]=1;break;
				default:
					printf("输入参数有误"); break;
			}
			else if(argv[i-1][1]=='e')
				stoplist=argv[i];
			else if (argv[i - 1][1] == 'o')
				result = argv[i];
            else source=argv[i];
        }
	dir = source;
	/*
	for (i = 0; i < argc; i++)
		printf("argv[%d]%s ",i, argv[i]);
    */
	//执行相应的统计并输出

	FILE *file,*stop,*out;
	errno_t err;
	long handle;//记录文件夹的属性
	struct _finddata_t fileinfo;
	int dirlen=0;//记录文件夹内的文件数目
	char ch;//用于中间结果的存取

	//功能测试
	//sign[6] = 1;
	//sign[2] = 1;
	//判断是否是文件夹
	if (sign[6] == 1)
	{
		if ((handle = _findfirst(dir, &fileinfo)) == -1L)
		{
			printf("没有找到匹配的目录");
			exit(0);
		}
		source = fileinfo.name;
		//测试是否找到文件
		/*
		printf("%s\n", fileinfo.name);
		while (_findnext(handle, &fileinfo) == 0)
			printf("%s\n", fileinfo.name);
		//sign[6] = 0;
		*/
	}
	//打开输出文件
	if ((err = fopen_s(&out, result, "w")) != 0)
	{
		printf("无法创建输出文件");
		exit(0);
	}

	//循环处理文件夹
	int thedir = 1,firstdir=1;
	//int len = 0;
	while (thedir==1)
	{
		//测试thedir
		//printf("%d ", thedir);
		if (sign[6] == 1)
		{
			if (firstdir != 1&&_findnext(handle, &fileinfo) == 0)//判断是否是第一个文件
				source = fileinfo.name;//这里给出的是文件名，需要变成路径，为简化操作，在当前目录下查找
			else if (firstdir != 1)break;//文件夹的尾部跳出循环
		}
		else thedir=0;
		firstdir = 0;

		//测试
		//printf("%s ", source);
		/*source末尾是\0
		for (i = 0; source[i] != '\0';i++)
		printf("%c ", source[i]);
		len = i;
		*/
		//if (source[0] == '0')break;
		//如何将相对路径与输入结合在一起形成绝对路径
		i = 0;
		j = 0;
		if (sign[6] == 1)
		{
			while (1)
			{
				ch = dir[i];
				if (ch != '*')
					direct[i] = ch;
				else
				{
					for (j = 0; source[j]!='\0'; j++)
					    direct[i++] = source[j];
					break;
				}
					i++;
			}
			direct[i] = '\0';
			source = (char*)direct;
			//测试
			//printf("%s ", source);
		}
		if ((err = fopen_s(&file, source, "r")) != 0)
		{
			printf("无法找到目标文件");
			if (file == out)
				continue;
			exit(0);
		}
		else
		{
				if (sign[0] == 1)
				{
					cn = countcn(file), fprintf(out, "%s 字符数: %d\n", source, cn);
					fseek(file, 0L, SEEK_SET);
				}
				if (sign[1] == 1)
				{
					if (sign[5] != 1)
						wn = countwn(file), fprintf(out, "%s 单词数: %d\n", source, wn);
					else
					{
						//统计禁用后的单词量
						if ((err = fopen_s(&stop, stoplist, "r")) != 0)
						{
							printf("无法打开禁用单词文件");
							//fputs("无法打开禁用单词文件",out);//用于测试
							exit(0);
						}
						wn = countwn2(file, stop);
						fprintf(out, "%s 单词数: %d\n", source, wn);
						fclose(stop);
					}

					fseek(file, 0L, SEEK_SET);
				}
				if (sign[2] == 1)
				{
					ln = countln(file), fprintf(out, "%s 行数: %d\n", source, ln);
					fseek(file, 0L, SEEK_SET);
				}
				if (sign[4] == 1)
				{
					three = countal(file);
					fprintf(out, "%s 代码行/空行/注释行: %d,%d,%d\n", source, three[0], three[1], three[2]);
					fseek(file, 0L, SEEK_SET);
				}
				
			}
			fclose(file);
	}
	fclose(out);
	if (sign[6]==1)
		_findclose(handle);
	//getchar();
	return 0;
}

//统计字符数
int countcn(FILE* f)
{
	char ch;
	int x=0;
	while((ch=fgetc(f))!=EOF)
		x++;
	return x;
}

//统计行数
int countln(FILE* f)
{
	int x=1;
	char ch;
	while((ch=fgetc(f))!=EOF)
		if(ch=='\n')
			x++;
	return x;
}

//统计单词数
int countwn(FILE* f)
{
	int x=0;
	char ch;
	int state=0;//记录进出单词状态
	while ((ch = fgetc(f)) != EOF)
	{
		if (ch == ' ' || ch == '\t' || ch == ',' || ch == '.' || ch == '\n' || ch == '{' || ch == '}' || ch == ';' || ch == '(' || ch == ')' || ch == '=' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '"' || ch == '!')
		{
			state = 0;
		}
		else if (state == 0)
		{
			state = 1, x++;
		}
	}
	return x;
}
//统计条件单词数
int countwn2(FILE* f,FILE* f2)
{
	int x = 0;
	int sign = 0;//记录单词是否相同
	int oneword = 0;//记录在源文件中读到了一个单词
	int point = 0;//用于单词字母的游标
	char ch;
	int len = 0;//记录禁用单词的个数
	int state = 0;//记录进出单词状态
	word* sw[MAX];
	charlink *p1, *p2;//临时存储字母
	
	charlink* ones;
	while ((ch=fgetc(f2))!=EOF)
	{
		 if ((ch >= 97 && ch <= 122) || (ch >= 65 && ch <= 90))
		     if (state==0)
		  {//开始记录单词
			len++;
			sw[len - 1] = (word*)malloc(sizeof(word));
			ones = (charlink*)malloc(sizeof(charlink));
			ones->c = ch;
			ones->next = NULL;
			sw[len-1]->mychar = ones;
			sw[len-1]->len = 1;
			p1 = ones;
			state = 1;
		  }
		   else
		  {//正在记录单词
			   ones = (charlink*)malloc(sizeof(charlink));
			   ones->c = ch;
			   ones->next = NULL;
			   p1->next = ones;
			   p1 = ones;
			   sw[len-1]->len++;
		}
		 else if (state == 1)//记单词时出了单词
		 {
			 ones = (charlink*)malloc(sizeof(charlink));
			 ones->c = '\0';
			 ones->next = NULL;
			 p1->next = ones;
			 sw[len-1]->len++;
			 state = 0;
		 }
	}
	    //处理EOF
	    if (state == 1)//记单词时出了单词
		{
			 ones = (charlink*)malloc(sizeof(charlink));
			 ones->c = '\0';
			 ones->next = NULL;
			 p1->next = ones;
			 sw[len - 1]->len++;
			 state = 0;
		 }
	
	//测试禁用单词的提取情况
	//printf("len=%d", len);

	int i, j, kword;
	word* myword;
	myword = (word*)malloc(sizeof(word));
	state = 0;
	oneword = 0;

	kword = 0;
	//只有在最后一个单词才会出现EOF
	while ((ch = fgetc(f)) != EOF) 
		kword++;
	fseek(f, 0L, SEEK_SET);
	while (kword--)
	{
		//对EOF进行防范
		if (kword > 0)
			ch = fgetc(f);
		else
			ch = '\0';
		//printf("%c ", ch);
		if (kword!=0&&(ch >= 97 && ch <= 122) || (ch >= 65 && ch <= 90))
		{
			if (state == 0)//开始记录单词
			{
				ones = (charlink*)malloc(sizeof(charlink));
				ones->c = ch;
				ones->next =NULL;
				myword->len = 1;
				myword->mychar = ones;
				p1 = ones;
				state = 1;
			}
			else //正在记录单词
			{
				ones = (charlink*)malloc(sizeof(charlink));
				ones->c = ch;
				ones->next = NULL;
				myword->len++;
				p1->next = ones;
				p1 = ones;
			}
		}
		else if (state == 1||kword==0)
		{
			state = 0;
			ones = (charlink*)malloc(sizeof(charlink));
			ones->c = '\0';
			ones->next = NULL;
			myword->len++;
			p1->next = ones;
			oneword = 1;
		}

		//每读到一个单词进行比较
		if (oneword==1)
		{
			oneword = 0;
			charlink *a, *b;
			sign = 0;
			for (point = 0; point < len; point++)//与每个单词比较
			{
				sign = 1;//假定相等
				a = sw[point]->mychar;
				b = myword->mychar;

				while (1)//与每个字符比较
				{
					if (a->c != b->c)
					{
						sign = 0;
						break;
					}
					if (a->c == '\0' || b->c == '\0')
						break;
					a = a->next;
					b = b->next;
				}
				if (sign ==1)//存在相等单词就结束比较
					break;
			}
			//记录净单词数
			if (sign != 1)x++;
			//printf("%d\n", myword->len);
			for (i = myword->len - 1, p1 = myword->mychar; i >= 0; i--)
			{
				if (i>0)
					p2 = p1->next;
				//printf("%c ", p1->c);
				free(p1);
				p1 = p2;
			}
		}
	}

	for (i = 0; i < len; i++)
	{
		for (j = sw[i]->len - 1, p1 = sw[i]->mychar; j >= 0; j--)
		{
			if (j>0)
			p2 = p1->next;
			free(p1);
			p1 = p2;
		}
		free(sw[i]);
	}
	free(myword);
	return x;
}

//统计单词行，空行，注释行
int* countal(FILE* f)
{ 
	int x[3]={0};
	int ln=0;//记录行状态
	char ch,ch1='a';
	int lm = 0;//记录注释类
	int keword = 0;
	while ((ch = fgetc(f)) != EOF) keword++;
	fseek(f,0L,SEEK_SET);
	while (keword--)
	{
		if (keword>0)
			ch = fgetc(f);
		else
			ch = '\n';
		//printf("%d ", ln);//测试状态迁移
		lm = 0;
		//进注释
		if (ch == '/'){
			 ch1= fgetc(f);
			 if (ch1 == '*')lm = 1;//出现/*
			 else if (ch1 == '\n') ch=ch1;// /后回车(假定不会出现)
			 else if (ch1 == '/')lm = 3;//出现//
			 else if (ch1!=' '&&ch1!='\t')
				 ch1 = 'x';//双字符
		}
		//出注释
		else if (ch == '*'){
			ch1 = fgetc(f);
			if (ch1 == '\n');//*后回车
			else if (ch1 == '/') lm = 2;//出现*/
			else if (ch1!= ' '&&ch1!= '\t')
				ch1 = 'x';//双字符
		}
		if ((ch == '\n'&&ln == 1 && ch1 != '\n') || (ch == '\n'&&ln == 0))//空状态本行有一个或0个字符
			x[1]++, ln = 0, ch1 = 'a';

		else if (ch1 == '\n'&&ln == 1)//空状态本行有2个字符
			x[0]++, ln = 0, ch1 = 'a';

		else if (ch == '\n' && ln == 5)//空注释
			x[1]++;

		else if (ch == '\n'&&(ln == 6||ln==8)) x[0]++, ln = 0;//表示本行有代码x
		else if (ch == '\n'&&ln == 2) x[0]++, ln = 5;//进入x/*后的回车
		else if (ch == '\n'&&ln == 7) x[2]++, ln = 5;//进入/*后的回车
		else if (ch == '\n' && ln == 9) x[2]++,ln = 0;//非空注释状态//下换行
		else if (ch == '\n' && ln == 3) x[2]++,ln = 0;//非空注释状态*/下换行

		else if (lm == 1 &&ln == 6) ln = 2;//代码后跟进注释/*
		else if (lm == 1 && ln == 3) ln = 7;//出注释后有/*

		else if (lm == 3 &&ln == 6) ln = 8;//代码后跟全注释x//
		else if (lm == 3 && ln == 3) ln = 9;//出注释后有//

		else if (lm == 2 &&(ln == 5||ln==7)) ln = 3;//注释/*,后出注释
		else if (lm == 2 &&ln == 2) ln = 6;//代码后进注释后出注释

		else if (lm == 1 && ln == 0) ln = 7;//直接进入注释状态/*
		else if (lm == 3 && ln == 0) ln = 9;//直接进入完全注释状态//

		else if (ch1 == 'x'&&ln == 0) ln = 6, ch1 = 'a';//空状态下两个字符

		else if (ch != ' '&&ch != '\t' &&ln == 3) ln = 6;//出注释后有代码
		else if (ch != ' '&&ch != '\t'&&ln == 5) ln = 7;//空注释状态下有字符
		else if (ch != ' '&&ch != '\t'&&ln == 1) ln = 6;//进入代码状态
		else if (ch != ' '&&ch != '\t'&&ln == 0) ln = 1;
	}

   return x;
}