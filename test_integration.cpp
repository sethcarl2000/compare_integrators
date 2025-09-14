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
    graph_trap->GetYaxis()->SetRangeUser( 1e-18, 1e-2 );  
    graph_trap->Draw(); 

    graph_simp->SetLineColor(kRed); 
    graph_simp->SetLineStyle(kDashed); 
    graph_simp->SetLineWidth(2); 
    graph_simp->Draw("SAME");
    
    graph_extrap->SetLineColor(kBlue); 
    graph_extrap->SetLineStyle(kDotted); 
    graph_extrap->SetLineWidth(2); 
    graph_extrap->Draw("SAME");

    //now let's build the legend 
    auto legend = new TLegend(0.12,0.1, 0.5,0.3); 

    legend->SetHeader("Differentiation Method"); 
    
    legend->AddEntry(graph_trap,   "forward"); 
    legend->AddEntry(graph_simp,   "central");
    legend->AddEntry(graph_extrap, "extrapolated");
    
    legend->Draw();  

    canv->SaveAs("Errors.png"); 

    return 0; 
}