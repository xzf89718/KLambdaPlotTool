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

void runCombined(){
    // Load ggFKlambdaCombinedTool and HistNameManager
    gROOT->ProcessLine(".L ggFKlambdaCombinedTool.cpp+");
    gROOT->ProcessLine(".L HistNameManager.cpp+");

    ggFKlambdaCombinedTool combine_tool("/scratchfs/atlas/mg20220214/CxAODReader_bbtautau_Bowen/run/output/KLReweight_py8_moreKLSignals.root", "./output/testcombine.root");
    // Set default HistNameManager
    combine_tool.SetDefaultHistNameManager();
    // You could also Create HistNameManager here and add it in ggFKlambdaCombinedTool
    // Dont delete my_hnm manually! It will be deleted automatically in ggFKlambdaCombinedTool::~ggFKlambdaCombinedTool()!
    // auto my_hnm = new HistNameManager;
    // my_hnm.SetDefaultBaseName();
    // my_hnm.SetDefaultRegions();
    // my_hnm.AddVariable("SOME_VARIABLE");
    // combine_tool.AddHistNameManager(combine_tool);

    // But it is really recommended modify in SetDefaultHistNameManager in ggFKlambdaCombinedTool, and SetDefaultXXX in HistNameManager!


    for (float klambda = -20.0; klambda <= 20.0; klambda = klambda + 0.2){
        combine_tool.LoopOverHistNameManagers(klambda);
    }

    
}