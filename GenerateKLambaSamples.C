// This macro should be used as 
// root -b -q -q THISMACRO.C+ only!

// Author: Zifeng, Xu
// Usage: Generate differente signal templates using reco-level histograms
// Search HERE_IS_BASE_FILE, modify it to your own generated .root file
// which contains KL=0, 1, 20 samples!
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <fstream>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TList.h"
#include "TDirectory.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TStyle.h"
#include "TLegend.h"
//#define DEBUG_KLREWEIGT 1

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

void GenerateKLambaSamples(const double KLambda)
{   
    using std::cout;
    using std::endl;
    using std::cerr;
    using std::clog;
    using std::pow;
    clog << "GenerateKLambaSamples::WARNING make sure you have the directory in CxAOD production!" << endl;
    // Base not reweight!
    //  if (KLambda == 1.0 || KLambda == 0.0 || KLambda == 20.0)
    //  {
    //      clog << "KL equals to one of the base, skip KLambda = " << KLambda << endl;
    //      return;
    //  }

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

    // STL containeers for variable, region and base
    vector<std::string> variable_names;
    vector<std::string> ML_variable_names;
    vector<std::string> region_names;
    //vector<std::string> base_names;
    std::map<std::string, std::string> base_names;

    //Add all variables to variable_names
    variable_names.push_back("mBB");
    variable_names.push_back("mMMC");
    variable_names.push_back("mHH");
    variable_names.push_back("mHHScaled");
    variable_names.push_back("dRBB");
    variable_names.push_back("pTBB");
    variable_names.push_back("dRTauTau");
    variable_names.push_back("pTTauTau");
    variable_names.push_back("dPhiBBTauTau");
    variable_names.push_back("dRBBTauTau");
    variable_names.push_back("pTBalance");
    variable_names.push_back("MET");
    variable_names.push_back("nJets");
    variable_names.push_back("Yield");
    variable_names.push_back("metSig");
    variable_names.push_back("metSigPU");
    variable_names.push_back("Jet0Pt");
    variable_names.push_back("Jet1Pt");
    variable_names.push_back("pTB0");
    variable_names.push_back("pTB1");
    variable_names.push_back("LeadJetPt");
    variable_names.push_back("SubleadJetPt");
    variable_names.push_back("LeadJetEta");
    variable_names.push_back("SubleadJetEta");
    variable_names.push_back("Jet0Eta");
    variable_names.push_back("Jet1Eta");
    variable_names.push_back("Tau0Pt");
    variable_names.push_back("Tau1Pt");
    variable_names.push_back("Tau0Eta");
    variable_names.push_back("Tau1Eta");
    variable_names.push_back("Tau0Ntrk");
    variable_names.push_back("Tau1Ntrk");
    variable_names.push_back("dPhiTauTau");
    // Variable names for BDT, PNN and NN
    // BDT
    ML_variable_names.push_back("SMBDT");
    ML_variable_names.push_back("PNN");
    ML_variable_names.push_back("SMNN");

    // Add region names
    region_names.push_back("2tag2pjet_0ptv_LL_OS");
    region_names.push_back("2tag2pjet_0ptv_LL_SS");
    region_names.push_back("1tag2pjet_0ptv_LL_OS");
    region_names.push_back("1tag2pjet_0ptv_LL_SS");
    // Add base_sample name
    // Here we use KL 1p0 10p0 generated and KL 20p0 reweighted from 10p0 
    // (now maybe we can switch to reweigted from 1p0 in the next step!)
    base_names.insert(std::pair<std::string, std::string>("base", "hhttbbKL"));
    base_names.insert(std::pair<std::string, std::string>("1p0", "hhttbbKL1p0"));
    base_names.insert(std::pair<std::string, std::string>("10p0", "hhttbbKL10p0"));
    // base_names.push_back("hhttbbKL20p0from1p0");
    base_names.insert(std::pair<std::string, std::string>("20p0", "hhttbbKL20p0from10p0"));
    base_names.insert(std::pair<std::string, std::string>("0p0", "hhttbbKL0p0from1p0"));
    // Check if base_file exist
    auto base_file = new TFile("./output/KLReweight_py8_moreKLSignals.root", "READ");
    if (!base_file)
    {
        cerr << "error: " << "Can't open basefile, Check HERE_IS_BASE!" << endl;
        return;
    }
    auto output_file = new TFile("./output/KLRecoReweight_py8.root", "UPDATE");
    if (!output_file)
    {
        cerr << "error: " << "Can't open outputfile, Check HERE_IS_BASE!" << endl;
        return;
    }
    // write base infomation into the output_file
    double base1 = 0;
    double base2 = 1;
    double base3 = 20;
    // Now only consider Preselection
    vector<std::string> vec_dirnames;
    std::string dirName("Preselection");
    std::string dirBDT("BDTScorePreselection");
    std::string dirPNN("PNNScorePreselection");
    std::string dirNN("NNScorePreselection");
    vec_dirnames.push_back(dirName);
    vec_dirnames.push_back(dirBDT);
    vec_dirnames.push_back(dirPNN);
    vec_dirnames.push_back(dirNN);

    for(auto dirname_input = vec_dirnames.begin(); dirname_input != vec_dirnames.end(); dirname_input++){
        auto dir_input = (TDirectory*)base_file->Get(dirname_input->c_str());
        if (!dir_input)
        {
            cerr << "error: " << "Can't open TDirectory, Check dirName! " << *dirname_input << endl;
            return;
        }
    }

    // Create all Preselection directory in output_file and write histogram
    if (!output_file->Get(dirName.c_str()))
    {   
        output_file->mkdir(dirName.c_str());
        clog << "Warning: Cant find Preselection in output file, mkdir!" << endl;
    }
    if(!output_file->Get(dirNN.c_str())){
        output_file->mkdir(dirNN.c_str());
    }
    if(!output_file->Get(dirBDT.c_str())){
        output_file->mkdir(dirBDT.c_str());
    }
    if(!output_file->Get(dirPNN.c_str())){   
        output_file->mkdir(dirPNN.c_str());
    }

    for(auto dirname_input = vec_dirnames.begin(); dirname_input != vec_dirnames.end(); dirname_input++){
        auto dir_input = (TDirectory*)base_file->Get(dirname_input->c_str());
        output_file->cd(dirname_input->c_str());
        cout << "Dirname we work now:" << *dirname_input << endl;
        enum variable_set {preselection, BDT_and_NN, PNN, do_nothing};
        auto expression = do_nothing;
        if(*dirname_input == "Preselection"){
            expression = preselection;
        }
        else if(*dirname_input == "BDTScorePreselection"){
            expression = BDT_and_NN;
        }
        else if (*dirname_input == "NNScorePreselection"){
            expression = BDT_and_NN;
        }
        else if(*dirname_input == "PNNScorePreselection"){
            expression = PNN;
        }
        else{
            expression = do_nothing;
        }

        switch(expression){
            case preselection:
                {
                    // start to combine the histogram and write ! 
                    for (auto iter_variable = variable_names.begin(); iter_variable != variable_names.end(); iter_variable ++)
                    {       
                        for(auto iter_region = region_names.begin(); iter_region != region_names.end(); iter_region ++)
                        {
                            // Reweighted histogram name
                            auto hist_name_reweighted = base_names.at("base") + string_KLambda + "fromReco" + \
                                                        "_" + *iter_region + "_" + *iter_variable;
                            // Get three base histogram
                            auto h0_name = base_names.at("0p0") + "_" + *iter_region + "_" + *iter_variable;
                            auto h1_name = base_names.at("1p0") + "_" + *iter_region + "_" + *iter_variable;
                            auto h20_name = base_names.at("20p0") + "_" + *iter_region + "_" + *iter_variable;

                            auto h0 = (TH1F*)dir_input->Get(h0_name.c_str());
                            auto h1 = (TH1F*)dir_input->Get(h1_name.c_str());
                            auto h20 = (TH1F*)dir_input->Get(h20_name.c_str());

                            if(!h0)
                            {
                                clog << "Warning: Cant find base histogram h0" << endl;
                            }
                            if(!h1)
                            {
                                clog << "Warning: Cant find base histogram h1" << endl;
                            }
                            if(!h20)
                            {
                                clog << "Warning: Cant find base histogram h20" << endl;
                            }
                            if(!h0 || !h1 || !h20)
                            {        
                                clog << "This histogram is not exist in the base_file, skip" << endl;
                                continue;
                            }
                            // Add h1, h10, h20 follow the fomula
                            //
                            delete h0;
                            delete h1;
                            delete h20;

                            h0 = (TH1F*)dir_input->Get(h0_name.c_str())->Clone();
                            h1 = (TH1F*)dir_input->Get(h1_name.c_str())->Clone();
                            h20 = (TH1F*)dir_input->Get(h20_name.c_str())->Clone();

                            // For more details about the method here, check Alessandra Betti's talk 
                            // Methods to obtain signal templates for HH signals with couplings variations
                            // We take KT = 1 here
                            auto h_cup = new TH1F(*h0);
                            // here are the 3 coefficient
                            //                        double k1 = 200. / 171. - KLambda * 10. / 57. + pow(KLambda, 2) * 1 / 171;
                            //                        double k2 = (-2. / 9.0) + KLambda * 7. / 30. - pow(KLambda, 2) * 1. / 90.;
                            //                        double k3 = 1. / 19. - KLambda * 11. / 190. + pow(KLambda, 2) * 1. / 190.;
                            //                      

                            // now consider kappa_t = 1, kappa_lambda
                            // Edited by Zifeng at March, 8th, 2021, correct the k1, k2, k3 here, add lambda!
                            auto Kappa_t = 1.0;
                            // now switch to lambda = 20.0
                            auto d_lambda = 20.0;
                            auto k1 = Kappa_t * Kappa_t * \
                                      ((pow(d_lambda, 2) - d_lambda) * pow(Kappa_t, 2) + \
                                       (d_lambda - 1) * pow(KLambda, 2) - (pow(d_lambda, 2) -1.) * Kappa_t * KLambda) / (pow(d_lambda, 2) - d_lambda);
                            auto k2 = Kappa_t * Kappa_t * \
                                      (pow(d_lambda, 2) * Kappa_t * KLambda - d_lambda * pow(KLambda, 2)) / (pow(d_lambda, 2) - d_lambda);
                            auto k3 = Kappa_t * Kappa_t * \
                                      (pow(KLambda, 2) - Kappa_t * KLambda) / (pow(d_lambda, 2) - d_lambda);
                            h_cup->Sumw2();
                            h_cup->Add(h0, h1, \
                                    k1,\
                                    k2);
                            auto h_cup_2 = new TH1F(*h_cup);
                            h_cup_2->Sumw2();
                            h_cup_2->Add(h_cup_2, h20,\
                                    1,
                                    k3);
                            h_cup_2->SetName(hist_name_reweighted.c_str());
                            h_cup_2->SetTitle(hist_name_reweighted.c_str());
#ifdef DEBUG_KLREWEIGT  
                            gROOT->SetStyle("ATLAS");
                            std::ofstream debug_k;
                            debug_k.open("./output/All_k.txt",std::ios::app);
                            debug_k << "KLambda: " <<KLambda <<" k1: " << k1<<" k2: " << k2<<" k3: " <<k3 <<endl;
                            debug_k.close();
                            auto c1 = new TCanvas("c1", "c1", 800*1.5, 600*1.5);
                            gStyle->SetOptStat(0);
                            h_cup_2->Draw("HIST");
                            h1->Draw("HIST SAME");
                            h20->Draw("HIST SAME");
                            h0->Draw("HIST SAME");
                            h0->SetLineColor(kRed);
                            h1->SetLineColor(kGreen);
                            h20->SetLineColor(kRed + 2);
                            h_cup_2->SetLineColor(kGreen + 2);
                            c1->BuildLegend();
                            // c1->SetLogy();
                            c1->SaveAs(("./output/plots/" + hist_name_reweighted + ".png").c_str());
                            //cout << "hist_name_reweighted: " << hist_name_reweighted << endl;
                            delete c1;

                            // here i want to show the error propagation!
                            auto c2 = new TCanvas("c2", "c2", 800*1.5, 600*1.5);
                            //gROOT->SetStyle(0);
                            // Get reweighted from basefile (truth-level) 
                            auto hist_name_truth = base_names.at("base") + string_KLambda + "from1p0" + "_" + *iter_region + "_" + *iter_variable;
                            //cout << "DEBUG: " << hist_name_truth << endl;
                            // create 3 TGraph to store error of 3 basis!
                            auto NBinX = (Int_t)h0->GetNbinsX();
                            auto error_0 = new float[NBinX];
                            auto error_1 = new float[NBinX];
                            auto error_20 = new float[NBinX];
                            auto h_truth = (TH1F*)dir_input->Get(hist_name_truth.c_str());
                            auto error_truth = new float[NBinX];
                            // validate error propagate in truth-reweight level!
                            auto bincontent_1 = new float[NBinX];
                            auto bincontent_truth = new float[NBinX];
                            if(!h_truth)
                            {
                                clog << "Cant find h_truth, no h_truth has been added!" << endl;
                                delete h_truth;
                            }
                            else
                            {
                                delete h_truth;
                                h_truth = (TH1F*)dir_input->Get(hist_name_truth.c_str())->Clone();
                            }

                            // create 1 TGraph to store error of combined one!
                            auto error_reweight = new float[NBinX];
                            auto error_calculated = new float[NBinX];

                            // XAxis for TGraph
                            auto x_for_tgraph = new float[NBinX];
                            for (auto i = 0; i < NBinX; i++)
                            {   
                                error_0[i]= h0->GetBinError(i);
                                error_1[i] = h1->GetBinError(i);
                                bincontent_1[i] = h1->GetBinContent(i);
                                error_20[i] = h20->GetBinError(i);
                                if(h_truth)
                                {   
                                    auto content_truth = h_truth->GetBinContent(i);
                                    if (content_truth > 0.0)
                                    {
                                        error_truth[i] = (h_truth->GetBinError(i)) / (content_truth) ;
                                    }
                                    else
                                    {
                                        error_truth[i] = -1;
                                    }
                                    bincontent_truth[i] = h_truth->GetBinContent(i) ;
                                }
                                // Get error of reweight one
                                auto content_cup_2 = h_cup_2->GetBinContent(i);
                                if (content_cup_2 > 0.0)
                                {
                                    error_reweight[i] = (h_cup_2->GetBinError(i)) / content_cup_2;
                                }
                                else
                                {
                                    error_reweight[i] = -1;
                                }
                                // try to recalculate!
                                error_calculated[i] = std::sqrt(pow(error_0[i] * k1, 2) + pow(error_1[i] * k2, 2) + pow(error_20[i] * k3, 2));
                                x_for_tgraph[i] = h0->GetBinCenter(i);

                                //cout << "cal error: " <<error_calculated[i]<< " reweighted error: "<< error_reweight[i] \
                                << " KL=1 error: " << error_1[i] << " propagate: " << bincontent_truth[i] / bincontent_1[i] \
                                    <<" is it fit my assumption?" << (bincontent_truth[i]/bincontent_truth[i] * error_1[i] - error_truth[i]  < 0.00001) << endl;

                            }
                            // Initialize TGraph for them
                            auto g0 = new TGraph(NBinX, x_for_tgraph, error_0); g0->SetMarkerStyle(10); g0->SetName("g0");
                            auto g1 = new TGraph(NBinX, x_for_tgraph, error_1); g1->SetMarkerStyle(12); g1->SetName("g1");
                            auto g20 = new TGraph(NBinX, x_for_tgraph, error_20); g20->SetMarkerStyle(14); g20->SetName("g20");
                            auto gtruth = new TGraph(NBinX, x_for_tgraph, error_truth); \
                                          gtruth->SetMarkerStyle(21);gtruth->SetMarkerColor(2); gtruth->SetName("reweight_from_truth"); gtruth->SetTitle("reweigth_from_truth");\
                                          gtruth->SetDrawOption("AP");
                            auto greweight = new TGraph(NBinX, x_for_tgraph, error_reweight); \
                                             greweight->SetMarkerStyle(22);greweight->SetMarkerColor(4); greweight->SetName("combine_at_reco-level"); greweight->SetTitle("combine_at_reco-level");\
                                             greweight->SetDrawOption("AP");
                            auto gcalculated = new TGraph(NBinX, x_for_tgraph, error_calculated); \
                                               gcalculated->SetMarkerStyle(23);gcalculated->SetMarkerColor(6); gcalculated->SetName("gcalculated"); gcalculated->SetTitle("gcalculated"); \
                                               gcalculated->SetDrawOption("AP");
                            //g0->Draw();
                            //g1->Draw("SAME");
                            //g20->Draw("SAME");
                            // Let's try multigraph
                            //
                            auto mg = new TMultiGraph("mg", "Shape_only");
                            if(h_truth)
                            {
                                mg->Add(gtruth);
                                mg->Add(greweight);
                                //mg->Add(greweight);
                                //mg->Add(gcalculated);

                            }
                            else
                            {
                                //mg->Add(gtruth);
                                //mg->Add(gcalculated);
                            }
                            mg->Draw("AP");
                            gPad->Update();
                            gPad->BuildLegend();
                            mg->GetXaxis()->SetTitle((*iter_variable).c_str());
                            mg->GetYaxis()->SetTitle("Error");
                            gPad->Update();
                            c2->SaveAs(("./output/debug_plots/" + hist_name_reweighted + ".png").c_str());

                            delete g0;
                            delete g1;
                            delete g20;
                            delete gtruth;
                            delete greweight;
                            delete gcalculated;
                            delete mg;

                            delete []x_for_tgraph;
                            delete []error_0;
                            delete []error_1;
                            delete []error_20;
                            delete []error_truth;
                            delete []error_reweight;
                            delete []error_calculated;
                            delete []bincontent_truth;
                            delete []bincontent_1;
                            delete c2;
#endif
                            h_cup_2->Write();
                            delete h0;
                            delete h1;
                            delete h20;
                            delete h_cup_2;
                            delete h_cup;
                        }
                    }
                    break;
                }
            case BDT_and_NN:
                {
                    for (auto iter_variable = ML_variable_names.begin(); iter_variable != ML_variable_names.end(); iter_variable ++)
                    {       
                        for(auto iter_region = region_names.begin(); iter_region != region_names.end(); iter_region ++)
                        {
                            // Reweighted histogram name
                            auto hist_name_reweighted = base_names.at("base") + string_KLambda + "fromReco" + \
                                                        "_" + *iter_region + "_" + *iter_variable;
                            cout << hist_name_reweighted << endl;
                            // Get three base histogram
                            auto h0_name = base_names.at("0p0") + "_" + *iter_region + "_" + *iter_variable;
                            auto h1_name = base_names.at("1p0") + "_" + *iter_region + "_" + *iter_variable;
                            auto h20_name = base_names.at("20p0") + "_" + *iter_region + "_" + *iter_variable;

                            auto h0 = (TH1F*)dir_input->Get(h0_name.c_str());
                            auto h1 = (TH1F*)dir_input->Get(h1_name.c_str());
                            auto h20 = (TH1F*)dir_input->Get(h20_name.c_str());

                            if(!h0)
                            {
                                clog << "Warning: Cant find base histogram h0" << endl;
                            }
                            if(!h1)
                            {
                                clog << "Warning: Cant find base histogram h1" << endl;
                            }
                            if(!h20)
                            {
                                clog << "Warning: Cant find base histogram h20" << endl;
                            }
                            if(!h0 || !h1 || !h20)
                            {        
                                clog << "This histogram is not exist in the base_file, skip" << endl;
                                continue;
                            }
                            // Add h1, h10, h20 follow the fomula
                            //
                            delete h0;
                            delete h1;
                            delete h20;

                            h0 = (TH1F*)dir_input->Get(h0_name.c_str())->Clone();
                            h1 = (TH1F*)dir_input->Get(h1_name.c_str())->Clone();
                            h20 = (TH1F*)dir_input->Get(h20_name.c_str())->Clone();

                            // For more details about the method here, check Alessandra Betti's talk 
                            // Methods to obtain signal templates for HH signals with couplings variations
                            // We take KT = 1 here
                            auto h_cup = new TH1F(*h0);
                            // here are the 3 coefficient
                            //                        double k1 = 200. / 171. - KLambda * 10. / 57. + pow(KLambda, 2) * 1 / 171;
                            //                        double k2 = (-2. / 9.0) + KLambda * 7. / 30. - pow(KLambda, 2) * 1. / 90.;
                            //                        double k3 = 1. / 19. - KLambda * 11. / 190. + pow(KLambda, 2) * 1. / 190.;
                            //                      

                            // now consider kappa_t = 1, kappa_lambda
                            // Edited by Zifeng at March, 8th, 2021, correct the k1, k2, k3 here, add lambda!
                            auto Kappa_t = 1.0;
                            // now switch to lambda = 20.0
                            auto d_lambda = 20.0;
                            auto k1 = Kappa_t * Kappa_t * \
                                      ((pow(d_lambda, 2) - d_lambda) * pow(Kappa_t, 2) + \
                                       (d_lambda - 1) * pow(KLambda, 2) - (pow(d_lambda, 2) -1.) * Kappa_t * KLambda) / (pow(d_lambda, 2) - d_lambda);
                            auto k2 = Kappa_t * Kappa_t * \
                                      (pow(d_lambda, 2) * Kappa_t * KLambda - d_lambda * pow(KLambda, 2)) / (pow(d_lambda, 2) - d_lambda);
                            auto k3 = Kappa_t * Kappa_t * \
                                      (pow(KLambda, 2) - Kappa_t * KLambda) / (pow(d_lambda, 2) - d_lambda);
                            h_cup->Sumw2();
                            h_cup->Add(h0, h1, \
                                    k1,\
                                    k2);
                            auto h_cup_2 = new TH1F(*h_cup);
                            h_cup_2->Sumw2();
                            h_cup_2->Add(h_cup_2, h20,\
                                    1,
                                    k3);
                            h_cup_2->SetName(hist_name_reweighted.c_str());
                            h_cup_2->SetTitle(hist_name_reweighted.c_str());
                            h_cup_2->Write();
                            delete h0;
                            delete h1;
                            delete h20;
                            delete h_cup_2;
                            delete h_cup;
                        }
                    }
                    break;
                }
            case PNN:
                {
                    // For PNN , something is more complicated!
                    // Because the exatly variable here is not simply PNN, we need to construct all variable with the function!

                    // Construct a list for variable here!
                    vector<std::string> vec_PNN_klambda;
                    for(float i=-20; i<21; i++){
                        vec_PNN_klambda.push_back(transformFloat(i));
                        cout << "Check it carefully:" << transformFloat(i) << endl;
                    }
                    /* I try two ways to combine PNNScore. The first is to use base with klambda of base, i.e. 
                     * hhttbbKL1p0...PNN1p0, httbbKL0p0...0p0, hhttbbKL20p0...20p0
                     * */
                    for(auto iter_PNN_klambda = vec_PNN_klambda.begin(); iter_PNN_klambda != vec_PNN_klambda.end(); iter_PNN_klambda++){
                        for (auto iter_variable = ML_variable_names.begin(); iter_variable != ML_variable_names.end(); iter_variable ++)
                        {       
                            for(auto iter_region = region_names.begin(); iter_region != region_names.end(); iter_region ++)
                            {
                                // Reweighted histogram name
                                auto hist_name_reweighted = base_names.at("base") + string_KLambda + "fromReco" + \
                                                            "_" + *iter_region + "_" + *iter_variable + *iter_PNN_klambda;
                                // Get three base histogram
                                auto h0_name = base_names.at("0p0") + "_" + *iter_region + "_" + *iter_variable + *iter_PNN_klambda;
                                auto h1_name = base_names.at("1p0") + "_" + *iter_region + "_" + *iter_variable + *iter_PNN_klambda;
                                auto h20_name = base_names.at("20p0") + "_" + *iter_region + "_" + *iter_variable + *iter_PNN_klambda;

                                auto h0 = (TH1F*)dir_input->Get(h0_name.c_str());
                                auto h1 = (TH1F*)dir_input->Get(h1_name.c_str());
                                auto h20 = (TH1F*)dir_input->Get(h20_name.c_str());

                                if(!h0)
                                {
                                    clog << "Warning: Cant find base histogram h0" << endl;
                                }
                                if(!h1)
                                {
                                    clog << "Warning: Cant find base histogram h1" << endl;
                                }
                                if(!h20)
                                {
                                    clog << "Warning: Cant find base histogram h20" << endl;
                                }
                                if(!h0 || !h1 || !h20)
                                {        
                                    clog << "This histogram is not exist in the base_file, skip" << endl;
                                    continue;
                                }
                                // Add h1, h10, h20 follow the fomula
                                //
                                delete h0;
                                delete h1;
                                delete h20;

                                h0 = (TH1F*)dir_input->Get(h0_name.c_str())->Clone();
                                h1 = (TH1F*)dir_input->Get(h1_name.c_str())->Clone();
                                h20 = (TH1F*)dir_input->Get(h20_name.c_str())->Clone();

                                // For more details about the method here, check Alessandra Betti's talk 
                                // Methods to obtain signal templates for HH signals with couplings variations
                                // We take KT = 1 here
                                auto h_cup = new TH1F(*h0);
                                // here are the 3 coefficient
                                //                        double k1 = 200. / 171. - KLambda * 10. / 57. + pow(KLambda, 2) * 1 / 171;
                                //                        double k2 = (-2. / 9.0) + KLambda * 7. / 30. - pow(KLambda, 2) * 1. / 90.;
                                //                        double k3 = 1. / 19. - KLambda * 11. / 190. + pow(KLambda, 2) * 1. / 190.;
                                //                      

                                // now consider kappa_t = 1, kappa_lambda
                                // Edited by Zifeng at March, 8th, 2021, correct the k1, k2, k3 here, add lambda!
                                auto Kappa_t = 1.0;
                                // now switch to lambda = 20.0
                                auto d_lambda = 20.0;
                                auto k1 = Kappa_t * Kappa_t * \
                                          ((pow(d_lambda, 2) - d_lambda) * pow(Kappa_t, 2) + \
                                           (d_lambda - 1) * pow(KLambda, 2) - (pow(d_lambda, 2) -1.) * Kappa_t * KLambda) / (pow(d_lambda, 2) - d_lambda);
                                auto k2 = Kappa_t * Kappa_t * \
                                          (pow(d_lambda, 2) * Kappa_t * KLambda - d_lambda * pow(KLambda, 2)) / (pow(d_lambda, 2) - d_lambda);
                                auto k3 = Kappa_t * Kappa_t * \
                                          (pow(KLambda, 2) - Kappa_t * KLambda) / (pow(d_lambda, 2) - d_lambda);
                                h_cup->Sumw2();
                                h_cup->Add(h0, h1, \
                                        k1,\
                                        k2);
                                auto h_cup_2 = new TH1F(*h_cup);
                                h_cup_2->Sumw2();
                                h_cup_2->Add(h_cup_2, h20,\
                                        1,
                                        k3);
                                h_cup_2->SetName(hist_name_reweighted.c_str());
                                h_cup_2->SetTitle(hist_name_reweighted.c_str());
                                h_cup_2->Write();
                                delete h0;
                                delete h1;
                                delete h20;
                                delete h_cup_2;
                                delete h_cup;
                            }
                        }
                    }
                    break;
                }
            default:
                clog << "There's no such type in the input file, continue!" << endl;
                break;
        }

        // Go back to ../
        gDirectory->cd("../");
    }
}
