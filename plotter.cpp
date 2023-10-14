#include <fstream>
#include <iostream>

#include "consts.h"
#include "plot_lib.hpp"
#include "plotter.h"

void Plotter::print_stats(){
  std::cout << "Calculating ... \n" << std::endl;
  
  int total{0};//, min=0, max=24;
  for(auto pair : data)
  {
    total+= pair.second;
    // min = std::min(min, (int)pair.second);
    // max = std::max(max, (int)pair.second);
  }
  float average = (float)total / data.size();
  
  std::cout << "Ok, here are your statistics:\n"
            << "> average: " << average << "h\n"
            << "> total hours:" << total << '\n'
            << "> That's " << (total/24.f) << " days of pure work!!!" << std::endl;
}

// Plotter::Plotter(std::ifstream& data_stream);
void Plotter::gnu_plot() {
  Gnuplot gp;
  // std::vector<std::pair<int, int>>

  gp << 
  "set yrange [0:*]\n"
  "set ylabel 'hours'\n"
  
  "set xlabel 'days'\n"
  "set xdata time\n"
  "set timefmt '%Y-%m-%d'\n"
  "set xtics format ''\n"
  "plot" << gp.file1d(data) << "u 1:2  w l  lc 'red'  ti 'progess'\n"
  << std::endl;
  // getchar();
}

void Plotter::term_plot(){
  using namespace plot;
  TerminalInfo term;
  term.detect();

  std::vector<Pointf> points(data.size());
  for(int i=0;i<data.size();i++)
  {
    tm tml;
    strptime(data[i].first.data(), date_format, &tml);
    int day_diff = (mktime(&tml) - mktime(&start_date)+ONE_DAY)/ONE_DAY;
    points[i] = {(float)day_diff, data[i].second};
    // std::cout << data[i].first << ' ' << points[i].x << ' ' << points[i].y << std::endl;
  }
  
  RealCanvas<BrailleCanvas> canvas({{0.f, 24.f}, {points.back().x, 0.f}}, Size{term.size().x-10, term.size().y-10}, term);
  auto layout = margin(frame(&canvas, term));

  auto bounds = canvas.bounds();
  auto size = canvas.size();
  
  canvas.path(palette::royalblue, points.begin(), points.end());


  std::cout << margin(0, 0, 0, 0, frame(u8"Progress", Align::Center, {BorderStyle::Solid, true}, &canvas, term)) << std::flush; 
}
