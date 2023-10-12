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
#include "gnu_plot.h"
#include "consts.h"

namespace po = boost::program_options;

#ifndef LOG_FILE_NAME
  #define LOG_FILE_NAME "~/progress_report.txt"
#endif

const char log_file_name[] = LOG_FILE_NAME;

struct {
  bool quiet = false;
} session_config;

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

void jump_last_line(std::fstream& f){
  f.seekg(-1, std::ios_base::end);
  bool keepLooping{true};
  while(keepLooping){
    char ch;
    f.get(ch);

    if((int)f.tellg() <= 1){
      f.seekg(0);
      keepLooping = false;
    }
    else if(ch == '\n')
      keepLooping = false;
    else
      f.seekg(-2, std::ios_base::cur);
  }
}

void jump_end(std::fstream& f){
  f.clear();
  f.seekg(0);
}


bool log_hours(int hours, int day_offset=0, bool override=true){
  std::fstream logfile(log_file_name, std::ios::in | std::ios::app);
  if(!logfile.is_open()) 
    throw std::invalid_argument("log file path not valid");

  const time_t ONE_DAY = 24*60*60;
  
  // get now time
  time_t now = std::time(nullptr) + day_offset * ONE_DAY;
  tm* now_tm = localtime(&now);
  std::string date_str(30, ' ');
  date_str.resize(strftime(date_str.data(), 30, date_format, now_tm));
  
  // read last line from file and parse date
  if(override){
    jump_last_line(logfile);

    
    std::string last_log;
    std::getline(logfile, last_log);
    
    if(!session_config.quiet)
      std::cout << "today is: " << now_tm->tm_year+1900 << '-' << now_tm->tm_mon << '-' << now_tm->tm_mday << std::endl;
    
    if(!last_log.empty())
    {
      struct tm last_log_tm;
      {
        std::string last_log_date = last_log.substr(0, last_log.find(' '));
        strptime(last_log_date.data(), date_format, &last_log_tm);
      }
      
      // override if last was today
      if((now_tm->tm_year==last_log_tm.tm_year &&
       now_tm->tm_mon==last_log_tm.tm_mon &&
       now_tm->tm_wday<=last_log_tm.tm_wday))
      {
        if(!session_config.quiet)
          std::cout << "Today's date already exsits, updating... " << std::endl;
        logfile.close();
        logfile.clear();
        logfile.open(log_file_name);
        logfile.seekp(-date_str.size()-4, std::ios_base::end);
      }
    }
  }
  // else
    // jump_end(logfile);
  // append to file
  logfile << '\n' << date_str << ' ' << std::setfill('0') << std::setw(2) << hours << std::flush;
  logfile.close();

  return true;
}



int main(int argc, char* const argv[]){
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("log,l", po::value<int>() ,"log today's working hours")
    ("gnuplot,p", "plot all time date with gnuplot (gui)")
    ("term-plot,tp", "plot all time data to terminal")
    // ("back,b", "edit ")
    ("export,e", po::value<std::string>(), "export plot to file");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help") || vm.empty()){
    std::cout << desc << std::endl;
    return 0;
  }

  if(vm.count("log")){
    int value = vm["log"].as<int>();
    if(value < 0 || value > 24){
      std::cout << "working hours must in range 0 to 24" << std::endl;
      return 1;
    }
    std::cout << "logging: " << value << " to: "<< log_file_name << std::endl;
    log_hours(value);
  }

  if(vm.count("gnuplot")){
    std::ifstream data(log_file_name);
    GnuPlotter plotter(data);
    // plotter.plot(false);
    data.close();
  }
  if(vm.count("term-plot")){
    
  }
  return 0;
}
