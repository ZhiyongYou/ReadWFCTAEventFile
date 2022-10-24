#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "WFCTAEvent.h"

using namespace std;

int main(int argc, char**argv)
{
	if(3!=argc)
	{
		printf("usage : %s infiledir/infilename outfiledir/outfilename\n", argv[0]);
		return 0;
	}

	//read observation file
	char Name1[300]="root://eos01.ihep.ac.cn/";
	char inrootfile[300];
	strcpy(inrootfile,Name1);
	strcat(inrootfile,argv[1]);
	TFile *infile = TFile::Open(inrootfile);
	if(!infile)									{	printf("%s does not exist\n",argv[1]);	return 0;	}
	if(infile->IsZombie()||infile->GetEND()<50) {	printf("%s file error!!\n",argv[1]);	infile->Close();	return 0;	}
	TTree *inTree = (TTree *)infile->Get("eventShow");
	if(inTree==nullptr)							{	printf("%s is null file\n",argv[1]);	infile->Close();	return 0;	}
	WFCTAEvent* wfctaevent = new WFCTAEvent();
	inTree->SetBranchAddress("WFCTAEvent", &wfctaevent);

	//output file and out tree
	Int_t iTel;
	Int_t nTube;
	Long64_t rabbitTime;
	Double_t rabbittime;
	Double_t deltaT;
	TFile* fout=TFile::Open(argv[2],"RECREATE");
	TTree *outtree = new TTree("event","info of event");
	outtree->Branch("iTel",&iTel,"iTel/I");
	outtree->Branch("nTube",&nTube,"nTube/I");
	outtree->Branch("rabbitTime",&rabbitTime,"rabbitTime/L");
	outtree->Branch("rabbittime",&rabbittime,"rabbittime/D");
	outtree->Branch("deltaT",&deltaT,"deltaT/D");

	//histgram to fill delta time
	TH1D* hdeltaT = new TH1D("deltaT", "deltaT", 100, 0, 30);
	hdeltaT->GetXaxis()->SetTitle("#Delta T (s)");
	hdeltaT->GetYaxis()->SetTitle("Number of Events");

	inTree->GetEntry(0);
	Long64_t rbT0 = wfctaevent->rabbitTime;
	Double_t rbt0 = wfctaevent->rabbittime;
	//loop files
	for(int entry=0;entry<inTree->GetEntries();entry++){
		inTree->GetEntry(entry);
		iTel = wfctaevent->iTel;
		rabbitTime = wfctaevent->rabbitTime;
		rabbittime = wfctaevent->rabbittime;

		nTube = 0;
		for(int ii=0;ii<wfctaevent->iSiPM.size();ii++){
			double adch = wfctaevent->AdcH.at(ii);
			if(adch>200)
			{
				nTube++;
			}
		}

		deltaT = 0;
		if(nTube>10&&nTube<1020)
		{
			deltaT = (rabbitTime-rbT0)+(rabbittime-rbt0)*20/1000000000.;
			rbT0 = rabbitTime;
			rbt0 = rabbittime;
			hdeltaT->Fill(deltaT);
		}

		outtree->Fill();
	}

	//fill into root file
	fout->cd();
	outtree->Write();
	hdeltaT->Write();
	fout->Close();

	return 0;
}

