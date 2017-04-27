#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <time.h>
#include <ncurses.h>

#define PORT "3490" //the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once
#define USERNAME 8
#define PASSWORD 8
#define MAX_ANSWER_TIME 10

//get sockaddr ,IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if(sa->sa_family ==AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

wrongRecv(ssize_t recvd, ssize_t expctd)
{

    if(recvd != expctd)
    {
        printf("Recvd(%zd) bytes not equal to expected(%zd) bytes\n",recvd,expctd);
        //getchar();
    }
}

void rtt_check(int sockfd)
{
    ssize_t send_ret, recv_ret;
    char rtt_check[1];
    recv_ret = recv(sockfd, rtt_check, 1,0);
    wrongRecv(recv_ret,1);
    sleep(1);//to check
    send_ret = send(sockfd, "r", 1, 0);
    wrongRecv(send_ret, 1);

    return;
}

int main(int argc, char *argv[])
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
    int parent_x, parent_y; 
	int score_size = 3;
	int log_size=15;
    srand(time(NULL));   // should only be called once 

    if(argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(rv));
        return 1;
    }

    //lopp through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
            perror("client: socket");
            continue;
        }

        if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if(p ==NULL) {
        fprintf(stderr,"client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    printf("client : connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    int totalQues=5;
    /*
    char login_det[17] = "abhishekabhishek";
    char login_retMsg[7], login_stat[3], totalQuesMsg[5];

    //sending login details
    ssize_t send_ret,recv_ret;
    send_ret = send(sockfd, login_det,16,0);
    wrongRecv(send_ret,16);

    //receiving login status
    recv_ret = recv(sockfd,login_retMsg,7,0);
    wrongRecv(recv_ret,7);

    strncpy(login_stat, login_retMsg, 2);
    login_stat[2] = '\0';
    printf("Login Status(%s)\n",login_stat);
    strncpy(totalQuesMsg, login_retMsg + 2, 5);
    totalQues = atoi(totalQuesMsg);
    printf("totalQues(%d)\n",totalQues);
	*/
    // fflush();
    char a[4]="";
    char* b=(char*)malloc(30*sizeof(char));
	ssize_t send_ret,recv_ret;
	sleep(1);
    recv_ret = recv(sockfd,a,3,0);
    a[4]='\0';
    printf("%s %ld\n",a,recv_ret);
	sleep(1);

	printf("Press 100 to register OR 101 to login in case you are have registered already\n");
    scanf("%s", b);
    if(strcmp(b, "100")==0){ //Register
    	int lou=0;
	    int lop=0;
	    int registerFlag1=0;
	    int registerFlag2=0;
	    char* login_det=(char*) malloc(sizeof(char)*(USERNAME+PASSWORD+2));
	    
	    while(registerFlag1==0 || registerFlag2==0){
	    	
	    	char* usernameDummy=(char*) malloc(sizeof(char)*30); 
	    	char* passwordDummy=(char*) malloc(sizeof(char)*30);
	    	int lou=0;
	    	int lop=0;

	        printf("Enter your username of max length %d\n", USERNAME);
	        scanf("%s",usernameDummy);
	        //printf("%c\n", usernameDummy[7]);
	        printf("Enter your password of max length %d\n", PASSWORD);
	        scanf("%s", passwordDummy);
	        for(int i=0;usernameDummy[i]!='\0';i++){
	            login_det[i]=usernameDummy[i];
	            lou++;
	        }
	        //printf("%d\n",lou);
	        if(lou>8){
	            printf("ERROR: Length of username can't be more than %d. Please re-register.\n", USERNAME);
	            registerFlag1=0;
	        }
	        else{
	            registerFlag1=1;
	            login_det[lou]='|';
	            //lou++;
	            //printf("%s\n", login_det);
	        }
			//printf("%c\n", login_det[lou]);
			lou++;
	        if(registerFlag1==1){
	            for(int i=0;passwordDummy[i]!='\0';i++){
	                login_det[i+lou]=passwordDummy[i];
	                //printf("%c\n", login_det[i+lou]);
	                lop++;
	            }
	            //printf("%s\n", login_det);
	            if(lop>8){
	                printf("ERROR: Length of password can't be more than %d. Please re-register.\n", PASSWORD);
	                registerFlag2=0;
	            }
	            else{
	                registerFlag2=1;
	                login_det[lou-1+lop+1]='\0';
	                //printf("%s\n", login_det);
	            }
	        }
	    }
	    //printf("%s\n", login_det);

	    strcpy(b+3, login_det);
	    //printf("%s\n", b);

	    //printf("hello\n");
	    send_ret = send(sockfd, b, 30, 0);
	    //wrongRecv(send_ret,18);

	    //receiving login status
	    recv_ret = recv(sockfd, a, 4, 0);
	    //printf("%s\n", login_retMsg);

	    //wrongRecv(recv_ret,7);

	    // strncpy(login_stat, login_retMsg, 2);
	    // login_stat[2] = '\0';
	    // printf("Login Status(%s)\n",login_stat);
	    // strncpy(totalQuesMsg, login_retMsg + 2, 1);
	    // totalQues = atoi(totalQuesMsg);
	    // printf("totalQues(%d)\n",totalQues);
	    // strncpy(idMsg, login_retMsg+3, 4);
	    // id=atoi(idMsg);
	    //printf("Returned reg. msg is %s\n",a);
	    //503 on success
	    if(*(a+3)){
		    printf("Registration Successful. Your unique ID is %c. Remember this for future purposes.\n", *(a+3) );
    	}
    	else{
    		printf("Unsuccessful registration. KIndly close the connection and register.\n");
    		close(sockfd);
    		return 0;
    	}
    }
    else{	//Login
    	//In case it's an existing player
		char* idFromUser=(char*)malloc(sizeof(char)*1);
		char* passwordFromUser=(char*)malloc(sizeof(char)*(PASSWORD+1));
		char* login_retMsg=(char*)malloc(sizeof(char)*30);
		//1 for ID and 8 for password, no NULL required 
		//char* msgToClientForLogin[1+PASSWORD];
		char msgToClientForLogin[1+PASSWORD];
		//char* login_retMsg=(char*)malloc(7*sizeof(char));


		printf("Enter you unique player ID\n");
		scanf("%s", idFromUser);
		printf("Enter your password\n");
		scanf("%s", passwordFromUser);

		// strncpy(msgToClientForLogin, idFromUser, 1);
		// //printf("%s\n", msgToClientForLogin);
		// strncpy(msgToClientForLogin+1, passwordFromUser, PASSWORD);
		// printf("C: Concatenation of id and passsword is %s\n", msgToClientForLogin);
		// printf("Login message from C to S: %s\n", msgToClientForLogin);
		strcpy(b+3, idFromUser);
		strcpy(b+4, passwordFromUser);
		//printf("%s\n",b);

		send(sockfd, b, strlen(b), 0);
		//wrongRecv(send_ret, 9);

		//receiving login status
		recv(sockfd, a, 4, 0);
		//wrongRecv(recv_ret, 7);

		if(a[0]=='5' && a[1]=='0' && a[2]=='4'){
			printf("Login Successful. Your unique id is %c. Remember this for future purposes.\n", a[3]);
		}
		else if(a[0]=='5' && a[1]=='0' && a[2]=='6'){
			printf("Login Unsuccessful. You entered wrong password.\n");
		}
		else{
			printf("Login Unsuccessful. You are not registered already. Close the connection and register.\n");
		}

		//Loop for wrong password.
		while(a[0]=='5' && a[1]=='0' && a[2]=='6'){
			printf("Enter you unique player ID\n");
			scanf("%s", idFromUser);
			printf("Enter your password\n");
			scanf("%s", passwordFromUser);

			// strncpy(msgToClientForLogin, idFromUser, 1);
			// //printf("%s\n", msgToClientForLogin);
			// strncpy(msgToClientForLogin+1, passwordFromUser, PASSWORD);
			// printf("C: Concatenation of id and passsword is %s\n", msgToClientForLogin);
			// printf("Login message from C to S: %s\n", msgToClientForLogin);
			strcpy(b+3, idFromUser);
			strcpy(b+4, passwordFromUser);
			printf("%s\n", b);

			send(sockfd, b, strlen(b), 0);
			//wrongRecv(send_ret, 9);

			//receiving login status
			recv(sockfd, a, 4, 0);
			//wrongRecv(recv_ret, 7);
			if(a[0]=='5' && a[1]=='0' && a[2]=='4'){
			printf("Login Successful. Your unique id is %c. Remember this for future purposes.\n", a[3]);
			}
			else if(a[0]=='5' && a[1]=='0' && a[2]=='6'){
				printf("Login Unsuccessful. You entered wrong password.\n");
			}
			else{
				printf("Login Unsuccessful. You are not registered already. Close the connection and register.\n");
			}
		}

    }



 //    send_ret = send(sockfd, b,strlen(b),0);
 //    printf("%ld\n", send_ret);
	// sleep(1);
 //    recv_ret = recv(sockfd,a,3,0);
 //    a[4]='\0';
 //    printf("%s %ld\n",a,recv_ret);
	// sleep(1);

	printf("YO\n");

    if(a[0]=='5' && a[1]=='0' && (a[2]=='3' || a[2]=='4')) {  //login ok
        //char quesId[5];
        //int maxQues_Len = 40, maxOpt_len = 10, maxQuesId_len = 5;//including '\0' this time
        // char quesMsg[400],hintMsg[255], scoreMsg[1];//score doesnot include \0
        // char ques[255], optA[30], optB[30], optC[30], optD[30];
        // char answer[10];
        char* quesId=(char*)malloc(5*sizeof(char));
        char* quesMsg=(char*)malloc(400*sizeof(char));
        char* hintMsg=(char*)malloc(255*sizeof(char));
        char* scoreMsg=(char*)malloc(1*sizeof(char));
        char* ques=(char*)malloc(255*sizeof(char));
        char* optA=(char*)malloc(30*sizeof(char));
        char* optB=(char*)malloc(30*sizeof(char));
        char* optC=(char*)malloc(30*sizeof(char));
        char* optD=(char*)malloc(30*sizeof(char));
        char* answer=(char*)malloc(10*sizeof(char));

        //=--------------CURSES CODE---------------------

        initscr(); 
        noecho(); 
		char* ans_c=malloc(10);
		curs_set(FALSE); 

		noecho();
	    cbreak();
	    //timeout(TICKRATE);
	    //keypad(stdscr, TRUE);

		// get our maximum window dimensions 
		getmaxyx(stdscr, parent_y, parent_x); 

		// set up initial windows 
		//WINDOW *field = newwin(parent_y - score_size, parent_x-log_size, 0, 0); 
		//WINDOW *score = newwin(score_size, parent_x, parent_y - score_size, 0);
		//WINDOW *logentry = newwin(parent_y - score_size, log_size, 0 , parent_x-log_size); 
        
        char* fieldx=(char*)malloc(500*sizeof(char));
        char logx[400]="15";

        // draw to our windows
		//mvwprintw(field, 0, 0, "MAIN WINDOW:"); 
		//mvwprintw(score, 0, 0, "SCORES:");
		//mvwprintw(logentry, 0, 0, "MESSAGE LOG:");
		move(0,0);
		printw("MAIN WINDOW:");
		move(20,0);
		printw("SCORES:");
		move(20,30);
		printw("MESSAGE LOG:");
		refresh();
        //-----------------R1--------------------------------------------------------------------
			

		while(totalQues--) 
		{
            //checking rtt
            rtt_check(sockfd);
            //receving question
            recv_ret = recv(sockfd, quesMsg, 400 ,0);
            wrongRecv(recv_ret,  400);

            strncpy(quesId,quesMsg,5);
            strncpy(ques, quesMsg + 05, 255);
         	strncpy(optA, quesMsg + 260, 30);
            strncpy(optB, quesMsg + 290, 30);
            strncpy(optC, quesMsg + 320, 30);
            strncpy(optD, quesMsg + 350, 30);
            
            if(recv_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }

	        strncpy(quesId,quesMsg,5);
            strncpy(ques, quesMsg + 05, 255);
            strncpy(optA, quesMsg + 260, 30);
            strncpy(optB, quesMsg + 290, 30);
            strncpy(optC, quesMsg + 320, 30);
            strncpy(optD, quesMsg + 350, 30);

            int a= strlen(ques);

            ques[a]='\0';
            ques[254]='\0';

            int b=strlen(optA);
            optA[b]='\0';
            optA[29]='\0';

            //printf("Q%s: %sA : %sB : %sC : %sD : %s\n", quesId, ques, optA, optB, optC, optD);
            move(1,0);
            printw("                                                                                                                                                                   ");
            move(2,0);
            printw("                                                                                                                                                                   ");
            move(3,0);
            printw("                                                                                                                                                                   ");
            move(4,0);
            printw("                                                                                                                                                                   ");
            move(5,0);
            printw("                                                                                                                                                                   ");
            refresh();
            move(1,0);
			printw("Q%s:",quesId);
			int i=0,j=0;
			while(ques[i]!='\0'){
				move(1,i+3);
				printw("%c",ques[i]);
				++i;
			}
			move(2,0);
			printw("A:");
			i=0;
			while(optA[i]!='\0'){
				move(2,i+3);
				printw("%c",optA[i]);
				++i;
			}
			move(3,0);
			printw("B:");
			i=0;
			while(optB[i]!='\0'){
				move(3,i+3);
				printw("%c",optB[i]);
				++i;
			}
			move(4,0);
			printw("C:");
			i=0;
			while(optC[i]!='\0'){
				move(4,i+3);
				printw("%c",optC[i]);
				++i;
			}
			move(5,0);
			printw("D:");
			i=0;
			while(optD[i]!='\0'){
				move(5,i+3);
				printw("%c",optD[i]);
				++i;
			}

			move(15,0);
			printw("ANSWER:");
			refresh();

			char* display;
            int time_start=time(NULL);
            int timeElapsed=0;
            // char ans;
            // if (poll (&mypoll, 1, MAX_ANSWER_TIME*1000)){
            // 	scanf(" %c",&ans);	
            // }
            
            // else{
            // 	ans='Z';
            // }

            while(timeElapsed<MAX_ANSWER_TIME)
            {
				char c;
				if( poll(&mypoll, 1, 1000) )
				{
					c=getch();
					if(c=='\n')
					{
						break;
					}
					
					/*if(c=='\n')
						break;*/

					int leng=strlen(ans_c);
					if(c==8 || c==127){
						move(15,8);
						printw("          ");
						refresh();
						//wrefresh(field);
						if(leng>0){
							display=(char*)malloc(leng-1);
							ans_c[leng-1]='\0';
							strcpy(display,ans_c);
						}
			    		
					}
			    	else{
			    		display=(char*)malloc(leng+1);
			    		strcpy(display,ans_c);
			    		display[leng] = c;
			    		display[leng+1] = '\0';
						strcpy(ans_c,display);
		    		}
		    	move(15,8);
		    	printw("%s",display);
		    	refresh();
		    	/*move(21,30);
		    	printw("%s",logx);
		    	refresh();*/
				//mvwprintw(field,15,0,display);
				//wrefresh(field);
				//mvwprintw(logentry, 2, 0, logx);
				//wrefresh(logentry);
				}
				int time_stop=time(NULL);
				timeElapsed=(int) difftime(time_stop,time_start);
				sprintf(logx,"%d",timeElapsed);
			}
		strcpy(ans_c,"");
		move(15,8);
		printw("          ");
		refresh();

		char ans;
		if(timeElapsed>=MAX_ANSWER_TIME) ans='Z';
		else
		{
			if(strlen(display)>0)
            ans=display[0];
        	else ans='Z';
		}
            
            strncpy(answer,quesId, 5);
         //    int r = rand()%4;
         //    if(r==0)
         //    answer[5] = 'A';
        	// if(r==1)
         //    answer[5] = 'B';
        	// if(r==2)
         //    answer[5] = 'C';
        	// if(r==3)
         //    answer[5] = 'D';
        	
        	answer[5]=ans;
            answer[6]='\0';
            answer[7]='\0';
            answer[8]='\0';
            answer[9]='\0';
            // sleep(1);

            //sending answer
            // printf("%s\n", answer);
            send_ret = send(sockfd, answer,10,0);
            wrongRecv(send_ret,10);
            if(send_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }
            //printf("%s\n","Answer Message Sent" );
            move(21,30);
            printw("                                                                                                                 ");
            refresh();
            //mvwprintw(logentry, 2, 0, "                       ");
			//wrefresh(logentry);
            strcpy(logx,"Checking");
            // printf("%s\n","Answer Message Sent" );
			move(21,30);
            printw("%s",logx);
            refresh();

            char scoMsg[80];
            recv_ret = recv(sockfd, scoMsg,  80 ,0);
            if(recv_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }
            //printf("Scores : %s\n",scoMsg);
            move(21,0);
            printw("      \n        \n      ");
            refresh();
            move(21,0);
            printw("%s",scoMsg);
            refresh();

            // if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
            //  perror("recv");
            //  exit(1);
            // }

            // buf[numbytes] = '\0';

            // printf("client: received '%s'\n",buf);
        }
		char msg[3];
        recv_ret = recv(sockfd, msg,  3 ,0);
        if(recv_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }
        if(msg[0]=='9' && msg[1]=='9' && msg[2]=='9')
        {
        	//printf("ROUND NOT CLEARED!BYE BYE!\n");
        	strcpy(logx,"ROUND NOT CLEARED!BYE BYE!\n");
			move(21,30);
			printw("%s",logx);
			refresh();
        	close(sockfd);
        	return 0;
        }
        else if(msg[0]=='9' && msg[1]=='9' && msg[2]=='8')
        {
       		//printf("CONGRATS!YOU MOVE TO SECOND ROUND!\n"); 	
        	strcpy(logx,"CONGRATS!YOU MOVE TO SECOND ROUND!\n");
        	move(21,30);
			printw("%s",logx);
			refresh();
        } 
        sleep(3);
        // endwin();
        //----------------------------------R2--------------------------------
        totalQues=5;
        while(totalQues--) 
		{
            //checking rtt
            rtt_check(sockfd);
            //receving question
            recv_ret = recv(sockfd, quesMsg,  400 ,0);
            wrongRecv(recv_ret,  400);
            if(recv_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }

	        strncpy(quesId,quesMsg,5);
            strncpy(ques, quesMsg + 05, 255);
            strncpy(optA, quesMsg + 260, 30);
            strncpy(optB, quesMsg + 290, 30);
            strncpy(optC, quesMsg + 320, 30);
            strncpy(optD, quesMsg + 350, 30);
            //printf("Q%s: %sA : %sB : %sC : %sD : %s\n", quesId, ques, optA, optB, optC, optD);
            int a= strlen(ques);

            ques[a]='\0';
            ques[254]='\0';

            int b=strlen(optA);
            optA[b]='\0';
            optA[29]='\0';

            //printf("Q%s: %sA : %sB : %sC : %sD : %s\n", quesId, ques, optA, optB, optC, optD);
            move(1,0);
            printw("                                                                                                                                                                   ");
            move(2,0);
            printw("                                                                                                                                                                   ");
            move(3,0);
            printw("                                                                                                                                                                   ");
            move(4,0);
            printw("                                                                                                                                                                   ");
            move(5,0);
            printw("                                                                                                                                                                   ");
            refresh();

            move(1,0);
			printw("Q%s:",quesId);
			int i=0,j=0;
			while(ques[i]!='\0'){
				move(1,i+3);
				printw("%c",ques[i]);
				++i;
			}
			move(2,0);
			printw("A:");
			i=0;
			while(optA[i]!='\0'){
				move(2,i+3);
				printw("%c",optA[i]);
				++i;
			}
			move(3,0);
			printw("B:");
			i=0;
			while(optB[i]!='\0'){
				move(3,i+3);
				printw("%c",optB[i]);
				++i;
			}
			move(4,0);
			printw("C:");
			i=0;
			while(optC[i]!='\0'){
				move(4,i+3);
				printw("%c",optC[i]);
				++i;
			}
			move(5,0);
			printw("D:");
			i=0;
			while(optD[i]!='\0'){
				move(5,i+3);
				printw("%c",optD[i]);
				++i;
			}
			
			move(15,0);
			printw("ANSWER:");


			refresh();




			char* display;
            int time_start=time(NULL);
            int timeElapsed=0;

            // printf("%s\n",quesMsg);
            int t1=time(NULL);
            while(timeElapsed<MAX_ANSWER_TIME)
            {
				char c;
				if( poll(&mypoll, 1, 1000) )
				{
					c=getch();
					if(c=='\n')
					{
						break;
					}
					
					/*if(c=='\n')
						break;*/

					int leng=strlen(ans_c);
					if(c==8 || c==127){
						move(15,8);
						printw("          ");
						refresh();
						//wrefresh(field);
						if(leng>0){
							display=(char*)malloc(leng-1);
							ans_c[leng-1]='\0';
							strcpy(display,ans_c);
						}
			    		
					}
			    	else{
			    		display=(char*)malloc(leng+1);
			    		strcpy(display,ans_c);
			    		display[leng] = c;
			    		display[leng+1] = '\0';
						strcpy(ans_c,display);
		    		}
		    	move(15,8);
		    	printw("%s",display);
		    	refresh();
		    	/*move(21,30);
		    	printw("%s",logx);
		    	refresh();*/
				//mvwprintw(field,15,0,display);
				//wrefresh(field);
				//mvwprintw(logentry, 2, 0, logx);
				//wrefresh(logentry);
				}
				int time_stop=time(NULL);
				timeElapsed=(int) difftime(time_stop,time_start);
				sprintf(logx,"%d",timeElapsed);
			}
		strcpy(ans_c,"");
		move(15,8);
		printw("          ");
		refresh();

		char ans;
		if(timeElapsed>=MAX_ANSWER_TIME) ans='Z';
		else
		{
			if(strlen(display)>0)
            ans=display[0];
        	else ans='Z';
		}
        int t2=time(NULL);
        int te=(int) difftime(t2,t1);
         //    int r = rand()%5;
         //    if(r==0)
         //    ans = 'A';
        	// if(r==1)
         //    ans = 'B';
        	// if(r==2)
         //    ans = 'C';
        	// if(r==3)
         //    ans = 'D';
        	// if(r==4)
        	// ans = 'H';
            if(ans=='H')
            {
                scoreMsg[0] = 'H';
                strncpy(answer,quesId, 5);
                answer[5] = scoreMsg[0];

                send_ret = send(sockfd, answer,6,0);
                // send(0,NULL,0,0);
                wrongRecv(send_ret,6);
                if(send_ret==0)
	            {
	            	printf("SERVER DISCONNECTED!\n");
	            	close(sockfd);
	            	return 0;
	            }
                //printf("%s\n","Hint Request Sent" );
	            move(21,30);
	            printw("                                                                                              ");
	            refresh();
	            strcpy(logx,"Hint Request Sent\n");
				move(21,30);
				printw("%s",logx);
				refresh();
                recv_ret = recv(sockfd, hintMsg,  80 ,0);
                if(recv_ret==0)
                {
                	close(sockfd);
                	return 0;
                }
                // printf("Hint : %s\n",hintMsg);
                //strcpy(logx,hintMsg);
				move(21,30);
	            printw("                                                                                              ");
	            refresh();
				int i=0;

				while(hintMsg[i]!='\0'){
					move(21,30+i);
					printw("%c",hintMsg[i]);
					++i;	
				}
				
				refresh();

				time_start=time(NULL);
				timeElapsed=0;
                while(timeElapsed<MAX_ANSWER_TIME-te)
            {
				char c;
				if( poll(&mypoll, 1, 1000) )
				{
					c=getch();
					if(c=='\n')
					{
						break;
					}
					
					/*if(c=='\n')
						break;*/

					int leng=strlen(ans_c);
					if(c==8 || c==127){
						move(15,8);
						printw("          ");
						refresh();
						//wrefresh(field);
						if(leng>0){
							display=(char*)malloc(leng-1);
							ans_c[leng-1]='\0';
							strcpy(display,ans_c);
						}
			    		
					}
			    	else{
			    		display=(char*)malloc(leng+1);
			    		strcpy(display,ans_c);
			    		display[leng] = c;
			    		display[leng+1] = '\0';
						strcpy(ans_c,display);
		    		}
		    	move(15,8);
		    	printw("%s",display);
		    	refresh();
		    	/*move(21,30);
		    	printw("%s",logx);
		    	refresh();*/
				//mvwprintw(field,15,0,display);
				//wrefresh(field);
				//mvwprintw(logentry, 2, 0, logx);
				//wrefresh(logentry);
				}
				int time_stop=time(NULL);
				timeElapsed=(int) difftime(time_stop,time_start);
				sprintf(logx,"%d",timeElapsed);
			}
		strcpy(ans_c,"");
		move(15,8);
		printw("          ");
		refresh();

		// char ans;
		if(timeElapsed>=MAX_ANSWER_TIME-te) ans='Z';
		else
		{
			if(strlen(display)>0)
            ans=display[0];
        	else ans='Z';
		}
          //       int r = rand()%4;
	         //    if(r==0)
	         //    ans = 'A';
	        	// if(r==1)
	         //    ans = 'B';
	        	// if(r==2)
	         //    ans = 'C';
	        	// if(r==3)
	         //    ans = 'D';
            }
            //choose answer
            // if(ans==1)
            // scoreMsg[0] = 'A';
            // else if(ans==2)
            // scoreMsg[0] = 'B';
            // else if(ans==3)
            // scoreMsg[0] = 'C';
            // else if(ans==4)
            // scoreMsg[0] = 'D';
            scoreMsg[0]=ans;
            
            strncpy(answer,quesId, 5);
            answer[5] = scoreMsg[0];
            answer[6]='\0';
            // sleep(1);

            //sending answer
            //printf("%s\n", answer);
            send_ret = send(sockfd, answer,7,0);
            if(send_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }
            // send(0,NULL,0,0);
            wrongRecv(send_ret,7);
            // printf("%s\n","Answer Message Sent" );

            move(21,30);
            printw("                                                                                                          ");
            refresh();
            //mvwprintw(logentry, 2, 0, "                       ");
			//wrefresh(logentry);
            strcpy(logx,"Checking");
            // printf("%s\n","Answer Message Sent" );
			move(21,30);
            printw("%s",logx);
            refresh();

            char scoMsg[80];
            recv_ret = recv(sockfd, scoMsg,  80 ,0);
            if(recv_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }
            // printf("Scores : %s\n",scoMsg);

            move(21,0);
            printw("      \n        \n      ");
            refresh();
            move(21,0);
            printw("%s",scoMsg);
            refresh();
            // if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
            //  perror("recv");
            //  exit(1);
            // }

            // buf[numbytes] = '\0';

            // printf("client: received '%s'\n",buf);
        }
		char msg2[3];
        recv_ret = recv(sockfd, msg2,  3 ,0);
        if(recv_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }
        if(msg[0]=='9' && msg[1]=='9' && msg[2]=='9')
        {
        	//printf("ROUND NOT CLEARED!BYE BYE!\n");
        	strcpy(logx,"ROUND NOT CLEARED!BYE BYE!\n");
			move(21,30);
			printw("%s",logx);
			refresh();
        	close(sockfd);
        	return 0;
        }
        else if(msg[0]=='9' && msg[1]=='9' && msg[2]=='8')
        {
       		//printf("CONGRATS!YOU MOVE TO SECOND ROUND!\n"); 	
        	strcpy(logx,"CONGRATS!YOU MOVE TO THIRD ROUND!\n");
        	move(21,30);
			printw("%s",logx);
			refresh();
        }        
        sleep(3);
        // endwin();
    //-------------------------r3-------------------------
        totalQues=5;
        while(totalQues--) 
		{
            //checking rtt
            rtt_check(sockfd);
            //receving question
            recv_ret = recv(sockfd, quesMsg,  400 ,0);
            if(recv_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }
            wrongRecv(recv_ret,  400);
            
            strncpy(quesId,quesMsg,5);
            strncpy(ques, quesMsg + 05, 255);
            strncpy(optA, quesMsg + 260, 30);
            strncpy(optB, quesMsg + 290, 30);
            strncpy(optC, quesMsg + 320, 30);
            strncpy(optD, quesMsg + 350, 30);
            // printf("Q%s: %sA : %sB : %sC : %sD : %s\n", quesId, ques, optA, optB, optC, optD);
            int a= strlen(ques);

            ques[a]='\0';
            ques[254]='\0';

            int b=strlen(optA);
            optA[b]='\0';
            optA[29]='\0';

            //printf("Q%s: %sA : %sB : %sC : %sD : %s\n", quesId, ques, optA, optB, optC, optD);
            move(1,0);
            printw("                                                                                                                                                                   ");
            move(2,0);
            printw("                                                                                                                                                                   ");
            move(3,0);
            printw("                                                                                                                                                                   ");
            move(4,0);
            printw("                                                                                                                                                                   ");
            move(5,0);
            printw("                                                                                                                                                                   ");
            refresh();

            move(1,0);
			printw("Q%s:",quesId);
			int i=0,j=0;
			while(ques[i]!='\0'){
				move(1,i+3);
				printw("%c",ques[i]);
				++i;
			}
			move(2,0);
			printw("A:");
			i=0;
			while(optA[i]!='\0'){
				move(2,i+3);
				printw("%c",optA[i]);
				++i;
			}
			move(3,0);
			printw("B:");
			i=0;
			while(optB[i]!='\0'){
				move(3,i+3);
				printw("%c",optB[i]);
				++i;
			}
			move(4,0);
			printw("C:");
			i=0;
			while(optC[i]!='\0'){
				move(4,i+3);
				printw("%c",optC[i]);
				++i;
			}
			move(5,0);
			printw("D:");
			i=0;
			while(optD[i]!='\0'){
				move(5,i+3);
				printw("%c",optD[i]);
				++i;
			}

			
			move(15,0);
			printw("ANSWER:");
			refresh();

			char* display;
            int time_start=time(NULL);
            int timeElapsed=0;
            // char ans;
            // if (poll (&mypoll, 1, MAX_ANSWER_TIME*1000)){
            // 	scanf(" %c",&ans);	
            // }
            
            // else{
            // 	ans='Z';
            // }

            while(timeElapsed<MAX_ANSWER_TIME)
            {
				char c;
				if( poll(&mypoll, 1, 1000) )
				{
					c=getch();
					if(c=='\n')
					{
						break;
					}
					
					/*if(c=='\n')
						break;*/

					int leng=strlen(ans_c);
					if(c==8 || c==127){
						move(15,8);
						printw("          ");
						refresh();
						//wrefresh(field);
						if(leng>0){
							display=(char*)malloc(leng-1);
							ans_c[leng-1]='\0';
							strcpy(display,ans_c);
						}
			    		
					}
			    	else{
			    		display=(char*)malloc(leng+1);
			    		strcpy(display,ans_c);
			    		display[leng] = c;
			    		display[leng+1] = '\0';
						strcpy(ans_c,display);
		    		}
		    	move(15,8);
		    	printw("%s",display);
		    	refresh();
		    	/*move(21,30);
		    	printw("%s",logx);
		    	refresh();*/
				//mvwprintw(field,15,0,display);
				//wrefresh(field);
				//mvwprintw(logentry, 2, 0, logx);
				//wrefresh(logentry);
				}
				int time_stop=time(NULL);
				timeElapsed=(int) difftime(time_stop,time_start);
				sprintf(logx,"%d",timeElapsed);
			}
		strcpy(ans_c,"");
		move(15,8);
		printw("          ");
		refresh();

		char ans[5];
		if(timeElapsed>=MAX_ANSWER_TIME) strcpy(ans,"Z");
		else
		{
			if(strlen(display)>3)
            {
            	ans[0]=display[0];
                ans[1]=display[1];
                ans[2]=display[2];
                ans[3]=display[3];
            }

        	else strcpy(ans,"Z");
		}
            // printf("%s\n",quesMsg);

            strncpy(answer,quesId, 5);
            answer[5] = ans[0];
            answer[6] = ans[1];
            answer[7] = ans[2];
            answer[8] = ans[3];
            answer[9]='\0';
            // sleep(1);

            //sending answer
            // printf("%s\n", answer);
            send_ret = send(sockfd, answer,10,0);
            // send(0,NULL,0,0);
            wrongRecv(send_ret,10);
            if(send_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }
            // printf("%s\n","Answer Message Sent" );
            move(21,30);
            printw("                                                                                                           ");
            refresh();
            //mvwprintw(logentry, 2, 0, "                       ");
			//wrefresh(logentry);
            strcpy(logx,"Checking");
            // printf("%s\n","Answer Message Sent" );
			move(21,30);
            printw("%s",logx);
            refresh();

            char scoMsg[80];
            recv_ret = recv(sockfd, scoMsg,  80 ,0);
            if(recv_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }
            // printf("Scores : %s\n",scoMsg);
            move(21,0);
            printw("      \n        \n      ");
            refresh();
            move(21,0);
            printw("%s",scoMsg);
            refresh();
            // if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
            //  perror("recv");
            //  exit(1);
            // }

            // buf[numbytes] = '\0';

            // printf("client: received '%s'\n",buf);
        }
		char msg3[3];
        recv_ret = recv(sockfd, msg3,  3 ,0);
        if(recv_ret==0)
            {
            	printf("SERVER DISCONNECTED!\n");
            	close(sockfd);
            	return 0;
            }
        if(msg[0]=='9' && msg[1]=='9' && msg[2]=='9')
        {
        	//printf("ROUND NOT CLEARED!BYE BYE!\n");
        	strcpy(logx,"ROUND OVER! YOU LOST !BYE BYE!\n");
			move(21,30);
			printw("%s",logx);
			refresh();
        	close(sockfd);
        	return 0;
        }
        else if(msg[0]=='9' && msg[1]=='9' && msg[2]=='8')
        {
       		//printf("CONGRATS!YOU MOVE TO SECOND ROUND!\n"); 	
        	strcpy(logx,"CONGRATS!YOU WIN!\n");
        	move(21,30);
			printw("%s",logx);
			refresh();
        } 
        sleep(3);
        endwin();    
    }
    //TODO wrong login
    close(sockfd);
    return 0;
}