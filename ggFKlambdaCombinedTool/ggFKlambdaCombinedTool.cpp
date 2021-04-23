#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <fstream>
#include <cstdlib>

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

ggFKlambdaCombinedTool::ggFKlambdaCombinedTool()
{
    clog << "ggFKlambdaCombinedTool::ggFKlambdaCombinedTool: WARNING: default initialize, you may meet some problems! " << endl;
}

ggFKlambdaCombinedTool::~ggFKlambdaCombinedTool()
{
    delete m_input_dir;
    delete m_output_dir;
    m_input_file->Close();
    m_output_file->Close();
    delete m_input_file;
    delete m_output_file;

    // delete m_h0;
    // delete m_h1;
    // delete m_h20;
    // delete m_combined;

    for (auto iter_p = m_vec_HistNameManagers.begin(); iter_p != m_vec_HistNameManagers.end(); iter_p++)
    {
        cout << "ggFKlambdaCombinedTool:: ~ggFKlambdaCombinedTool: INFO delete HistNameManager" << endl;
        delete *iter_p;
    }
    cout << "ggFKlambdaCombinedTool::~ggFKlambdaCombinedTool: Deleted" << endl;
}

ggFKlambdaCombinedTool::ggFKlambdaCombinedTool(const string &input_file, const string &output_file)
{
    // For input file, read only!
    m_input_file = TFile::Open(input_file.c_str(), "READ");
    // For output file, we should update it!
    m_output_file = TFile::Open(output_file.c_str(), "UPDATE");
    if (!m_input_file)
    {
        clog << "ggFKlambdaCombinedTool::ggFKlambdaCombinedTool: input_file not found, exit!" << endl;
        std::abort();
    }
    if (!m_output_file)
    {
        clog << "ggFKlambdaCombinedTool::ggFKlambdaCombinedTool: could not open output_file, exit!" << endl;
        std::abort();
    }

    m_input_dir = nullptr;
    m_output_dir = nullptr;
    m_h0 = nullptr;
    m_h1 = nullptr;
    m_h20 = nullptr;
    m_combined = nullptr;
    m_vec_HistNameManagers = vector<HistNameManager *>();

    cout << "ggFKlambdaCombinedTool::ggFKlambdaCombinedTool: Initialize with input and output file" << endl;
}

void ggFKlambdaCombinedTool::CombineOneHist(float klambda)
{

    // For more details about the method here, check Alessandra Betti's talk
    // Methods to obtain signal templates for HH signals with couplings variations
    // We take KT = 1 here
    auto h_cup = TH1F(*m_h0);

    // now consider kappa_t = 1, kappa_lambda
    // Edited by Zifeng at March, 8th, 2021, correct the k1, k2, k3 here, add lambda!
    auto Kappa_t = 1.0;
    // now switch to lambda = 20.0
    auto d_lambda = 20.0;
    auto k1 = Kappa_t * Kappa_t *
              ((pow(d_lambda, 2) - d_lambda) * pow(Kappa_t, 2) +
               (d_lambda - 1) * pow(klambda, 2) - (pow(d_lambda, 2) - 1.) * Kappa_t * klambda) /
              (pow(d_lambda, 2) - d_lambda);
    auto k2 = Kappa_t * Kappa_t *
              (pow(d_lambda, 2) * Kappa_t * klambda - d_lambda * pow(klambda, 2)) / (pow(d_lambda, 2) - d_lambda);
    auto k3 = Kappa_t * Kappa_t *
              (pow(klambda, 2) - Kappa_t * klambda) / (pow(d_lambda, 2) - d_lambda);
    h_cup.Sumw2();
    h_cup.Add(m_h0, m_h1,
              k1,
              k2);
    m_combined = new TH1F(h_cup);
    m_combined->Sumw2();
    m_combined->Add(m_combined, m_h20,
                    1,
                    k3);

    delete m_h0;
    delete m_h1;
    delete m_h20;
}

bool ggFKlambdaCombinedTool::GetHistAndCheck(const string &h0_name, const string &h1_name, const string &h20_name)
{

    m_h0 = (TH1F *)m_input_dir->Get(h0_name.c_str());
    m_h1 = (TH1F *)m_input_dir->Get(h1_name.c_str());
    m_h20 = (TH1F *)m_input_dir->Get(h20_name.c_str());

    if (!m_h0)
    {
        clog << "Warning: Cant find base histogram h0" << endl;
    }
    if (!m_h1)
    {
        clog << "Warning: Cant find base histogram h1" << endl;
    }
    if (!m_h20)
    {
        clog << "Warning: Cant find base histogram h20" << endl;
    }
    if (!m_h0 || !m_h1 || !m_h20)
    {
        clog << "ggFKlambdaCombinedTool::GetHistAndCheck: This histogram is not exist in the base_file, skip" << endl;
        delete m_h0;
        delete m_h1;
        delete m_h20;
        return false;
    }
    else
    {
        m_h0 = (TH1F *)m_input_dir->Get(h0_name.c_str())->Clone();
        m_h1 = (TH1F *)m_input_dir->Get(h1_name.c_str())->Clone();
        m_h20 = (TH1F *)m_input_dir->Get(h20_name.c_str())->Clone();
        return true;
    }
}

void ggFKlambdaCombinedTool::LoopOverHistNameManagers(float klambda)
{
    if (m_vec_HistNameManagers.size() == 0)
    {
        clog << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: ERROR m_vec_HistNameManagers is empty, try SetDefaultHistNameManager first if you want default setup!" << endl;
        std::abort();
    }
    else
    {
        cout << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: INFO There are"
             << " " << m_vec_HistNameManagers.size() << " HistNameManager" << endl;
    }

    // Get m_input_dir and m_output_dir

    // Iter all HistNameManager in m_vec_HistNameManagers
    for (auto iter_hnm = m_vec_HistNameManagers.begin(); iter_hnm != m_vec_HistNameManagers.end(); iter_hnm++)
    {
        // Get all containers HistNameManager hold
        auto vec_regions = (*iter_hnm)->GetRegions();
        auto vec_variables = (*iter_hnm)->GetVariables();
        auto map_basenames = (*iter_hnm)->GetBaseMap();
        auto str_dirname = (*iter_hnm)->GetDirName();

        // Check or Create the diretory!
        m_input_dir = (TDirectory *)m_input_file->Get(str_dirname.c_str());
        if (!m_input_dir)
        {
            clog << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: ERROR: Input directory not exist, skip and check the HistNameManager DirName! Skip." << endl;
        }

        m_output_dir = (TDirectory *)m_output_file->Get(str_dirname.c_str());
        if (!m_output_dir)
        {
            clog << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: WARNING: Output directory not exist in output file, now try to mkdir!" << endl;
            m_output_file->mkdir(str_dirname.c_str());
        }
        else
        {
            cout << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: Now switch to output directory." << endl;
        }
        m_output_file->cd(str_dirname.c_str());

        // Loop over these containers!
        for (auto iter_region = vec_regions.begin(); iter_region != vec_regions.end(); iter_region++)
        {

            for (auto iter_variable = vec_variables.begin(); iter_variable != vec_variables.end(); iter_variable++)
            {
                auto hist_name_reweighted = map_basenames.at("base") + TransformFloat(klambda) + "fromReco" +
                                            "_" + *iter_region + "_" + *iter_variable;
                // Get three base histogram
                auto h0_name = map_basenames.at("0p0") + "_" + *iter_region + "_" + *iter_variable;
                auto h1_name = map_basenames.at("1p0") + "_" + *iter_region + "_" + *iter_variable;
                auto h20_name = map_basenames.at("20p0") + "_" + *iter_region + "_" + *iter_variable;

                // GetHistAndCheck
                if (GetHistAndCheck(h0_name, h1_name, h20_name))
                {
                    // CombineOneHist
                    CombineOneHist(klambda);

                    // Write histogram to ouput file!
                    m_combined->SetName(hist_name_reweighted.c_str());
                    m_combined->SetTitle(hist_name_reweighted.c_str());
                    m_combined->Write();

                    cout << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: Write one histogram into output file" << endl;
                    // delte m_combined
                    delete m_combined;
                }
                else
                {
                    cout << "ggFKlambdaCombinedTool::LoopOverHistNameManagers: WARNING: Base histograms not exist! Skipped." << endl;
                    cout << h0_name << "\n"
                         << h1_name << "\n"
                         << h20_name << endl;
                }
            }
        }

        // Go back to top of the TDirectory
        m_output_file->cd("../");
    }
}

std::string ggFKlambdaCombinedTool::TransformFloat(float KLambda)
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

void ggFKlambdaCombinedTool::SetDefaultHistNameManager()
{
    // Setup Preselection
    auto hnm_Preselection = new HistNameManager;
    hnm_Preselection->SetDefaultBaseName();
    hnm_Preselection->SetDefaultRegions();
    hnm_Preselection->SetDefaultVariables();
    hnm_Preselection->SetDirName("Preselection");

    AddHistNameManager(hnm_Preselection);

    // Setup PNNScorePreselection
    auto hnm_PNNScorePreselection = new HistNameManager;
    hnm_PNNScorePreselection->SetDefaultBaseName();
    hnm_PNNScorePreselection->SetDefaultRegions();
    hnm_PNNScorePreselection->SetDefaultPNNVariables();
    hnm_PNNScorePreselection->SetDirName("PNNScorePreselection");

    AddHistNameManager(hnm_PNNScorePreselection);

    // Setup NN
    auto hnm_NNScorePreselection = new HistNameManager;
    hnm_NNScorePreselection->SetDefaultBaseName();
    hnm_NNScorePreselection->SetDefaultRegions();
    hnm_NNScorePreselection->SetDefaultNNVariables();
    hnm_NNScorePreselection->SetDirName("NNScorePreselection");

    AddHistNameManager(hnm_NNScorePreselection);

    // Setup SMBDT
    auto hnm_BDTScorePreselection = new HistNameManager;
    hnm_BDTScorePreselection->SetDefaultBaseName();
    hnm_BDTScorePreselection->SetDefaultRegions();
    hnm_BDTScorePreselection->SetDefaultBDTVariables();
    hnm_BDTScorePreselection->SetDirName("BDTScorePreselection");

    AddHistNameManager(hnm_BDTScorePreselection);
}