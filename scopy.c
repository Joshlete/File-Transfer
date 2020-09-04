#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <sys/stat.h>

int checkForUser(FILE *fp, char *realname);
void copyFile(char *fileToCopy, char *nameOfCopy);

 int main (int argc, char *argv[]) {
 	// check for command line arguments
 	if(argc != 3){
 		printf("Command line arguments not met. Try again.\n");
 		exit(0);
 	}
 	uid_t real = getuid();
 	uid_t euid = geteuid();
 	seteuid(real); // setting effective uid to the uid of real user

 	if(seteuid(real) != 0) { // check for seteuid return value
 		printf("seteuid() error");
 		return(-1);
 	}

 	// save command line arguments as strings
 	char *fileToCopy = argv[1];
 	// creating string for name of config file
 	char *fileConfig = calloc(strlen(argv[1]) + 5, strlen(argv[1])); // file we are looking for, to see if user can copy
 	strncpy(fileConfig, fileToCopy, strlen(fileToCopy));
 	strncat(fileConfig, ".acl", 4); // adding configuration file type to name
 	char *nameOfCopy = argv[2];
 	
 	// finding name of user running this file
 	struct passwd *uidname = getpwuid(real);
 	char *realname = uidname->pw_name;

 	// check if .acl file exists

 	seteuid(euid);
 	FILE* acl = fopen(fileConfig, "r");
 	seteuid(real);
 	if(acl){ /*do nothing*/}
 	else{
 		printf("Access configuration file not found\n");
 		exit(0);
 	}


 	//check if access configuration is a symbolic link
 	struct stat buf;
 	int status = lstat(fileConfig, &buf);
 	if(lstat(fileConfig, &buf) != 0) {
 		printf("lstat returned error. Exiting...\n");
 		exit(-1);
 	}
 	if(S_ISLNK(buf.st_mode)){
 		printf("symbolic link found. exiting....\n");
 		exit(-1);
 	}

 	//check configuration file if uid has access
 	seteuid(geteuid());
 	FILE* userfile = fopen(fileConfig, "r");
 	seteuid(getuid());
 	if(userfile) {
 		int isaUser = checkForUser(userfile, realname); // checkForUser returns 1 for true, 0 for false
 		if(isaUser) {
 			copyFile(fileToCopy, nameOfCopy); // copyFile copies 'fileToCopy' to directory user is in
 		} else {
 			printf("You do not have access!\n");
 		}
 	}else{
 		printf("file does not exist\n");
 		return 0;
 	}

 	fclose(acl);
 	fclose(userfile);
 	free(fileConfig); // fileConfig no longer needed, freeing up its allocation

 	return 0;
}


void copyFile(char* fileToCopy, char *nameOfCopy){
	FILE* fp = fopen(nameOfCopy, "w");
	if(fp == NULL) {
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *token; // assuming name wont be longer than 100 char

	seteuid(geteuid());
	FILE* toCopy = fopen(fileToCopy, "r");
	seteuid(getuid());

	if(fp == NULL) {
		printf("File to copy failed to open. Exiting...\n");
		exit(EXIT_FAILURE);
	}

	while ((read = getline(&line, &len, toCopy)) != -1) {
		fprintf(fp, "%s", line);
	}
	fclose(fp);
	fclose(toCopy);

	return;
}

int checkForUser(FILE *fp, char *realname){
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *token; // assuming name wont be longer than 100 char

	while ((read = getline(&line, &len, fp)) != -1) {
        token = strtok(line, " ");
        if(strcmp(token, realname) == 0) {
        	return 1;
        }
    }
    return 0;
}
