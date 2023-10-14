
log_file:=work_report.txt
exe_name:=plan

install: log_file:=~/${log_file}

build:
	@echo "build with log_file in ${log_file} ..." 
	@g++ -o ${exe_name} ./main.cpp ./plotter.cpp -lboost_filesystem -lboost_iostreams -lboost_program_options -lm -DLOG_FILE_NAME=\"${log_file}\"  
	@touch ${log_file}
install: build
	@echo "copying bin to /usr/bin ..."
	@cp ${exe_name} /usr/bin

.PHONY: build install