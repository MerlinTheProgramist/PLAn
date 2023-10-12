#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>

#include "consts.h"

class Plotter
{
  public:
   std::vector<std::pair<long, long>> data{};

  Plotter(std::ifstream& data_stream){
    if(!data_stream.is_open()) return;

    
    std::string line, date_str;
    int hours;
    while(std::getline(data_stream, line)){
      std::istringstream iss(line);
      if(!(iss >> date_str >> hours)) break;
      
      tm tml;
      strptime(date_str.data(), date_format, &tml);     

      data.push_back(std::make_pair(mktime(&tml), hours));
    }
  }

  virtual void plot(bool avg, int avg_span=5);
  virtual void plot_save(std::string out_name, bool show=false);
};