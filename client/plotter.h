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

  Plotter(std::ifstream& data_stream)
  {
      if(!data_stream.is_open()) throw std::exception();
      
    
      std::string line, date_str;
      int avg_count = 0;
      float hours;
      while(data_stream >> date_str >> hours){
      
        // tm tml;
        // strptime(date_str.data(), date_format, &tml);     
        // std::cout << hours << std::endl;
        /*mktime(&tml)*/
        data.push_back(std::make_pair(date_str, hours));
      }
  }
  void print_stats();
  void gnu_plot(bool avg, int avg_span=5);
  void plot_save(std::string out_name, bool show=false);
  
};