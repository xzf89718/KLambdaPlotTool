// \file
/// \ingroup tutorial_hist
/// Example displaying two histograms and their ratio. This macro does not use the
/// class TRatioPlot. For ROOT version >= 6.08 TRatioPlot should be used. See
/// the other ratio plots examples in this folder.
///
/// \macro_image
/// \macro_code
///
/// \author Olivier Couet
//
//
// \Zifeng Xu modify
// Run this macro like this: root -l -q ratioplotOld.C+
//
// \Warning
// 
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>


#include "TStyle.h"
#include "TFile.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TH1F.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TCanvas.h"
#include "TList.h"
#include "TLegend.h"



#define DEBUG_COMPARE 1
std::string transformFloat(float KLambda){
    // input: any float KLambda
    // output: somethin like (string) 1p0

    // A kindly remind here: dure to the limit of std:to_string, this program cant handle 
    // input more than 5 digit decimal
    // Transform input KLambda to string form, such as 1p00000
    std::string string_KLambda = std::to_string(KLambda);
    auto index_of_point = string_KLambda.find(".");
    if(index_of_point != -1)
        string_KLambda.replace(index_of_point, 1, "p");
    //for now, we only need 2 digit decimal
    //string_KLambda = string_KLambda.substr(0, string_KLambda.length() - 4);
    // now, we noly need 1 digit decimal
    string_KLambda = string_KLambda.substr(0, string_KLambda.length() - 5);
    //if KLambda<0, we need to substitute the "-" with "n"
    auto index_of_minus = string_KLambda.find("-");
    if (index_of_minus != -1)
        string_KLambda.replace(index_of_minus, 1, "n");

    return string_KLambda;

}

void DrawRatioPlot(TH1F* h1, TH1F* h2){
    //cout<< h1->GetNbinsX() <<endl;
    h1->Rebin(rebin_factors.at(*iter_variable));
    h2->Rebin(rebin_factors.at(*iter_variable));
    // Define the Canvas
    TCanvas *c = new TCanvas("c", "canvas", 800, 800);
    gROOT->SetStyle("ATLAS");
    gStyle->SetOptStat(0);
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(1); // Upper and lower plot are joined
    // pad1->SetGridx();         // Vertical grid
    pad1->Draw();             // Draw the upper pad: pad1
    pad1->cd();               // pad1 becomes the current pad
    h1->SetStats(0);          // No statistics on upper plot
    h1->Draw("HIST E1");               // Draw h1
    h2->Draw("HIST E1 SAME");         // Draw h2 on top of h1
    // Create Legend for histogram!
    //auto legend = new TLegend(0.7, 0.9, 0.48, 0.9);
    auto legend = new TLegend(0.75, 0.8, 0.95, 0.95);
    gStyle->SetLegendTextSize(0.02);
    legend->AddEntry(h1, "truth_level");
    legend->AddEntry(h2, "combine_at_reco_level");
    legend->Draw();


    // Do not draw the Y axis label on the upper plot and redraw a small
    // axis instead, in order to avoid the first label (0) to be clipped.
    //h1->GetYaxis()->SetLabelSize(0.);
    // TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
    // axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    // axis->SetLabelSize(15);
    // axis->Draw();

    // lower plot will be in pad
    c->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    //pad2->SetGridx(); // vertical grid
    pad2->SetGridy(); // horizontal grid
    h1->GetXaxis()->SetTitle("");
    h1->GetYaxis()->SetTitle("Events");
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad

    // Define the ratio plot
    TH1F *h3 = (TH1F*)h1->Clone("h3");
    h3->SetLineColor(kBlack);
    h3->SetMinimum(0.8);  // Define Y ..
    h3->SetMaximum(1.35); // .. range
    h3->Sumw2();
    h3->SetStats(0);      // No statistics on lower plot
    h3->Divide(h2);
    h3->SetMarkerStyle(21);
    h3->Draw("ep");       // Draw the ratio plot

    // h1 settings
    h1->SetLineColor(kBlue+1);
    h1->SetLineWidth(2);

    // Y axis h1 plot settings
    h1->GetYaxis()->SetTitleSize(20);
    h1->GetYaxis()->SetTitleFont(43);
    h1->GetYaxis()->SetTitleOffset(1.55);

    // h2 settings
    h2->SetLineColor(kRed);
    h2->SetLineWidth(2);

    // Ratio plot (h3) settings
    h3->SetTitle(""); // Remove the ratio title

    // Y axis ratio plot settings
    h3->GetYaxis()->SetTitle("ratio h1/h2 ");
    h3->GetYaxis()->SetNdivisions(505);
    h3->GetYaxis()->SetTitleSize(20);
    h3->GetYaxis()->SetTitleFont(43);
    h3->GetYaxis()->SetTitleOffset(1.55);
    h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    h3->GetYaxis()->SetLabelSize(15);

    // X axis ratio plot settings
    if(*iter_variable == "mHH" || *iter_variable == "mMMC" || *iter_variable == "mHH")
    {
        h3->GetXaxis()->SetTitle((*iter_variable + "[GeV]").c_str());
    }
    else
    {
        h3->GetXaxis()->SetTitle((*iter_variable).c_str());
    }
    h3->GetXaxis()->SetTitleSize(20);
    h3->GetXaxis()->SetTitleFont(43);
    h3->GetXaxis()->SetTitleOffset(4.);
    h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    h3->GetXaxis()->SetLabelSize(15);

    gPad->Update();
    c->SaveAs((output_path + "/" + hist_name + ".png").c_str());
    delete h1;
    delete h2;
    delete h3;
    delete pad1;
    delete pad2;

    delete c;

}

std::vector<std::string>* GetRegions(){
}

std::vector<std::string>* GetVariables(){
}

std::map<std::string, int>* GetRebinFactors(){
}

std::vector<std::string>* GetBaseNames(){
}

std::map<std::string, std::string>* GetRecoBaseName(){
}

void KLambdaCompare( ) {
    // Define two gaussian histograms. Note the X and Y title are defined
    // at booking time using the convention "Hist_title ; X_title ; Y_title"
    // TH1F *h1 = new TH1F("h1", "Two gaussian plots and their ratio;x title; h1 and h2 gaussian histograms", 100, -5, 5);
    // TH1F *h2 = new TH1F("h2", "h2", 100, -5, 5);
    // h1->FillRandom("gaus");
    // h2->FillRandom("gaus");


    //std::string Dir = {"Preselection"};
    std::string input_dir = {"BDTScorePreselection"};
    auto KLReweight_py8 = new TFile("./output/KLReweight_py8_moreKLSignals.root", "READ");
    //auto KLReweight_herwig7 = new TFile("./output/KLReweight.root", "READ");
    auto KLReweight_Reco = new TFile("./output/KLRecoReweigt_py8.root", "READ");
    std::string output_path = {"./plotsForGenerateKL"};
    // Initialize STL containers, wich includes names of histograms
    std::vector<std::string>* regions;
    std::vector<std::string>* variables;
    std::map<std::string, int>* rebin_factors;
    std::vector<std::string>* base_names;
    std::map<std::string, std::string>* Reco_Basename;

    // Add regions
    regions.push_back("2tag2pjet_0ptv_LL_OS");
    // Add variables
    variables.push_back("mBB");
    variables.push_back("mMMC");
    variables.push_back("mHH");
    variables.push_back("mHHScaled");
    variables.push_back("dRBB");
    variables.push_back("pTBB");
    variables.push_back("dRTauTau");
    variables.push_back("pTTauTau");
    variables.push_back("dPhiBBTauTau");
    variables.push_back("dRBBTauTau");
    variables.push_back("pTBalance");
    variables.push_back("MET");
    variables.push_back("nJets");
    variables.push_back("Yield");
    variables.push_back("metSig");
    variables.push_back("metSigPU");
    variables.push_back("Jet0Pt");
    variables.push_back("Jet1Pt");
    variables.push_back("pTB0");
    variables.push_back("pTB1");
    variables.push_back("LeadJetPt");
    variables.push_back("SubleadJetPt");
    variables.push_back("LeadJetEta");
    variables.push_back("SubleadJetEta");
    variables.push_back("Jet0Eta");
    variables.push_back("Jet1Eta");
    variables.push_back("Tau0Pt");
    variables.push_back("Tau1Pt");
    variables.push_back("Tau0Eta");
    variables.push_back("Tau1Eta");
    variables.push_back("Tau0Ntrk");
    variables.push_back("Tau1Ntrk");
    variables.push_back("dPhiTauTau");
    variables.push_back("SMBDT");
    variables.push_back("SMNN");
    variables.push_back("PNN");


    // Add rebin factors
    rebin_factors.insert(std::pair<std::string, int>("mBB", 10));
    rebin_factors.insert(std::pair<std::string, int>("mMMC", 12));
    rebin_factors.insert(std::pair<std::string, int>("mHH", 12));
    rebin_factors.insert(std::pair<std::string, int>("mHHScaled", 12));
    rebin_factors.insert(std::pair<std::string, int>("dRBB", 4 ));
    rebin_factors.insert(std::pair<std::string, int>("pTBB", 10));
    rebin_factors.insert(std::pair<std::string, int>("dRTauTau", 4 ));
    rebin_factors.insert(std::pair<std::string, int>("pTTauTau", 10));
    rebin_factors.insert(std::pair<std::string, int>("dPhiBBTauTau", 4 ));
    rebin_factors.insert(std::pair<std::string, int>("dRBBTauTau", 5 ));
    rebin_factors.insert(std::pair<std::string, int>("pTBalance", 5 ));
    rebin_factors.insert(std::pair<std::string, int>("MET", 5 ));
    rebin_factors.insert(std::pair<std::string, int>("nJets", 1 ));
    rebin_factors.insert(std::pair<std::string, int>("Yield", 1 ));
    rebin_factors.insert(std::pair<std::string, int>("metSig", 2 ));
    rebin_factors.insert(std::pair<std::string, int>("metSigPU", 2 ));
    rebin_factors.insert(std::pair<std::string, int>("Jet0Pt", 10));
    rebin_factors.insert(std::pair<std::string, int>("Jet1Pt", 10));
    rebin_factors.insert(std::pair<std::string, int>("pTB0", 10));
    rebin_factors.insert(std::pair<std::string, int>("pTB1", 10));
    rebin_factors.insert(std::pair<std::string, int>("LeadJetPt", 10));
    rebin_factors.insert(std::pair<std::string, int>("SubleadJetPt", 10));
    rebin_factors.insert(std::pair<std::string, int>("LeadJetEta", 2 ));
    rebin_factors.insert(std::pair<std::string, int>("SubleadJetEta", 2 ));
    rebin_factors.insert(std::pair<std::string, int>("Jet0Eta", 2 ));
    rebin_factors.insert(std::pair<std::string, int>("Jet1Eta", 2 ));
    rebin_factors.insert(std::pair<std::string, int>("Tau0Pt", 10));
    rebin_factors.insert(std::pair<std::string, int>("Tau1Pt", 10));
    rebin_factors.insert(std::pair<std::string, int>("Tau0Eta", 2 ));
    rebin_factors.insert(std::pair<std::string, int>("Tau1Eta", 2 ));
    rebin_factors.insert(std::pair<std::string, int>("Tau0Ntrk", 1 ));
    rebin_factors.insert(std::pair<std::string, int>("Tau1Ntrk", 1 ));
    rebin_factors.insert(std::pair<std::string, int>("dPhiTauTau", 4 ));
    rebin_factors.insert(std::pair<std::string, int>("PNN", 20 ));
    rebin_factors.insert(std::pair<std::string, int>("SMBDT", 20 ));
    rebin_factors.insert(std::pair<std::string, int>("SMNN", 20 ));

    // push_back basenames
    std::vector<std::string> vec_basenames = {"hhttbbKL"};
    std::vector<std::string> vec_fromname = {"from1p0", "from10p0"};
    for(auto base_name = vec_basenames.begin(); base_name != vec_basenames.end(); base_name++){
        for(auto from_name = vec_fromname.begin(); from_name != vec_fromname.end(); from_name++){   
            for(float _KLambda=-20.; _KLambda <= 20.0; _KLambda = _KLambda ++){
                auto _basename = *base_name + transformFloat(_KLambda) + *from_name;
                base_names.push_back(_basename);
            }
        }
    }

    for(auto base_name = base_names.begin(); base_name != base_names.end();base_name++)
    {       
        if (*base_name == "hhttbbKL1p0" || *base_name == "hhttbbKL10p0" )
        {
            Reco_Basename.insert(std::pair<std::string, std::string>(*base_name, *base_name));       
            continue;
        }
        std::string basename_reco(*base_name);
        auto index_L = basename_reco.find("L");
        auto index_f = basename_reco.find("f");
        basename_reco = basename_reco.substr(0, index_f);
        basename_reco.insert(basename_reco.length(), "fromReco");
        cout << basename_reco << endl;
        Reco_Basename.insert(std::pair<std::string, std::string>(*base_name, basename_reco));

    }

    auto dir_py8 = (TDirectory*)KLReweight_py8->Get(input_dir.c_str());
    // auto dir_herwig7 = (TDirectory*)KLReweight_herwig7->Get("Preselection");
    auto dir_Reco = (TDirectory*)KLReweight_Reco->Get(input_dir.c_str());
    // Construct the full variable set!
    for (auto iter_basename = base_names.begin(); iter_basename != base_names.end(); iter_basename++)
    {
        for (auto iter_variable = variables.begin(); iter_variable != variables.end(); iter_variable++)
        {
            for (auto iter_region = regions.begin(); iter_region != regions.end(); iter_region++)
            {   
                auto hist_name = *iter_basename + '_' + *iter_region + '_' + *iter_variable;
                auto hist_name_reco = Reco_Basename.at(*iter_basename) + '_' + *iter_region +"_"+ *iter_variable; 
                if(!dir_py8->Get(hist_name.c_str()) || !dir_Reco->Get(hist_name_reco.c_str()))
                {
                    cout << "WARNING:hist_name not found, continue" << endl;
                    cout << hist_name << endl;
                    cout << hist_name_reco << endl;
                    continue;
                }
                auto h1 = (TH1F*)dir_py8->Get(hist_name.c_str())->Clone();
                //auto h2 = (TH1F*)dir_herwig7->Get(hist_name.c_str())->Clone();
                auto h2 = (TH1F*)dir_Reco->Get(hist_name_reco.c_str())->Clone();
                DrawRatioPlot(h1, h2);
            }
        }
    }

    KLReweight_py8->Close();
    //KLReweight_herwig7->Close();
}


