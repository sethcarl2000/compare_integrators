#include "integrators.hpp"
#include <iostream> 
#include <TFile.h> 
#include <cmath> 
#include <vector> 

using namespace std; 



double fcn(double x) { return exp(x); }
double fcn_indef_integral(double x) { return exp(x); }

int main(int argc, char* argv[])
{
    //this must be at least 3, and each value must be odd. 
    const int N_min = 3; 
    const int N_max = 159; 

    //range over which to integrate 
    const double x_max = 1.; 
    const double x_min = 0.; 

    //create a vector of evenly-spaced, odd N-values
    vector<int> N_vals;  
    for (int N=N_min; N<=N_max; N += 2) N_vals.push_back(N); 

    //initialize the gaussian integrator
    GaussInt gauss_int("gauss_quad_points_160.dat"); 

    //h-vals and erorrs are drawn in a log-log plot. 
    vector<double> error_trap{}, error_simp{}, error_gauss{};  
/*
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

    for (auto y : error_fwd)    find_min_max(y); 
    for (auto y : error_cent)   find_min_max(y); 
    for (auto y : error_extrap) find_min_max(y); 

    //now, create the graphs
    auto graph_fwd    = new TGraph( h_vals.size(), h_vals.data(), error_fwd.data() );
    auto graph_cent   = new TGraph( h_vals.size(), h_vals.data(), error_cent.data() );
    auto graph_extrap = new TGraph( h_vals.size(), h_vals.data(), error_extrap.data() );

    //set the log scale for x & y 
    gPad->SetLogx(1); gPad->SetLogy(1); 

    //set margin so we can see the histo titles
    gPad->SetLeftMargin(0.12); 
    gPad->SetRightMargin(0.05); 

    //draw both of the graphs
    graph_fwd->SetTitle(Form("x = %.1f;h;relative error",x));
    graph_fwd->GetYaxis()->SetRangeUser( y_min, y_max );  
    graph_fwd->Draw(); 

    graph_cent->SetLineColor(kRed); 
    graph_cent->SetLineStyle(kDashed); 
    graph_cent->SetLineWidth(2); 
    graph_cent->Draw("SAME");
    
    graph_extrap->SetLineColor(kBlue); 
    graph_extrap->SetLineStyle(kDotted); 
    graph_extrap->SetLineWidth(2); 
    graph_extrap->Draw("SAME");

    //now let's build the legend 
    auto legend = new TLegend(0.12,0.1, 0.5,0.3); 

    legend->SetHeader("Differentiation Method"); 
    
    legend->AddEntry(graph_fwd,    "forward"); 
    legend->AddEntry(graph_cent,   "central");
    legend->AddEntry(graph_extrap, "extrapolated");
    
    legend->Draw();  
*/ 

    return 0; 
}