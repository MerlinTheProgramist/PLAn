
log_file:=/home/merlin/progress_report.txt
exe_name:=plan

build:
	@echo "build with log_file in ${log_file} ..." 
	@g++ -o ${exe_name} ./main.cpp ./plotter.cpp -lboost_filesystem -lboost_iostreams -lboost_program_options -lm -DLOG_FILE_NAME=\"${log_file}\"  
	@touch ${log_file}
<<<<<<< HEAD
install: 
=======
install:
>>>>>>> 3eaf3db (fixed Makefile's install procedure (permissions given installing with sudo))
	@echo "copying bin to /usr/bin ..."
	@cp ${exe_name} /usr/bin

.PHONY: build install
