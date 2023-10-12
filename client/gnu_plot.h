/* 
  gnuplot --persist -e "
  set xlabel 'date';
  set ylabel 'hours';
  set xdata time;
  set xtics format '';
  set timefmt '%Y-%m-%d';
  plot 'progress_report.txt' using 1:2 w linespoints;
  "
*/
#include "gnuplot-iostream/gnuplot-iostream.h"
#include <vector>
#include <fstream>

#include "plotter.h"

class GnuPlotter : public Plotter{
  public:
  GnuPlotter(std::ifstream& data_stream):Plotter(data_stream){}

  void plot(bool avg, int avg_span=5) override{
    Gnuplot gp;
    // std::vector<std::pair<int, int>>
  
    gp << 
    "set xlabel 'day';"
    "set ylabel 'hours';"
    "set timefmt %Y-%m"
    "set xdata time;"
    "set xtics format ''"
    "plot" << gp.file1d(data)
    << std::endl;
    // getchar();
  }
  void plot_save(std::string out_name, bool show=false)override{
    
  }
};