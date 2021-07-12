Description:
Develop and implement a small file system, a combination of Unix and
CP/M concepts, the file system (in this example) is called “FS”.
This file system will not be part of an operating system, but similar to
Many modern file systems it will run in several different operating systems to
provide a “portable” file system (a group of files).

Operations Supported:
1. Createfs #ofblocks
2. Formatfs #DABPTentries
3. Save FS(Done internally after each operations like file add, remove, change name etc)
4. Openfs and read a file
5. List of files
6. Remove a file
7. File Rename
8. Put ExternalFile – put (store) Host OS file into the disk
9. Get ExternalFile – get disk file, copy from “disk” to host OS file system
10. User name – this user owns this user’s files
11. Read-Write access modification
12. Linux Link/Unlink

Code compilation: Default gcc compiler(4.1.2). execute the makefile.