#include <fstream>
#include <ios>
#include <iostream>
#include <ctime>

using namespace std;

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

#define DATE_FMT_SIZE (4+1+2+1+2+1)
int main(){
  fstream logfile("progress.txt", std::ios::app | ios::in);
  // logfile.open("progress.txt", ios::out | ios::app);
  // jump_last_line(logfile);
  logfile.seekg(-DATE_FMT_SIZE-4, logfile.end);
  string last;
  std::getline(logfile, last);
  cout <<'\n'<< "last line: " << last << ", len : " << last.size() << std::flush;

  time_t now = std::time(nullptr);
  string last_log_date;
  last_log_date.resize(DATE_FMT_SIZE);
  tm* ltm = std::localtime(&now);
  strftime(last_log_date.data(), DATE_FMT_SIZE, "%Y-%0m-%0d", ltm);
  
  logfile << last_log_date << ' ' << 10 << std::endl;
  // logfile << std::flush;
  logfile.close();
}
