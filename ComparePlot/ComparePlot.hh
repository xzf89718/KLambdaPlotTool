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

#include "CompareHistManager.hh"

class ComparePlot
{
private:
    // input files and directories!
    TFile *m_input_file1;
    TFile *m_input_file2;
    TDirectory *m_input_directory1;
    TDirectory *m_input_directory2;

    TFile *m_output_file;
    TDirectory *m_output_dir;

    

public:
};
