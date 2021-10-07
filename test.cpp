#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "string.h"

FILE *fileopen;
FILE *filedown;
char *keep[7] = {"if","else","for","while","int","write","read"};
char oneS[20] = "+-*();{}";
char twoS[20] = "=><!";
char temp[30];
char annotation[100];
int annoflag = 0;
int flag = 0;
int line = 1;

int space(char letter)
{
	if(letter == '\n' || letter == ' ' || letter == '\t')
	{
		if(letter == '\n')
		{
			line++;
		}
		return 1;
	}
	return 0;
} 

int identify(char letter)
{
	if(isalpha(letter) || isdigit(letter))
	{
		temp[flag++] = letter;
		return 1;
	}
	return 0;
}

int uninteger(char letter)
{
	if(letter == '0')
	{
		temp[flag++] = letter;
		temp[flag] = '\0';
		return 1;
	}
	return 0;
}

int twoStep(char letter)
{
	if(letter == '=')
	{
		temp[flag++] = letter;
		temp[flag] = '\0';	
		return 1;
	}
	else
	{
		if(temp[0] == '!')
		{
			return 2;
		}
	}
	return 3;
}

int Judge()
{
	char letter;
	if((fileopen = fopen("file.txt", "r")) == NULL)
	{
		printf("文件打开错误");
	}
	if((filedown = fopen("one111.txt", "w")) == NULL)
	{
		printf("lex文件打开错误");
	}
	letter = getc(fileopen);
	while(letter != EOF)
	{
		flag = 0;
		memset(temp, 0, 30);
		annoflag = 0;
		memset(annotation,0,100);
		while(space(letter) == 1)
		{
			letter = getc(fileopen);
		}
		if(isalpha(letter))
		{
			while(identify(letter) == 1)
			{
				letter = getc(fileopen);
			}
			for(int i=0; i<7; i++)
			{
				if(strcmp(temp,keep[i]) == 0)
				{
					fprintf(filedown, "%s %s\n", temp, temp);
					break;
				}
				if(i == 6)
				{
					fprintf(filedown, "%s %s\n", "ID", temp);
				}
			}
		}
		else if(isdigit(letter))
		{
			if(uninteger(letter) == 1)
			{
				fprintf(filedown, "%s %s\n", "Num", temp);
				letter = getc(fileopen);
			}
			else
			{
				while(isdigit(letter))
				{
					temp[flag++] = letter;
					letter = getc(fileopen);
				}
				temp[flag] = '\0';
				fprintf(filedown, "%s %s\n", "Num", temp);		
			}
		}
		else if(strchr(oneS, letter))
		{
			temp[flag++] = letter;
		    temp[flag] = '\0';
		    letter = getc(fileopen);
			fprintf(filedown, "%s %s\n", temp, temp);
		}
		else if(strchr(twoS,letter))
		{
			temp[flag++] = letter;
			letter = getc(fileopen);
			if(twoStep(letter) == 1)
			{
				fprintf(filedown, "%s %s\n", temp, temp);
				letter = getc(fileopen);
			}
			else if(twoStep(letter) == 2)
			{
				annotation[annoflag++] = temp[0];
				annotation[annoflag++] = letter;
				printf("the line-%d find the error:%s\n",line,annotation);
			}
			else if(twoStep(letter) == 3)
			{
				fprintf(filedown, "%s %s\n", temp, temp);
			}
		}
		else if(letter == '/')
		{
			annotation[annoflag++] = letter;
			letter = getc(fileopen);
			if(letter == '*')
			{
				char next;
				next = getc(fileopen);
				while(!(letter=='*'&& next=='/') && next!=EOF)
				{
					annotation[annoflag++] = letter;
					letter = next;
					next = getc(fileopen);
				}
				if(letter == '*' && next == '/') 
				{
					// fprintf(filedown, "comment /**/\n");
				}
				else
				{
					annotation[annoflag++] = letter;
					printf("the line-%d comment isn't match:%s\n",line,annotation);
				}	
				letter = getc(fileopen);
			}
			else
			{
				temp[flag++] = '/';
				temp[flag] = '\0';
				fprintf(filedown, "%s %s\n", temp, temp);
			}
		}
		else
		{
			temp[flag++] = letter;
			temp[flag] = '\0';
			printf("the line-%d find the error:%s\n",line,temp);
			letter = getc(fileopen);
		}
	}
	fclose(fileopen);
    fclose(filedown);
	return 0;
}

int main()
{
	Judge();
	return 0;
}