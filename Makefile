
log_file:=/home/merlin/progress_report.txt
exe_name:=plan

build:
	@echo "build with log_file in ${log_file} ..." 
	@g++ -o ${exe_name} ./main.cpp ./plotter.cpp -lboost_filesystem -lboost_iostreams -lboost_program_options -lm -DLOG_FILE_NAME=\"${log_file}\"  
	@touch ${log_file}
install:
	@echo "copying bin to /usr/bin ..."
	@cp ${exe_name} /usr/bin

.PHONY: build install
