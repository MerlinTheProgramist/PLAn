#include <exception>
#include <filesystem>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <boost/program_options.hpp>
#include <ctime>
#include <fstream>
#include <string_view>
#include <filesystem>

#include "plot_lib.hpp"
#include "consts.h"
#include "plotter.h"

namespace po = boost::program_options;

#ifndef LOG_FILE_NAME
  #define LOG_FILE_NAME "~/progress_report.txt"
#endif

const char log_file_name[] = LOG_FILE_NAME;

struct {
  bool quiet = false;
} session_config;

#define LOG if(!session_config.quiet) std::cout 

void term_plot(std::ifstream& in){
  struct log_t{
    int date, hours;
  };
  std::string line_s;
  std::vector<log_t> logs;
  while(std::getline(in, line_s)){
    std::stringstream line(line_s);
    logs.push_back({});
    line >> logs.back().date >> logs.back().hours;
  }
}

void jump_last_line(std::fstream& f, int line_len){
  f.seekg(-1, std::ios_base::end);
  
  while((int)f.tellg()>0){
    char ch;
    f.get(ch);

    if(ch == '\n'){
      f.seekg(-line_len, std::ios_base::cur);
      return;
    }
    
    f.seekg(-1, std::ios_base::cur);
  }
}

void jump_front(std::fstream& f){
  f.clear();
  f.seekg(0);
}


bool log_hours(int hours, bool add, int day_offset=0, bool override=true){
  std::fstream logfile(log_file_name, std::ios::in | std::ios::app);
  if(!logfile.is_open()) 
    throw std::invalid_argument("log file path not valid");

  
  // get now time
  time_t now = std::time(nullptr) + day_offset * ONE_DAY;
  tm* now_tm = localtime(&now);
  std::string date_str(30, ' ');
  date_str.resize(strftime(date_str.data(), 30, date_format, now_tm));
  
  const int line_len = date_str.size()+4;
  
  // read last line from file and parse date
  if(override){
    jump_last_line(logfile, line_len);
    
    std::string last_log;
    std::getline(logfile, last_log);
    
    LOG << "today is: " << now_tm->tm_year+1900 << '-' << now_tm->tm_mon << '-' << now_tm->tm_mday << std::endl;
    
    if(!last_log.empty())
    {
      struct tm last_log_tm;
      {
        std::string last_log_date = last_log.substr(0, last_log.find(' '));
        if(add)
        {
          hours+= stof(last_log.substr(last_log.find(' ')));
          if(hours>24)
          {
            LOG << "can't log more than 24 hours a day" << std::endl;
            return false;
          }
        }
        strptime(last_log_date.data(), date_format, &last_log_tm);
      }
      
      // override if last was today
      if((now_tm->tm_year==last_log_tm.tm_year &&
       now_tm->tm_mon==last_log_tm.tm_mon &&
       now_tm->tm_wday<=last_log_tm.tm_wday))
      {
        LOG << "Today's date already exsits, updating... " << std::endl;
        logfile.close();
        logfile.clear();
        logfile.open(log_file_name);
        logfile.seekp(-line_len, std::ios_base::end);
      }
    }
    else
      jump_front(logfile);
  }
  else
    jump_front(logfile);

  LOG << "logging: " << hours << "h to: "<< log_file_name << std::endl;
  // append to file
  logfile << date_str << ' ' << std::setfill('0') << std::setw(2) << hours << std::endl;
  logfile.close();

  return true;
}



int main(int argc, char* const argv[]){
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("log,l", po::value<int>() ,"log today's working hours")
    ("add,a", "if today's record exists, add this log to it")
    // ("end", "end work day")//@not_impl
    ("show,s", "show progress statistics")
    ("gnuplot,p", "plot all time date with gnuplot (gui)")
    ("term-plot,t", "plot all time data to terminal") //@not_impl
    // ("export,e", po::value<std::string>(), "export plot to file") //@not_impl
    ("quiet,q", "run quietly");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help") || vm.empty()){
    LOG << desc << std::endl;
    return 0;
  }

  if(vm.count("quiet"))
    session_config.quiet = true;

  
  if(vm.count("log")){
    int value = vm["log"].as<int>();
    if(value < 0 || value > 24){
      LOG << "working hours must in range 0 to 24" << std::endl;
      return 1;
    }
    
    log_hours(value, vm.count("add"));
  }

  // reading logs
  std::ifstream data(log_file_name, std::ios::in);
  Plotter plotter(data);

    
  if(vm.count("gnuplot"))
    plotter.gnu_plot();
  
  if(vm.count("term-plot"))
     plotter.term_plot();
  
  if(vm.count("show"))
    plotter.print_stats();
  
  return 0;
}
