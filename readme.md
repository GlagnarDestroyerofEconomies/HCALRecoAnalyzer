# Overview
This is a primitive EDAnalyzer to compare rechits to simhits. The rechit collection is read in through the primary input while the simhit collection is read in as a secondary file. This is technically a bad practice.

# Installation
Follow these instructions to install:
1. `cmsrel CMSSW_5_3_8_patch2`
2. `cd CMSSW_5_3_8_patch2`
2. `mkdir Analyzers && cd Analyzers`
2. `git clone https://github.com/elliot-hughes/HCALRecoAnalyzer.git`
2. `cd HCALRecoAnalyzer`
2. `scram b -j8`
2. Modify the lines in `hcalrecoanalyzer_cfg.py` to specify the correct input and output directories and filenames.
3. `cmsRun hcalrecoanalyzer_cfg.py`

# Usage
## Input
This analyzer takes two inputs: a GEN-SIM-RAW file and the corresponding RECO file. You can use `test/make_reco_cfg.py` to create a RECO file from a GEN-SIM-RAW one.
## Output
This analyzer produces two TNtuples in a single ROOT file: one for the simhit collection (s) and one for the rechit collection (r).

# Known Issues
1. It would be better to produce TTrees with TBranches instead of TNtuples in the output.
2. The configuration file, `hcalrecoanalyzer_cfg.py`, should be modified to accommodate command-line arguments.
3. Pileup is not treated.

# Contact
Contact `tote@physics.rutgers.edu` for more information.
