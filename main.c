/*
 * Copyright (c) 2016 Simon Schmidt
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#define caseof(v,c) case v:c;break;

int ishelp(const char* str){
	int OK = 0;
	if(*str!='-') return 0;

	for(str++;*str;str++) switch(*str){
		caseof('?',OK=1)
		case '/':return 0;
	}
	return OK;
}

int main(int argc,const char* const * argv){
	const char* root;
	const char* flags="-";
	int i;
	const char* const initfinds[] = {
		"/init",
		"/sbin/init",
		(const char*)NULL
	};
	int usf = CLONE_NEWPID|CLONE_NEWUTS|CLONE_NEWIPC;
	switch(argc){
	case 2:
		root = argv[1];
		if(ishelp(root)) goto PRINT_HELP;
		break;
	case 3:
		flags = argv[1];
		root = argv[2];
		break;
	default:
		PRINT_HELP:
		fprintf(stderr,"Usage:\n"
			"\tc-microvisor `path'\n"
			"\tc-microvisor `flags' `path'\n"
			"Flags:\n"
			"\t-0\tNo namespace at all\n"
			"\t-P\tNo PID namespace\n"
			"\t-H\tNo UTS namespace (Hostname, NIS domain name)\n"
			"\t-I\tNo IPC namespace\n"
			"\t-m\tOwn mount namespace\n"
			"\t-u\tOwn user namespace\n"
			"\t-n\tOwn net namespace\n"
			"\t-c\tOwn cgroups namespace\n"

			"\t-?\tPrint this help\n"
		);
		return 1;
	}
	if(*flags=='-')flags++;
	while(*flags)switch(*(flags++)){
		caseof('?',goto PRINT_HELP)
		caseof('0',usf = 0)
		caseof('P',usf &= ~CLONE_NEWPID)
		caseof('H',usf &= ~CLONE_NEWUTS)
		caseof('I',usf &= ~CLONE_NEWIPC)
		caseof('m',usf |=  CLONE_NEWNS)
		caseof('u',usf |=  CLONE_NEWUSER)
		caseof('n',usf |=  CLONE_NEWNET)
#ifdef CLONE_NEWCGROUP
		caseof('c',usf |=  CLONE_NEWCGROUP)
#endif // CLONE_NEWCGROUP
	}
	if(usf) if(unshare(usf)<0){
		perror("unshare");
		return 1;
	}
	if(chroot(root)<0){
		perror("chroot");
		return 1;
	}
	chdir("/");
	for(i=0;initfinds[i];++i)
		if(execl(initfinds[i],initfinds[i],NULL)<0) perror(initfinds[i]);
	return 1;
}


