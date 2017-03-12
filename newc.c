//This Programs Convert Given Assembly code into binary
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
//structure for storing various hash tables
struct mnmValue
{
    char text[20];
    char binary[50];
};
typedef struct mnmValue mnmValue;

mnmValue Opcodes[50];
mnmValue Register[50];
mnmValue Label[50];

//different constants
int instructionLength= 16;
int registerLength =4;
int labelLength =4;
int dataLength =8;
int maximumOpcodes= 32;

int nOp=0,nReg=0,nLab=0,instLen=0;
char filename[10];

void reverse(char *str){
	char *end=str;
	char temp;
	if (str){
		while (*end){
			end++;
		}
		--end;
		while(str<end){
			temp=*str;
			*str++=*end;
			*end--=temp;
		}
	}

}
int binary(int num,int length,char *temp)
{
    int k;
    for(k=0;k<length;k++)
    {
        temp[k]=num%2+'0';
        num>>=1;
    }
    temp[k]='\0';
    reverse(temp);
	return 0;
}
int errorcheck(char s[])
{
    int i=0,j=0,n,w=0,k;
    char mem[50];
    while(s[i]!=' ')
    {
        i++;
    }
    if (s[i-1]=='I' && s[i+1]!='#')
    {
        return 0;
    }
    else if (s[i-1]=='I' && s[i+1]=='#')
    {
        for(k=0;k<=strlen(s);k++)
        {
            if(s[k]==' '||s[k]==',')
            {
                w++;
                int q=0;
                if (w==2)
                {
                    n=getnumber(s,j+1,k);
                    break;
                }
                j=i+1;
            }
        }
        //printf("%d\n",n);
        if (numbercheck(n))
            return 1;
        else
        {
            printf("Value to be transferred is out of index\n");
            return 0;
        }
    }

    else if (s[i-1]=='R' && (s[i+1]!='R' || s[i+5]!='R'))
    {
        return 0;
    }
    else if (s[0]=='J' && s[i+1]!='L')
    {
        return 0;
    }
    return 1;
}

int getnumber(char* s,int j,int i)
{
    int num=0,k;
    for(k=j;k<i;k++)
    {
        num=num*10+(s[k]-'0');
    }
    return num;
}

int firstpass()
{
    int i;
    FILE *f=fopen(filename,"r");
    FILE *l=fopen("labels.txt","w");
    char s[50],lab[10],temp[10];
    int line_cnt=0,lab_cnt=0;
    while(fgets(s,50,f)!=NULL)
    {
        s[strlen(s)-1]='\0';
        line_cnt++;
        int flag=errorcheck(s);
        if (flag==0)
            return flag;
        if(s[0]=='L')
        {
            lab_cnt++;
            lab[0]=s[0];
            lab[1]=s[1];
            if(s[2]==':')
            {
                lab[2]='\0';
            }
            else
            {
                lab[2]=s[2];
                lab[2]='\0';
            }
            binary(line_cnt-(lab_cnt-1),labelLength,temp);
            fprintf(l,"%s %s\n",lab,temp);
        }
    }
    fclose(f);
    fclose(l);
    return 1;
}
//function to generate hash table for labels
void genLabels()
{
    nLab=0;
    FILE *f=fopen("labels.txt","r");
    if(f==NULL)
    {
        printf("Label Table not found\n");
    }
    else
    {
        while(fscanf(f,"%s %s",Label[nLab].text,Label[nLab].binary)!=EOF){nLab++;}
    }
    fclose(f);
}


void Assemblycode()
{
    FILE *as=fopen(filename,"r");
    char s[50];
    while(fgets(s,50,as)!=NULL)
    {
        printf("%s",s);
    }
    fclose(as);
}

void getRegister()
{
    int i=0;
    for(i=0;i<nReg;i++)
    {
        printf("%s %s\n",Register[i].text,Register[i].binary);
    }
}

void getOpcode()
{
    int i=0;
    for(i=0;i<nOp;i++)
    {
        printf("%s %s\n",Opcodes[i].text,Opcodes[i].binary);
    }
}

void Machinecode()
{
    FILE *f=fopen("bincov.txt","r");
    char s[50];
    while(fgets(s,50,f)!=NULL)
    {
        printf("%s",s);
    }
    fclose(f);
}

void gLabel()
{
    int i=0;
    for(i=0;i<nLab;i++)
    {
        printf("%s %s\n",Label[i].text,Label[i].binary);
    }
}

int numbercheck(int num)
{
    if (num>255 || num <-256)
        return 0;
    else
        return 1;
}

//function to convert a number in form of string to binary string
void get_binary(char *s,int l,int h,char *temp)
{
    int num,i;
    num=getnumber(s,l,h);
    for(i=dataLength-1;i>=0;i--)
    {
        temp[i]=num%2+'0';
        num>>=1;
    }
    temp[dataLength]='\0';
    //reverse(temp);
}
//This is utility function for fromAssemblyToMachine()
void bingenrator(char *s,int l,int h,int *flag,int wordcount,FILE *out,int* x)
{
    char temp[50];
    int i;
    if(wordcount==1)
    {
        if(s[h-1]=='I')//If there is any immediate addressing mode then value of flag would be 1
        {
            (*flag)=1;
        }
        for(i=l;i<h;i++)
        {
            temp[i]=s[i];
        }
        temp[i]='\0';
        for(i=0;i<nOp;i++)
        {
            if(!strcmp(Opcodes[i].text,temp))
            {
                fprintf(out,"%s",Opcodes[i].binary);
                return;
            }
        }
    }
    else
    {
        if(*flag==1&&wordcount==2)//constant value
        {
            if (s[l]=='#')
            {
                get_binary(s,l+1,h,temp);
                fprintf(out,"%s",temp);
                return;
            }
            else
            {
                (*x)=2;
                return;
            }
        }
        else if(wordcount==2)//registers
        {
            for(i=l;i<h;i++)
            {
                temp[i-l]=s[i];
            }
            temp[i-l]='\0';
            for(i=0;i<nReg;i++)
            {
                if(!strcmp(Register[i].text,temp))
                {
                    fprintf(out,"%s",Register[i].binary);
                    return;
                }
            }
            for(i=0;i<nLab;i++)//if its not in register then its label
            {
                if(!strcmp(Label[i].text,temp))
                {
                    fprintf(out,"%s",Label[i].binary);
                    return;
                }
            }
        }
        else if(wordcount==3)//3rd part of instruction if exists is always register
        {
            for(i=l;i<h;i++)
            {
                temp[i-l]=s[i];
            }
            temp[i-l]='\0';
            for(i=0;i<nReg;i++)
            {
                if(!strcmp(Register[i].text,temp))
                {
                    fprintf(out,"%s",Register[i].binary);
                    return;
                }
            }
        }
    }
}
//This function uses utilityMachineFunction and converts the machine code into assembly code
void SecondPass()
{
    FILE *as=fopen(filename,"r");
    FILE *out=fopen("bincov.txt","w");
    char s[50];
    int i,j,flag,wordcount,x;
    if(as==NULL)
    {
        printf("Unable to find the assembly code\n");
        return;
    }
    while(fgets(s,50,as)!=NULL)
    {
        s[strlen(s)-1]='\0';
        j=0;
        wordcount=0;
        flag=0;
        int z=0;
        int fl=0;
         for(i=0;i<=strlen(s);i++)
        {
            if(s[i]==' '||s[i]=='\0'||s[i]==',')
            {

                wordcount++;
               if(wordcount==1&&s[j]=='L')
               {
                   fl=1;
                   break;
               }
                bingenrator(s,j,i,&flag,wordcount,out,&x);
                j=i+1;
                if(x==2)
                {
                    printf("Correct Syntax not found\n");
                    break;
                }
            }

        }
       if(!fl)
            fprintf(out,"\n");
       if(x==2)
            break;
    }
    fclose(as);
    fclose(out);
    if (x!=2)
        printf("Successfully Converted to Machine instruction\n");
}

//This is the main function which use cases to print the options available to us
int main()
{
    int option=0;

    FILE *file=fopen("registers.txt","r");
    while(fscanf(file,"%s %s",Register[nReg].text,Register[nReg].binary)!=EOF){nReg++;}
    fclose(file);

	FILE *file1=fopen("opcodes.txt","r");
    while(fscanf(file1,"%s %s",Opcodes[nOp].text,Opcodes[nOp].binary)!=EOF){nOp++;}
    fclose(file1);

    do
    {
        printf("~~~OPTIONS AVAILABLE FOR YOU~~~\n");
        printf("1.To print the register table\n");
        printf("2.To print the opcode table\n");
        printf("3.To convert assembly to binary\n");
        printf("4.To print the label Table\n");
        printf("5.To print the assembly code\n");
        printf("6.To print the machine code\n");
        printf("7.Exit\n");

        scanf("%d",&option);
        printf("\n");

        if (option==1)
        	getRegister();
        else if (option==2)
        {
        	/* code */
        	getOpcode();
        }
        else if(option==3)
        {
            printf("Enter your file name\n");
            scanf("%s",filename);
            int flag=firstpass();//this is pass 1 checks for errors in syntax and labels
            if (flag==0)
            {
                printf("Error in syntax\n");
                break;
            }
            genLabels();
            SecondPass();//pass 2  CONVERTS FROM ASSEMBLY TO MACHINE
        }
        else if(option==4)
        {
            gLabel();
        }
        else if (option==5)
        {
        	Assemblycode();
        }
        else if (option==6)
        {
        	Machinecode();
        }

        printf("\n");
    }while(option!=7);
    return 0;
}