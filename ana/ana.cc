#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <chrono>

#include <TFile.h>
#include <TTree.h>
#include <TAxis.h>
#include <TH1D.h>
#include <TH2D.h>

class TFile;
class TTree;
class TH1D;
class TH2D;

struct Data
{
    // double CopyNo;
    // double VolumeNo;

    // double StartZpos;
    // double StartTime;

    // double endZpos;
    // double endStep;
    // double endTime;

    double DetStep;
    double DetTime;
    double DetStartZpos;
    double DetEndZpos;
    double DetStartEndDiff;
    double DetDistance;
    double DetTimeTraveled;
    double distanceTraveled;
    double CopyNo;
    int EventNo;
};

void SetAxis(std::string xLabel, std::string yLabel, TH1D* histo)
{
    histo->GetXaxis()->SetTitle(xLabel.c_str());
    histo->GetYaxis()->SetTitle(yLabel.c_str());
}

void SetAxis(std::string xLabel, std::string yLabel, TH2D* histo)
{
    histo->GetXaxis()->SetTitle(xLabel.c_str());
    histo->GetYaxis()->SetTitle(yLabel.c_str());
}

class Ana
{
    private:
        // Variables for Data Storage
        Data fFiberData;
        Data fCoilData;

        // Files Names and Tree Names
        std::string fROOTfilename;
        TFile* fFile;
        TFile* fOutputFile;
        TTree* fCoilDetTree;
        TTree* fFiberDetTree;

        // Map to hold Histograms
        std::map<std::string, TH1D*> fHisto1D;
        std::map<std::string, TH2D*> fHisto2D;

    public:
        // Inits the Class Ana 
        Ana(std::string filename):
            fROOTfilename(filename)
        {
            fFile = new TFile(("../../data/sim/"+fROOTfilename).c_str(), "READ");
            fOutputFile = new TFile(("../../data/ana/ana_"+fROOTfilename).c_str(), "RECREATE");

            fCoilDetTree = (TTree*) fFile->Get("CoilDetTree");
            fFiberDetTree = (TTree*) fFile->Get("FiberDetTree");

            SetBranchToData();
            CreateHistograms();
        };

        ~Ana()
        {
            fFile->Close();
            fOutputFile->Write();
            fOutputFile->Close();
        }

        // Creates Individual 1-D Histograms
        void CreateHisto1D(std::string name, std::string title, std::string x_axis, std::string y_axis, double x_min, double x_max, int binCount = 200, bool diff = 0)
        {
            std::string coilName = name+"_Coil";
            std::string fiberName = name+"_Fiber";
            std::string diffName = name+"_Diff";
            fHisto1D[coilName] = new TH1D((coilName).c_str(), (title+"(Coil)").c_str(), binCount, x_min, x_max);
            fHisto1D[fiberName] = new TH1D((fiberName).c_str(), (title+"(Fiber)").c_str(), binCount, x_min, x_max);
            SetAxis(x_axis, y_axis, fHisto1D[coilName]);
            SetAxis(x_axis, y_axis, fHisto1D[fiberName]);

            if(diff)
            {
                fHisto1D[diffName] = new TH1D((diffName).c_str(), (title+"(Differences)").c_str(), binCount, x_min, x_max);
                SetAxis(x_axis, y_axis, fHisto1D[diffName]);
            }
        };

        // Creates Individual 2-D Histograms
        void CreateHisto2D(std::string name, std::string title, std::string x_axis, std::string y_axis, double x_min, double x_max, double y_min, double y_max, int binCount = 200, bool diff = 0)
        {
            std::string coilName = name+"_Coil";
            std::string fiberName = name+"_Fiber";
            std::string diffName = name+"_Diff";
            fHisto2D[coilName] = new TH2D((coilName).c_str(), (title+"(Coil)").c_str(), binCount, x_min, x_max, binCount, y_min, y_min);
            fHisto2D[fiberName] = new TH2D((fiberName).c_str(), (title+"(Fiber)").c_str(), binCount, x_min, x_max, binCount, y_min, y_min);
            SetAxis(x_axis, y_axis, fHisto2D[coilName]);
            SetAxis(x_axis, y_axis, fHisto2D[fiberName]);

            if(diff)
            {
                fHisto2D[diffName] = new TH2D((diffName).c_str(), (title+"(Differences)").c_str(), binCount, x_min, x_max, binCount, y_min, y_min);
                SetAxis(x_axis, y_axis, fHisto2D[diffName]);
            }
        };

        // Sets all branch data to variables that will be read later
        void SetBranchToData()
        {
            fCoilDetTree->SetBranchAddress("DetTime", &fCoilData.DetTime);
            fCoilDetTree->SetBranchAddress("DetStep", &fCoilData.DetStep);
            fCoilDetTree->SetBranchAddress("DetStartZpos", &fCoilData.DetStartZpos);
            fCoilDetTree->SetBranchAddress("DetEndZpos", &fCoilData.DetEndZpos);
            fCoilDetTree->SetBranchAddress("DetStartEndDiff", &fCoilData.DetStartEndDiff);
            fCoilDetTree->SetBranchAddress("DetDistance", &fCoilData.DetDistance);
            fCoilDetTree->SetBranchAddress("DetTimeTravelled", &fCoilData.DetTimeTraveled);
            fCoilDetTree->SetBranchAddress("DetCopyNo", &fCoilData.CopyNo);
            fCoilDetTree->SetBranchAddress("EventNo", &fCoilData.EventNo);


            fFiberDetTree->SetBranchAddress("DetTime", &fFiberData.DetTime);
            fFiberDetTree->SetBranchAddress("DetStep", &fFiberData.DetStep);
            fFiberDetTree->SetBranchAddress("DetStartZpos", &fFiberData.DetStartZpos);
            fFiberDetTree->SetBranchAddress("DetEndZpos", &fFiberData.DetEndZpos);
            fFiberDetTree->SetBranchAddress("DetStartEndDiff", &fFiberData.DetStartEndDiff);
            fFiberDetTree->SetBranchAddress("DetDistance", &fFiberData.DetDistance);
            fFiberDetTree->SetBranchAddress("DetTimeTravelled", &fFiberData.DetTimeTraveled);
            fFiberDetTree->SetBranchAddress("DetCopyNo", &fFiberData.CopyNo);
            fFiberDetTree->SetBranchAddress("EventNo", &fFiberData.EventNo);
        };

        // Creates all the specific histograms for the Ana class
        void CreateHistograms()
        {
            //
            // All 1-D Histograms
            //
            CreateHisto1D("DetTime", "Detection Time", "Time [ns]", "Counts", 0.0, 25.0);
            CreateHisto1D("DetDistance", "Distance Traveled", "Distance [mm]", "Counts", 0.0, 5000);
            CreateHisto1D("DetStartZpos", "Start Z-Position", "Z-Position [mm]", "Counts", -500, 500);
            CreateHisto1D("EventCount", "Phtoton Count for each Events", "Event", "Counts", 0.0, 10000.0, 100000.0);
            CreateHisto1D("CopyNo", "Photon Count for each Copy Number", "Z-Position [mm]", "Counts", 0.0, 100, 100);

            CreateHisto1D("AvgStartZPos", "Start Z-Position (Average per Event)", "Z-Position [mm]", "Counts", -500, 500);
            CreateHisto1D("AvgDetTim", "Detection Time (Average per Event)", "Time [ns]", "Counts", 0.0, 25.0, 200, true);

            //
            // All 2-D Histograms
            //
            CreateHisto2D("DetDisVsDetTim", "Detection Time vs Distance Traveled", "Distance [mm]", "Time [ns]", 0.0, 5000.0, 0.0, 25.0);
            CreateHisto2D("SZPosVsDetTim", "Detection Time vs Start Z-Position", "Z-Position [mm]", "Time [ns]", -500.0, 500.0, 0.0, 25.0);
            CreateHisto2D("AvgSZPosVsDetTim", "Detection Time vs Start Z-Position (Average per Event)", "Z-Position [mm]", "Time [ns]", -500.0, 500.0, 0.0, 25.0, 200, true);

        }

        // Fill Histograms with data
        void FillHistograms()
        {
            std::map<int, double> AveTime0;
            std::map<int, double> AveTime1;

            std::map<int, double> AveStartZPos0;
            std::map<int, double> AveStartZPos1;

            int EventNumber = 0;
            double AveStartZPos = 0;
            double AveDetTime = 0;
            int Counter = 0;

            double AveDetTimeCopy = 0;
            std::map<int, double> AveTimeCopy0;
            std::map<int, double> CounterTimeCopy0;
            fHisto1D["AvgDetTim4"] = new TH1D("AvgDetTim4", "Detection Time (Average per Copy) (Coil Det)", 200, 0.0, 25.0);

            for(int i = 0; i < 100;i++)
            {
                AveTimeCopy0[i] = 0;
                CounterTimeCopy0[i] = 0;
            }

            // Fills data from TFile to TH1D Objects
            for (Long64_t i = 0; i < fCoilDetTree->GetEntries(); ++i) {
                AveTime1[EventNumber] = 0;
                AveStartZPos1[EventNumber] = 0;
                if(EventNumber == fCoilData.EventNo)
                {
                    Counter++;
                    AveStartZPos += fCoilData.DetStartZpos;
                    AveDetTime += fCoilData.DetTime;

                    AveTimeCopy0[floor(fCoilData.CopyNo/100)] += fCoilData.DetTime;
                    CounterTimeCopy0[floor(fCoilData.CopyNo/100)] ++;

                    std::cout << "Event: " << fCoilData.EventNo << ", CopyNo: " << fCoilData.CopyNo << "/" << floor(fCoilData.CopyNo/100) << std::endl;
                }
                else
                {
                    AveStartZPos = (double)AveStartZPos/Counter;
                    AveDetTime = (double)AveDetTime/Counter;
                    fHisto1D["AvgStartZPos_Coil"]->Fill(AveStartZPos);
                    fHisto1D["AvgDetTim_Coil"]->Fill(AveDetTime);
                    fHisto2D["AvgSZPosVsDetTim_Coil"]->Fill(AveStartZPos, AveDetTime);
                    AveTime0[EventNumber] = AveDetTime;
                    AveStartZPos0[EventNumber] = AveStartZPos;


                    Counter = 0;
                    AveStartZPos = 0;
                    AveDetTime = 0;
                    EventNumber = fCoilData.EventNo;

                    for(int i = 0; i < 100;i++)
                    {
                        if(AveTimeCopy0[i] != 0)
                        {
                            fHisto1D["AvgDetTim4"]->Fill((double)AveTimeCopy0[i]/CounterTimeCopy0[i]);
                        }
                        AveTimeCopy0[i] = 0;
                        CounterTimeCopy0[i] = 0;
                    }

                    std::cout << std::endl;
                }

                fCoilDetTree->GetEntry(i);
                fHisto1D["DetTime_Coil"]->Fill(fCoilData.DetTime);
                fHisto1D["DetDistance_Coil"]->Fill(fCoilData.DetDistance);
                fHisto1D["DetStartZpos_Coil"]->Fill(fCoilData.DetStartZpos);
                fHisto2D["SZPosVsDetTim_Coil"]->Fill(fCoilData.DetStartZpos, fCoilData.DetTime);
                fHisto2D["DetDisVsDetTim_Coil"]->Fill(fCoilData.DetDistance, fCoilData.DetTime);
                fHisto1D["EventCount_Coil"]->Fill(fCoilData.EventNo);
                fHisto1D["CopyNo_Coil"]->Fill(floor(fCoilData.CopyNo/100));
            }

            EventNumber = 0;
            AveStartZPos = 0;
            AveDetTime = 0;
            Counter = 0;

            for (Long64_t i = 0; i < fFiberDetTree->GetEntries(); ++i) {
                AveTime1[EventNumber] = 0;
                AveStartZPos1[EventNumber] = 0;
                if(EventNumber == fFiberData.EventNo)
                {
                    Counter++;
                    AveStartZPos += fFiberData.DetStartZpos;
                    AveDetTime += fFiberData.DetTime;
                }
                else
                {
                    AveStartZPos = (double)AveStartZPos/Counter;
                    AveDetTime = (double)AveDetTime/Counter;
                    fHisto1D["AvgStartZPos_Fiber"]->Fill(AveStartZPos);
                    fHisto1D["AvgDetTim_Fiber"]->Fill(AveDetTime);
                    fHisto2D["AvgSZPosVsDetTim_Fiber"]->Fill(AveStartZPos, AveDetTime);
                    AveTime1[EventNumber] = AveDetTime;
                    AveStartZPos1[EventNumber] = AveStartZPos;

                    Counter = 0;
                    AveStartZPos = 0;
                    AveDetTime = 0;
                    EventNumber = fFiberData.EventNo;
                }

                fFiberDetTree->GetEntry(i);
                fHisto1D["DetTime_Fiber"]->Fill(fFiberData.DetTime);
                fHisto1D["DetDistance_Fiber"]->Fill(fFiberData.DetDistance);
                fHisto1D["DetStartZpos_Fiber"]->Fill(fFiberData.DetStartZpos);
                fHisto2D["SZPosVsDetTim_Fiber"]->Fill(fFiberData.DetStartZpos, fFiberData.DetTime);
                fHisto2D["DetDisVsDetTim_Fiber"]->Fill(fFiberData.DetDistance, fFiberData.DetTime);
                fHisto1D["EventCount_Fiber"]->Fill(fFiberData.EventNo);
                fHisto1D["CopyNo_Fiber"]->Fill(fFiberData.CopyNo);
            }

            for(int i = 0;i < 10000000; i++)
            {
                if(AveTime0[i] && AveTime1[i])
                {
                    fHisto1D["AvgDetTim_Diff"]->Fill(AveTime0[i]-AveTime1[i]);
                    fHisto2D["AvgSZPosVsDetTim_Diff"]->Fill(AveStartZPos0[i], AveTime0[i]-AveTime1[i]);
                }
            }

        }
};

int main(int argc,char** argv)
{
    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();

    std::string ROOTfilename = argv[1];
    
    auto MyAna = new Ana(ROOTfilename);
    MyAna->FillHistograms();
    delete MyAna;

    // Stop the timer
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    std::chrono::duration<double> duration = end - start;
    double durationInSeconds = duration.count();
    std::cout << "Timer: " << durationInSeconds << " seconds." << std::endl;


}