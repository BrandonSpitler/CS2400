// 
// tsh - A tiny shell program with job control
// 
// Annie Kelly
// anke2597
//

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

#include "globals.h"
#include "jobs.h"
#include "helper-routines.h"


static char prompt[] = "tsh> ";
int verbose = 0;


void eval(char *cmdline);		//ok
int builtin_cmd(char **argv); 	//ok
void do_bgfg(char **argv); 
void waitfg(pid_t pid);			//ok

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);


int main(int argc, char **argv) 
{
  int emit_prompt = 1; // emit prompt (default)

  //
  // Redirect stderr to stdout (so that driver will get all output
  // on the pipe connected to stdout)
  //
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
  // Install the signal handlers
  //

  //
  // These are the ones you will need to implement
  //
  Signal(SIGINT,  sigint_handler);   // ctrl-c
  Signal(SIGTSTP, sigtstp_handler);  // ctrl-z
  Signal(SIGCHLD, sigchld_handler);  // Terminated or stopped child

  //
  // This one provides a clean way to kill the shell
  //
  Signal(SIGQUIT, sigquit_handler); 

  //
  // Initialize the job list
  //
  initjobs(jobs);

  //
  // Execute the shell's read/eval loop
  //
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
  

void eval(char *cmdline) 
{
	char *argv[MAXARGS];
	char buf[MAXLINE];
	strcpy(buf, cmdline);
	int bg = parseline(buf, argv); 
	if (argv[0] == NULL)  
		return;   /* ignore empty lines */
    
    pid_t pid;	// process id

    
	if(!builtin_cmd(argv)){
		if((pid = fork()) == 0){
			if(execve(argv[0], argv, environ) < 0){						//error-handling wrapper, basically just doing pid = fork()
				printf("%s: Command not found.\n", argv[0]);
				exit(0);
			}
		}
		if(!bg){
			int status;
			if(waitpid(pid, &status, 0) < 0){
				unix_error("waitfg: waitpid error");
			}
		}
		else printf("%d %s", pid, cmdline);
	}
	return;
		

}


int builtin_cmd(char **argv) 
{
  string cmd(argv[0]);
  if(!cmd.compare("quit")){
	  exit(0);
  }
  else if(!cmd.compare("jobs")){
	  listjobs(jobs);
  }
  else if(!cmd.compare("bg")) return 0;     /* not a builtin command */
}

///////////////////////////////////////////////////
// do_bgfg - Execute the builtin bg and fg commands
//
void do_bgfg(char **argv) 
{
  struct job_t *jobp=NULL;
    
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
  if(kill() < 0){
	  //idk
  }

  return;
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
	//int jid = pid2jid(pid);
	if(pid != 0){
		kill(-pid, SIGINT);
		if(sig < 0){
			//printf(lalala);
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
	if(pid != 0){
		getjobpid(jobs, pid);
		kill(pid, SIGTSTP);
	}
  return;
}

/*********************
 * End signal handlers
 *********************/




