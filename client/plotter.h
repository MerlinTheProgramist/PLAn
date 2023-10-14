#pragma once

#include <exception>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>

#include "gnuplot-iostream/gnuplot-iostream.h"
#include "consts.h"

struct Plotter
{
  std::vector<std::pair<std::string, float>> data{}, avg_data{};
  tm start_date{};

  Plotter(std::ifstream& data_stream)
  {
      if(!data_stream.is_open()) throw std::exception();
    
      std::string line, date_str;
      int avg_count = 0;
      float hours;
      while(data_stream >> date_str >> hours){
        if(start_date.tm_year==0)
          strptime(date_str.data(), date_format, &start_date);     
        /*mktime(&tml)*/

        data.push_back(std::make_pair(date_str, hours));
      }
  }
  void print_stats();
  void gnu_plot();
  void term_plot();
};