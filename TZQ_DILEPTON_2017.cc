// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/ChargedLeptons.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/AnalysisLoader.hh"

namespace Rivet {


  /// @brief Add a short analysis description here
  class TZQ_DILEPTON_2017 : public Analysis {
  public:

    /// Constructor
    TZQ_DILEPTON_2017() : Analysis("TZQ_DILEPTON_2017")
    {
    }


    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // Initialise and register projections
      // generic final state
      FinalState fs(-5.0,5.0,0*GeV);
      
      // leptons (edited for dilepton cuts that are given in the AN)
      ChargedLeptons lfs(FinalState((Cuts::abseta < 2.5 && Cuts::pT > 15*GeV && Cuts::abspid == PID::ELECTRON) || (Cuts::abseta < 2.4 && Cuts::pT > 20*GeV && Cuts::abspid == PID::MUON)));
      declare(lfs,"LFS");
     
 
      // jets
      VetoedFinalState jet_fs(fs);
      jet_fs.addVetoOnThisFinalState(lfs);
      declare(FastJets(jet_fs, FastJets::ANTIKT, 0.4), "Jets");
      

      // Book histograms
      _h_njets = bookHisto1D("jet_mult", 10, -0.5, 10.5);
      _h_jet_HT = bookHisto1D("jet_HT", logspace(50, 100.0, 2000.0));
      _h_lep_pT = bookHisto1D("lep_pT", logspace(20, 20.0, 500.0));
      _h_lep_eta = bookHisto1D("lep_eta", 25, -3.0, 3.0);
      _h_lep2_pT = bookHisto1D("lep2_pT", logspace(20, 20.0, 300.0)); 
      _h_lep2_eta = bookHisto1D("lep2_eta", 25, -3.0, 3.0); 
      _h_jet_1_pT = bookHisto1D("jet_1_pT", logspace(50, 20.0, 1000.0));
      _h_jet_2_pT = bookHisto1D("jet_2_pT", logspace(25, 20.0, 500.0));
      _h_jet_1_eta = bookHisto1D("jet_1_eta", 25, -3.5, 3.5);
      _h_jet_2_eta = bookHisto1D("jet_2_eta", 25, -6.0, 6.0);
      _h_bjet_pT = bookHisto1D("jetb_pT", logspace(25, 20.0, 1000.0));
      _h_bjet_eta = bookHisto1D("jetb_eta", 50, -3.0, 3.0);
      _h_W_mass = bookHisto1D("W_mass", 25, 0, 150); 
      _h_t_mass = bookHisto1D("t_mass", 25, 100, 300); 
      _h_t_pT = bookHisto1D("t_pT", logspace(30, 20, 700.0)); 
      _h_jetb_W_dR = bookHisto1D("jetb_W_dR", 20, 0.0, 10.0); 
      _h_jetb_W_deta = bookHisto1D("jetb_W_deta", 20, 0, 10.0);
      _h_jetb_W_dphi = bookHisto1D("jetb_W_dphi", 20, 0.0, M_PI);
      _h_Z_mass = bookHisto1D("Z_mass", 25, 70, 110); 
      _h_quark1jet_pT = bookHisto1D("quark1jet_pT", logspace(25, 20.0, 350.0));
      _h_quark2jet_pT = bookHisto1D("quark2jet_pT", logspace(25, 30.0, 150.0));
      _h_quark1jet_eta = bookHisto1D("quark1jet_eta", 25, -5.0, 5.0); 
      _h_quark2jet_eta = bookHisto1D("quark2jet_eta", 25, -5.0, 5.0); 
      _h_alljets_eta = bookHisto1D("alljets_eta", 25, -5.0, 5.0);
      _h_alljets_pT = bookHisto1D("alljets_pT", logspace(25, 30.0, 500.0));
      _h_leadingbjet_mass = bookHisto1D("leadingbjet_mass", 30, 0, 150.0);
	}


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      /// @todo Do the event by event analysis here
      const double weight = event.weight();

      const ChargedLeptons& lfs = apply<ChargedLeptons>(event, "LFS");
      MSG_DEBUG("Charged lepton multiplicity = " << lfs.chargedLeptons().size());
      if (lfs.chargedLeptons().empty()) vetoEvent;
      if (lfs.chargedLeptons().size() != 2) vetoEvent;      

      //reconstructing the Z boson
      double ZBosonMass = 91.2;
      Particle lepton = lfs.chargedLeptons()[0];
      Particle lepton2 = lfs.chargedLeptons()[1];
      double z_mass_reco;

      if((lepton.abspid() == 11 && lepton.pT() > 38*GeV) || (lepton.abspid() == 13 && lepton.pT() > 29*GeV)){
      	z_mass_reco = (lepton.momentum() + lepton2.momentum()).mass();
      }
      else{vetoEvent;}

      double mass_diff = abs(ZBosonMass - z_mass_reco);
      FourMomentum Z;

      if(lepton.pid()*lepton2.pid()<0 && mass_diff < 20*GeV){
      Z = lepton.momentum() + lepton2.momentum();
      _h_Z_mass->fill(Z.mass(), weight);
      }


      // fill lepton histograms
      _h_lep_pT->fill(lepton.pT()/GeV, weight);
      _h_lep_eta->fill(lepton.eta(), weight);
      _h_lep2_pT->fill(lepton2.pT()/GeV, weight);
      _h_lep2_eta->fill(lepton2.eta(), weight);
      

      // jets projection: consider all jets with pT > 30 GeV and abs(eta) < 4.7 (for tZq dilepton final state)
      Cut cut_jets =  Cuts::abseta < 4.7 && Cuts::pT > 30*GeV;
      const Jets& jets = apply<FastJets>(event, "Jets").jetsByPt(cut_jets);
      _h_njets->fill(jets.size(), weight);
      double ht = 0.0;
      foreach (const Jet& j, jets) { ht += j.pT(); } 

      _h_jet_HT->fill(ht/GeV, weight);
      Jets Wpair, Tracker_bTaggedjets, otherjets; 
      // only events with jets that do not overlap with hard lepton selected
      foreach (const Jet& jet, jets) { 
        bool isolated = true;
        if ((deltaR(jet.momentum(), lepton.momentum()) < 0.4) || (deltaR(jet.momentum(), lepton2.momentum()) < 0.4)) {
          isolated = false;
          break;
        }
        if (!isolated) {
          MSG_DEBUG("Jet failed lepton isolation cut");
          break;
        }


      size_t JetSize = jets.size();
      
      if(JetSize < 4 || JetSize > 6){
	MSG_DEBUG("The event failed the jet multiplicity cut");
	vetoEvent;
      }	

      if (jet.bTagged(Cuts::abseta < 2.5)) Tracker_bTaggedjets.push_back(jet);
     
      if (Tracker_bTaggedjets.size() < 1 || Tracker_bTaggedjets.size() > 2) {
        MSG_DEBUG("Event failed b jet multiplicity cut");
        vetoEvent;
      }



      double w_reco_mass = std::numeric_limits<double>::infinity();
      size_t jet_index_1{std::numeric_limits<size_t>::max()};
      size_t jet_index_2{std::numeric_limits<size_t>::max()};

      double WBosonMass = 80.4;
      

      for(size_t i = 0; i < JetSize; ++i){
	for(size_t k = i+1; k < JetSize; ++k){


			if((deltaR(jets.at(i), Tracker_bTaggedjets.at(0)) > 0.01) && deltaR(jets.at(k), Tracker_bTaggedjets.at(0)) > 0.01){


				double mass_reco = (jets.at(i).momentum() + jets.at(k).momentum()).mass();
					
				if((std::abs(WBosonMass - mass_reco) < std::abs(WBosonMass - w_reco_mass))){
					w_reco_mass = mass_reco;
					jet_index_1 = i;
					jet_index_2 = k;
				}

			}	
	
		}

	}
	


  //    if(abs(WBosonMass - w_reco_mass) < 20*GeV){
     
      	Wpair.push_back(jets.at(jet_index_1));
      	Wpair.push_back(jets.at(jet_index_2));

//	}
  //    else otherjets.push_back(jet);

      }


       if (Wpair.size() < 2) {
        MSG_DEBUG("Event failed W pair multiplicity cut");
        vetoEvent;
      }
     

      //Finding the four momentum of the leading W jet and filling the W mass plot
      FourMomentum W = Wpair[0].momentum()+Wpair[1].momentum();
      _h_W_mass->fill(W.mass(), weight);	
      // reconstruct top quark from the W and leading bTagged jet from the tracker
      FourMomentum t = W + Tracker_bTaggedjets[0].momentum();
      _h_t_mass->fill(t.mass(), weight);
      _h_t_pT->fill(t.pT()/GeV, weight);



      // fill jet histograms
      _h_jet_1_pT->fill(jets[0].pT()/GeV, weight);
      _h_jet_2_pT->fill(jets[1].pT()/GeV, weight);
      _h_jet_1_eta->fill(jets[0].eta(), weight);
      _h_jet_2_eta->fill(jets[1].eta(), weight);
      _h_bjet_pT->fill(Tracker_bTaggedjets[0].pT()/GeV, weight);
      _h_bjet_eta->fill(Tracker_bTaggedjets[0].eta(), weight);
      _h_leadingbjet_mass->fill(Tracker_bTaggedjets[0].mass(), weight);
      _h_quark1jet_pT->fill(Wpair[0].pT()/GeV, weight);
      _h_quark2jet_pT->fill(Wpair[1].pT()/GeV, weight);
      _h_quark1jet_eta->fill(Wpair[0].eta(), weight);
      _h_quark2jet_eta->fill(Wpair[1].eta(), weight);
    

     foreach (const Jet& j, jets) {
     _h_alljets_eta->fill(j.eta(), weight);
     _h_alljets_pT->fill(j.pT()/GeV, weight);
     }

      // more histograms 
      _h_jetb_W_dR->fill(deltaR(Tracker_bTaggedjets[0].momentum(), W),weight);
      _h_jetb_W_deta->fill(fabs(Tracker_bTaggedjets[0].eta()-W.eta()),weight);
      _h_jetb_W_dphi->fill(deltaPhi(Tracker_bTaggedjets[0].momentum(),W),weight);


  }

    // Normalise histograms etc., after the run

    void finalize(){

      normalize(_h_njets);
      normalize(_h_jet_HT);
      normalize(_h_lep_pT);
      normalize(_h_lep_eta);
      normalize(_h_jet_1_pT);
      normalize(_h_jet_2_pT);
      normalize(_h_jet_1_eta);
      normalize(_h_jet_2_eta);
      normalize(_h_bjet_pT);
      normalize(_h_bjet_eta);
      normalize(_h_W_mass);
      normalize(_h_t_mass);
      normalize(_h_t_pT);
      normalize(_h_jetb_W_dR);
      normalize(_h_jetb_W_deta);
      normalize(_h_jetb_W_dphi);
      normalize(_h_lep2_pT);
      normalize(_h_lep2_eta); 
      normalize(_h_Z_mass);  
      normalize(_h_quark1jet_pT);
      normalize(_h_quark2jet_pT);
      normalize(_h_quark1jet_eta);
      normalize(_h_quark2jet_eta);
      normalize(_h_alljets_eta);
      normalize(_h_alljets_pT);
      normalize(_h_alljets_particles);
      normalize(_h_leadingbjet_mass);
  }

    //@}


    /// @name Histograms
    //@{
    Histo1DPtr _h_njets;
    Histo1DPtr _h_jet_HT;
    Histo1DPtr _h_lep_pT, _h_lep_eta;
    Histo1DPtr _h_jet_1_pT, _h_jet_2_pT;
    Histo1DPtr _h_jet_1_eta, _h_jet_2_eta;
    Histo1DPtr _h_bjet_pT;
    Histo1DPtr _h_bjet_eta;
    Histo1DPtr _h_W_mass;
    Histo1DPtr _h_t_mass;
    Histo1DPtr _h_t_pT;
    Histo1DPtr _h_jetb_W_dR, _h_jetb_W_deta, _h_jetb_W_dphi;
    Histo1DPtr _h_lep2_pT, _h_lep2_eta; 
    Histo1DPtr _h_Z_mass; 
    Histo1DPtr _h_quark1jet_pT, _h_quark2jet_pT, _h_quark1jet_eta, _h_quark2jet_eta;
    Histo1DPtr _h_otherjets_eta, _h_otherjets_pT;
    Histo1DPtr _h_alljets_eta, _h_alljets_pT;
    Histo1DPtr _h_alljets_particles;
    Histo1DPtr _h_leadingbjet_mass;

    //@}


  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(TZQ_DILEPTON_2017);


}
