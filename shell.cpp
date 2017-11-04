#include <unistd.h>    
#include <sys/types.h>  
#include <sys/wait.h>   
#include <signal.h>     
#include <iostream>
#include <vector>
#include <string.h>
#include <errno.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <signal.h>

using namespace std;

static int signal_flag=0;

int checkparenbalance(char input[])
{
  int i=0,k,flag=0,flag1=0,inq=0,inq1=0;
  k=strlen(input);
  while(i<k)
  {
    if(input[i]=='\"' && inq==0)
    {
      inq=1;
      flag=1;
    }
    else if (input[i]=='\"' && inq==1)
    {
      inq=0;
      flag=0;
    }

    if(input[i]=='\'' && inq1==0)
    {
      inq1=1;
      flag1=1;
    }
    else if (input[i]=='\'' && inq1==1)
    {
      inq1=0;
      flag1=0;
    }

    
    i++;
  }

  if (flag==0 && flag1==0)
      return 1;
    else
      return 0;
}

void echonopipe(char input[])
{
 // check if parenthesis are balanced
  int i=0,k,sign=0;
  k=strlen(input);
  string var;
  char env[10];
  if(k>=6 && input[5]=='$')
  {
    //print env variable
    string in=input;
    var =in.substr(6,k);
    strncpy(env,var.c_str(),sizeof(env));
    env[sizeof(env)-1]=0;
    //cout<<env;
    if(getenv(env)!=NULL)
    cout<<getenv(env)<<endl;
    else
    cout<<endl;
  }
  else if(checkparenbalance(input)==1 && k>=4)
  {    
    i=4;
    while(i<k)
    {
      // /cout<<"here";
      if(input[i]=='\"' && sign==0)
        {
          //ignore "
          sign=1;
         }
         else if(input[i]=='\'' && sign ==0)
        {
          //ignore "
          sign=2;
         }
        i++;
    }
    i=4;
    if(k>=6)
    {
      i=5;
    }
    while(i<k)
    {
      if(sign==1)
      {
        if(input[i]!='\"')
          cout<<input[i];
      }
      else if (sign ==2)
      {
        if(input[i]!='\'')
          cout<<input[i];
      }
      else if (sign ==0)
        cout<<input[i];
      i++;
    }

    cout<<endl;
  }
  else
    cout<<"parenthesis not balanced\n";

}

int ifpipepresent(char input[])
{
  int i,k,insidequote,flag;
  insidequote=0;
  flag=0;
  i=0;
  k=strlen(input);
  while(i<k)
  {
    if ((input[i]=='\'' || input[i]=='\"' ) && insidequote==0)
    {
      insidequote=1;
    }

    else if ((input[i]=='\'' || input[i]=='\"' ) && insidequote==1)
    {
      insidequote=0;
    }

    if(input[i]=='|' && insidequote==0)
     flag=1;

   i++;
  }
  return flag;
}

int ifredirinpresent(char input[])
{
  int i,k,insidequote,flag;
  insidequote=0;
  flag=0;
  i=0;
  k=strlen(input);
  while(i<k)
  {
    if ((input[i]=='\'' || input[i]=='\"' ) && insidequote==0)
    {
      insidequote=1;
    }

    else if ((input[i]=='\'' || input[i]=='\"' ) && insidequote==1)
    {
      insidequote=0;
    }

    if(input[i]=='<' && insidequote==0)
     flag=1;

   i++;
  }
  return flag;
}

int ifrediroutpresent(char input[])
{
  int i,k,insidequote,flag;
  insidequote=0;
  flag=0;
  i=0;
  k=strlen(input);
  while(i<k)
  {
    if ((input[i]=='\'' || input[i]=='\"' ) && insidequote==0)
    {
      insidequote=1;
    }

    else if ((input[i]=='\'' || input[i]=='\"' ) && insidequote==1)
    {
      insidequote=0;
    }

    if(input[i]=='>' && insidequote==0)
     flag=1;

   i++;
  }
  return flag;
}
void exebuiltin(char input[])
{

  if((input[0]=='e' && input[1]=='c' && input[2]=='h' && input[3]=='o') && ifpipepresent(input) == 0 )
  {
    //cout<<"Herllo";
    echonopipe(input);
  }
}

void execom(char input[])
{
  //cout<<"here<<endl;"<<input<<endl;;
  vector<char*> args;
  char * prog = strtok(input, " " );
    char * tmp = prog;
    while ( tmp != NULL )
    {
      args.push_back(tmp);
      tmp = strtok( NULL, " " );
    }

    char** argv = new char*[args.size()+1];

    for ( int j = 0; j < args.size(); j++ )
      argv[j] = args[j];

    argv[args.size()] = NULL;

    if (strcmp( input,"exit") == 0 )
    {
      return ;
    }
    else
    {
      pid_t pid = fork();
      if ( pid < 0 )
      {
        cout<< "Fork error" ;
        return ;
      }
      else if ( pid == 0 )
      {
      // cout<<"here";
        execvp( prog, argv );
        return ;
      }
      else
      {
        if (waitpid( pid, 0, 0 ) < 0 )
        {
         cout<<"Error";
          return ;
        }
      }
    }
  }

  void computeredirout(char input[])
  {
    //cout<<"asdas";
    vector<char*> args;
    int i=0;
    while(input[i]!='>')
      i++;

    string str=input;

    string str1=str.substr(0,i);
    string str2=str.substr(i+1,strlen(input)-1);
    //cout<<str1<<" "<<str2;
   // string tmp = "cat";
    char tab2[1024];
    strncpy(tab2, str1.c_str(), sizeof(tab2));
    tab2[sizeof(tab2) - 1] = 0;

    char tab3[1024];
    strncpy(tab3, str2.c_str(), sizeof(tab3));
    tab3[sizeof(tab3) - 1] = 0;

    char * prog = strtok(tab2, " " );
    char * tmp = prog;
    while ( tmp != NULL )
    {
      args.push_back(tmp);
      tmp = strtok( NULL, " " );
    }

    char** argv = new char*[args.size()+1];

    for ( int k = 0; k < args.size(); k++ )
      argv[k] = args[k];
     // for ( int k = 0; k < args.size(); k++ )
     //  cout<<argv[k]<<endl;
     argv[args.size()] = NULL;
     int saved_stdout = dup(1); // save it to restore
     int out = open(tab3,O_WRONLY|O_CREAT,0666); 
    dup2(out,STDOUT_FILENO);
    close(out);
    pid_t pid = fork();
      if ( pid < 0 )
      {
        cout<< "Internal error: cannot fork." ;
        return ;
      }
      else if ( pid == 0 )
      {
        execvp( prog, argv );
        //change it back to stdout
        dup2(saved_stdout, 1);
        close(saved_stdout);
        return ;
      }
      else
      {
        if (waitpid( pid, 0, 0 ) < 0 )
        {
         cout<<"Error";
          return ;
        }
        dup2(saved_stdout, 1);
        close(saved_stdout);
      }
}


  void computeredirin(char input[])
  {
    //cout<<"asdas";
    vector<char*> args;
    int i=0;
    while(input[i]!='<')
      i++;

    string str=input;

    string str1=str.substr(0,i);
    string str2=str.substr(i+1,strlen(input)-1);
    //cout<<str1<<" "<<str2;
   // string tmp = "cat";
    char tab2[1024];
    strncpy(tab2, str1.c_str(), sizeof(tab2));
    tab2[sizeof(tab2) - 1] = 0;

    char tab3[1024];
    strncpy(tab3, str2.c_str(), sizeof(tab3));
    tab3[sizeof(tab3) - 1] = 0;

    char * prog = strtok(tab2, " " );
    char * tmp = prog;
    while ( tmp != NULL )
    {
      args.push_back(tmp);
      tmp = strtok( NULL, " " );
    }

    char** argv = new char*[args.size()+1];

    for ( int k = 0; k < args.size(); k++ )
      argv[k] = args[k];
     // for ( int k = 0; k < args.size(); k++ )
     //  cout<<argv[k]<<endl;
     argv[args.size()] = NULL;
     int saved_stdout = dup(1); // save it to restore
     int in = open(tab3,O_RDONLY); 
    dup2(in,STDIN_FILENO);
    close(in);
    pid_t pid = fork();
      if ( pid < 0 )
      {
        cout<< "Internal error: cannot fork." ;
        return ;
      }
      else if ( pid == 0 )
      {
        execvp( prog, argv );
        //change it back to stdout
        // dup2(saved_stdout, 1);
        // close(saved_stdout);
        return ;
      }
      else
      {
        if (waitpid( pid, 0, 0 ) < 0 )
        {
         cout<<"Error";
          return ;
        }
        // dup2(saved_stdout, 1);
        // close(saved_stdout);
      }
}
void execompipe(char input[])
{

  //cout<<"here:";
 
  vector<char*> args;
  char *c[100][100];
  int count=0;
  char * prog = strtok(input, "|" );
    char * tmp = prog;
    while ( tmp != NULL )
    {
      args.push_back(tmp);
      tmp = strtok( NULL, "|" );
      count++;
    }

    char** argv = new char*[args.size()+1];

    for (int k = 0;k < args.size();k++ )
    {
      
      argv[k] = args[k];
    }

   // cout<<args.size();

    argv[args.size()] = NULL;


    int k=0,j=0,cntr=0,i=0;

  char *com[100][100];

  for(i=0;i<count;i++)
  {

    k=0;j=0;
    char * prog = strtok(argv[i], " ");
    char * tmp = prog;
    while ( tmp != NULL )
    {
      int x=0;
      com[i][k]=tmp;
     // cout<<com[i][k]<<endl;
      tmp = strtok( NULL, " " );
      k++;
    }
    com[i][k]=NULL;
    //j++;
  }
  *com[i]=NULL;
 
 int num = count;


    int status;
     i = 0;
    pid_t pid;

    int pipefds[2*num];

    for(i = 0; i < (num); i++){
        if(pipe(pipefds + i*2) < 0) {
            perror("couldn't pipe");
            exit(EXIT_FAILURE);
        }
    }

    int cc=0;
    j = 0;
    while(cc<count) 
    {
        pid = fork();
        if(pid == 0) 
        {
            if(cc<(count-1))
            {
                if(dup2(pipefds[j + 1], 1) < 0){
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

  
            if(j != 0 )
            {
                if(dup2(pipefds[j-2], 0) < 0)
                {
                    perror(" dup2");

                }
            }


            for(i = 0; i < 2*num; i++)
            {
                    close(pipefds[i]);
            }

           execvp(com[cc][0], com[cc]);

        } 
        else if(pid < 0)
        {
            perror("error");
           
        }

        cc++;
        j+=2;
    }
  

    for(i = 0; i < 2 * num; i++)
    {
        close(pipefds[i]);
    }

    for(i = 0; i < num + 1; i++)
        wait(&status);
}



void export_builtin(char input[])
{
  char arg1[10];
  char arg2[10];
  int l=strlen(input);
  int i=7;
  int pos=0;int cntr=0;
  //cout<<"helloooo";
  if(l>=8)
  {
    while(input[i]!='=')
    {
      arg1[cntr]=input[i];
      cntr++;
      i++;
    }
    arg1[cntr]=0;
    //cout<<pos<<endl;
    string in=input;
    string var=in.substr(7,7);
    string val=in.substr(i+1,l);
    //cout<<"("<<var<<")";
    //strncpy(arg1,var.c_str(),sizeof(arg1));
    //arg1[sizeof(arg1)-1]=0;

    strncpy(arg2,val.c_str(),sizeof(arg2));
    arg2[sizeof(arg2)-1]=0;

    //cout<<arg1<<" "<<arg2;
    setenv(arg1,arg2,1);
  }
}

void history_builtin(char input[])
{
    string getcontent,s,s1,val;
    int range;
    char buf[10];

   // cout<<"Loo";
    if(strlen(input)>=9)
    {
      s1=input;
      val=s1.substr(8,strlen(input));
      strncpy(buf,val.c_str(),sizeof(buf));
      buf[sizeof(buf)-1]=0;
      range = atoi(buf);
     // cout<<range;


    int i=1;
    int count=0;
    ifstream openfile ("history.txt");
    if(openfile.is_open())
    {
        while(getline(openfile,getcontent))
        {
            
           // s=getcontent;
            //openfile >> getcontent;
            //if(s!=getcontent)
            //cout <<i++ << "  "<< getcontent<<endl;
            count++;
        }
    }

    ifstream openfile1 ("history.txt");
    if(openfile1.is_open())
    {
        while(getline(openfile1,getcontent))
        {
            
          if(i>(count-range))
            cout <<i<< "  "<< getcontent<<endl;

          i++;
          
        }
    }
    }
    else
    {
    int i=1;
    ifstream openfile1 ("history.txt");
    if(openfile1.is_open())
    {
        while(getline(openfile1,getcontent))
        {
            
           // s=getcontent;
            //openfile >> getcontent;
            //if(s!=getcontent)
            cout <<i++ << "  "<< getcontent<<endl;
            //count++;
        }
    }
  }


}

void cd(char input[])
{
 // cout<<"Hello";
  if (input[2]!=' ')
  {
    cout<<"Bad command";
  }
  else
  {
    if (strlen(input)==5 && input[3]=='.' && input[4]=='.')
    {
      // go back on directory above
      //cout<<"aksdh";
      chdir("..");
    }
    else 
    {
      // get the directory name
      char newdir[100];
      int i=3; int k=0;
      while(i<strlen(input))
      {
        newdir[k]=input[i];
        k++;
        i++;
      }
      newdir[k]='\0';

      // append it to the current directory
      string s1=getcwd(NULL, 0);
      s1.append("/");
      s1.append(newdir);
      //cout<<s1;
      char newdir2[100];
       strncpy(newdir2,s1.c_str(),sizeof(newdir2));
       newdir2[sizeof(newdir2)-1]=0;
      int r=chdir(newdir2);
      if(r==-1)
      {
        cout<<"Bad directory"<<endl;
      }

    }
  }
}

void bang (char input[])
{
  if(strlen(input)==2 and input[1] =='!')
  {
    // read the last command from the history file and execute.
    char prev[200];
    string getcontent,s2;
    ifstream openfile1 ("history.txt");
    if(openfile1.is_open())
    {
        while(getline(openfile1,getcontent))
        {       
          
            s2=getcontent;
        }
    }

   // cout<<s2<<endl;`

    strncpy(prev,s2.c_str(),sizeof(prev));
    prev[sizeof(prev)-1]=0;

    // execute the command
    //cout<<"***"<<prev<<"*****"<<endl;

    execom(prev);

    if(strcmp(prev,"history")==0)
      history_builtin(prev);
    else
      execom(prev);
  }

}

void handle_signal(int signum)
{

  
    char * user_name = getenv("USER");
    // /cout<<endl<<user_name<<getcwd(NULL, 0)<<"$ "<<flush; 
    cout<<endl<<"["<<user_name<<"]"<<getcwd(NULL, 0)<<"$ "<<flush; 
    signal_flag=1;
    fflush(stdout);

}

int main()
{
  char input[256],prev[256];
  
  while (1)
  {
    signal(SIGINT, handle_signal);
    char * user_name = getenv("USER");
    // if(signal_flag==0)
    // {
    cout<<"["<<user_name<<"]"<<getcwd(NULL, 0)<<"$ "<<flush;    
    // }
    signal_flag=0;
    cin.getline(input,256); 
    string getcontent,s2;
    ifstream openfile1 ("history.txt");
    if(openfile1.is_open())
    {
        while(getline(openfile1,getcontent))
        {       
          
            s2=getcontent;
        }
    }

   // cout<<s2<<endl;`

    strncpy(prev,s2.c_str(),sizeof(prev));
    prev[sizeof(prev)-1]=0;

    std::ofstream outfile;
    outfile.open("history.txt", std::ios_base::app);
    if(strcmp(prev,input)!=0 && input[0]!='!')
    outfile <<input<<endl; 

    string s=input;
    if (s=="exit")
    {
      cout<<"Bye.."<<endl;
      return 0;
    }
    if(ifrediroutpresent(input)==1 || ifredirinpresent(input)==1)
    {
      if(ifrediroutpresent(input)==1)
      computeredirout(input);
      else
      {
        //cout<<"aksdfhask"<<endl;
       computeredirin(input); 
      }
    }
    else
    {
    if((input[0]=='e' && input[1]=='c' &&input[2]=='h' && input[3]=='o') && ifpipepresent(input)==0)
    {
    exebuiltin(input);
    }
    else if((input[0]=='e' && input[1]=='x' &&input[2]=='p' && input[3]=='o' && input[4]=='r' && input[5]=='t') )
    {
    export_builtin(input);
    }

    else if((input[0]=='h' && input[1]=='i' &&input[2]=='s' && input[3]=='t' && input[4]=='o' && input[5]=='r' && input[6]=='y') )
    {
    history_builtin(input);
    }

    else if ((input[0]=='c') && (input[1]=='d'))
    {
      cd(input);
    }
    else if ((input[0]=='!'))

    {
      bang(input);
    }

    else
    {
      if(ifpipepresent(input))
      {
        execompipe(input);
      }
      else
        execom(input);
    }
  }
  }
  return 0;
} 