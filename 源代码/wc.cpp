#include<stdio.h>
#include<stdlib.h>
#include<io.h>

#define MAX  50
//�����Խ��õĵ�����

//�ֱ�����ַ�������������������(�����У����У�ע����)
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
	char direct[MAX] = { 0 };//���ڽ�������ļ��ľ���·��
	int sign[7]={0};//��¼����-c,-w,-l,-o,-a,-e
	char* result="result.txt";//��¼����ļ�
	char* stoplist="stoplist.txt";//��¼ͣ�õ���
	char* source = "0";//�������Դ�ļ�
	char* dir = "0";//�����ļ���
	int ln=0,cn=0,wn=0;//�ֱ��¼�������ַ����������������е������������У����У�ע����
	int* three;
	int i=0,j=0;

	for(i=1;i<argc;i++)
		{
		    if( argv[i][0]=='-')//�ж��������
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
					printf("�����������"); break;
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
	//ִ����Ӧ��ͳ�Ʋ����

	FILE *file,*stop,*out;
	errno_t err;
	long handle;//��¼�ļ��е�����
	struct _finddata_t fileinfo;
	int dirlen=0;//��¼�ļ����ڵ��ļ���Ŀ
	char ch;//�����м����Ĵ�ȡ

	//���ܲ���
	//sign[6] = 1;
	//sign[2] = 1;
	//�ж��Ƿ����ļ���
	if (sign[6] == 1)
	{
		if ((handle = _findfirst(dir, &fileinfo)) == -1L)
		{
			printf("û���ҵ�ƥ���Ŀ¼");
			exit(0);
		}
		source = fileinfo.name;
		//�����Ƿ��ҵ��ļ�
		/*
		printf("%s\n", fileinfo.name);
		while (_findnext(handle, &fileinfo) == 0)
			printf("%s\n", fileinfo.name);
		//sign[6] = 0;
		*/
	}
	//������ļ�
	if ((err = fopen_s(&out, result, "w")) != 0)
	{
		printf("�޷���������ļ�");
		exit(0);
	}

	//ѭ�������ļ���
	int thedir = 1,firstdir=1;
	//int len = 0;
	while (thedir==1)
	{
		//����thedir
		//printf("%d ", thedir);
		if (sign[6] == 1)
		{
			if (firstdir != 1&&_findnext(handle, &fileinfo) == 0)//�ж��Ƿ��ǵ�һ���ļ�
				source = fileinfo.name;//������������ļ�������Ҫ���·����Ϊ�򻯲������ڵ�ǰĿ¼�²���
			else if (firstdir != 1)break;//�ļ��е�β������ѭ��
		}
		else thedir=0;
		firstdir = 0;

		//����
		//printf("%s ", source);
		/*sourceĩβ��\0
		for (i = 0; source[i] != '\0';i++)
		printf("%c ", source[i]);
		len = i;
		*/
		//if (source[0] == '0')break;
		//��ν����·������������һ���γɾ���·��
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
			//����
			//printf("%s ", source);
		}
		if ((err = fopen_s(&file, source, "r")) != 0)
		{
			printf("�޷��ҵ�Ŀ���ļ�");
			if (file == out)
				continue;
			exit(0);
		}
		else
		{
				if (sign[0] == 1)
				{
					cn = countcn(file), fprintf(out, "%s �ַ���: %d\n", source, cn);
					fseek(file, 0L, SEEK_SET);
				}
				if (sign[1] == 1)
				{
					if (sign[5] != 1)
						wn = countwn(file), fprintf(out, "%s ������: %d\n", source, wn);
					else
					{
						//ͳ�ƽ��ú�ĵ�����
						if ((err = fopen_s(&stop, stoplist, "r")) != 0)
						{
							printf("�޷��򿪽��õ����ļ�");
							//fputs("�޷��򿪽��õ����ļ�",out);//���ڲ���
							exit(0);
						}
						wn = countwn2(file, stop);
						fprintf(out, "%s ������: %d\n", source, wn);
						fclose(stop);
					}

					fseek(file, 0L, SEEK_SET);
				}
				if (sign[2] == 1)
				{
					ln = countln(file), fprintf(out, "%s ����: %d\n", source, ln);
					fseek(file, 0L, SEEK_SET);
				}
				if (sign[4] == 1)
				{
					three = countal(file);
					fprintf(out, "%s ������/����/ע����: %d,%d,%d\n", source, three[0], three[1], three[2]);
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

//ͳ���ַ���
int countcn(FILE* f)
{
	char ch;
	int x=0;
	while((ch=fgetc(f))!=EOF)
		x++;
	return x;
}

//ͳ������
int countln(FILE* f)
{
	int x=1;
	char ch;
	while((ch=fgetc(f))!=EOF)
		if(ch=='\n')
			x++;
	return x;
}

//ͳ�Ƶ�����
int countwn(FILE* f)
{
	int x=0;
	char ch;
	int state=0;//��¼��������״̬
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
//ͳ������������
int countwn2(FILE* f,FILE* f2)
{
	int x = 0;
	int sign = 0;//��¼�����Ƿ���ͬ
	int oneword = 0;//��¼��Դ�ļ��ж�����һ������
	int point = 0;//���ڵ�����ĸ���α�
	char ch;
	int len = 0;//��¼���õ��ʵĸ���
	int state = 0;//��¼��������״̬
	word* sw[MAX];
	charlink *p1, *p2;//��ʱ�洢��ĸ
	
	charlink* ones;
	while ((ch=fgetc(f2))!=EOF)
	{
		 if ((ch >= 97 && ch <= 122) || (ch >= 65 && ch <= 90))
		     if (state==0)
		  {//��ʼ��¼����
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
		  {//���ڼ�¼����
			   ones = (charlink*)malloc(sizeof(charlink));
			   ones->c = ch;
			   ones->next = NULL;
			   p1->next = ones;
			   p1 = ones;
			   sw[len-1]->len++;
		}
		 else if (state == 1)//�ǵ���ʱ���˵���
		 {
			 ones = (charlink*)malloc(sizeof(charlink));
			 ones->c = '\0';
			 ones->next = NULL;
			 p1->next = ones;
			 sw[len-1]->len++;
			 state = 0;
		 }
	}
	    //����EOF
	    if (state == 1)//�ǵ���ʱ���˵���
		{
			 ones = (charlink*)malloc(sizeof(charlink));
			 ones->c = '\0';
			 ones->next = NULL;
			 p1->next = ones;
			 sw[len - 1]->len++;
			 state = 0;
		 }
	
	//���Խ��õ��ʵ���ȡ���
	//printf("len=%d", len);

	int i, j, kword;
	word* myword;
	myword = (word*)malloc(sizeof(word));
	state = 0;
	oneword = 0;

	kword = 0;
	//ֻ�������һ�����ʲŻ����EOF
	while ((ch = fgetc(f)) != EOF) 
		kword++;
	fseek(f, 0L, SEEK_SET);
	while (kword--)
	{
		//��EOF���з���
		if (kword > 0)
			ch = fgetc(f);
		else
			ch = '\0';
		//printf("%c ", ch);
		if (kword!=0&&(ch >= 97 && ch <= 122) || (ch >= 65 && ch <= 90))
		{
			if (state == 0)//��ʼ��¼����
			{
				ones = (charlink*)malloc(sizeof(charlink));
				ones->c = ch;
				ones->next =NULL;
				myword->len = 1;
				myword->mychar = ones;
				p1 = ones;
				state = 1;
			}
			else //���ڼ�¼����
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

		//ÿ����һ�����ʽ��бȽ�
		if (oneword==1)
		{
			oneword = 0;
			charlink *a, *b;
			sign = 0;
			for (point = 0; point < len; point++)//��ÿ�����ʱȽ�
			{
				sign = 1;//�ٶ����
				a = sw[point]->mychar;
				b = myword->mychar;

				while (1)//��ÿ���ַ��Ƚ�
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
				if (sign ==1)//������ȵ��ʾͽ����Ƚ�
					break;
			}
			//��¼��������
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

//ͳ�Ƶ����У����У�ע����
int* countal(FILE* f)
{ 
	int x[3]={0};
	int ln=0;//��¼��״̬
	char ch,ch1='a';
	int lm = 0;//��¼ע����
	int keword = 0;
	while ((ch = fgetc(f)) != EOF) keword++;
	fseek(f,0L,SEEK_SET);
	while (keword--)
	{
		if (keword>0)
			ch = fgetc(f);
		else
			ch = '\n';
		//printf("%d ", ln);//����״̬Ǩ��
		lm = 0;
		//��ע��
		if (ch == '/'){
			 ch1= fgetc(f);
			 if (ch1 == '*')lm = 1;//����/*
			 else if (ch1 == '\n') ch=ch1;// /��س�(�ٶ��������)
			 else if (ch1 == '/')lm = 3;//����//
			 else if (ch1!=' '&&ch1!='\t')
				 ch1 = 'x';//˫�ַ�
		}
		//��ע��
		else if (ch == '*'){
			ch1 = fgetc(f);
			if (ch1 == '\n');//*��س�
			else if (ch1 == '/') lm = 2;//����*/
			else if (ch1!= ' '&&ch1!= '\t')
				ch1 = 'x';//˫�ַ�
		}
		if ((ch == '\n'&&ln == 1 && ch1 != '\n') || (ch == '\n'&&ln == 0))//��״̬������һ����0���ַ�
			x[1]++, ln = 0, ch1 = 'a';

		else if (ch1 == '\n'&&ln == 1)//��״̬������2���ַ�
			x[0]++, ln = 0, ch1 = 'a';

		else if (ch == '\n' && ln == 5)//��ע��
			x[1]++;

		else if (ch == '\n'&&(ln == 6||ln==8)) x[0]++, ln = 0;//��ʾ�����д���x
		else if (ch == '\n'&&ln == 2) x[0]++, ln = 5;//����x/*��Ļس�
		else if (ch == '\n'&&ln == 7) x[2]++, ln = 5;//����/*��Ļس�
		else if (ch == '\n' && ln == 9) x[2]++,ln = 0;//�ǿ�ע��״̬//�»���
		else if (ch == '\n' && ln == 3) x[2]++,ln = 0;//�ǿ�ע��״̬*/�»���

		else if (lm == 1 &&ln == 6) ln = 2;//��������ע��/*
		else if (lm == 1 && ln == 3) ln = 7;//��ע�ͺ���/*

		else if (lm == 3 &&ln == 6) ln = 8;//������ȫע��x//
		else if (lm == 3 && ln == 3) ln = 9;//��ע�ͺ���//

		else if (lm == 2 &&(ln == 5||ln==7)) ln = 3;//ע��/*,���ע��
		else if (lm == 2 &&ln == 2) ln = 6;//������ע�ͺ��ע��

		else if (lm == 1 && ln == 0) ln = 7;//ֱ�ӽ���ע��״̬/*
		else if (lm == 3 && ln == 0) ln = 9;//ֱ�ӽ�����ȫע��״̬//

		else if (ch1 == 'x'&&ln == 0) ln = 6, ch1 = 'a';//��״̬�������ַ�

		else if (ch != ' '&&ch != '\t' &&ln == 3) ln = 6;//��ע�ͺ��д���
		else if (ch != ' '&&ch != '\t'&&ln == 5) ln = 7;//��ע��״̬�����ַ�
		else if (ch != ' '&&ch != '\t'&&ln == 1) ln = 6;//�������״̬
		else if (ch != ' '&&ch != '\t'&&ln == 0) ln = 1;
	}

   return x;
}