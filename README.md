# File Transfer
 Secure file sharing system

The overall structure of this scopy program goes as such:
	1. The program checks for proper commandline arguments
	2. Next it finds the name of the user running scopy
	3. Then it checks if an access configuration file exists
	4. The acl file is verified that it isn't a symbolic link
	5. Next scopy checks if the user running scopy is given access
	6. Once the user is verified, the desired file is copied

Sharing a file:
	1. Put a copy of scopy into file directory
	2. run the command "chmod u+s scopy"
	3. create a new file and name it as the desired file you want to share, and add '.acl' to the end of the name
	4. Type in the name of user you want your file to be shared with in the .acl file, putting it on a new line

Copying File From Another User:
	1. Type in the console the program, name of file to copy, and name of new file you want to create
		ex: /home/user/public/scopy /home/user/document_to_copy.txt name_of_copy.txt
			*change 'user' with the user you want to copy from
			*first argument must be the location of scopy
			*second argument must be location of desired file to copy

Security:
	Files in its normal state will not allow other users to copy your files. The only way for files to be copied is to be given access by you. The configuration file will allow you to choose who can have access to your file, and no one but the user can modify the configuration file. 
