/* C++
Path: Analyzers/HCALRecoAnalyzer/src/HCALRecoAnalyzer.cc
Package: HCALRecoAnalyzer
Class: HCALRecoAnalyzer

Author: Elliot Hughes
Last Updated: 140708
Purpose: To compare simhits and rechits.
*/

// INCLUDES
// system include files
#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <typeinfo>

using std::map;
using std::string;
using std::cout;
using std::endl;

// user include files
	// basic includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

	// class includes
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"		// For looking at pilup
#include "SimDataFormats/CrossingFrame/interface/CrossingFramePlaybackInfoExtended.h"

//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
//#include "Geometry/Records/interface/CaloGeometryRecord.h"
//#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

	// ROOT includes
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
//#include "TFileDirectory.h"
#include "TTree.h"
#include "TNtuple.h"
// \INCLUDES

// CLASS DEFINITION
class HCALRecoAnalyzer : public edm::EDAnalyzer {
	public:
		explicit HCALRecoAnalyzer(const edm::ParameterSet&);	// Set the class argument to be (a reference to) a parameter set (?)
		~HCALRecoAnalyzer();	// Create the destructor.
		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

	private:
		virtual void beginJob();
		virtual void analyze(const edm::Event&, const edm::EventSetup&);
		virtual void endJob();
		virtual void beginRun(const edm::Run&, const edm::EventSetup&);
		virtual void endRun(const edm::Run&, const edm::EventSetup&);
		virtual void beginLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&);
		virtual void endLuminosityBlock(const edm::LuminosityBlock&, const edm::EventSetup&);

	// member data
	unsigned int minHits_;
	std::vector<double> sf_hb_;
	std::vector<double> sf_he_;
	
	// CONSTRUCT ROOT OBJECT CONTAINERS
	TNtuple* ntuple_r;
	TNtuple* ntuple_s;
	map<string, TH1F*> h1;
	map<string, TH2F*> h2;
};

//
// constants, enums and typedefs
//
	// DEFINE CUTS
	float cut_er = 5;

//
// static data member definitions
//


// constructors and destructors
//
HCALRecoAnalyzer::HCALRecoAnalyzer(const edm::ParameterSet& iConfig) :
minHits_(iConfig.getUntrackedParameter<unsigned int>("minHits",0)),
sf_hb_(iConfig.getParameter<std::vector<double>>("sf_hb")),
sf_he_(iConfig.getParameter<std::vector<double>>("sf_he"))
{
//do what ever initialization is needed
	// Set up output root file
	edm::Service<TFileService> fs;
//	TFileDirectory loop_rec = fs->mkdir("loop_r");
//	TFileDirectory loop_sim = fs->mkdir("loop_s");
	// Set up ntpules
	ntuple_r = fs->make<TNtuple>("ntuple_r", "NTuple for the rec loop of the hcal_reco analysis", "e:sd:ie:ip:es:n");
	ntuple_s = fs->make<TNtuple>("ntuple_s", "NTuple for the sim loop of the hcal_reco analysis", "e:ensf:sd:ie:ip:n");
	h2["tsum_tratio"] = fs->make<TH2F>("tsum_tratio", 100, 200, 600, 100, 0, 3);
}

// DEFINE THE DESTRUCTOR
HCALRecoAnalyzer::~HCALRecoAnalyzer()
{
}


// CLASS METHODS ("method" = "member function")

// ------------ called for each event  ------------
void HCALRecoAnalyzer::analyze(
	const edm::Event& iEvent,
	const edm::EventSetup& iSetup
){
	using namespace edm;
	
	Handle<HBHERecHitCollection> hits_rec;
	iEvent.getByLabel("hbhereco", hits_rec);
	Handle<PCaloHitContainer> hits_sim;
	iEvent.getByLabel("g4SimHits", "HcalHits", hits_sim);
	Handle<CrossingFramePlaybackInfoExtended> cf_sim;
//	bool got;
//	got = iEvent.getByLabel("mix", cf_sim);
	
	cout << "The number of rechits is " << hits_rec->size() << endl;
	cout << "The number of simhits is " << hits_sim->size() << endl;
	
	int n_rec = -1;
	// LOOP OVER RECHITS, LOOP OVER SIMHITS FOR EACH
	// Loop over each rechit:
	for (HBHERecHitCollection::const_iterator hit_rec = hits_rec->begin(); hit_rec != hits_rec->end(); ++ hit_rec) {
		n_rec ++;
		
		unsigned int r_id = hit_rec->id();
		HcalDetId hcal_id_rec(r_id);			// Make an HcalDetId object from the rec detId value.
		int r_sd = hcal_id_rec.subdet();
		int r_ieta = hcal_id_rec.ieta();
		int r_iphi = hcal_id_rec.iphi();
		float r_e = hit_rec->energy();
		float r_t = hit_rec->time();
		
		int matches = 0;
		float e_sum = 0;
//		cout << "Here 197" << endl;
		// Now, loop over each simhit:
		for( PCaloHitContainer::const_iterator hit_sim = hits_sim->begin(); hit_sim != hits_sim->end(); ++ hit_sim ) {
//			cout << "Here 199" << endl;
			unsigned int s_id = hit_sim->id();
			HcalDetId hcal_id_sim(s_id);			// Make an HcalDetId object from the sim detId value.
			int s_sd = hcal_id_sim.subdet();
			int s_ieta = hcal_id_sim.ieta();
			int s_iphi = hcal_id_sim.iphi();
			float s_e_nsf = hit_sim->energy();
			float s_e;
			float s_t = hit_sim->time();
			if (s_sd == 1) {
				s_e = s_e_nsf*sf_hb_[abs(s_ieta)-1];
			}
			else if (s_sd == 2) {
				s_e = s_e_nsf*sf_he_[abs(s_ieta)-16];
			}
			else {
				s_e = -10;
			}
			// Match simhits to rechit
			if (s_ieta == r_ieta && s_iphi == r_iphi) {
				matches ++;
				e_sum += s_e;
			}
		}
		ntuple_r->Fill(r_e, r_sd, r_ieta, r_iphi, e_sum, matches);
	}
	// LOOP OVER SIMHITS, LOOP OVER RECHITS FOR EACH
	// Loop over each simhit:
	for( PCaloHitContainer::const_iterator hit_sim = hits_sim->begin(); hit_sim != hits_sim->end(); ++ hit_sim ) {
		int matches = 0;
		unsigned int s_id = hit_sim->id();
		HcalDetId hcal_id_sim(s_id);			// Make an HcalDetId object from the rec detId value.
		int s_sd = hcal_id_sim.subdet();
		int s_ieta = hcal_id_sim.ieta();
		int s_iphi = hcal_id_sim.iphi();
		float s_e_nsf = hit_sim->energy();
		float s_e;
		if (s_sd == 1) {
			s_e = s_e_nsf*sf_hb_[abs(s_ieta)-1];
		}
		else if (s_sd == 2) {
			s_e = s_e_nsf*sf_he_[abs(s_ieta)-16];
		}
		else {
			s_e = -10;
		}
		// Now, loop over each rechit:
		for (HBHERecHitCollection::const_iterator hit_rec = hits_rec->begin(); hit_rec != hits_rec->end(); ++ hit_rec) {
			unsigned int r_id = hit_rec->id();
			HcalDetId hcal_id_rec(r_id);			// Make an HcalDetId object from the rec detId value.
			int r_sd = hcal_id_rec.subdet();
			int r_ieta = hcal_id_rec.ieta();
			int r_iphi = hcal_id_rec.iphi();
			float r_e = hit_rec->energy();
			if (r_ieta == s_ieta && r_iphi == s_iphi) {
				matches ++;
			}
		}
		ntuple_s->Fill(s_e, s_e_nsf, s_sd, s_ieta, s_iphi, matches);
	}
}


// ------------ called once each job just before starting event loop  ------------
void HCALRecoAnalyzer::beginJob()
{
}

// ------------  called once each job just after ending the event loop  ------------
void HCALRecoAnalyzer::endJob()
{
}

// ------------ method called when starting to processes a run  ------------
void 
HCALRecoAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HCALRecoAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HCALRecoAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HCALRecoAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HCALRecoAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HCALRecoAnalyzer);
