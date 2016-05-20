#include <stdio.h>
#include <iostream>
#include <time.h>
#include <vector>

#include "TRootanaEventLoop.hxx"
#include "TDataContainer.hxx"
#include "TV1730RawData.hxx"

#include "TFile.h"
#include "TTree.h"

#include <string>
#include <TH2.h>
#include <TGraph.h>

#define cline std::cout << "line = " << __LINE__ << std::endl;


class Analyzer: public TRootanaEventLoop 
{

public:

 //     TH2 * histo;
      TH1 * TOF1;
      TH1 * TOF2;
      TH1 * Crystal;
      TH1 * Upstream;
      TH1 * Middle;
      TH1 * Downstream1;
      TH1 * Downstream2;
      TH1 * Belle2;
    double max;
    std::vector< std::vector <double> > MyData;
    std::vector< std::vector <double> > sample_num;
    std::vector<TGraph *> MyGraphs;

    int mycounter;
    std::vector<int> stop;
  TTree * tree;
  
  Analyzer() 
  {
    UseBatchMode();
    
  };
  
  
  //Set file name according to input file name
  std::string SetFullOutputFileName(int run, std::string midasFilename)
  {
    
    //Return a std::string with an output file name
    //Change to your liking
    return "test.root";
  }

  virtual ~Analyzer() {};

  void Initialize()
  {
    
    
  }

  void BeginRun(int transition,int run,int time)
  {
    //Perform things before the start of the analysis of the file
    //Create tree, branches, etc.

TOF1 = new TH1D("TOF1","",1000,100,600);
TOF2 = new TH1D("TOF2","",1000,-300,300);

      //histo = new TH2D("histo","",1000,0,20000.,1000,0,8280.);
      Crystal = new TH1D("Crystal","",1000,-2000,2000);
      Upstream = new TH1D("Upstream","",1000,-2000,2000);
      Middle = new TH1D("Middle","",1000,-2000,2000);
      Downstream1 = new TH1D("Downstream1","",1000,-2000,2000);
      Downstream2 = new TH1D("Downstream2","",1000,-2000,2000);
      Belle2 = new TH1D("Belle2","",1000,-2000,2000);

      MyData.resize(6);
      sample_num.resize(6);
      stop.resize(6);
      //stop = 0;
      max = 0.0;
      mycounter=0;
  }

  void Finalize()
  {
    //Perform things at the end of the analysis

  //histo->Write();
  //TOF->Write();
  /*TFile *f = new TFile("./Draw.root","recreate");
  f->cd();
  TOF1->Write();
  TOF2->Write();
  Crystal->Write();
  Upstream->Write();
  Middle->Write();
  Downstream1->Write();
  Downstream2->Write();
  Belle2->Write();
  std::cout << "HERE?" << std::endl;
  f->Write();
  for(int y=0;y<MyGraphs.size();y++){
    std::cout << "Writing Graph:" << y << std::endl;
    MyGraphs[y]->Write();
  }*/

    //Write out tree, etc.
  }
  
  void EndRun(int transition,int run,int time)
  {

  }

  bool ProcessMidasEvent(TDataContainer& dataContainer)
  {
    TV1730RawData *v1730 = dataContainer.GetEventData<TV1730RawData>("V730");

    //Event serial number
    serial = dataContainer.GetMidasEvent().GetSerialNumber();
    if(mycounter>10)  return true;
    if(v1730 )
    {      
      //measurements contains the active channels of the V1730
      std::vector<RawChannelMeasurement> measurements = v1730->GetMeasurements();
      //if(mycounter==0){
      //mycounter++;
      for(size_t i = 0; i < measurements.size(); i++)
      {
	chan = measurements[i].GetChannel();
	//std::cout << i << std::endl;	
	//Loop on the samples of channel 'chan' in event 'serial'
	for(size_t iSample = 0; iSample < measurements[i].GetNSamples(); ++iSample)
	{
	//std::cout << iSample << " "  << std::endl;	
         
	  uint32_t sample = measurements[i].GetSample(iSample);
          //std::cout << measurements.size() << std::endl;
          //sample_num[i].push_back(iSample);
          //MyData[i].push_back(measurements[i].GetSample(iSample)); 
          if(sample > max) {
            max = sample;
            stop[i] = iSample;
            TOF1->Fill(measurements[1].GetSample(iSample) - measurements[5].GetSample(iSample));
            TOF1->Fill(measurements[2].GetSample(iSample) - measurements[5].GetSample(iSample));
            TOF1->Fill(measurements[1].GetSample(iSample) - measurements[4].GetSample(iSample));
            TOF1->Fill(measurements[2].GetSample(iSample) - measurements[4].GetSample(iSample));
          }

            //histo->Fill(iSample,sample);

            //Crystal->Fill(iSample,measurements[0].GetSample(iSample));
            //Upstream->Fill(iSample,measurements[1].GetSample(iSample));
            //Middle->Fill(iSample,measurements[2].GetSample(iSample));
            //Downstream1->Fill(iSample,measurements[3].GetSample(iSample));
            //Downstream2->Fill(iSample,measurements[4].GetSample(iSample));
            //Belle2->Fill(iSample,measurements[5].GetSample(iSample));
            //if(iSample <10) std::cout << i << "  channel = " << chan << ", sample(" << iSample << ") = " << sample << std::endl;
 
	  //std::cout << "channel = " << chan << ", sample(" << iSample << ") = " << sample << std::endl;
	}
        //stop =1;
        //std::cout << stop[i] << std::endl;
        //MyGraphs.push_back(new TGraph(sample_num[i].size(),&sample_num[i][0],&MyData[i][0]));
        max = 0.0;
      }
      Crystal->Fill(stop[0]);
      Upstream->Fill(stop[1]);
      Middle->Fill(stop[2]);
      Downstream1->Fill(stop[3]);
      Downstream2->Fill(stop[4]);
      Belle2->Fill(stop[5]);
      //TOF1->Fill(stop[1]-stop[5]);
      //TOF1->Fill(stop[2]-stop[5]);
      //TOF1->Fill(stop[1]-stop[4]);
      //TOF1->Fill(stop[2]-stop[4]);

  //for(int y=0;y<MyGraphs.size();y++){
    //std::cout << "Writing Graph:" << y << std::endl;
    //MyGraphs[y]->Write();
  //}



     //}

    }


    return true;

  }



  private:
    
    long serial;
    int chan; 
}; 


int main(int argc, char *argv[])
{
  Analyzer::CreateSingleton<Analyzer>();
  return Analyzer::Get().ExecuteLoop(argc, argv);

}

