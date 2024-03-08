#pragma once

#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <iomanip> // For std::put_time
#include <sstream> // For std::ostringstream

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TAxis.h>
#include <TH1D.h>

#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "ThreadData.hh"

class TFile;
class TTree;

class ThreadData;

struct Parameters
{
    // Main Parameters
    int batchMode = 1;
    std::string particle = "e-";
    unsigned int nThreads = 500;
    unsigned int nEvents = 100;
    G4double energy = 1;

    // Detector Construction Parameters
    unsigned int coilDensity = 3;
    G4double disApart = 3.0*cm;
    G4double copperBlockSize = 25.0*cm;

    // Primary Generator Action Parameters
    int randomPosition = 1;
    int randomAngle = 1;
    G4ThreeVector position = {5.0*cm, 0.0*cm, 0.0*cm};
    G4ThreeVector angle  = {0.0, 0.0, 1.0};

    // Event Action Parameters
    unsigned int printNumber = 1000;
};

class CoilTree
{
    private:

        std::vector<ThreadData*> fThreadsData; // Vector of ThreadData
        Parameters fParameters;
        unsigned int fEventCounter;

        CoilData fCoilData;
        CoilData fFiberData;
        CoilStep fStep;

        TFile* fFile;
        TTree* fCoilTree;
        TTree* fCoilDetTree;
        TTree* fFiberTree;
        TTree* fFiberDetTree;
        TTree* fStepData;

    public:
        CoilTree();
        ~CoilTree();
    
    // Thread Handling
    void AddThreadData(unsigned int threadID);
    ThreadData* GetThreadData(unsigned int threadID) {return fThreadsData[threadID];}

    // Event Tracking
    inline void PlusEventCounter() {fEventCounter++;}
    inline unsigned int GetEventCounter() {return fEventCounter;}

    // Parameters
    inline Parameters GetParameters() {return fParameters;}
    void SetParameters(const std::string& macFile);

};  


