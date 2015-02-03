import FWCore.ParameterSet.Config as cms

process = cms.Process("compare")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(
	input = cms.untracked.int32(1000)
)

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
		'file:input_RECO.root',
	),
	secondaryFileNames = cms.untracked.vstring(
		'file:input_GEN-SIM.root',
	)
)

process.sr = cms.EDAnalyzer('HCALRecoAnalyzer',
	minHits = cms.untracked.uint32(10000),
	sf_hb = cms.vdouble(				# HB sampling factors
		125.44, 125.54, 125.32, 125.13,
		124.46, 125.01, 125.22, 125.48,
		124.45, 125.90, 125.83, 127.01,
		126.82, 129.73, 131.83, 143.52
	),
	sf_he = cms.vdouble(				# HE sampling factors
		210.55, 197.93, 186.12, 189.64,
		189.63, 190.28, 189.61, 189.60,
		190.12, 191.22, 190.90, 193.06,
		188.42, 188.42
	),
)

process.TFileService = cms.Service("TFileService",
	fileName = cms.string('output.root')
)

process.p = cms.Path(process.sr)
