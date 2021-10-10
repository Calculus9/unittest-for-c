#include "stdio.h"
#include "ctype.h"
#include "stdlib.h"
#include "string.h"

FILE *fileopen;
FILE *filedown;
// 关键字
char *keep[7] = {"if", "else", "for", "while", "int", "write", "read"};
// 单分符
char oneS[20] = "+-*();{}";
char twoS[20] = "=><!";
char temp[30];
char annotation[100];
int annoflag = 0;
int flag = 0;
int line = 1;

/**
 * @brief 统计当前的行数，如果此时换行，则继续读取下一个字符
 * 
 * @param letter 
 * @return int 
 */
int space(char letter)
{
	if (letter == '\n' || letter == ' ' || letter == '\t')
	{
		if (letter == '\n')
		{
			line++;
		}
		return 1;
	}
	return 0;
}



/**
 * @brief 如果是字符或数字，则进行组合成temp
 * 
 * @param letter 
 * @return int 
 */
int identify(char letter)
{
	if (isalpha(letter) || isdigit(letter))
	{
		temp[flag++] = letter;
		return 1;
	}
	return 0;
}


/**
 * @brief 前导零处理
 * 
 * @param letter 
 * @return int 
 */
int uninteger(char letter)
{
	if (letter == '0')
	{
		temp[flag++] = letter;
		temp[flag] = '\0';
		return 1;
	}
	return 0;
}


/**
 * @brief 双分界符处理
 * 
 * @param letter 
 * @return int 
 */
int twoStep(char letter)
{
	if (letter == '=') //如果是=，组合双分界符
	{
		temp[flag++] = letter;
		temp[flag] = '\0'; //组合双分界符结束
		return 1;
	}
	else
	{
		if (temp[0] == '!')
		{
			return 2;
		}
	}
	// n = 00123450;||abc=abc+i;
	return 3;
}

int Judge()
{
	char letter;
	if ((fileopen = fopen("file.txt", "r")) == NULL)
	{
		printf("文件打开错误");
	}
	if ((filedown = fopen("one111.txt", "w")) == NULL)
	{
		printf("lex文件打开错误");
		// cout<<"lex文件打开错误！\n"<<endl;
	}
	letter = getc(fileopen);
	while (letter != EOF)
	{
		flag = 0;
		memset(temp, 0, 30);
		annoflag = 0;
		memset(annotation, 0, 100);
		while (space(letter))
		{
			letter = getc(fileopen); //统计当前的行数，如果此时换行、空格、\t，则继续读取下一个字符
		}

		if (isalpha(letter)) //此时是字符，开始组合
		{
			// 组合当前的单词，只有数字和字母才进行组合。
			while (identify(letter) == 1)
			{
				letter = getc(fileopen);
			}
			// 判断是否为关键字
			for (int i = 0; i < 7; i++)
			{
				if (strcmp(temp, keep[i]) == 0) //是保留字，输出保留字
				{
					fprintf(filedown, "%s %s\n", temp, temp); //输出保留字符号
					break;
				}
				if (i == 6) //不是保留字，输出标识符
				{
					fprintf(filedown, "%s %s\n", "ID", temp);
				}
			}
		}
		else if (isdigit(letter)) //数字处理
		{
			if (uninteger(letter) == 1) //前导零处理
			{
				fprintf(filedown, "%s %s\n", "Num--", temp);
				letter = getc(fileopen);
			}
			else
			{
				while (isdigit(letter)) //如果是数字则组合整数；如果不是则整数组合结束
				{
					temp[flag++] = letter;
					letter = getc(fileopen);
				}
				temp[flag] = '\0';
				fprintf(filedown, "%s %s\n", "Num", temp);
			}
		}
		else if (strchr(oneS, letter)) //单分符处理+-*();{}
		{
			temp[flag++] = letter;
			temp[flag] = '\0';
			letter = getc(fileopen);				  //读下一个符号以便识别下一个单词
			fprintf(filedown, "%s %s\n", temp, temp); //输出单分界符符号
		}
		else if (strchr(twoS, letter)) //双分界符处理
		{
			temp[flag++] = letter;
			letter = getc(fileopen);  //读下一个字符，判断是否为双分界符
			if (twoStep(letter) == 1) //当前字符为=
			{
				fprintf(filedown, "%s %s\n", temp, temp);
				letter = getc(fileopen);
			}
			else if (twoStep(letter) == 2) //当前为错误格式!x
			{
				annotation[annoflag++] = temp[0];
				annotation[annoflag++] = letter;
				printf("the line%d find the error:%s\n", line, annotation);
			}
			else if (twoStep(letter) == 3) //当前字符为><
			{
				fprintf(filedown, "%s %s\n", temp, temp);
			}
		}
		else if (letter == '/') //注释处理
		{
			annotation[annoflag++] = letter;
			letter = getc(fileopen);
			if (letter == '*')
			{
				char next;
				next = getc(fileopen);
				while (!(letter == '*' && next == '/') && next != EOF) //一直读取注释内容
				{
					annotation[annoflag++] = letter; //保存注释内容
					letter = next;
					next = getc(fileopen);
				}
				// 读完了所有内容
				if (letter == '*' && next == '/') // 注释结束
				{
					fprintf(filedown, "comment /**/\n"); //输出到文件
				}
				else //注释错误
				{
					annotation[annoflag++] = letter;
					printf("the line-%d comment isn't match:%s\n", line, annotation);
				}
				letter = getc(fileopen);
			}
			else //不是*则处理单分界符/
			{
				temp[flag++] = '/';
				temp[flag] = '\0';
				printf("%s %s\n", temp, temp);
				fprintf(filedown, "%s %s\n", temp, temp); //输出单分界符/
			}
		}
		else //错误处理
		{
			temp[flag++] = letter;
			temp[flag] = '\0';
			printf("the line-%d find the error:%s\n", line, temp); //输出错误符号
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