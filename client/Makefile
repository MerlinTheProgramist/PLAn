log_path:=./progress_report.txt

build:
	g++ ./main.cpp -lboost_program_options -lm -DLOG_FILE_NAME=\"${log_path}\" 
	touch ${log_path}
