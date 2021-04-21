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

#include "ggFKlambdaCombinedTool.hh"
#include "HistNameManager.hh"

using std::cerr;
using std::clog;
using std::cout;
using std::endl;
using std::pow;

std::string transformFloat(float KLambda)
{
    // input: any float KLambda
    // output: somethin like (string) 1p0

    // A kindly remind here: dure to the limit of std:to_string, this program cant handle
    // input more than 5 digit decimal
    // Transform input KLambda to string form, such as 1p00000
    std::string string_KLambda = std::to_string(KLambda);
    auto index_of_point = string_KLambda.find(".");
    if (index_of_point != -1)
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

int CheckHist(TDirectory *dir_input)
{
    auto h0 = (TH1F *)dir_input->Get(h0_name.c_str());
    auto h1 = (TH1F *)dir_input->Get(h1_name.c_str());
    auto h20 = (TH1F *)dir_input->Get(h20_name.c_str());

    if (!h0)
    {
        clog << "Warning: Cant find base histogram h0" << endl;
    }
    if (!h1)
    {
        clog << "Warning: Cant find base histogram h1" << endl;
    }
    if (!h20)
    {
        clog << "Warning: Cant find base histogram h20" << endl;
    }
    if (!h0 || !h1 || !h20)
    {
        clog << "This histogram is not exist in the base_file, skip" << endl;
        delete h0;
        delete h1;
        delete h20;
        return 0;
    }
    // Add h1, h10, h20 follow the fomula
    //
    delete h0;
    delete h1;
    delete h20;
    return 1;
}

TH1F *GetCombinedHist(TH1F *h0, TH1F *h1, TH1F *h20)
{

    h0 = (TH1F *)dir_input->Get(h0_name.c_str())->Clone();
    h1 = (TH1F *)dir_input->Get(h1_name.c_str())->Clone();
    h20 = (TH1F *)dir_input->Get(h20_name.c_str())->Clone();

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
    auto k1 = Kappa_t * Kappa_t *
              ((pow(d_lambda, 2) - d_lambda) * pow(Kappa_t, 2) +
               (d_lambda - 1) * pow(KLambda, 2) - (pow(d_lambda, 2) - 1.) * Kappa_t * KLambda) /
              (pow(d_lambda, 2) - d_lambda);
    auto k2 = Kappa_t * Kappa_t *
              (pow(d_lambda, 2) * Kappa_t * KLambda - d_lambda * pow(KLambda, 2)) / (pow(d_lambda, 2) - d_lambda);
    auto k3 = Kappa_t * Kappa_t *
              (pow(KLambda, 2) - Kappa_t * KLambda) / (pow(d_lambda, 2) - d_lambda);
    h_cup->Sumw2();
    h_cup->Add(h0, h1,
               k1,
               k2);
    auto h_cup_2 = new TH1F(*h_cup);
    h_cup_2->Sumw2();
    h_cup_2->Add(h_cup_2, h20,
                 1,
                 k3);
    h_cup_2->SetName(hist_name_reweighted.c_str());
    h_cup_2->SetTitle(hist_name_reweighted.c_str());

    return h_cup_2
}