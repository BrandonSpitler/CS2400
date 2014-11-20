void eval(char *cmdline) 
{
	char *argv[MAXARGS];

  // The 'bg' variable is TRUE if the job should run
  // in background mode or FALSE if it should run in FG
	int bg = parseline(cmdline, argv); 
	if (argv[0] == NULL)  
		return;   /* ignore empty lines */
    
    pid_t pid;	// process id
	sigset_t block;	// for blocking signals
    
	if(!builtin_cmd(argv)){
			//Page 757, section 8.5
			//Sigemptyset(&mask)
			//Sigaddset(&mask, SIGCHILD)
			//Sigprocmask(SIG_UNBLOCK, &mask, NULL)
			//on successful completion these functions return 0, otherwise they return an error
			/*
			 * 
			SIG_BLOCK
				The resulting set will be the union of the current set and the signal set pointed to by set. 
			SIG_SETMASK
				The resulting set will be the signal set pointed to by set. 
			SIG_UNBLOCK
				The resulting set will be the intersection of the current set and the complement of the signal set pointed to by set. 
			*/
			if(sigemptyset(&block) != 0){		//sigemptyset initializes the signal to 0 so nothing gets blocked
				unix_error("sigemptyset error");
			}
			if(sigaddset(&block, SIGCHILD) != 0){
				unix_error("sigaddset error");
			}
			if(sigprocmask(SIG_BLOCK, &block, NULL) != 0){
				unix_error("sigprocmask error");
			}
			
			//fork
			if((pid = fork()) < 0){
				unix_error("forking error");
			}
			
			else if(pid == 0){
				//unblock child process
				//NOTE: when unblocking use the argument SIG_UNBLOCK
				if(sigprocmask(SIG_UNBLOCK, &block, NULL) != 0){
					unix_error("sigprocmask error");
				}
				if(setpgid(0, 0) < 0){
					unix_error("setpgid error");
				}
				if(execvp(argv[0], argv) < 0){
					//printf command not found
					exit(1);
				}
				
			}
			else{
				if(bg){	//if bg is true that means job should run in background
					addjob(BG);
				}
				else{
					addjob(FG);
				}
				if(sigprocmask(SIG_UNBLOCK, &block, NULL) != 0){
					unix_error("sigprocmask error");
				}
				if(!bg){
					waitfg(pid);
				}
				else{
					//uhhhh
				}
				
			}
		
	}

}
