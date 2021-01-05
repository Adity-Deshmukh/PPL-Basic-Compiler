compileAll :
	@echo "Compiling all files and creating program.out"
	@gcc ./*.c ./*.h -o program.out
	@echo "Program dependencies compiled successfuly !"
	@echo "Run the file as ./program.out with testcase as argument. Example: ./program.out t1.txt"
	
