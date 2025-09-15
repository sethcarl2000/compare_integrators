#include "integrators.hpp"
#include <iostream> 
#include <TFile.h> 
#include <cmath> 
#include <vector> 
#include <TGraph.h> 
#include <TLegend.h> 
#include <TStyle.h> 
#include <TPad.h> 
#include <TAxis.h> 
#include <TCanvas.h> 

using namespace std; 

const double twoPi = 6.28318530718; 

//the name of the function to print on the plot
const string fcn_name{"sin^{2}(20#pi x)"};

//the function to integrate, and its analytical integral. 
double fcn(double x) { return pow( sin(10.*twoPi*x), 2); }
double fcn_indef_integral(double x) { return (x/2.) + sin(20.*twoPi*x)/(40.*twoPi); }

//the minimum and maximum N.pts to integrate with, for all methods. keep in mind that: 
// 1. simpson's rule needs at least 3.
// 2. simpson's rule can only work with an odd number of points, so both 'N_min' and 'N_max' must be odd. 
// 3. if 'N_max' is more than the biggest order of gauss quad-weights given in 'gauss_quad_points_[N].dat', then an error will be thrown. 
const int N_min = 3;
const int N_max = 159; 

//integration limits: 
const double x_min = 0.; 
const double x_max = 1.; 

//path to 'dat' file with the quad point data. generated with 'print_legendre_roots.py' 
const char* path_quadpoints = "gauss_quad_points_160.dat"; 

//path to the output graphic png
const char* path_outgraphic = "BadErrors.png"; 

int main(int argc, char* argv[])
{
    //create a vector of evenly-spaced, odd N-values
    vector<int> N_vals; 
    for (int N=N_min; N<=N_max; N += 2) { N_vals.push_back(N); } 

    //initialize the gaussian integrator
    GaussInt gauss_int(path_quadpoints); 

    //h-vals and erorrs are drawn in a log-log plot. 
    vector<double> error_trap{}, error_simp{}, error_gauss{};  

    for (int N : N_vals) {

        double analytical = fcn_indef_integral(x_max) - fcn_indef_integral(x_min); 

        error_trap .push_back( fabs((trapez (fcn, N, x_min,x_max) - analytical) / analytical) ); 
        error_simp .push_back( fabs((simpson(fcn, N, x_min,x_max) - analytical) / analytical) ); 
        error_gauss.push_back( fabs((gauss_int.Integral(fcn, N, x_min,x_max) - analytical) / analytical) ); 
    }   

    //find the minimum and maximum 
    double y_min{1e30}, y_max{-1e30}; 

    auto find_min_max = [&y_min,&y_max](double y) { 
        y_min = min<double>(y_min, y); 
        y_max = max<double>(y_max, y); 
    }; 

    for (auto y : error_trap)  find_min_max(y); 
    for (auto y : error_simp)  find_min_max(y); 
    for (auto y : error_gauss) find_min_max(y); 

    vector<double> N_vals_double; 
    for (int N : N_vals) N_vals_double.push_back((double)N); 

    for (int i=0; i<error_trap.size(); i++) {
        printf("n, trap,simp,gauss:  %3i : %-.4e, %-.4e, %-.4e\n", 
            N_vals[i], 
            error_trap[i],
            error_simp[i],
            error_gauss[i]
        ); 
    }
    
    auto canv = new TCanvas; 

    //now, create the graphs
    auto graph_trap   = new TGraph( N_vals.size(), N_vals_double.data(), error_trap.data() );
    auto graph_simp   = new TGraph( N_vals.size(), N_vals_double.data(), error_simp.data() );
    auto graph_extrap = new TGraph( N_vals.size(), N_vals_double.data(), error_gauss.data() );

    //set the log scale for x & y 
    gPad->SetLogy(1); 

    //set margin so we can see the histo titles
    gPad->SetLeftMargin(0.12); 
    gPad->SetRightMargin(0.05); 

    //draw both of the graphs
    const char* graph_name = Form("Errors: Int_{%.0f}^{%.0f} %s", x_min, x_max, fcn_name.c_str()); 
    graph_trap->SetTitle(graph_name); 
    graph_trap->GetYaxis()->SetRangeUser( 1e-18, 10. );  
    graph_trap->Draw(); 

    graph_simp->SetMarkerStyle(kPlus);
    graph_simp->SetMarkerColor(kRed);
    graph_simp->SetLineColor(kRed); 
    graph_simp->SetLineWidth(1); 
    graph_simp->Draw("SAME PL");
    
    graph_extrap->SetMarkerStyle(kMultiply); 
    graph_extrap->SetMarkerColor(kBlue); 
    graph_extrap->SetLineColor(kBlue); 
    graph_extrap->SetLineWidth(1); 
    graph_extrap->Draw("SAME PL");

    //now let's build the legend 
    auto legend = new TLegend(0.55,0.7, 0.95,0.9); 

    legend->SetHeader("Integration method"); 
    
    legend->AddEntry(graph_trap,   "Trapezoids"); 
    legend->AddEntry(graph_simp,   "Simpson's Rule");
    legend->AddEntry(graph_extrap, "Gauss Quads");
    
    legend->Draw();  

    canv->SaveAs(path_outgraphic); 

    return 0; 
}