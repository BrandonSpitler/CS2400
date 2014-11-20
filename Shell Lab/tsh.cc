// 
// tsh - A tiny shell program with job control
// 
// Annie Kelly
// anke2597
// Jaden Pieper
// japi1549

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>
#include <iostream>

#include "globals.h"
#include "jobs.h"
#include "helper-routines.h"


static char prompt[] = "tsh> ";
int verbose = 0;


void eval(char *cmdline);		//ok
int builtin_cmd(char **argv); 	//ok
void do_bgfg(char **argv); 		//wtf
void waitfg(pid_t pid);			//ok

void sigchld_handler(int sig);  //ok
void sigtstp_handler(int sig);  //eh
void sigint_handler(int sig);	//eh


int main(int argc, char **argv) 
{
  int emit_prompt = 1; // emit prompt (default)


  dup2(1, 2);

  /* Parse the command line */
  char c;
  while ((c = getopt(argc, argv, "hvp")) != EOF) {
    switch (c) {
    case 'h':             // print help message
      usage();
      break;
    case 'v':             // emit additional diagnostic info
      verbose = 1;
      break;
    case 'p':             // don't print a prompt
      emit_prompt = 0;  // handy for automatic testing
      break;
    default:
      usage();
    }
  }

  //
  // These are the ones you will need to implement
  //
  Signal(SIGINT,  sigint_handler);   // ctrl-c
  Signal(SIGTSTP, sigtstp_handler);  // ctrl-z
  Signal(SIGCHLD, sigchld_handler);  // Terminated or stopped child

  Signal(SIGQUIT, sigquit_handler); 
  initjobs(jobs);


  for(;;) {
    //
    // Read command line
    //
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }

    char cmdline[MAXLINE];

    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) {
      app_error("fgets error");
    }
    //
    // End of file? (did user type ctrl-d?)
    //
    if (feof(stdin)) {
      fflush(stdout);
      exit(0);
    }

    //
    // Evaluate command line
    //
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
  } 

  exit(0); //control never reaches here
}
  

void eval(char *cmdline) //figure 8.23
{
	char *argv[MAXARGS];
	int bg = parseline(cmdline, argv); //takes command and puts it into argv
	if (argv[0] == NULL){ 
		return;   	/* ignore empty lines */
	}
    sigset_t mask;
    pid_t pid;	// process id
    

    
	if(!builtin_cmd(argv)){	//checks if builtin command, if it is one execute it immediately. otherwise fork a child process and execute there
		if(sigemptyset(&mask) != 0){
			unix_error("sigemptyset error");
		}
		if(sigaddset(&mask, SIGCHLD) != 0){ //adding children to the mask
			unix_error("sigaddset error");
		}
		if(sigprocmask(SIG_BLOCK, &mask, NULL) != 0){ //blocks the signals in the mask set
			unix_error("sigprocmask error");
		}
		if((pid = fork()) < 0){
			unix_error("forking error");
		}
		else if(pid == 0){	//fork returns 0 to child process and returns nonzero pid of child process to the parent
			//pid_t pgid = 0;
			//pgid = setpgid(pid, pgid);
			if(sigprocmask(SIG_UNBLOCK, &mask, NULL) != 0){ //unblocks the signals in the set mask
				unix_error("sigprocmask error");
			}
			//setpgid something
			if(setpgid(0,0) < 0){
				unix_error("setpgid error");
			}
			if(execvp(argv[0], argv) < 0){	//changed execv to execvp //error-handling wrapper, basically just doing pid = fork()
				printf("%s: Command not found.\n", argv[0]);
				exit(1);  //exit(1) implies we exited because of an error
			}
		}
		else{
			if(!bg){ //run in FG
				//int status;
				addjob(jobs, pid, FG, cmdline);
				//if(waitpid(pid, &status, 0) < 0){  //if child process terminates something something
				//	unix_error("waitfg: waitpid error");
				}
				//else printf("%d %s\n", pid, cmdline);

			else{ //run in BG
				addjob(jobs, pid, BG, cmdline);
			}
			if(sigprocmask(SIG_UNBLOCK, &mask, NULL) != 0){
				unix_error("sigprocmask error");
			}
			if(!bg){
				waitfg(pid);
			}
			else printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
		}
	}
	
	//return;
		

}


int builtin_cmd(char **argv) 
{
  string cmd(argv[0]);
  if(!cmd.compare("quit")){
	  exit(0);
  }
  else if(!cmd.compare("jobs")){
	  listjobs(jobs);
	  return 1;
  }
  else if(!cmd.compare("bg") || !cmd.compare("fg")){ //these are also builtin commands, use do_bgfg to execute
	  do_bgfg(argv);
	  return 1;
  }
  else if(!cmd.compare("&")){
	  return 1;
  }
	  
  	return 0;				     /* not a builtin command */

}

///////////////////////////////////////////////////
// do_bgfg - Execute the builtin bg and fg commands
//
void do_bgfg(char **argv) 
{
  struct job_t *jobp = NULL;
    
  /* Ignore command if no argument */
  if (argv[1] == NULL) {
    printf("%s command requires PID or %%jobid argument\n", argv[0]);
    return;
  }
    
  /* Parse the required PID or %JID arg */
  if (isdigit(argv[1][0])) {
    pid_t pid = atoi(argv[1]);
    if (!(jobp = getjobpid(jobs, pid))) {
      printf("(%d): No such process\n", pid);
      return;
    }
  }
  else if (argv[1][0] == '%') {
    int jid = atoi(&argv[1][1]);
    if (!(jobp = getjobjid(jobs, jid))) {
      printf("%s: No such job\n", argv[1]);
      return;
    }
  }	    
  else {
    printf("%s: argument must be a PID or %%jobid\n", argv[0]);
    return;
  }

  //
  // You need to complete rest. At this point,
  // the variable 'jobp' is the job pointer
  // for the job ID specified as an argument.
  //
  // Your actions will depend on the specified command
  // so we've converted argv[0] to a string (cmd) for
  // your benefit.
  //
 string cmd(argv[0]);


  	if(kill(-(jobp->pid), SIGCONT) < 0) {
		if(errno != ESRCH){
			unix_error("kill error");
		}
	}

// To determine the bg and fg
	if(!cmd.compare("fg")) {
		jobp->state = FG;
		waitfg(jobp->pid); //block until process ID is no longer the fg process
	}
	else if(!cmd.compare("bg")) {
		printf("[%d] (%d) %s", jobp->jid, jobp->pid, jobp->cmdline);
		jobp->state = BG;
	}
	else {
		printf("bgfg error: %s\n", argv[0]);
	}

}


void waitfg(pid_t pid)
{
	while(pid == fgpid(jobs)){
		sleep(0);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
//     a child job terminates (becomes a zombie), or stops because it
//     received a SIGSTOP or SIGTSTP signal. The handler reaps all
//     available zombie children, but doesn't wait for any other
//     currently running children to terminate.  
//
void sigchld_handler(int sig) 
{
	// running while loop checking pid's prolly > 0, set pid using waitpid, check that pid, check different exit status
	pid_t pid;
	// = fgpid(jobs);
	int status;

	
	while((pid = waitpid(fgpid(jobs), &status, WNOHANG|WUNTRACED)) > 0){
	//while((pid = waitpid(-1,&status,WNOHANG|WUNTRACED)) > 0){
		if(WIFSTOPPED(status)){
			sigtstp_handler(20); //by doing this the sigstp handler will print out that job was terminated by this signal. don't have to
								 //"set a state with a job ID" the handler will take care of it and set the state to ST
								 //signal 20 is the sigstop signal (CTRL-Z)
		}
		else if(WIFSIGNALED(status)){ //returns true if child terminated by signal that was not caught, (CTRL-C)
			sigint_handler(-2);
		}
		else if(WIFEXITED(status)){ //child terminated normally
			deletejob(jobs, pid);
		}
	}
	
	if (errno != ECHILD) {
		unix_error("waitpid error");
	} 

  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigint_handler - The kernel sends a SIGINT to the shell whenver the
//    user types ctrl-c at the keyboard.  Catch it and send it along
//    to the foreground job.  
//
void sigint_handler(int sig) 
{

	int pid = fgpid(jobs);
	int jid = pid2jid(pid);
	if(pid != 0){
	    /*
		if (kill(-pid, SIGINT) < 0) { //should only get errno = 3 if pid == -1, and -pid should be < -1.
			deletejob(jobs, pid); 
			printf("Job [%d] (%d) terminated by signal %d\n", jid, pid, (-sig)); //shouldnt be getting an error, just print out the status
		}
		*/
		//(kill(-pid, SIGINT) < 0) only when there is an error
		//if sig == 0 then the signal was NOT sent to every process, if <0 then it was
		
		
		kill(-pid, SIGINT); //-pid means the signal gets sent to every process with same pid
		if(sig < 0){ //because a SIGINT has a signal of -2, if sig<0 signal sent to whole process group
			printf("Job [%d] (%d) terminated by signal %d\n", jid, pid, (-sig));
			deletejob(jobs, pid);
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
//     the user types ctrl-z at the keyboard. Catch it and suspend the
//     foreground job by sending it a SIGTSTP.  
//
void sigtstp_handler(int sig) 
{

  
	int pid = fgpid(jobs);
	int jid = pid2jid(pid);
	if(pid==0){
		printf("no foreground job found\n");
		return;
    }

    if(pid != 0){
		printf("Job [%d] (%d) Stopped by signal %d\n", jid, pid, sig);
		getjobpid(jobs, pid)->state = ST; //stop the job
		kill(-pid, SIGTSTP); //sends the sig stop signal to the group
		
    }

    return;
}

/*********************
 * End signal handlers
 *********************/




