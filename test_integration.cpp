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
const string fcn_name{"exp(-x)"};//{"sin^{2}(20#pi x)"};

//the function to integrate, and its analytical integral. 
double fcn(double x) { return exp(-x); }//{ return pow( sin(10.*twoPi*x), 2); }
double fcn_indef_integral(double x) { return -1.*exp(-x); }//{ return (x/2.) + sin(20.*twoPi*x)/(40.*twoPi); }

//the minimum and maximum N.pts to integrate with, for all methods. keep in mind that: 
// 1. simpson's rule needs at least 3.
// 2. simpson's rule can only work with an odd number of points, so both 'N_min' and 'N_max' must be odd. 

vector<int> Get_integration_steps(int n_min, const int npts, const bool odd_only=false) 
{
    vector<int> Nvals; 

    if (odd_only && n_min % 2 == 0) n_min +=1; 

    for (int i=0; i<npts; i++) {
        Nvals.push_back(odd_only ? n_min + 2*i : n_min + i); 
    }
    return Nvals; 
}

vector<int> Get_integration_steps_exponential(int n_min, const int npts, const double step=1.05, const bool odd_only=false)
{
    if (odd_only && n_min % 2 == 0) n_min +=1; 

    vector<int> Nvals{ n_min }; 

    double N_double = (double)Nvals.back(); 

    for (int i=0; i<npts; i++) {
    
        N_double *= step; 
        int new_val = (int)round(N_double);
        //for small step sizes, this may not produce a new integers if the starting ints are small.
        //this bit of logic is to avoid having the same integer be double-counted
        if ( new_val > Nvals.back() ) Nvals.push_back( (odd_only && new_val % 2==0) ? new_val+1 : new_val ); 
    }

    return Nvals; 
}

//integration limits: 
const double x_min = 0.; 
const double x_max = 1.; 

//path to 'dat' file with the quad point data. generated with 'print_legendre_roots.py' 
const char* path_quadpoints = "gauss_quad_points_160.dat"; 

//path to the output graphic png
const char* path_outgraphic = "Errors.png"; 

int main(int argc, char* argv[])
{
    //create a vector of evenly-spaced, odd N-values
    vector<int> Nvals_trap  = Get_integration_steps_exponential(2, 400, 1.035, false);  
    vector<int> Nvals_simp  = Get_integration_steps_exponential(3, 250, 1.035, true); 
    vector<int> Nvals_gauss = Get_integration_steps(2, 160); 

    //initialize the gaussian integrator
    GaussInt gauss_int(path_quadpoints); 

    const double integral_analytical = fcn_indef_integral(x_max) - fcn_indef_integral(x_min); 

    //h-vals and erorrs are drawn in a log-log plot. 
    vector<double> error_trap{}, error_simp{}, error_gauss{};  
    
    for (int N : Nvals_trap) error_trap.push_back(
        fabs((trapez (fcn, N, x_min,x_max) - integral_analytical) / integral_analytical)
    );

    for (int N : Nvals_simp) error_simp.push_back(
        fabs((simpson(fcn, N, x_min,x_max) - integral_analytical) / integral_analytical)
    );

    for (int N : Nvals_gauss) error_gauss.push_back(
        fabs((gauss_int.Integral(fcn, N, x_min,x_max) - integral_analytical) / integral_analytical)
    );

    //find the minimum and maximum 
    double y_min{1e30}, y_max{-1e30}; 

    auto find_min_max = [&y_min,&y_max](double y) { 
        y_min = min<double>(y_min, y); 
        y_max = max<double>(y_max, y); 
    }; 

    for (auto y : error_trap)  find_min_max(y); 
    for (auto y : error_simp)  find_min_max(y); 
    for (auto y : error_gauss) find_min_max(y); 

    auto Make_vec_double = [](const vector<int>& V) { 
        vector<double> Vd; 
        for (int i : V) Vd.push_back((double)i); 
        return Vd; 
    }; 

    vector<double>&& Nvals_double_trap  = Make_vec_double(Nvals_trap); 
    vector<double>&& Nvals_double_simp  = Make_vec_double(Nvals_simp); 
    vector<double>&& Nvals_double_gauss = Make_vec_double(Nvals_gauss); 

    auto canv = new TCanvas; 

    //now, create the graphs
    auto graph_trap   = new TGraph( Nvals_trap.size(),  Nvals_double_trap.data(),  error_trap.data() );
    auto graph_simp   = new TGraph( Nvals_simp.size(),  Nvals_double_simp.data(),  error_simp.data() );
    auto graph_extrap = new TGraph( Nvals_gauss.size(), Nvals_double_gauss.data(), error_gauss.data() );

    //set the log scale for x & y 
    gPad->SetLogx(1); 
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
    graph_simp->Draw("SAME L");
    
    graph_extrap->SetMarkerStyle(kMultiply); 
    graph_extrap->SetMarkerColor(kBlue); 
    graph_extrap->SetLineColor(kBlue); 
    graph_extrap->SetLineWidth(1); 
    graph_extrap->Draw("SAME L");

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