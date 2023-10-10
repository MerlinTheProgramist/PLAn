#include <ios>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <ctime>
#include <fstream>
#include <string_view>

#include "plot_lib.hpp"

namespace po = boost::program_options;

void jump_last_line(std::fstream& f){
  f.seekg(-1, std::ios_base::end);
  bool keepLooping{true};
  while(keepLooping){
    char ch;
    f.get(ch);
    if(f.tellg() <= 1){
      f.seekg(0);
      keepLooping = false;
    }
    else if(ch == '\n')
      keepLooping = false;
    else
      f.seekg(-2, std::ios_base::cur);
  }
}

void jump_start(std::fstream& f){
  f.clear();
  f.seekg(0);
}

constexpr char date_format[] = "%Y-%m-%d";

bool log_hours(int hours, int day_offset=0, bool override=true){
  std::fstream logfile("~/progress_report.txt");
  if(!logfile.is_open()) return false;

  // read last line from file and parse date
  jump_last_line(logfile);
    override = false;

  // get now time
  auto now = std::time(nullptr);
  tm* now_tm = localtime(&now);

  if(override){
    std::string last_log;
    std::getline(logfile, last_log);
    if(last_log.empty())
      jump_start(logfile);
    else
    {
      struct tm last_log_tm;
      {
      std::string last_log_date = last_log.substr(0, last_log.find(' '));
      strptime(last_log_date.data(), date_format, &last_log_tm);
      }

      // override if last was today
      if(!(now_tm->tm_year==last_log_tm.tm_year &&
           now_tm->tm_mon==last_log_tm.tm_mon &&
           now_tm->tm_wday-day_offset<=last_log_tm.tm_wday))
        jump_start(logfile);
    }
  }

  // append to file
  std::string date_str;
  strftime(date_str.data(), 255, date_format, now_tm);
  logfile << date_str << ' ' << hours << '\n';
    
  return true;
}

int main(int argc, char* const argv[]){
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("log,l", po::value<int>() ,"log today's working hours")
    ("gnuplot,p", "plot all time date with gnuplot (gui)")
    ("term-plot,tp", "plot all time data to terminal")
    ("export,e", po::value<std::string>(), "export plot to file");
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help") || vm.empty()){
    std::cout << desc << std::endl;
    return 1;
  }

  if(vm.count("log")){
    int value = vm["log"].as<int>();
    if(value < 0 || value > 24){
      std::cout << "working hours must in range 0 to 24" << std::endl;
      return 1;
    }
    log_hours(value);
  }
}
