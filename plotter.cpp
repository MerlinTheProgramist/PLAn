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
  "# number of points in moving average\n"
  "n = 18\n"
  "# weekly averaged\n"
  "w =  7 \n"

  "# initialize the variables\n"
  "do for [i=1:n] {\n"
  "    eval(sprintf('back%d=0', i))\n"
  "}\n"
  "do for [i=1:w] {\n"
  "    eval(sprintf('wback%d=0', i))\n"
  "}\n"

  "# build shift function (back_n = back_n-1, ..., back1=x)\n"
  "shift = '('\n"
  "do for [i=n:2:-1] {\n"
  "    shift = sprintf('%sback%d = back%d, ', shift, i, i-1)\n"
  "} \n"
  "shift = shift.'back1 = x)'\n"

  "wshift = '('\n"
  "do for [i=w:2:-1] {\n"
  "    wshift = sprintf('%swback%d = wback%d, ', wshift, i, i-1)\n"
  "} \n"
  "wshift = wshift.'wback1 = x)'\n"
  "# uncomment the next line for a check\n"
  "# print shift\n"

  "# build sum function (back1 + ... + backn)\n"
  "sum = '(back1'\n"
  "do for [i=2:n] {\n"
  "    sum = sprintf('%s+back%d', sum, i)\n"
  "}\n"
  "sum = sum.')'\n"

  "wsum = '(wback1'\n"
  "do for [i=2:w] {\n"
  "    wsum = sprintf('%s+wback%d', wsum, i)\n"
  "}\n"
  "wsum = wsum.')'\n"

  "# uncomment the next line for a check\n"
  "# print sum\n"

  "# define the functions like in the gnuplot demo\n"
  "# use macro expansion for turning the strings into real functions\n"
  "samples(x) = $0 > (n-1) ? n : ($0+1)\n"
  "avg_n(x) = (shift_n(x), @sum/samples($0))\n"
  "shift_n(x) = @shift\n"

  "wsamples(x) = $0 > (w-1) ? w : ($0+1)\n"
  "wavg_n(x) = (wshift_n(x), @wsum/wsamples($0))\n"
  "wshift_n(x) = @wshift\n"

  "# the final plot command looks quite simple\n"
  "set xdata time\n"
  "set timefmt '%Y-%m-%d'\n"
  "plot " << gp.file1d(data) << " using 1:2 w l lc rgb 'blue' lw 1 title 'performance' , "
  "     " << gp.file1d(data) << " using 1:(wavg_n($2)) w l lc rgb 'green' lw 3 title 'averaged 7 days', "
  "     " << gp.file1d(data) << " using 1:(avg_n($2)) w l lc rgb 'red' lw 5 title 'averaged  ' . n . ' days'\n" 
  << std::endl;
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
