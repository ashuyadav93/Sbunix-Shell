#define _GNU_SOURCE
#include <libc.h>
#include <stdio.h>
#include <string.h>
#include <sys/defs.h>


#define MAX_LENGTH 1024
#define MAX_BUF_LENGTH 1024*32
#define MAX_CMD 32

struct envstruct{
	char varName[100];
	char varValue[MAX_BUF_LENGTH];
};

struct envstruct ENV[100];
int envCount = 0;

char PS1_DEFAULT[MAX_LENGTH] = "\\h@\\H-\\u:\\w: ";
char BINARY_PATH[MAX_LENGTH] = "";

void performExpansion(char* input);
void performOperation(char* input, char* envp[], int* fd, int previn);
void performWordExpansion(char* tempWord, char* tempExpandedWord);
void performCDOperation(char* commandArg);
void findEnvVal(char *envName, char val[]);
void findEnvp(char *envp[]);
void performExportOperation(char commandArg[MAX_CMD][MAX_LENGTH], int argVal);
void setEnvVal(char *envName, char val[]);
void parsePS1(char val[], char tempExpandedWord[MAX_LENGTH]);
void performpreOperation(char* input, char* envp[]);
int containsPipe(char* input);
void populateBinaryPath(char* argv);
void addBinaryPath(char* command);

char* strtok(char* s, char* d)
{
	if(d == NULL)
		return NULL;
	static int index;
	static char str[MAX_LENGTH];

	if(s != NULL)
	{
		strcpy(str, s);
		index = 0;
	}

	int startPos = index;
	int delFound = 0;
	for(; str[index] != '\0'; index++)
	{
		for(int i = 0; i < strlen(d); i++)
		{
			if(d[i] == str[index])
			{
				str[index] = '\0';
				delFound = 1;
				break;
			}
		}
		if(delFound)
		{
			if(str[startPos] != '\0')
			{
				index++;
				return (&(str[startPos]));
			}
			else
			{
				delFound = 0;
				startPos = index + 1;
			}
		}
	}
	if(str[startPos] != '\0')
		return (&(str[startPos]));
	else
		return NULL;
}

int main(int argc, char* argv[], char *envp[]) {
	char welcomeStr[] = "Hello: Welcome to our Shell\n";
	int ret = write(1, welcomeStr, strlen(welcomeStr));
	if(ret == -1) return -1;
	char **tempEnvp = envp;
	int i = 0;
	while(tempEnvp[i] != NULL)
	{
		char ch[MAX_BUF_LENGTH];
		int j = 0;
		while((tempEnvp[i][j] != '=') && (tempEnvp[i][j] != '\0')) {
			ch[j] = tempEnvp[i][j];
			j++;
		}
		ch[j] = '\0';
		//ch = strtok(tempEnvp[i], "=");
		if(ch != NULL)
		{
			strcpy(ENV[envCount].varName, ch);
			//ch = strtok(NULL, "\0");
			int k = 0;
			j++;
			while(tempEnvp[i][j] != '\0') {
				ch[k] = tempEnvp[i][j];
				k++;
				j++;
			}
			ch[k] = '\0';
			if(ch != NULL)
			{
				strcpy(ENV[envCount].varValue, ch);
			}
			envCount++;
		}
		i++;
	}

	populateBinaryPath(argv[0]);
	char input[MAX_LENGTH];
	int len = 0;
	char in;
	ssize_t inputread;
	char exit[5] = "exit";
	if(argc > 1)
	{
		//puts("Entering file mode");
		for(int i = 1; i < argc; i++)
		{
			char* filename = argv[i];
			int fd = open(filename, O_RDONLY);
			int j;
			do
			{
				char fileInput[MAX_LENGTH] = "";
				len = 0;
				for(j = fgetc(fd); j != '\n' && j != EOF; j = fgetc(fd))
				{
					fileInput[len++] = j;
				}
				if(len && fileInput[0] != '#')
				{
					fileInput[len] = '\0';

                	                //performExpansion(fileInput);
             		                //performOperation(fileInput, envp);
					performpreOperation(fileInput, envp);
                                        
				}
			}
			while(j != EOF);
			close(fd);
		}
	}
	else
	{
		while(1)
		{
			len = 0;
			char tempExpandedWord[MAX_LENGTH];
			parsePS1(PS1_DEFAULT, tempExpandedWord);
			ret = write(1, tempExpandedWord, strlen(tempExpandedWord));
			while((inputread = read(0, &in, 1)) >= 0)
			{
				if(in != '\n')
					input[len++] = in;
				else
					break;
			}
			input[len] = '\0';
			if(strcmp(exit, input) == 0)
			{
				return 0;
			}
			/*int fd = open(input, O_RDONLY);
			if(fd >= 3)
			{
				char err[] = "Error: Cannot execute a file. Please check\n";
				ret = write(1, err, strlen(err));
				close(fd);
			}
			else     
                        {
                		performExpansion(input);
             			performOperation(input, envp);*/
				performpreOperation(input, envp);
			//}
		}
	}
	return 0;
}

void findEnvVal(char *envName, char val[])
{
	if(strcmp(envName, "PS1") == 0)
	{
		strcpy(val, PS1_DEFAULT);
	}
	else
	{
		for(int i = 0; i < envCount; i++)
		{
			if(strcmp(ENV[i].varName, envName) == 0)
			{
				strcpy(val, ENV[i].varValue);
				break;
			}
		}
	}
}

void setEnvVal(char *envName, char val[])
{
	for(int i = 0; i < envCount; i++)
	{
		if(strcmp(ENV[i].varName, envName) == 0)
		{
			int j;
			for(j = 0; j < strlen(val); j++)
				ENV[i].varValue[j] = val[j];
			ENV[i].varValue[j] = '\0';
			break;
		}
	}
}

void catEnvp(char* envp[])
{
	for(int i = 0; i < envCount; i++)
	{
		char* var = ENV[i].varName;
		char* val = ENV[i].varValue;
		char env[MAX_BUF_LENGTH];
		strcpy(env,var);
		strcat(env, "=");
		strcat(env, val);
		for(int j = 0; j < strlen(env); j++)
			envp[i][j] = env[j];
	}
}

void performExpansion(char input[])
{
	char tempInput[MAX_LENGTH] = "";
	int strlength = strlen(input);
	int i = 0, inputLen = 0;
	while(i < strlength)
	{
		tempInput[i] = input[i];
		i++; 
	}
	tempInput[i] = '\0';
	input[0] = '\0';
	
	for(i = 0; i < strlength; i++)
	{
		char tempWord[MAX_LENGTH] = "";
		char tempExpandedWord[MAX_LENGTH] = "";
		int len = 0;
		while(i < strlength && tempInput[i] != '\0' && tempInput[i] != ' ')
		{
			tempWord[len++] = tempInput[i++];
		}
		tempWord[len] = '\0';
		performWordExpansion(tempWord, tempExpandedWord);
		int tempWordExpandedLength = strlen(tempExpandedWord);
		for(int j = 0; j < tempWordExpandedLength; j++, inputLen++)
		{
			input[inputLen] = tempExpandedWord[j];
		}
		input[inputLen++] = tempInput[i];
	}
	input[++inputLen] = '\0';
}

void performWordExpansion(char* tempWord, char* tempExpandedWord)
{
	char previousChar = '\0';
	int expandedWordLen = 0;
	int len = strlen(tempWord);
	for(int i = 0; i < len; i++)
	{
		switch(tempWord[i])
		{
			case '~':
					if(previousChar == '\0')
					{
						
						char path[MAX_BUF_LENGTH];
						findEnvVal("HOME", path);
						if(strlen(path))
						{
							int expandedLen = 0;
							while(expandedLen < strlen(path))
							{
                                                        	tempExpandedWord[expandedWordLen++] = path[expandedLen++];
							}
							tempExpandedWord[expandedWordLen++] = '/';
						}
                                                else
						{
							tempExpandedWord[expandedWordLen] = tempWord[i];
							previousChar = '~';
						}
					}
					else
					{
						tempExpandedWord[expandedWordLen] = tempWord[i];
						previousChar = '~';
					}
					break;
			case '$':
					if(previousChar == '/' || previousChar == '\0')
					{
						i++;
						char tempEnvWord[MAX_LENGTH];
						int tempEnvWordLen = 0;
						while((i < len) && tempWord[i] != '/' && tempWord[i] != ':' && tempWord[i] != '\0')
						{
							tempEnvWord[tempEnvWordLen++] = tempWord[i++]; 
						}
						tempEnvWord[tempEnvWordLen] = '\0';
						char path[MAX_BUF_LENGTH];
						findEnvVal(tempEnvWord, path);
						int expandedLen = 0;
						if(strlen(path))
						{
							while(expandedLen < strlen(path))
							{
								tempExpandedWord[expandedWordLen++] = path[expandedLen++];
							}
						}
						else
						{
							while(expandedLen < strlen(tempEnvWord))
							{
								tempExpandedWord[expandedWordLen++] = tempEnvWord[expandedLen++];
							}
						}
						tempExpandedWord[expandedWordLen++] = tempWord[i];
					}
					else
					{
						tempExpandedWord[expandedWordLen++] = tempWord[i];
						previousChar = tempWord[i];
					}
					break;
			default:
					tempExpandedWord[expandedWordLen++] = tempWord[i];
					previousChar = tempWord[i];
					break;
		}
	}
	tempExpandedWord[expandedWordLen] = '\0';
}

void performCDOperation(char* commandArg)
{
	int res = chdir(commandArg);
	if(res < 0)
	{
		char err[] = "Error in performing cd operation. Please check the path provided in cd.\n";
		int res = write(1, err, strlen(err));
		if(res == -1) return;
	}
	else
	{
		char val[MAX_LENGTH];
		findEnvVal("PWD", val);
		//puts("PWD:");
		//puts(val);
		setEnvVal("OLDPWD", val);
		getcwd(val, MAX_LENGTH);
		//puts("PWD:");
		//puts(val);	
		setEnvVal("PWD", val);
		//puts("PWD after update:");
	}
}

void parsePS1(char *val, char tempExpandedWord[MAX_LENGTH])
{
	char previousChar = '\0';
	int tempWordIndex = 0;
	int len = strlen(val);
	for(int i = 0; i < len; i++)
	{	if(val[i] != '\0')
		{
			switch(val[i])
			{
				case '\\':
						if(previousChar == '\\')
							tempExpandedWord[tempWordIndex++] = val[i];
						else
							previousChar = '\\';
						break;
				case 'h':
						if(previousChar == '\\')
						{
							int i = 0;
							char hostname[MAX_LENGTH];
							int fd = open("/etc/hostname", O_RDONLY);
							for(int j = fgetc(fd); j != '\n' && j != EOF; j = fgetc(fd))
							{
								hostname[i++] = j;	
							}
							hostname[i] = '\0';
							int len = strlen(hostname);
							for(i = 0; i < len && hostname[i] != '.'; i++)
								tempExpandedWord[tempWordIndex++] = hostname[i];
							previousChar = '\0';
							close(fd);
						}
						else
						{
							tempExpandedWord[tempWordIndex++] = val[i];
						}
						break;
				case 'H':
						if(previousChar == '\\')
						{
							int i = 0;
							char hostname[MAX_LENGTH];
							int fd = open("/etc/hostname", O_RDONLY);
							for(int j = fgetc(fd); j != '\n' && j != EOF; j = fgetc(fd))
							{
								hostname[i++] = j;	
							}
							hostname[i] = '\0';
							int len = strlen(hostname);
							for(i = 0; i < len; i++)
								tempExpandedWord[tempWordIndex++] = hostname[i];
							previousChar = '\0';
							close(fd);
						}
						else
						{
							tempExpandedWord[tempWordIndex++] = val[i];
						}
						break;
				case 'u':
						if(previousChar == '\\')
						{
							char user[MAX_LENGTH];
							findEnvVal("USER", user);
							int len = strlen(user);
							for(int i = 0; i < len; i++)
								tempExpandedWord[tempWordIndex++] = user[i];
							previousChar = '\0';
						}
						else
						{
							tempExpandedWord[tempWordIndex++] = val[i];
						}
						break;
				case 'w':
						if(previousChar == '\\')
						{	
							char buf[MAX_BUF_LENGTH];
							findEnvVal("PWD", buf);
							//setEnvVal("OLDPWD", val);
							//setEnvVal("PWD", commandArg);
							//findEnvVal("PWD", buf);
							if(buf != NULL)
							{
								for(int i = 0; i < strlen(buf); i++)
								{
									tempExpandedWord[tempWordIndex++] = buf[i];
								}
							}
							previousChar = '\0';
						}
						else
						{
							tempExpandedWord[tempWordIndex++] = val[i];
						}
						break;
				default:
						tempExpandedWord[tempWordIndex++] = val[i];
						break;
			}
		}
		else
			break;
	}
	tempExpandedWord[tempWordIndex] = '\0';
}

void performExportOperation(char commandArg[MAX_CMD][MAX_LENGTH], int argVal)
{
	if(argVal)
	{
		char* ch;
		ch = strtok(commandArg[argVal-1], "=\"");
		if(ch != NULL)
		{
			if(strcmp(ch, "PS1") == 0)
			{
				ch = strtok(NULL, "\"\0");
				if(ch != NULL)
				{
					//char tempExpandedWord[MAX_LENGTH];
					//parsePS1(ch, tempExpandedWord);
					//printf("String is: %s\n", tempExpandedWord);
					int i;
					for(i = 0; i < strlen(ch); i++)
						PS1_DEFAULT[i] = ch[i];
					PS1_DEFAULT[i] = '\0';
				}
			}
			else
			{
				char* varName = ch;
				char tempExpandedWord[MAX_LENGTH];
				ch = strtok(NULL, "\"\0");
				if(ch)
				{
					performWordExpansion(ch, tempExpandedWord);
					setEnvVal(varName, tempExpandedWord);
				}
			}
		}
	}
}

void performOperation(char* input, char* envp[], int * fd, int previn)
{       
	char command[1024] = "";
	char commandArg[MAX_CMD][MAX_LENGTH];
        int argVal = 0, testcount = 0, len = 0, i = 0;
	int strlength = strlen(input);
	int backgroundProcess = 0;
        //int w = write(1,input,strlen(input));
        //if(w);
	while((i < strlength) && (input[i] != '\0') && (input[i] != ' '))
	{
		command[len++] = input[i++];	
	}
	command[len] = '\0';
	while(i < strlength)
	{
		int length = 0;
		int present = 0;
		while((i < strlength) && (input[i] != '\0') && (input[i] != ' '))
		{
			commandArg[argVal][length++] = input[i++];
			present = 1;
		}
		if(present)
		{
			if(length == 1 && commandArg[argVal][0] == '&')
			{
				commandArg[argVal][0] = '\0'; //making the arg empty
				backgroundProcess = 1;
			}
			else
			{
				commandArg[argVal][length] = '\0';
				argVal++;
			}
		}
		else
			i++;
	}
	//check for built-in[cd, exit] or check binary[ls, cat]
	if(strcmp(command,"cd") == 0)
	{
		performCDOperation(commandArg[0]);
		return;
	}
	else if(strcmp(command, "export") == 0)
	{
		performExportOperation(commandArg, argVal);
		return;
	}
	if(argVal)
		testcount = argVal+2;
	else
		testcount = 3;

	//add binary path to command if not already present
	addBinaryPath(command);

	char* test[testcount];
	test[0] = command;
        //w = write(1,command,strlen(command));
	if(argVal)
	{
		for(int j = 1; j < testcount-1; j++)
			test[j] = commandArg[j-1];
	}
	else
		test[1] = NULL;
	test[testcount-1] = (char*) NULL;
        //printf("%s\n",test[0]);
        
	catEnvp(envp);
	//puts("Command is:");
	//puts(command);
	//puts("Binary Path:");
	//puts(test[0]);
	pid_t pid = fork();
	if(pid > 0) {
                if (previn != 0) {
                   close(previn);
                }
                if (fd[1] != 1) {
                   close(fd[1]);
                }
                int status;
                if(!backgroundProcess) //Parent process will not wait for child process in case of background process
		{  
                  // printf("%s\n",test[0]);
	           waitpid(pid,&status, 0);
		}     
	} else if(pid == 0) {
                 if (previn != 0) {
                    dup2(previn, 0);
                 }
                 if (fd[1] != 1) {
                    close(fd[0]);
                    dup2(fd[1],1);
                 }
		//if(backgroundProcess) to test background process
		//	sleep(10);
		int err = execve(test[0], test, envp);
		char errStr[] = "Error in running command\n";
		if(err == -1)
	        {
                   err = write(1, errStr,strlen(errStr));
                   exit(1);
                }
                exit(0);
	}
}


void performpreOperation(char* input, char* envp[]) { 
     
      // char* commands[MAX_LENGTH];
       int ispipe = 0;
       if(containsPipe(input)) {
          ispipe = 1;
       } 
       char *prevch = strtok(input, "|");       
       char *ch = NULL; 
      
       int fd[2];
       int previn = 0;
       while(prevch != NULL) {
            ch = strtok(NULL,"|");
	    if(ch == NULL) {
		 fd[1] = 1;
            } else {
	        int err = pipe(fd);
                if (err == -1) {
                    err = write(1, "Error in creating pipe\n", 25);
                }
            }
             //printf("%d\n", j);
             //printf("%s\n",prevch);
             while(*prevch == ' ') {
                 prevch++;
             }
             //printf("%s\n",prevch);
	     if(!ispipe) {
                performExpansion(prevch);
             }
             performOperation(prevch, envp, fd, previn);
             prevch = ch;
             previn = fd[0];
        }
}

int containsPipe(char* input) {
    for(int i=0;i<strlen(input);i++) {
       if(input[i] == '|') {
          return 1;
       }
    }
    
    return 0;       
}

void populateBinaryPath(char* argv)
{
	if(argv != NULL)
	{
		int len = strlen(argv);
		for(int i = len-1; i >= 0; i--)
		{
			if(i != len-1 && argv[i] == '/')
			{
				argv[i+1] = '\0';
				break;
			}
		}
		//puts("ARGV:");
		//puts(argv);
		if(argv[0] == '/')
		{
			strcpy(BINARY_PATH, argv);
			//puts("Hello");
			//puts(BINARY_PATH);
		}
		else
		{
			char path[MAX_LENGTH];
			findEnvVal("PWD", path);
			//puts("PATH");
			//puts(path);
			strcpy(BINARY_PATH, path);
			strcat(BINARY_PATH, "/");
			strcat(BINARY_PATH, argv);
			//puts("Changed Binary Path");
			//puts(BINARY_PATH);
		}
	}
}

void addBinaryPath(char* command) {

	if(command != NULL)
	{
		if(command[0] == '/')
			return;
		char bin_path[MAX_LENGTH];
		strcpy(bin_path, BINARY_PATH);
		strcat(bin_path, "/");
		strcat(bin_path, command);
		strcpy(command, bin_path);
		//puts("BINARY_PATH");
		//puts(BINARY_PATH);
		//puts("Command");
		//puts(command);
	}
/*        char* testSlash = command;
        int slashExists = 0;
	//find if forward slash exits in the input command
        while(*testSlash != '\0') {
                if(*testSlash == '/') {
                        slashExists = 1;
                        break;
                }
                testSlash++;
        }
	//if the input command does not contain forward slash add binary path fetched from env. variable "_"
        if(!slashExists) {
		char binpath[MAX_LENGTH];
		char pwd[MAX_LENGTH];
                
		findEnvVal("_", binpath);
		findEnvVal("PWD", pwd);
		if(binpath[0] == '.') {
			char* temp = binpath+1;
			strcpy(binpath, temp);
			strcat(pwd, binpath);
			strcpy(binpath, pwd);
		}

                int i = -1;
                for(i = strlen(binpath) - 1; binpath[i] != '/'; i--);
		if(i >= 0) {
                	binpath[i+1] = '\0';
                	strcat(binpath, command);
                	strcpy(command, binpath);
		}
        }*/
}
