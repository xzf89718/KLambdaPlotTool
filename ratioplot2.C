/// \file
/// \ingroup tutorial_hist
/// \notebook
/// Example of a fit residual plot.
///
/// Creates a histogram filled with random numbers from a gaussian distribution
/// and fits it with a standard gaussian function. The result is passed to the `TRatioPlot`
/// constructor. Additionally, after calling `TRatioPlot::Draw` the upper and lower y axis
/// titles are modified.
/// Confidence interval bands are automatically drawn on the bottom (but can be disabled by draw option `nobands`.
///
/// \macro_image
/// \macro_code
///
/// \author Paul Gessinger
#include <vector>
#include <string>
#include "TFile.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TH1F.h"


void ratioplot2() 
{

        std::string Dir = {"Preselection"};
        auto KLReweight_py8 = new TFile("KLReweight_py8.root", "READ");
        auto KLReweight_herwig7 = new TFile("KLReweight_herwig7.root", "READ");
        std::string h1_name = {"hhttbbKL10p0from1p0_2tag2pjet_0ptv_LL_OS_mHH"}; 
        std::string h2_name = {"hhttbbKL5p0from1p0_2tag2pjet_0ptv_LL_OS_mHH"};
        std::string figure_name = {"./plots/test.pdf"};

        gStyle->SetOptStat(0);
        auto c1 = new TCanvas("c1", figure_name.c_str());
        auto dir = (TDirectory*)KLReweight_py8->Get("Preselection");
        auto h1 = (TH1F*)dir->Get(h1_name.c_str())->Clone();
        auto h2 = (TH1F*)dir->Get(h2_name.c_str())->Clone();
        cout<< h1->GetNbinsX() <<endl;
        h1->Rebin(12);
        h2->Rebin(12);
        h1->GetXaxis()->SetTitle("m_{HH}[GeV]");
        auto rp1 = new TRatioPlot(h1, h2);
        rp1->Draw();
        rp1->GetLowerRefYaxis()->SetTitle("ratio");
        rp1->GetUpperRefYaxis()->SetTitle("Events");
        
        c1->SaveAs(figure_name.c_str());
        ofstream fout;
        fout.open("./plots/Yields.txt", ios::app);
        if(!fout)
        {
                cout<<"Cant open output file!"<<endl;
        }
        else
        {
                fout << h1_name <<" "<< h1->Integral() <<" "<<h2_name<<" "<<h2->Integral()<<endl;
        }
        KLReweight_py8->Close();
        KLReweight_herwig7->Close();
        
}
