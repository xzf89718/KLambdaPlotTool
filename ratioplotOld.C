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
#include <map>

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



void ratioplotOld( ) {
        // Define two gaussian histograms. Note the X and Y title are defined
        // at booking time using the convention "Hist_title ; X_title ; Y_title"
        // TH1F *h1 = new TH1F("h1", "Two gaussian plots and their ratio;x title; h1 and h2 gaussian histograms", 100, -5, 5);
        // TH1F *h2 = new TH1F("h2", "h2", 100, -5, 5);
        // h1->FillRandom("gaus");
        // h2->FillRandom("gaus");
        //
        std::string Dir = {"Preselection"};
        auto KLReweight_py8 = new TFile("./output/KLReweight_py8.root", "READ");
        auto KLReweight_herwig7 = new TFile("./output/KLReweight.root", "READ");
        std::string output_path = {"./comparePythia8AndHerwig7"};
        // Initialize STL containers, wich includes names of histograms
        std::vector<std::string> regions;
        std::vector<std::string> variables;
        std::map<std::string, int> rebin_factors;
        std::vector<std::string> base_names;


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

        // Add base names
        base_names.push_back("hhttbbKL1p0");
        base_names.push_back("hhttbbKL2p0from1p0");
        base_names.push_back("hhttbbKL3p0from1p0");
        base_names.push_back("hhttbbKL5p0from1p0");
        base_names.push_back("hhttbbKL10p0");
        base_names.push_back("hhttbbKL20p0from1p0");
        base_names.push_back("hhttbbKL0p0from1p0");
        base_names.push_back("hhttbbKLn1p0from1p0");
        base_names.push_back("hhttbbKLn3p0from1p0");
        base_names.push_back("hhttbbKLn4p0from1p0");
        base_names.push_back("hhttbbKLn5p0from1p0");
        base_names.push_back("hhttbbKLn10p0from1p0");
        base_names.push_back("hhttbbKLn15p0from1p0");
        base_names.push_back("hhttbbKL1p0");
        base_names.push_back("hhttbbKL2p0from10p0");
        base_names.push_back("hhttbbKL3p0from10p0");
        base_names.push_back("hhttbbKL5p0from10p0");
        base_names.push_back("hhttbbKL10p0");
        base_names.push_back("hhttbbKL20p0from10p0");
        base_names.push_back("hhttbbKL0p0from10p0");
        base_names.push_back("hhttbbKLn1p0from10p0");
        base_names.push_back("hhttbbKLn3p0from10p0");
        base_names.push_back("hhttbbKLn4p0from10p0");
        base_names.push_back("hhttbbKLn5p0from10p0");
        base_names.push_back("hhttbbKLn10p0from10p0");
        base_names.push_back("hhttbbKLn15p0from10p0");
        
        gStyle->SetOptStat(0);
        auto dir_py8 = (TDirectory*)KLReweight_py8->Get("Preselection");
        auto dir_herwig7 = (TDirectory*)KLReweight_herwig7->Get("Preselection");

        for (auto iter_basename = base_names.begin(); iter_basename != base_names.end(); iter_basename++)
        {
                for (auto iter_variable = variables.begin(); iter_variable != variables.end(); iter_variable++)
                {
                        for (auto iter_region = regions.begin(); iter_region != regions.end(); iter_region++)
                        {
                                auto hist_name = *iter_basename + '_' + *iter_region + '_' + *iter_variable;
                                cout << hist_name << endl;
                                if(!dir_py8->Get(hist_name.c_str()))
                                {
                                        cout << "hist_name not found, continue" << endl;
                                        continue;
                                }
                                auto h1 = (TH1F*)dir_py8->Get(hist_name.c_str())->Clone();
                                auto h2 = (TH1F*)dir_herwig7->Get(hist_name.c_str())->Clone();
                                //cout<< h1->GetNbinsX() <<endl;
                                h1->Rebin(rebin_factors.at(*iter_variable));
                                h2->Rebin(rebin_factors.at(*iter_variable));

                                // Define the Canvas
                                TCanvas *c = new TCanvas("c", "canvas", 800, 800);

                                // Upper plot will be in pad1
                                TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
                                pad1->SetBottomMargin(1); // Upper and lower plot are joined
                                pad1->SetGridx();         // Vertical grid
                                pad1->Draw();             // Draw the upper pad: pad1
                                pad1->cd();               // pad1 becomes the current pad
                                h1->SetStats(0);          // No statistics on upper plot
                                h1->Draw();               // Draw h1
                                h2->Draw("same");         // Draw h2 on top of h1
                                // Add legend for pad1
                                auto legend1 = new TLegend(0.1, 0.7, 0.48, 0.9);
                                legend1->SetHeader("Legend", "C");
                                legend1->AddEntry(h1, "Pythia8", "f");
                                legend1->AddEntry(h2, "Herwig7", "f");
                                legend1->Draw();
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
                                pad2->SetGridx(); // vertical grid
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
                                h3->GetXaxis()->SetTitle((*iter_variable + "[GeV]").c_str());
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
                                delete legend1;

                                delete c;
                        }
                }
        }

        KLReweight_py8->Close();
        KLReweight_herwig7->Close();
}
