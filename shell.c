#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"

void tokenize(char *c,char **arg)
{
    char *b=c;
    int j=0;
    int i=0;
    char del='\0';
    while(*b!=del && *b!='\n')
    {
       while(*b==' ')
	       b++;
       c=b;
       while(*b!=' ' && *b!=del && *b!='\n')
	       b++;
       if(*b==del || *b=='\n')
	       i=1;
       *b=del;
       if(c!=b)
	       arg[j]=c;
       if(i==1)
	       break;
       j++;
       b++;
    }
    arg[j+1]='\0';
}
int executepipe(char **arg1,char **arg2)
{
   int pipefd[2];
   pipe(pipefd) ;
   if(fork()==0)
   {
      close(1);
      dup(pipefd[1]);
      close(pipefd[0]);
      close(pipefd[1]);
      if(exec(arg1[0],arg1)<0)
	      printf(1,"invalid command");
   }
    else
    {
    
   if(fork()==0)
   {
     close(0);
     dup(pipefd[0]);
     close(pipefd[0]);
     close(pipefd[1]);
     if(exec(arg2[0],arg2)<0)
	     printf(1,"invalid command");
   }
   else 
   {
	  close(pipefd[0]);
	  close(pipefd[1]);
	  wait(0);
	  wait(0);
	 return 0; 
   }
  }
 return 0;
}
int executedoubleor(char **arg1,char **arg2)
{
	int status;
	if(fork()==0)
	{
	   if(exec(arg1[0],arg1)<0)
	   {
	       exit(-1);
	   }
	   else
		   exit(0);
	}
	else
	{
	   wait(&status);
	   if(status!=0)
	   {
	      if(fork()==0)
	      {
	         if(exec(arg2[0],arg2)<0)
			 printf(1,"illegal command or argument");
	      }
	      else
		      wait(0);
	   }
	}
return 0;
}
void pipes(char *c,char **arg1,char **arg2)
{
   char *p=c;
   char del='\0';
   int n=strlen(c);
   for(int j=0;j<n;j++)
   {
      if(c[j]!='|')
	      p++;
      if(c[j]=='|' && c[j+1]!='|')
      {
	      c[j]=del;
	      p++;
	      tokenize(c,arg1);
	      tokenize(p,arg2);
	      executepipe(arg1,arg2);
	      break;
      }
      else{
      if(c[j]=='|' && c[j]=='|')
      {
          c[j]=del;
	  c[j+1]=del;
	  p=p+2;
	  tokenize(c,arg1);
	  tokenize(p,arg2);
	 executedoubleor(arg1,arg2);
	break; 
      }
      }
   }
}
void simple(char *c,char **arg)
{
	tokenize(c,arg);
       	if(fork()==0)
       	{
		if(exec(arg[0],arg)<0)
         	printf(1,"illegal command or argument");
        }
        else
        wait(0);
}
int executeparellal(char **arg1,char **arg2)
{
   int pid1,pid2;
   pid1=fork();
   if(pid1==0)
   {
      if(exec(arg1[0],arg1)<0)
	      printf(1,"illegal command or argument");
   }
   else 
   {
      pid2=fork();
      if(pid2==0)
      {
         if(exec(arg2[0],arg2))
		 printf(1,"illegal command or argument");
      }
      else
      {
         wait(0);
	 wait(0);
	 return 0;
      }
   }
   return 0;
}
void semicolon(char *c,char **arg1,char **arg2)
{
    char *p;
    p=c;
    int n=strlen(c);
    char del='\0';
    for(int i=0;i<n;i++)
    {
       if(c[i]!=';')
	       p++;
       if(c[i]==';')
       {
           c[i]=del;
	   p++;
	   break;
       }
    }
    tokenize(c,arg1);
    tokenize(p,arg2);
    executeparellal(arg1,arg2);
}
int executesuccesive(char **arg1,char **arg2)
{ 
     int status;
     if(fork()==0)
     {
	     if(exec(arg1[0],arg1)<0)
	     {
		     printf(1,"illegal command or argument");
		     exit(-1);
	     }
	     else
	     {
		     exit(0);
	     }
     }
     else
     {
	     wait(&status);
	     if(status==0)
	     {
		     if(fork()==0)
		     {
			     if(exec(arg2[0],arg2)<0)
				     exit(-1);
			     else
				     exit(0);
		     }
		     else
			     wait(0);
	     }
     }
     return 0;
}  
void successive(char *c,char **arg1,char **arg2)
{
    char *p;
    p=c;
    char del='\0';
    int n=strlen(c);
    for(int i=0;i<n;i++)
    {
      if(c[i]!='&')
	      p++;
      if(c[i]=='&' && c[i+1]=='&')
      {
	    p=p+2;
            c[i]=del;
	    c[i+1]=del;
	    break;
      }
    }
    tokenize(c,arg1);
    tokenize(p,arg2);
    executesuccesive(arg1,arg2);
}

int executeredirection(char *c,char **arg1,char *inputfile,char *outputfile)
{
      if(fork()==0)
      {
	      int fd[2];
	       fd[0]=open(inputfile,O_RDONLY);
	       fd[1]=open(outputfile,O_RDWR|O_CREATE);
	      if(fd[1]==-1 || fd[0]==-1){
		      printf(1,"illegal command or argument");
		      exit(0);
	      }
	      close(0);
	      dup(fd[0]);
	      close(1);
	      dup(fd[1]);
	      if(exec(arg1[0],arg1)<0)
		      printf(1,"illegal command or argument");
      }
      else
	      wait(0);
      return 0;
}

void redirection(char *c,char **arg1)
{
	char *p=c;
	char *s=p;
        char del='\0';
	int n=strlen(c);
	int m=0;
	for(int i=0;i<n;i++)
	{
		if(c[i]!='<')
			p++;
		if(c[i]=='<')
		{
		   p++;
		   c[i]=del;
		   s=p;
                   break;
		}
	}
	int l=strlen(p);
	while(p[m]==' ')
	{
		p++;
		m++;
	}
	for(int j=0;j<l;j++)
	{
	    if(p[j]!='>')
		    s++;
	    if(p[j]=='>')
	    {
		    s++;
		    p[j]=del;
		    break;
	    }
	}
	m=0;
	while(s[m]==' ')
	{
		s++;
		m++;
	}
	tokenize(c,arg1);
	executeredirection(c,arg1,p,s);
}

int executepipewithredirection(char **arg1,char **arg2,char *inputfile,char *outputfile)
{
	int fd[2];
	pipe(fd);
	if(fork()==0)
	{
		int p=open(inputfile,O_RDONLY);
		if(p<-1)
		{
			printf(1,"illegal command or argument");
			exit(0);
		}
		close(0);
		dup(p);
		close(1);
		dup(fd[1]);
		close(fd[0]);
		close(fd[1]);
		if(exec(arg1[0],arg1)<0)
			printf(1,"illegal command or argument");
		else
			exit(0);
	}
	else
	{
		if(fork()==0)
		{
			int q=open(outputfile,O_RDWR|O_CREATE);
			if(q<-1)
			{
				printf(1,"illegal command or argument");
				exit(0);
			}
			close(1);
			dup(q);
			close(0);
			dup(fd[0]);
			close(fd[1]);
			close(fd[0]);
			if(exec(arg2[0],arg2)<0)
				printf(1,"illegal command or argument");
			else
				exit(0);
		}
		else
		{
			close(fd[0]);
			close(fd[1]);
			wait(0);
			wait(0);
		}
	}
	free(inputfile);
	free(outputfile);
	return 0;
}

void pipewithredirection(char *c,char **arg1,char **arg2)
{
      char *p=c;
      char del='\0';
      int n=strlen(c);
      for(int i=0;i<n;i++)
      {
	      if(c[i]!='|')
		      p++;
	      if(c[i]=='|')
	      {
	         c[i]=del;
		 p++;
		 break;
	      }
      }
      int l=strlen(c);
      char *s=c;
      for(int i=0;i<l;i++)
      {
	      if(c[i]!='<')
		      s++;
	      if(c[i]=='<')
	      {
		      c[i]=del;
		      s++;
		      break;
	      }
      }
      int q=0;
      while(s[q]==' ')
      {
	      s++;
	      q++;
      }
      int m=strlen(p);
      char *t=p;
      for(int i=0;i<m;i++)
      {
	      if(p[i]!='>')
		      t++;
	      if(p[i]=='>')
	      {
		      p[i]=del;
		      t++;
		      break;
	      }
      }
      q=0;
      while(t[q]==' ')
      {
	      t++;
	      q++;
      }
      tokenize(c,arg1);
      tokenize(p,arg2);
      executepipewithredirection(arg1,arg2,s,t);
}
void parser(char *c,char **arg,char **arg1,char **arg2)
{
   if(strchr(c,'|') && (strchr(c,'<') || strchr(c,'>')))
	  pipewithredirection(c,arg1,arg2);
   else if(strchr(c,'|'))
          pipes(c,arg1,arg2);
   else if(strchr(c,';'))
          semicolon(c,arg1,arg2);
   else if(strchr(c,'&'))
          successive(c,arg1,arg2);
   else if(strchr(c,'<') || strchr(c,'>'))
	   redirection(c,arg1);
   else
        simple(c,arg);

}
int main()
{
  char c[100];
  char *arg[20],*arg1[20],*arg2[20];
  while(1)
    {	  
      printf(1,"MyShell>");
      gets(c,sizeof(c));
      if(c[0]=='e' && c[1]=='x' && c[2]=='i' && c[3]=='t')
      {
            break;
      }
      parser(c,arg,arg1,arg2);
    }
  exit(0);
  return 0;
}
