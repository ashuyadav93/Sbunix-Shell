Contributors:

Akshat Singhal
Ashish Yadav
Vaibhav Rustagi

Below are the functionalities implemented:

1. Support of basic functionalities of cd, ls, cat and echo.
	Example: ls
		 ls include/
		 cat test.sbush
		 cat test.sbush Makefile
		 cd /
		 cd ..
		 cd include/
		 echo abc
		 echo $PATH

2. Execution of binaries interactively
3. Execution of script [Script file should be specified as a command line option while running sbush]
	Example: rootfs/bin/sbush test.sh
4. set and use of PATH, PS1 and other variables. Default value used for PS1 is: "\h@\H-\u:\w:" instead of taking from .bashrc 
	Example: export PATH="$PATH:/new_path"

5. Launch of background processes.
6. Launch of pipes as supported in linux shell.
	Example: cat Makefile | /bin/grep obj
	Example: ls | /bin/grep .txt




