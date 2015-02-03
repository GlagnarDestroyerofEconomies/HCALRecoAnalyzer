# Auto generated configuration file
# using: 
# Revision: 1.381.2.17 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step2 --filein /store/mc/Summer13dr53X/TTbar_TuneZ2star_13TeV-pythia6-tauola/GEN-SIM-RAW/PU45bx25_START53_V19D-v2/20000/005F7BCD-E023-E311-A9DD-7845C4FC3A0D.root --fileout file:test.root --mc --eventcontent RECO --datatier RECO --conditions START53_V19D::All --step RAW2DIGI,RECO --python_filename test_cfg.py --no_exec -n 4
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("PoolSource",
	secondaryFileNames = cms.untracked.vstring(),
#	fileNames = cms.untracked.vstring('file:input.root')
	fileNames = cms.untracked.vstring('/store/mc/Summer13dr53X/TTbar_TuneZ2star_13TeV-pythia6-tauola/GEN-SIM-RAW/PU25bx25_START53_V19D-v1/20000/0068A47D-17E3-E211-8383-003048D4604C.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.381.2.17 $'),
    annotation = cms.untracked.string('step2 nevts:4'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOEventContent.outputCommands,
    fileName = cms.untracked.string('file:test.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V19D::All', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)

