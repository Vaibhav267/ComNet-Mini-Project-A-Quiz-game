#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <poll.h>


#define PORT "3490" //the port user will be connecting to
#define BACKLOG 10  //how many pending connection queue will hold
#define maxUser 10
#define MAX_USERS 10
#define MAX_ANSWER_TIME 15
#define LOGIN_WAIT 60
#define TOTAL_QUES "5"
#define USERNAME 8
#define PASSWORD 8

int users[MAX_USERS][3] = {};  //index is userID, 0 is no user
char passwordDB[MAX_USERS][PASSWORD+1];

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

//get sockaddr, IPv4 or IPv6

int timer;
void alarm_handler(int s) {
    timer = 0;
}

wrongRecv(ssize_t recvd, ssize_t expctd)
{

    if(recvd != expctd)
    {
        printf("Recvd(%zd) bytes not equal to expected(%zd) bytes\n",recvd,expctd);
        perror("socket");
        //getchar();
    }
}
int add_duplicate_entry()
{
	return 1;
}
int check_username(){return 1;}
int check_password(){return 1;}
int check_credentials()
{
	return 0;
}

//void answerCheck(char* ques, char* optA, char* optB, char* optC, char* optD, char* usrResponse, int rtt, int timeTaken)
void answerCheck(int fd, char usrResponse[7], int rtt, int timeTaken,char realAnswer[5],int hintTaken)
{
    int responseTime, i;
    char quesId[5];
    printf("fd(%d) quesid(%s) response(%c) rtt(%d) timeTaken(%d)\n", fd, usrResponse, usrResponse[5], rtt, timeTaken );
    printf("ANSWER IS:%c\n",realAnswer[0] );
    strncpy(quesId, usrResponse, 5);
    if(usrResponse[5]=='Z')
    {
    	printf(":::USER ABSTAINED FROM ANSWERING:::\n");
    }
    else if(realAnswer[0] == usrResponse[5])
    {
    	printf(":::RIGHT ANSWER!:::\n");
        //printf("%s\n","+++++" );
        responseTime = timeTaken - rtt;
        //printf("Response Time(%d)\n",responseTime);
        //save it with user id

        //finding userid
        for(i = 0; i < MAX_USERS; i++) 
        {
            if(users[i][1] == fd) 
            {
            	if(hintTaken)
        		users[i][0] += 5;
        		else
        		users[i][0] +=10; 
                users[i][2] = responseTime;//saving it
                //printf("%d\n",i );
            }
        }
    }
    else
    {
    	printf(":::WRONG ANSWER!:::\n");
    	for(i = 0; i < MAX_USERS; i++) 
        {
            if(users[i][1] == fd) 
            {
        		users[i][0] -= 5; 
                users[i][2] = responseTime;//saving it
                //printf("%d\n",i );
            }
        }
    }
}

int answerCheck1(int fd, char usrResponse[7], int rtt, int timeTaken,char realAnswer[5])
{
    int responseTime, i, ret=0;
    char quesId[5];
    printf("fd(%d) quesid(%s) response(%c) rtt(%d) timeTaken(%d)\n", fd, usrResponse, usrResponse[5], rtt, timeTaken );
    printf("ANSWER IS:%c\n",realAnswer[0] );
    strncpy(quesId, usrResponse, 5);
    if(usrResponse[5]=='Z')
    {

    	printf(":::USER ABSTAINED FROM ANSWERING:::\n");
        ret = 3;
        for(i = 0; i < MAX_USERS; i++) 
        {
            if(users[i][1] == fd) 
            {
                users[i][2] = 999;//saving it
                //printf("%d\n",i );
            }
        }
    }
    else if(realAnswer[0] == usrResponse[5])
    {
    	printf(":::RIGHT ANSWER!:::\n");
        ret = 1;
        responseTime = timeTaken - rtt;
        //save it with user id

        //finding userid
        for(i = 0; i < MAX_USERS; i++) 
        {
            if(users[i][1] == fd) 
            {
                users[i][2] = responseTime;//saving it
                //printf("%d\n",i );
            }
        }
    }
    else
    {
    	printf(":::WRONG ANSWER!:::\n");
        ret = 2;
    	for(i = 0; i < MAX_USERS; i++) 
        {
            if(users[i][1] == fd) 
            {
        		users[i][0] -= 5; 
                users[i][2] = 999;//saving it
                //printf("%d\n",i );
            }
        }
    }
    return ret;
}

void answerCheck2(int fd, char usrResponse[10], int rtt, int timeTaken,char realAnswer[5])
{
    int responseTime, i;
    // char quesId[5];
    printf(":::%s\n",realAnswer);
    printf("fd(%d) quesid(%s) response(%c) rtt(%d) timeTaken(%d)\n", fd, usrResponse, usrResponse[5], rtt, timeTaken );
    printf("ANSWER IS:%c%c%c%c\n",realAnswer[0],realAnswer[1],realAnswer[2],realAnswer[3] );
    // strncpy(quesId, usrResponse, 5);
    if(usrResponse[5]=='Z')
    {
    	printf(":::USER ABSTAINED FROM ANSWERING:::\n");
    }
    else if(realAnswer[0] == usrResponse[5] && realAnswer[1] == usrResponse[6] && realAnswer[2] == usrResponse[7] && realAnswer[3] == usrResponse[8] )
    {
    	printf("RIGHT ANSWER!\n");
        //printf("%s\n","+++++" );
        responseTime = timeTaken - rtt;
        //printf("Response Time(%d)\n",responseTime);
        //save it with user id

        //finding userid
        for(i = 0; i < MAX_USERS; i++) 
        {
            if(users[i][1] == fd) 
            {
        		users[i][0] += 10; 
                users[i][2] = responseTime;//saving it
                //printf("%d\n",i );
            }
        }
    }
    else
    {
    	printf("WRONG ANSWER!\n");
    	for(i = 0; i < MAX_USERS; i++) 
        {
            if(users[i][1] == fd) 
            {
        		users[i][0] -= 5; 
                users[i][2] = responseTime;//saving it
                //printf("%d\n",i );
            }
        }
    }
}

int compareAnswer() {
    int i, min = 2 * MAX_ANSWER_TIME, userIndex;
    for(i = 0; i < MAX_USERS; i++) {
        if(users[i][2] < min) {
            min = users[i][2];
            userIndex = i;
        }
    }
    //Increasing Score
    users[userIndex][0]++;

    //returning fd
    return users[userIndex][1];
}

void users_deleteFd(int fd) {
    int i;
    for (i = 0; i < MAX_USERS; ++i)
    {
        if(users[i][1] == fd) {
            users[i][1] =0;
            return;
        }
    }
}

int rtt_check(int client_fd)
{
    ssize_t send_ret, recv_ret;
    char rtt_check[1];
    time_t rtt1, rtt2;
    // printf("h1\n");
    // recv_ret = recv(client_fd, rtt_check, 1,0);
    // if(recv_ret == 0)
    // {
    //     return -2;
    // }
    // printf("h2\n");
    rtt1 = time(NULL);
    send_ret = send(client_fd, "r", 1, 0);
    if(send_ret == 0)
    {
        return -2;
    }
    // printf("h3\n");
    wrongRecv(send_ret, 1);
    //printf("%s\n","Between two phase of rttCheck" );
    recv_ret = recv(client_fd, rtt_check, 1,0);
    rtt2 = time(NULL);
    if(recv_ret == 0)
    {
        return -2;
    }
    wrongRecv(recv_ret,1);
    // printf("h4\n");
    //printf("diff(%d)\n",(int) difftime(rtt2,rtt1));

    return  (int) difftime(rtt2,rtt1);
}

int login(char user[], char pass[])
{
    //for user
    static int Id = 0; //when have function getUserID, make it not static and also remove Id++;
    if(!strcmp(user,"abhishek") && !strcmp(pass,"abhishek")) {
        //Id = getUserID(user);
        return ++Id;
    }else if(!strcmp(user,"abhishek")){
        return 0; //wrong password
    }
    return -1; //wrong username
}

int totalQues;

int login_setup(int new_fd)
{
    //login inititalizations
    char login_det[16];
    char username[9],password[9], login_statMsg[7], totalQuesMsg[5] = TOTAL_QUES;
    totalQues = atoi(totalQuesMsg);
    //for user
    int userId;

    //for wrongRecv
    ssize_t send_ret,recv_ret;

    //getting username and password
    recv_ret = recv(new_fd,login_det,16,0);
    if(recv_ret == 0)
    {
        return -2;
    }
    wrongRecv(recv_ret,16);

    //extracting username nad password
    strncpy(username,login_det,8);  
    strncpy(password,login_det+8,8);
    username[8]='\0'; password[8]='\0';
    //printf("username(%s) and password(%s)\n",username,password);

    if( (userId = login(username,password)) > 0) {
        //printf("%d\n",userId);

        //sending status
        //strncpy(login_statMsg, LOGIN_OK, 2);
        strncpy(login_statMsg + 2, totalQuesMsg , 5);
        send_ret = send(new_fd, login_statMsg,7,0);
        if(send_ret == 0)
        {
            return -2;
        }
        wrongRecv(send_ret,7);

        //TODO error checking then handling if error

        //users[userId][0] = 0; //score
        users[userId][1] = new_fd; //file descriptor associated with this user
        //users[userId][2] = 0; //answer time
        return 1;
    }
    else if(userId == -1) { //wrong username
        //strncpy(login_statMsg, LOGIN_WrongUsername, 2);
        strncpy(login_statMsg + 2, totalQuesMsg , 5);
        send_ret = send(new_fd, login_statMsg,7,0);
        if(send_ret == 0)
        {
            return -2;
        }
        wrongRecv(send_ret,7);
        return 0;
    }
    else{
        //strncpy(login_statMsg, LOGIN_WrongPassword, 2);
        strncpy(login_statMsg + 2, totalQuesMsg , 5);
        send_ret = send(new_fd, login_statMsg,7,0);
        if(send_ret == 0)
        {
            return -2;
        }
        wrongRecv(send_ret,7);      
        return 0;
    }
    //TODO erorr handling of above two case
    //TODO make login a loop
}


void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int getIDInRegister(){
    static int i=0;
    return i++;
}

int actualLogin(char* buffer, int new_fd){
    //char* msgFromClientForLogin=(char*)malloc(sizeof(char)*30);
    char password[PASSWORD];
    char* id=(char*)malloc(sizeof(char)*4);

    strncpy(id, buffer+3, 1);
    strcpy(password, buffer+4);

    printf("%s\n", id);
    printf("%s\n", password);
    printf("%d\n", users[atoi(id)][1]);
    //char login_statMsg[7];

    if(users[atoi(id)][1]!=0 && strcmp(passwordDB[atoi(id)], password)==0){
        //Successful login
        char *msg=(char*)malloc(sizeof(char)*30);
        strcpy(msg, "504");
        strcpy(msg+3, id);
        send(new_fd, msg, strlen(msg), 0);
        printf("%s\n", msg);
        return 1;
    }
    else if (users[atoi(id)][1]==0){
        //User not registered
        char *msg=(char*)malloc(sizeof(char)*30);
        strcpy(msg, "505");
        //strcpy(msg+3, id);
        send(new_fd, msg, strlen(msg), 0);

        return 0;
    }
    else if(strcmp(passwordDB[atoi(id)], password)!=0){
        //User registered but wrong password
        char *msg=(char*)malloc(sizeof(char)*30);
        strcpy(msg, "506");
        strcpy(msg+3, id);
        send(new_fd, msg, strlen(msg), 0);

        return -1;
    }
    else{
        return -3;
    }
    return 0;
}

int main(void)
{
    int listen_fd;int  client_fd; // listen on sock_fd, new connection on client_fd
    struct addrinfo address,st_addr, *servinfo, *p;
    struct sockaddr_storage their_addr;//connection's address info
    socklen_t sin_size,addrlen;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;int i;int sd;
    char buffer[80];
    int timeElapsed;
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
    FILE* fpToUserDB=fopen("userDB.txt", "w");

    char questions[100][400];
    char hints[100][255];
    char answers[100][10];
    int num_questions=0;
    //LOAD QUESTIONS FROM FILE
    FILE* fp;
    fp=fopen("questions.txt","r");
	char ques[255],hin[255], optA[30], optB[30], optC[30], optD[30], quesId[5],optAnswer[5];
    memset(ques, '\0', sizeof(ques));
    memset(hin, '\0', sizeof(hin));
    memset(optA, '\0', sizeof(optA));
    memset(optB, '\0', sizeof(optB));
    memset(optC, '\0', sizeof(optC));
    memset(optD, '\0', sizeof(optD));
    memset(quesId, '\0', sizeof(quesId));
    memset(optAnswer, '\0', sizeof(optAnswer));
    while(fgets(ques,255,(FILE*)fp ) )
    {
	    memset(answers[num_questions], '\0', sizeof(answers[num_questions]));
	    memset(questions[num_questions], '\0', sizeof(questions[num_questions]));
	    memset(hints[num_questions], '\0', sizeof(hints[num_questions]));

        fgets(optA, 30, (FILE*)fp);    
        fgets(optB, 30, (FILE*)fp);    
        fgets(optC, 30, (FILE*)fp);    
        fgets(optD, 30, (FILE*)fp);
        fgets(hints[num_questions], 255, (FILE*)fp);
        fgets(answers[num_questions], 10, (FILE*)fp);

        sprintf(quesId,"%d",num_questions+1);    
        strncpy(questions[num_questions],quesId,5);
        strncpy(questions[num_questions] + 05,ques,255);
        strncpy(questions[num_questions] + 260,optA,30);
        strncpy(questions[num_questions] + 290,optB,30);
        strncpy(questions[num_questions] + 320,optC,30);
        strncpy(questions[num_questions] + 350,optD,30);

        //questions[num_questions]=quesMsg;
        num_questions++;
    }
    fclose(fp);
    //QUESTION LOADING END

    memset(&st_addr, 0, sizeof st_addr);
    st_addr.ai_family = AF_UNSPEC;//IPv4 or IPv6
    st_addr.ai_socktype = SOCK_STREAM;
    st_addr.ai_flags = AI_PASSIVE; // use my IP

    if((rv = getaddrinfo(NULL,PORT, &st_addr, &servinfo)) != 0)
    { //getting which IPv server supports
        fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(rv));
        return 1;
    }

    //loop through all the result and bind to the first we can
    for(p = servinfo; p != NULL; p  = p->ai_next)
    {
        if((listen_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("server : socket");
            continue;
        }

        if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
            perror("set sockopt");
            exit(1);
        }

        if(bind(listen_fd, p->ai_addr, p->ai_addrlen) == -1){
            close(listen_fd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if(p == NULL) {
        fprintf(stderr, "server:failed to bind\n");
        return 2;
    }

    freeaddrinfo(servinfo);//all done with this structure

    if(listen(listen_fd, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }
    //printf("listen_fd(%d)\n",listen_fd );

//  sa.sa_handler = sigchld_handler; // reap all dead processes
//  sigemptyset(&sa.sa_mask);
//  sa.sa_flags = SA_RESTART;
//  if(sigaction(SIGCHLD, &sa, NULL) == -1){
//      perror("sigaction");
//      exit(1);
//  }

    printf("Server Established Successfully!\nServer waiting for Connections:\n");

    int potential_player[MAX_USERS];
    for (i = 0; i < MAX_USERS; i++)  
    {  
        potential_player[i] = 0;  
    }  

    fd_set master_fds; //master file descriptor list
    fd_set read_fds; //temp file descriptor list
    int fdmax;
    
    FD_ZERO(&master_fds);
    FD_ZERO(&read_fds);

    FD_SET(listen_fd, &master_fds);

    fdmax = listen_fd;

    ssize_t recv_ret_size, send_ret_size;
    ssize_t time_start, time_stop;

    struct timeval tv;
    tv.tv_sec=LOGIN_WAIT;
    tv.tv_usec=0;
    time_start=time(NULL);

    printf("\n----Waiting for login for %d seconds.-----\n",LOGIN_WAIT);
    while(select(fdmax + 1, &master_fds , NULL , NULL, &tv) != 0)
    {
    	time_stop=time(NULL);       
        if (FD_ISSET(listen_fd, &master_fds))  
        {
		    sin_size = sizeof (their_addr);
	        if( (client_fd = accept(listen_fd, (struct sockaddr *)&their_addr, &sin_size)) < 0 )
	        {
	        	if(errno == EINTR)
	        	{
	        		perror("::::ACCEPT EINTR::::\n");
	        		continue;
	        	}
	        	else
	        		perror(":::::ACCEPT ERROR:::::\n");
	        }
	        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
	        printf("Server : Got connection from CLIENT: %s\n", s);
	        char* welcome=(char*)malloc(3*sizeof(char));
	        strcpy(welcome,"001");
	        if( (send_ret_size=send(client_fd, welcome, strlen(welcome) , 0)) != strlen(welcome))
	        {
	        	perror("SEND FAILED!");
	        }
	        
	        for (i = 0; i < MAX_USERS; i++)  
            {  
                if( potential_player[i] == 0 )  
                {  
                    potential_player[i] = client_fd;  
                    printf("POTENTIAL PLAYER:::Adding to list of sockets as %d:::::\n" , i);  
                    break;  
                }  
            }
    	}
    	else
    	{
    		for (i = 0; i < MAX_USERS; i++)  
        	{
        		int valread;
	            sd = potential_player[i];    
	            if (FD_ISSET( sd , &master_fds))  
	            {  
	                //Check if it was for closing , and also read the 
	                //incoming message 
	                if ((valread = read( sd , buffer, 80)) == 0)  
	                {  
	                    //Somebody disconnected , get his details and print 
	                    // getpeername(sd , (struct sockaddr*)&address , \
	                    //     (socklen_t*)&addrlen);  
	                    printf("Host disconnected , FD: %d\n" , sd);  
	                        
	                    //Close the socket and mark as 0 in list for reuse 
	                    close( sd );  
	                    potential_player[i] = 0;  
	                    // users[i][1]=0;
	                }  
	                    
	                //Echo back the message that came in 
	                else
	                {  
	                    //set the string terminating NULL byte on the end 
	                    //of the data read 
	                    // buffer[valread] = '\0';  
	                    // printf("%s\n", buffer);
	                    // if(buffer[0]=='1' && buffer[1]=='0' && buffer[2]=='0')
	                    // {
	                    // 	//REGISTER
	                    // 	// char* username=substr();
	                    // 	// char* password=substr();
	                    // 	int u=check_username();
	                    // 	int p=check_password();
	                    // 	if(u==0 || p==0)
	                    // 	{
	                    // 		char* msg="501";
	                    // 		send(sd ,msg , strlen(msg) , 0);//wrong credentials
	                    // 	}
	                    // 	else
	                    // 	{
	                    // 	int d=add_duplicate_entry();
		                   //  if(d==1)//duplicate entry
		                   //  	{
		                   //  		char* msg="502";
		                   //  		send(sd , msg , strlen(msg) , 0 );  
		                   //  	}
		                   //  else
			                  //   {
			                  //   	char* msg="503";//reg success
		                   //  		send(sd , msg , strlen(msg) , 0 );  	
			                  //   }
	                    // 	}

	                    //AUthor: Vaibhav Shrimal
	                	//printf("%d\n", i);
	                    //set the string terminating NULL byte on the end 
	                    //of the data read 
	                    buffer[valread] = '\0';  
	                    //printf("%s\n", buffer);
	                    if(buffer[0]=='1' && buffer[1]=='0' && buffer[2]=='0')
	                    {
	                    	//REGISTER
	                    	// char* username=substr();
	                    	// char* password=substr();
	                    	//int u=check_username();
	                    	//int p=check_password();
	                    	// if(u==0 || p==0)
	                    	// {
	                    	// 	char* msg="501";
	                    	// 	send(sd ,msg , strlen(msg) , 0);//wrong credentials
	                    	// }
	                    	// else
	                    	// {
	                    	// int d=add_duplicate_entry();
		                    // if(d==1)//duplicate entry
		                    // 	{
		                    // 		char* msg="502";
		                    // 		send(sd , msg , strlen(msg) , 0 );  
		                    // 	}
		                    // else
			                   //  {
			                   //  	char* msg="503";//reg success
		                    // 		send(sd , msg , strlen(msg) , 0 );  	
			                   //  }
	                    	// }
                            char* username=(char*)malloc(sizeof(char)*(USERNAME+1));
                            char* password=(char*)malloc(sizeof(char)*(PASSWORD+1));
                            int userID;
                            int q=0;
                            int j=0;
                            for(q=3;buffer[q]!='|';q++){
                                username[j]=buffer[q];
                                j++;
                                //printf("%c\n", username[i]);
                            }
                            username[j]='\0';
                            //printf("%c\n", username[i]);
                            //printf("%s\n", username);
                            j=0;
                            q++;
                            for(;buffer[q]!='\0';q++){
                                password[j]=buffer[q];
                                j++;
                            }
                            //printf("%d\n",j);
                            password[j]='\0';
                            //printf("%s\n", password);

                            //userID=getIDInRegister();
                            userID=i;

                            //Writing to the file to handle server crash
                            if(fputs(username, fpToUserDB )<0){
                                printf("ERROR: In writing into the file\n");
                            }
                            else{
                                fputs(",", fpToUserDB );
                            }
                            if(fputs(password, fpToUserDB )<0){
                                printf("ERROR: In writing into the file\n");
                            }
                            else{
                                fputs(",", fpToUserDB);
                            }
                            fprintf(fpToUserDB , "%d", userID);
                            fputs(",", fpToUserDB );
                            fputs("#", fpToUserDB );
                            fprintf(fpToUserDB , "\n");
                            fflush(fpToUserDB );

                            //printf("Written to the file\n");

                            //Storing the password in an array for Login purposes 
                            users[userID][1] = sd; //file descriptor associated with this user
                            strcpy(passwordDB[userID], password);
                            //printf("Password of %d is %s and entry in users is %d\n", userID, passwordDB[userID], users[userID][1]);

                            //Sending successful status message to client
                            char *msg=(char*)malloc(sizeof(char)*30);
                            strcpy(msg, "503");
                            sprintf(msg+3, "%d", userID);
                            printf("%s\n", msg);
                            send(sd, msg, strlen(msg), 0);

	                    	
	                    }
	                    else if(buffer[0]=='1' && buffer[1]=='0' && buffer[2]=='1')
	                    {
	                    	//LOGIN
	                    	// char* username=substr();
	                    	// char* password=substr();
	                    	// int u=check_credentials();
	                    	// if(u==0)
	                    	// {
	                    	// 	users[i][1] = sd;
	                    	// 	char* msg="504";
	                    	// 	send(sd ,msg , strlen(msg) , 0);//LOGIN SUCCESS
	                    	// 	printf("SUCCESS LOGIN\n");

	                    	// }
	                    	// else if(u==1)
	                    	// {
		                    // 	char* msg="505";
	                    	// 	send(sd ,msg , strlen(msg) , 0);//USERNAME NOT FOUND
	                    	// }
	                    	// else if(u==2)
	                    	// {
		                    // 	char* msg="506";
	                    	// 	send(sd ,msg , strlen(msg) , 0);//PASSWORD WRONG
	                    	// }

	                    	int userActualLoginStatus=actualLogin(buffer, sd);
                            //printf("hello\n");
                            //TODO FROM here
                            if(userActualLoginStatus==-3){
                                    printf("Unknown Error in actualLogin function\n");
                            }
                            else if(userActualLoginStatus==-2){
                                printf("Error in sending in function actualLogin\n");
                            }
                            else if(userActualLoginStatus==-1){
                                printf("User already registered but entered wrong password\n");
                            }
                            else if(userActualLoginStatus==0){
                                printf("User is not registered\n");
                                //Close the connection from client side.
                            }
                            else{
                                printf("User succesfully logged in\n");
                            }

                            while(userActualLoginStatus==-1){
                            	memset(buffer, '\0', sizeof(buffer));
                            	recv(sd, buffer, 80, 0);
                            	userActualLoginStatus=actualLogin(buffer, sd);
                                if(userActualLoginStatus==-3){
                                    printf("Unknown Error in actualLogin function\n");
                                }
                                else if(userActualLoginStatus==-2){
                                    printf("Error in sending in function actualLogin\n");
                                }
                                else if(userActualLoginStatus==-1){
                                    printf("User already registered but entered wrong password\n");
                                }
                                else if(userActualLoginStatus==0){
                                    printf("User is not registered\n");
                                    //Close the connection from client side.
                                }
                                else{
                                    printf("User succesfully logged in\n");
                                }
                                userActualLoginStatus=actualLogin(buffer, sd);
                            }

	                    }
	                    else
	                    {	//TO DO
	                    	//send(sd , buffer , strlen(buffer) , 0 );  
	                    }
	                }  //ELSE FD SET
	            }  //IF USER i FD SET
       		}  //FOR FOR USER PARSE
       	} //ELSE CLIENT SOCKET ACTIVITY

        //clear the socket set 
        FD_ZERO(&master_fds);  
    
        //add master socket to set 
        FD_SET(listen_fd, &master_fds);  
        fdmax = listen_fd;  
            
        //add child sockets to set 
        for ( i = 0 ; i < MAX_USERS ; i++)  
        {  
            //socket descriptor 
            sd = potential_player[i];  
                
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &master_fds);  
                
            //highest file descriptor number, need it for the select function 
            if(sd > fdmax)  
                fdmax = sd;  
        }  
        timeElapsed=(int) difftime(time_stop,time_start);
        tv.tv_sec=tv.tv_sec+1;
        printf("TIME LEFT: %ld %d\n",tv.tv_sec,timeElapsed );
        time_start=time(NULL);
    }
    printf("::::LOGIN TIME UP:CLOSING NOT REGISTERED CLIENTS:::\n");
    for (i = 0; i < MAX_USERS; ++i)
    {
    	if(potential_player[i]!=0)
    	{
    		if(users[i][1]==0)
    		{
    			printf("BYE %d\n", i);
    			close(potential_player[i]);
    			potential_player[i]=0;
    		}
    	}
    }
    printf("---Login Closed. Now starting the QUIZ.----\n");

    //for randome seek
    srand(time(NULL));

    //for main loop counter
    int win_fd;

    //for questions
    int QID = -1;
    int maxQues_Len = 40, maxOpt_len = 10, maxQuesId_len = 5;//including '\0' this time
    char quesMsg[80], answer[10];
    //char ques[40], optA[10], optB[10], optC[10], optD[10];

    //for time calculation of each answer
    ssize_t time_ques, time_ans;
    ssize_t time1, time2;

    //getting all available participants
    fdmax = 0;
    FD_ZERO(&master_fds);
    for(i = 0; i < MAX_USERS; i++) {
        if( (client_fd = users[i][1]) != 0){
            FD_SET(client_fd, &master_fds);
            if(client_fd > fdmax)
                fdmax = client_fd;
            //printf("%d\n",new_fd);
        }
    }
    printf("START\n");
    int current_rtt;
    //while for main quiz
    int round=0;
    totalQues=5;
    while(totalQues--) 
    {
        // printf("NU QUESTION\n");
        int yy;int flag=0;
        for(yy=0;yy<=fdmax;yy++)
        {
            if(FD_ISSET(yy, &master_fds))
            {
                flag=1;
                
            }
        }
        if(flag==0)
            {
                printf("NO CLIENTS!\n");
                close(listen_fd);
                return 0;
            }
        // printf("NU QUESTION2\n");
        //checking who are ready for witing
        if(select(fdmax+1, NULL, &master_fds, NULL, NULL) == -1){//here select will return withh all the descriptors which are 
                                                                //ready to write , all others have to miss this question
            perror("select");
            exit(1);
        }
        if(select(fdmax+1, NULL, &master_fds, NULL, NULL) == -1){//here select will return withh all the descriptors which are 
                                                                //ready to write , all others have to miss this question
            perror("select");
            exit(1);
        }
        // printf("NU QUESTION3\n");

        //setting which question to send
        QID++;

        //for sending questions to all
        for(i = 0; i <= fdmax; i++) {
            if(FD_ISSET(i, &master_fds)) {
                //rtt check
                printf("IS SET %d\n", i);
                current_rtt = rtt_check(i);
                if(current_rtt == -2) {//connection closed
                    printf("CLOSED\n");
                    FD_CLR(i, &master_fds);
                    users_deleteFd(i);
                    continue;
                }
                //setting question
                
                //nextQues(realAnswer,quesMsg, QID);
                char* send_ques=questions[QID];
                printf("Sending Question QID(%s) fd(%d)\n",send_ques,i);
                //send a question
                time_ques = time(NULL);
                //send_ret_size = send(i, send_ques, strlen(send_ques), 0);
                send_ret_size = send(i, send_ques, 400, 0);
                // printf("%s %ld\n", send_ques,send_ret_size);
                if(send_ret_size == 0) {//connection closed
                    FD_CLR(i, &master_fds);
                    users_deleteFd(i);
                    continue;
                }
                wrongRecv(send_ret_size, 400);  
            }
        }

        //ASSUMING Question is send to all the users at same time       
        //receiving and waiting for answers
        FD_ZERO(&read_fds);
        read_fds = master_fds;

	    tv.tv_sec=MAX_ANSWER_TIME;
	    tv.tv_usec=0;
	    time1=time(NULL);

	    int check=select(fdmax + 1, &read_fds , NULL , NULL, &tv);
	    printf("SELECT IS: %d\n", check);
	    int processedFD[MAX_USERS]={0};
	    int num_processedFD=-1;
        while(check != 0) 
        {
        	time2=time(NULL);
            for(i = 0; i <= fdmax; i++) 
            {
                //printf("Recving answer I(%d)\n",i);
                if(FD_ISSET(i, &read_fds)) 
                {
                    //receiving answer
                    //TODO if we get answer to wrong ques
                    printf("Recving answer I(%d) fdmax (%d)\n",i,fdmax);
					recv_ret_size = recv(i,answer,10,0);
                    time_ans = time(NULL);
                    wrongRecv(recv_ret_size,10);
                    printf("%s\n",answer );
                    if(recv_ret_size == 0)//connection closed
                    {
                        FD_CLR(i, &read_fds);
                        FD_CLR(i, &master_fds);
                        users_deleteFd(i);
                        printf("User %d dropped\n",i);
						continue;
                    }
                    else if(recv_ret_size > 0)
                    {
                    	char ans[6];int z;
                    	for(z=0;z<5;z++)
                    	{
                    		ans[z]=answer[z];	
                    	}
                        if(QID+1 == atoi(ans)) { //we have received the answer to this question so remove the user from wait answer loop
                            read_fds=master_fds;
                            num_processedFD++;
                            processedFD[num_processedFD]=i;
                            int kkk=0;
                            while(kkk<=num_processedFD)
                            {
                            	FD_CLR(processedFD[kkk],&read_fds);
                            	printf("CLEARED %d \n",processedFD[kkk]);
                            	kkk++;
                            }
                            //FD_CLR(i, &read_fds);
                            //printf("%s i(%d)\n","#######",i );
                            int hsl =answerCheck1(i ,answer, current_rtt, (int) difftime(time_ans,time_ques),answers[QID]);
                            //printf("Answer(%c)\n",answer[0]);
                        }
                        else
                        {
                        //we have recvd something unexpectable so ignore for NOW
                        	printf("ANSWER TO WRONG QUES. NO RECEIVED!\n");
                        	read_fds=master_fds;
                        }
                    }

                    //time_t cccc = time(NULL);
                    //printf("%s I(%d)\n",ctime(&cccc),i);
                }
            }
            timeElapsed=(int) difftime(time2,time1);
            tv.tv_sec=tv.tv_sec+1;
            printf("TIME LEFT: %ld %d\n",tv.tv_sec,timeElapsed );
            time1=time(NULL);
            check=select(fdmax + 1, &read_fds , NULL , NULL, &tv);
		    printf("SELECT IS: %d\n", check);
        }
        flag=0;
        for(yy=0;yy<=fdmax;yy++)
        {
            if(FD_ISSET(yy, &master_fds))
            {
                flag=1;
                break;
            }
        }
        if(flag==0)
            {
                printf("NO CLIENTS!\n");
                close(listen_fd);
                return 0;
            }
        //checking who are ready for witing
        if(select(fdmax+1, NULL, &master_fds, NULL, NULL) == -1){//here select will return withh all the descriptors which are 
                                                                //ready to write , all others have to miss this question
            perror("select");
            exit(1);
        }
        printf("CALCULATING SCORES!\n");
        int minResTime=900;
        int minUser=-1;
        for(i=0;i<MAX_USERS;i++)
        {
        	if(users[i][1]!=0 && users[i][2]!=999)
        	{
        		if(users[i][2]<minResTime)
        		{
        			minResTime=users[i][2];
        			minUser=i;
        		}
        	}
        }
        if(minUser!=-1)
      		{
		      	users[minUser][0]+=10;
      			printf("USER %d AWARDED POINTS\n" ,users[minUser][1]);
      		}
      	else
      		printf("NO USER ANSWERED CORRECTLY!\n");

        //setting question
        //nextQues(quesMsg, ques, optA, optB, optC, optD);
        printf(":::Sending Score for Q%d::: \n",QID+1);

        char scor[100];
        memset(scor, '\0', sizeof(scor));
        for(i = 0; i < MAX_USERS; i++) 
        {
            if(FD_ISSET(users[i][1],&master_fds))
			{
				char id_user[3];char score_user[4];
				sprintf(id_user,"%d",users[i][1]);
				strcat(scor,id_user);
				strcat(scor,":");
				sprintf(score_user,"%d",users[i][0]);
				strcat(scor,score_user);  
				strcat(scor,"\n");
		    }  
    	}
        //send a question
        for(i = 0; i <= fdmax; i++) 
        {
            if(FD_ISSET(i, &master_fds))
			{
				send_ret_size = send(i, scor, 80, 0);
		        if(send_ret_size == 0) 
		        {//connection closed
		        	FD_CLR(i, &master_fds);
		          	users_deleteFd(i);
		          	continue;
		        }
		        wrongRecv(send_ret_size, 80);
		    }  
    	}
        printf("HERE2!\n");
    }
    int uid[MAX_USERS];
    int cscore[MAX_USERS];
    int jj=0;
    for(i=0;i<MAX_USERS;i++)
    {
    	if(FD_ISSET(users[i][1],&master_fds))
    	{
    		uid[jj]=users[i][1];
    		users[i][0]=0;
    		users[i][2]=0;
    		cscore[jj]=users[i][0];
    		printf("%d %d\n",uid[jj],i);
    		jj++;
    	}
    }
    printf("%d\n",jj);
    for(i=0;i<jj-1;i++)
    {
    	int k;
    	for(k=i+1;k<jj;k++)
    	{
    		if(cscore[k]>cscore[i])
    		{
    			int temp=cscore[k];
    			cscore[k]=cscore[i];
    			cscore[i]=cscore[k];
    			temp=uid[k];
    			uid[k]=uid[i];
    			uid[i]=uid[k];
    		}
    	}
    }
    int to_drop=0;
    while(to_drop<(jj/2))
    {
	    for(i=0;i<MAX_USERS;i++)
	    {
	    	if(users[i][1]==uid[to_drop])
	    	{
	    		char* msg="999";
	    		send_ret_size = send(users[i][1],msg , strlen(msg), 0);
		        if(send_ret_size == 0) 
		        {//connection closed
		        	FD_CLR(i, &master_fds);
		          	users_deleteFd(i);
		          	continue;
		        }
	    		close(users[i][1]);
	    		FD_CLR(users[i][1], &master_fds);
	    		users[i][1]=0;
	    		users[i][0]=0;
	    		users[i][2]=0;
			    to_drop++;
			    break;
	    	}
	    }
    }
    for(i=0;i<=fdmax;i++)
    {
    	if(FD_ISSET(i, &master_fds))
			{
				char* msg="998";
				send_ret_size = send(i, msg, strlen(msg), 0);
		        if(send_ret_size == 0) 
		        {//connection closed
		        	FD_CLR(i, &master_fds);
		          	users_deleteFd(i);
		          	continue;
		        }
		        wrongRecv(send_ret_size, strlen(msg));
		    }  
    }

    //-------------------------------------------------R2----------------------------------------------------------------
    totalQues=5;
    while(totalQues--) 
    {
    	int yy;int flag=0;
    	int hintTaken[MAX_USERS+10]={0};
    	for(yy=0;yy<=fdmax;yy++)
    	{
    		if(FD_ISSET(yy, &master_fds))
    		{
    			flag=1;
    			// if( poll(&mypoll, 1, 100) )
       //          {
       //              char temp[1];
       //              recv(i,temp,1,0);
       //          }
    		}
    	}
    	if(flag==0)
    		{
    		printf("NO CLIENTS!\n");
    		close(listen_fd);
		    return 0;
    		}
        //checking who are ready for witing
        if(select(fdmax+1, NULL, &master_fds, NULL, NULL) == -1){//here select will return withh all the descriptors which are 
                                                                //ready to write , all others have to miss this question
            perror("select");
            exit(1);
        }

        //setting which question to send
        QID++;

        //for sending questions to all
        for(i = 0; i <= fdmax; i++) {
            if(FD_ISSET(i, &master_fds)) {
                //rtt check
                current_rtt = rtt_check(i);
                if(current_rtt == -2) {//connection closed
                    FD_CLR(i, &master_fds);
                    users_deleteFd(i);
                    continue;
                }
                //setting question
                
                //nextQues(realAnswer,quesMsg, QID);
                char* send_ques=questions[QID];
                printf("Sending Question QID(%s) fd(%d)\n",send_ques,i);
                //send a question
                time_ques = time(NULL);
                //send_ret_size = send(i, send_ques, strlen(send_ques), 0);
                send_ret_size = send(i, send_ques, 400, 0);
                if(send_ret_size == 0) {//connection closed
                    FD_CLR(i, &master_fds);
                    users_deleteFd(i);
                    continue;
                }
                wrongRecv(send_ret_size, 400);  
            }
        }

        //ASSUMING Question is send to all the users at same time       
        //receiving and waiting for answers
        FD_ZERO(&read_fds);
        read_fds = master_fds;

	    tv.tv_sec=MAX_ANSWER_TIME;
	    tv.tv_usec=0;
	    time1=time(NULL);

	    int processedFD[MAX_USERS]={0};
	    int num_processedFD=-1;
	    
        while(select(fdmax + 1, &read_fds , NULL , NULL, &tv) != 0) 
        {
        	time2=time(NULL);
            for(i = 0; i <= fdmax; i++) 
            {
                //printf("Recving answer I(%d)\n",i);
                if(FD_ISSET(i, &read_fds)) 
                {
                    //receiving answer
                    //TODO if we get answer to wrong ques
                    printf("Recving answer I(%d) fdmax (%d)\n",i,fdmax);
                    recv_ret_size = recv(i,answer,7,0);
                    time_ans = time(NULL);
                    wrongRecv(recv_ret_size,7);
                    printf("%s\n",answer );
                    if(recv_ret_size == 0)//connection closed
                    {
                        FD_CLR(i, &read_fds);
                        FD_CLR(i, &master_fds);
                        users_deleteFd(i);
                        printf("User %d dropped\n",i);
						continue;
                    }
                    else if(recv_ret_size > 0)
                    {
                    	char ans[6];int z;
                    	for(z=0;z<5;z++)
                    	{
                    		ans[z]=answer[z];
                    	}
                        if(QID+1 == atoi(ans)) 
                        { //we have received the answer to this question so remove the user from wait answer loop
                            read_fds=master_fds;
                            if(answer[5]=='H')
                            {
                            	hintTaken[i]=1;
                            	char* send_hint=hints[QID];
				                printf("Sending Hint \n");
				                //send_ret_size = send(i, send_hint, strlen(send_hint), 0);
				                send_ret_size = send(i, send_hint, strlen(send_hint), 0);
				                if(send_ret_size == 0) {//connection closed
				                    FD_CLR(i, &master_fds);
				                    users_deleteFd(i);
				                    continue;
				                }
				                wrongRecv(send_ret_size, strlen(send_hint));  
                            }
                            else
                            {
                            num_processedFD++;
                            processedFD[num_processedFD]=i;
                            int kkk=0;
                            while(kkk<=num_processedFD)
                            {
                            	FD_CLR(processedFD[kkk],&read_fds);
                            	printf("CLEARED %d \n",processedFD[kkk]);
                            	kkk++;
                            }
                            answerCheck(i ,answer, current_rtt, (int) difftime(time_ans,time_ques),answers[QID],hintTaken[i]);
                            }
                            //FD_CLR(i, &read_fds);
                            //printf("%s i(%d)\n","#######",i );
                            //printf("Answer(%c)\n",answer[0]);
                        }
                        else
                        {
                        //we have recvd something unexpectable so ignore for NOW
                        	printf("ANSWER TO WRONG QUES. NO RECEIVED!\n");
                        	read_fds=master_fds;
                        }
                    }

                    //time_t cccc = time(NULL);
                    //printf("%s I(%d)\n",ctime(&cccc),i);
                }
            }
            timeElapsed=(int) difftime(time2,time1);
            tv.tv_sec=tv.tv_sec+1;
            printf("TIME LEFT: %ld %d\n",tv.tv_sec,timeElapsed );
            time1=time(NULL);
        }
        // printf("HERE!\n");
        //setting question
        //nextQues(quesMsg, ques, optA, optB, optC, optD);
        printf("Sending Score::: QID(%s) fd(%d)\n",quesMsg,i);

        char scor[100];
        memset(scor, '\0', sizeof(scor));
        for(i = 0; i <MAX_USERS; i++) 
        {
            if(FD_ISSET(users[i][1],&master_fds))
			{
				char id_user[3];char score_user[4];
				sprintf(id_user,"%d",users[i][1]);
				strcat(scor,id_user);
				strcat(scor,":");
				sprintf(score_user,"%d",users[i][0]);
				strcat(scor,score_user);  
				strcat(scor,"\n");
		    }  
    	}
        //send a question
        for(i = 0; i <= fdmax; i++) 
        {
            if(FD_ISSET(i, &master_fds))
			{
				send_ret_size = send(i, scor, 80, 0);
		        if(send_ret_size == 0) 
		        {//connection closed
		        	FD_CLR(i, &master_fds);
		          	users_deleteFd(i);
		          	continue;
		        }
		        wrongRecv(send_ret_size, 80);
		    }  
    	}
    }
    int uid2[MAX_USERS];
    int cscore2[MAX_USERS];
    int jj2=0;
    for(i=0;i<MAX_USERS;i++)
    {
    	if(FD_ISSET(users[i][1],&master_fds))
    	{
    		uid2[jj2]=users[i][1];
    		users[i][0]=0;
    		users[i][2]=0;
    		cscore2[jj2]=users[i][0];
    		printf("%d %d\n",uid2[jj2],i);
    		jj2++;
    	}
    }
    printf("%d\n",jj2);
    for(i=0;i<jj2-1;i++)
    {
    	int k;
    	for(k=i+1;k<jj2;k++)
    	{
    		if(cscore2[k]>cscore2[i])
    		{
    			int temp=cscore2[k];
    			cscore2[k]=cscore2[i];
    			cscore2[i]=cscore2[k];
    			temp=uid2[k];
    			uid2[k]=uid2[i];
    			uid2[i]=uid2[k];
    		}
    	}
    }
    int to_drop2=0;
    while(to_drop2<(jj2/2))
    {
	    for(i=0;i<MAX_USERS;i++)
	    {
	    	if(users[i][1]==uid2[to_drop2])
	    	{
	    		char* msg="999";
	    		send_ret_size = send(users[i][1],msg , strlen(msg), 0);
		        if(send_ret_size == 0) 
		        {//connection closed
		        	FD_CLR(i, &master_fds);
		          	users_deleteFd(i);
		          	continue;
		        }
	    		close(users[i][1]);
	    		FD_CLR(users[i][1], &master_fds);
	    		users[i][1]=0;
	    		users[i][0]=0;
	    		users[i][2]=0;
			    to_drop2++;
			    break;
	    	}
	    }
    }
    for(i=0;i<=fdmax;i++)
    {
    	if(FD_ISSET(i, &master_fds))
			{
				char* msg="998";
				send_ret_size = send(i, msg, strlen(msg), 0);
		        if(send_ret_size == 0) 
		        {//connection closed
		        	FD_CLR(i, &master_fds);
		          	users_deleteFd(i);
		          	continue;
		        }
		        wrongRecv(send_ret_size, strlen(msg));
		    }  
    }  
    //---------------------------------------------R3------------------------------------------------------
    totalQues=5;
    while(totalQues--) 
    {
    	int yy;int flag=0;
    	for(yy=0;yy<=fdmax;yy++)
    	{
    		if(FD_ISSET(yy, &master_fds))
    		{
    			flag=1;
    			break;
    		}
    	}
    	if(flag==0)
    		{
    		printf("NO CLIENTS!\n");
    		close(listen_fd);
		    return 0;
    		}
        //checking who are ready for witing
        if(select(fdmax+1, NULL, &master_fds, NULL, NULL) == -1){//here select will return withh all the descriptors which are 
                                                                //ready to write , all others have to miss this question
            perror("select");
            exit(1);
        }

        //setting which question to send
        QID++;

        //for sending questions to all
        for(i = 0; i <= fdmax; i++) {
            if(FD_ISSET(i, &master_fds)) {
                //rtt check
                current_rtt = rtt_check(i);
                if(current_rtt == -2) {//connection closed
                    FD_CLR(i, &master_fds);
                    users_deleteFd(i);
                    continue;
                }
                //setting question
                
                //nextQues(realAnswer,quesMsg, QID);
                char* send_ques=questions[QID];
                printf("Sending Question QID(%s) fd(%d)\n",send_ques,i);
                //send a question
                time_ques = time(NULL);
                //send_ret_size = send(i, send_ques, strlen(send_ques), 0);
                send_ret_size = send(i, send_ques, 400, 0);
                if(send_ret_size == 0) {//connection closed
                    FD_CLR(i, &master_fds);
                    users_deleteFd(i);
                    continue;
                }
                wrongRecv(send_ret_size, 400);  
            }
        }

        //ASSUMING Question is send to all the users at same time       
        //receiving and waiting for answers
        FD_ZERO(&read_fds);
        read_fds = master_fds;

	    tv.tv_sec=MAX_ANSWER_TIME;
	    tv.tv_usec=0;
	    time1=time(NULL);

        while(select(fdmax + 1, &read_fds , NULL , NULL, &tv) != 0) 
        {
        	time2=time(NULL);
            for(i = 0; i <= fdmax; i++) 
            {
                //printf("Recving answer I(%d)\n",i);
                if(FD_ISSET(i, &read_fds)) 
                {
                    //receiving answer
                    //TODO if we get answer to wrong ques
                    printf("Recving answer I(%d) fdmax (%d)\n",i,fdmax);
                    recv_ret_size = recv(i,answer,10,0);
                    time_ans = time(NULL);
                    wrongRecv(recv_ret_size,10);
                    printf("%s\n",answer );
                    if(recv_ret_size == 0)//connection closed
                    {
                        FD_CLR(i, &read_fds);
                        FD_CLR(i, &master_fds);
                        users_deleteFd(i);
                        printf("User %d dropped\n",i);
						continue;
                    }
                    else if(recv_ret_size > 0)
                    {
                    	char ans[6];int z;
                    	for(z=0;z<5;z++)
                    	{
                    		ans[z]=answer[z];
                    	}
                        if(QID+1 == atoi(ans)) 
                        { //we have received the answer to this question so remove the user from wait answer loop
                            read_fds=master_fds;
                            answerCheck2(i ,answer, current_rtt, (int) difftime(time_ans,time_ques),answers[QID]);
                            //FD_CLR(i, &read_fds);
                            //printf("%s i(%d)\n","#######",i );
                            //printf("Answer(%c)\n",answer[0]);
                        }
                        else
                        {
                        //we have recvd something unexpectable so ignore for NOW
                        	printf("ANSWER TO WRONG QUES. NO RECEIVED!\n");
                        	read_fds=master_fds;
                        }
                    }

                    //time_t cccc = time(NULL);
                    //printf("%s I(%d)\n",ctime(&cccc),i);
                }
            }
            timeElapsed=(int) difftime(time2,time1);
            tv.tv_sec=tv.tv_sec+1;
            printf("TIME LEFT: %ld %d\n",tv.tv_sec,timeElapsed );
            time1=time(NULL);
        }
        printf("HERE!\n");
        //setting question
        //nextQues(quesMsg, ques, optA, optB, optC, optD);
        printf("Sending Score::: QID(%s) fd(%d)\n",quesMsg,i);

        char scor[100];
        memset(scor, '\0', sizeof(scor));
        for(i = 0; i < MAX_USERS; i++) 
        {
            if(FD_ISSET(users[i][1],&master_fds))
			{
				char id_user[3];char score_user[4];
				sprintf(id_user,"%d",users[i][1]);
				strcat(scor,id_user);
				strcat(scor,":");
				sprintf(score_user,"%d",users[i][0]);
				strcat(scor,score_user);  
				strcat(scor,"\n");
		    }  
    	}
        //send a question
        for(i = 0; i <= fdmax; i++) 
        {
            if(FD_ISSET(i, &master_fds))
			{
				send_ret_size = send(i, scor, 80, 0);
		        if(send_ret_size == 0) 
		        {//connection closed
		        	FD_CLR(i, &master_fds);
		          	users_deleteFd(i);
		          	continue;
		        }
		        wrongRecv(send_ret_size, 80);
		    }  
    	}
    }
    int uid3[MAX_USERS];
    int cscore3[MAX_USERS];
    int jj3=0;
    for(i=0;i<MAX_USERS;i++)
    {
    	if(FD_ISSET(users[i][1],&master_fds))
    	{
    		uid3[jj3]=users[i][1];
    		users[i][0]=0;
    		users[i][2]=0;
    		cscore3[jj3]=users[i][0];
    		printf("%d %d\n",uid3[jj3],i);
    		jj3++;
    	}
    }
    if(jj3==0)return 0;
    printf("%d\n",jj3);
    int maxscore=-10000;int maxuid;
    for(i=0;i<jj3;i++)
    {
    	if(cscore3[i]>maxscore)
    	{
    		maxscore=cscore3[i];
    		maxuid=uid3[i];
    	}
    }

    char* msg2="998";
	send_ret_size = send(maxuid, msg2, strlen(msg2), 0);
    wrongRecv(send_ret_size, strlen(msg2));
    FD_CLR(maxuid, &master_fds);
    close(maxuid);

    for(i=0;i<=fdmax;i++)
    {
    	if(FD_ISSET(i, &master_fds))
			{
				char* msg="999";
				send_ret_size = send(i, msg, strlen(msg), 0);
		        if(send_ret_size == 0) 
		        {//connection closed
		        	FD_CLR(i, &master_fds);
		          	users_deleteFd(i);
		          	continue;
		        }
		        wrongRecv(send_ret_size, strlen(msg));
		        close(i);
		    }  
    }
    close(listen_fd);
    return 0;
}