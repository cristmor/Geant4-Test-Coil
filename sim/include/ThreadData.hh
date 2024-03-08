#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TAxis.h>
#include <TH1D.h>

#include "G4Step.hh"
#include "G4SystemOfUnits.hh"

class TFile;
class TTree;
class TH1D;

struct CoilStep
{
    double photonPosX; // Position of the photon
    double photonPosY;
    double photonPosZ;

    double stepLength;

    double time; // Time of the photon

    int step; // Step of the photon
    int event; // Event of the photon

    // std::string process; // Process of the photon
    int volumeNo; // Photon in the volume
    int copyNo;

    double momentumX;
    double momentumY;
    double momentumZ;

    double angle;

    G4TrackStatus trackStatus;
    std::string PVName;
};

struct CoilData
{
    double CopyNo;
    double VolumeNo;
    int EventNo;

    double StartZpos;
    double StartTime;

    double endZpos;
    double endStep;
    double endTime;

    double MomentumX;
    double MomentumY;
    double MomentumZ;

    double DetStep;
    double DetTime;
    double DetStartZpos;
    double DetEndZpos;
    double DetStartEndDiff;
    double DetDistance;
    double DetTimeTravelled;
    double DetMomentumX;
    double DetMomentumY;
    double DetMomentumZ;

    double distanceTraveled;
};

class ThreadData
{
    private:
        unsigned int fThreadID;

        CoilStep fStep;
        CoilStep fStepTemp;
        CoilData fCoilData;
        CoilData fFiberData;

        TFile* fFile;
        TTree* fCoilTree;
        TTree* fCoilDetTree;
        TTree* fFiberTree;
        TTree* fFiberDetTree;

        TTree* fCoilStepData;
        TTree* fFiberStepData;

        std::map<std::string, TH1D*> fHist1D;

    public:
        ThreadData(unsigned int threadID);
        ~ThreadData();

        void AddToTree(const G4Step* step, const int& eventNumber);
        inline unsigned int GetThreadID() {return fThreadID;}
        void Close();

        inline CoilData GetCoilData() {return fCoilData;}
        inline CoilData GetFiberData() {return fFiberData;}
};