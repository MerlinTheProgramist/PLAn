#include <fstream>

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
            << "> That's " << (total/24.f) << " days of pure work!!!";
}

// Plotter::Plotter(std::ifstream& data_stream);
void Plotter::gnu_plot(bool avg, int avg_span) {
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
// void GnuPlotter::plot_save(std::string out_name, bool show){
  
// }

