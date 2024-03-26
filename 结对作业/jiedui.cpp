#include<stdio.h>
#include<stdlib.h> 
#include<time.h>
#include <string.h>

/*
1、使用 -n 参数控制生成题目的个数，例如Myapp.exe -n 10
2、使用 -r 参数控制题目中数值（自然数、真分数和真分数分母）的范围，例如Myapp.exe -r 10
3、生成的题目中计算过程不能产生负数
4、生成的题目中如果存在形如e1/e2的子表达式，那么其结果应是真分数
5、每道题目中出现的运算符个数不超过3个
6、程序一次运行生成的题目不能重复
7、在生成题目的同时，计算出所有题目的答案，并存入执行程序的当前目录下的Answers.txt文件
8、程序应能支持一万道题目的生成

char random_symbol(); //生成随机运算符
int random_number(int a);//生成随机数 
int CreatQuestion(int m,int digital_index,int *answer); //生成题目
int Answer(char *str,int i);//生成答案 
void WriteQuestion(char *str,int digital_index);//写入问题
void WriteAnswer(int digital_index,int result);//写入答案 
void CheckAnswer(char exercisefile[],char answerfile[]);//检查答案是否正确，并写入Grade文件 

int main(int argc,char*argv[]){
	
	if(argc<2){
		printf("you must input argc!");
		return 0;
	}
	srand((int)time(0));//设置rand()产生随机数时的随机数种子
	FILE *fp1,*fp2;
	
	int n,r;
	if(!strcmp(argv[1],"-n") && !strcmp(argv[3],"-r")){//生成题目和答案 
		fp1=fopen("Exercises.txt","w");
	    fp2=fopen("Answers.txt","w");
	    fclose(fp1);
	    fclose(fp2);
		n=atoi(argv[2]);
		r=atoi(argv[4]);
		int answer;
	    int digital_index=1;
	    while(digital_index<=n){
	        CreatQuestion(r,digital_index,&answer);
	        if(answer>=0){
	        digital_index++;
		    }
        }   
	}else if(!strcmp(argv[1],"-e") && !strcmp(argv[3],"-a")){//检查答案 
	    CheckAnswer(argv[2],argv[4]);
	}
	 
	
    return 0;
}

int random_number(int a)//随机数 生成1~a的数字 
{
   int b;
   b=rand()%a+1;
   return b;
}

char random_symbol()//随机运算符 
{
    char symbol[4]={'+','-','*','/'};
    return symbol[rand()%4];
}

 //生成算术表达式
int CreatQuestion (int m,int digital_index,int *answer) 
{
	char str[250] = {'0'};  //定义一个字符数组存放算术表达式
	int x, num, ran_num, j = 0;  
	num = random_number( 3 );//生成随机运算符数目 
	
	//循环生成算术表达式
	for ( int k = 0; k < num; k++ ) 
	{
		ran_num = random_number ( m );  //生成一个参与表达式的数值，m是数值范围 （1~10） 
		x = 1;
		while ( ran_num / x )
		{
			x *= 10;
		}
		x /= 10;//x=10,x=1

		while ( x )//x=10执行两次 
		{
			str[j++] = ran_num / x + '0';//将数字型转换为字符串型 
			ran_num = ran_num % x;
			x /= 10;
		}

		str[j++] = random_symbol();  //随机生成一个运算符

	}

    //生成一个随机结尾数值
	ran_num =  random_number( m );  
	x = 1;
	while ( ran_num / x )
	{
		x *= 10;
	}
	x /= 10;

	while ( x )
	{
		str[j++] = ran_num / x + '0';
		ran_num = ran_num % x;
		x /= 10;
	}
    
	int result = Answer(str,digital_index);
	*answer = result;
	if(result>=0){
		WriteQuestion(str,digital_index);
		WriteAnswer(digital_index,result);
	}
}

//生成答案模块
int Answer(char *str,int digital_index) 
{
	char* p =str;
	
	int num[100] = {0};//数字数组
	char symbol[100] = {0};//运算符数组 
	int index = 0;	//索引  
	
	char stack[100] = {0};//运算符栈 
	int stacki[100] = {0};//结果栈 
	int top = -1;//栈的索引 -1表示栈为空 
	
	int temp = 0;//正在拼写的数字 
	int flag = 0;//表示当前是否正在拼写数字
	
	//中缀表达式转化为逆波兰表达式
	//最后的结果就是得到两个数组，分别是num和symbol 
	while(1){
		if(*p>='0' && *p<='9')//读到了一个数字 
		{	flag = 1;
			temp *= 10;//第一个为数字时temp=0,第二个为数字时才乘以10 
			temp += *p - '0';//0的ASCII值为48
		}
		else//读到了一个符号，或字符串已经结束了
		{
			if(flag)//拼写完1个数字先将数字输出 
			{
			   num[index] = temp;
			   symbol[index] = '!';
			   index++;
			   flag = temp =0;//重新置0 
			}
			if(!*p)//如果字符串已经结束 
			{
				//最终，将栈元素全部出栈
				while(top>=0) symbol[index++]=stack[top--];
				break;//跳出循环 
			}
			else// 字符串还未结束 
			{
				 if(top == -1||*p == '(') //如果栈空时，或符号为左括号 		 
				 	stack[++top] = *p;//入栈 
				 else if(*p == ')')//如果是右括号，出栈到左括号
				 {
				 	while(top>=0&&stack[top]!='(')//如果不为左括号。出栈 
				 	{				 							
						symbol[index++]=stack[top--];
					} 
					--top;//如果为左括号，--top 
				 }		 
				else if(*p == '*'||*p == '/')//如果新符号是乘除
				 {
				 	while(top>=0&&(stack[top]=='*'||stack[top]=='/'))//乘除出栈
				 	{				 							
						symbol[index++]=stack[top--];
					} 
					stack[++top] = *p;//新符号入栈
				 }
				 else //如果新运算符是加减
				 {
				 	while(top>=0&&stack[top]!='(')//四则运算出栈
				 	{				 							
						symbol[index++]=stack[top--];
					} 
					stack[++top] = *p;//新符号入栈
				 }
			}
		}
		p++;
	}
	
	//逆波兰表达式求解
	top =-1;
	int temp1 = 0;
	int temp2 = 0;
	for(int i =0;i<index;i++){
		if(symbol[i]=='!'){//这时的i索引表示的是数字，数字入栈stacki 
			stacki[++top] = num[i];	
		}
       	//符号运算
       	else
       	{
       		temp1 = stacki[top--];
			temp2 = stacki[top--];
			switch (symbol[i])
			{
			case '+':
				stacki[++top] = temp2 + temp1;//将结果入栈 
				break;
			case '-':
				stacki[++top] = temp2 - temp1;//后面减去前面 
				break;
			case '*':
				stacki[++top] = temp2 * temp1;
				break;
			case '/':
				stacki[++top] = temp2 / temp1;
				break;
			}
		}
	}
	//最终结果是stacki[0]
	int result = stacki[0];
//	printf("%s=%d\n",str,result);	
//	WriteAnswer(digital_index,result);
	return result; 
}

//写入答案 
void WriteQuestion(char *str,int digital_index){
	int k=0;
    FILE *fp;
   	fp=fopen("Exercises.txt","a");//若文件不存在则建立该文件,这里不能用w,要用a 
   	fprintf(fp,"%d:",digital_index);
   	while(str[k]!=NULL){
   		if(str[k] == '+' ||str[k] == '-' ||str[k] == '*' ||str[k] == '/' || str[k] == '='){
   		     fprintf(fp," %c ",str[k]);
		}else fprintf(fp,"%c",str[k]);
		k++;
	   }
	fprintf(fp," =\n");
	fclose(fp);
} 

//写入问题 
void WriteAnswer(int digital_index,int result){
	FILE *fp;
   	fp=fopen("Answers.txt","a");//若文件不存在则建立该文件,这里不能用w,要用a 
   	fprintf(fp,"%d:",digital_index);
   	fprintf(fp,"%d\n",result);
   	fclose(fp);
}

//检查答案 
void CheckAnswer(char exercisefile[],char answerfile[]){
	FILE *fp1,*fp2,*fp3;

    fp1=fopen("Answers.txt","r");
    fp2=fopen(answerfile,"r");     
    fp3=fopen("Grade.txt","w");
    
    int index=1; //题目序号
    
    char correct_answer[30]={};  //存放正确答案 
    char answer[30]={};  //存放从答案文件取出的答案
     
    
    int c_num=0,w_num=0;   //用于计算对和错的总题数 
    
    int c_index[10000]={};   //用于储存对的和错的题号
    int w_index[10000]={};
    
    while(fgets(correct_answer,30,fp1)!=NULL && fgets(answer,30,fp2)!=NULL)
    {      
        if(strcmp(correct_answer,answer)==0)  //比较前n个字节的大小 
        {
            c_index[c_num++]=index;
            index++; 
        }
        else if(strcmp(correct_answer,answer)!=0)
        {
            w_index[w_num++]=index;
            index++;
        }
    }
 
    fprintf(fp3,"Correct: %d (",c_num);
    for(int i=0;i<c_num;i++)
    {
        fprintf(fp3,"%d",c_index[i]);
        if(i!=c_num-1) fprintf(fp3,",");
    }
    fprintf(fp3,")\n");
     
    fprintf(fp3,"Wrong: %d (",w_num);
    for(int i=0;i<w_num;i++)
    {
        fprintf(fp3,"%d",w_index[i]);
        if(i!=w_num-1) fprintf(fp3,",");
    }
    fprintf(fp3,")\n");
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);        
}
