#include "ThreadData.hh"

ThreadData::ThreadData(unsigned int threadID): fThreadID(threadID)
{
    TROOT root(("Thread" + std::to_string(threadID) + ".root").c_str(), "Thread-specific ROOT interpreter");

    fFile = new TFile(("data/Thread" + std::to_string(threadID) + ".root").c_str(), "RECREATE");

    fCoilTree = new TTree("CoilTree", "Photon Step Data in Coil Tree");
    fCoilDetTree = new TTree("CoilDetTree", "Photon Step Data in Coil Detector Tree");
    fFiberTree = new TTree("FiberTree", "Photon Step Data in Fiber Tree");
    fFiberDetTree = new TTree("FiberDetTree", "Photon Step Data in Fiber Detector Tree");
    fCoilStepData = new TTree("CoilStepData", "Photon Step Data");
    fFiberStepData = new TTree("FiberStepData", "Photon Step Data");

    memset(&fStep, 0, sizeof(fStep));
    memset(&fCoilData, 0, sizeof(fCoilData));
    memset(&fFiberData, 0, sizeof(fFiberData));

    fCoilTree->Branch("CopyNo", &fCoilData.CopyNo);
    fCoilTree->Branch("VolumeNo", &fCoilData.VolumeNo);
    fCoilTree->Branch("StartZpos", &fCoilData.StartZpos);
    fCoilTree->Branch("StartTime", &fCoilData.StartTime);
    fCoilTree->Branch("endZpos", &fCoilData.endZpos);
    fCoilTree->Branch("endStep", &fCoilData.endStep);
    fCoilTree->Branch("endTime", &fCoilData.endTime);
    fCoilTree->Branch("MomentumX", &fCoilData.MomentumX);
    fCoilTree->Branch("MomentumY", &fCoilData.MomentumY);
    fCoilTree->Branch("MomentumZ", &fCoilData.MomentumZ);


    fFiberTree->Branch("CopyNo", &fFiberData.CopyNo);
    fFiberTree->Branch("VolumeNo", &fFiberData.VolumeNo);
    fFiberTree->Branch("StartZpos", &fFiberData.StartZpos);
    fFiberTree->Branch("StartTime", &fFiberData.StartTime);
    fFiberTree->Branch("endZpos", &fFiberData.endZpos);
    fFiberTree->Branch("endStep", &fFiberData.endStep);
    fFiberTree->Branch("endTime", &fFiberData.endTime);
    fFiberTree->Branch("MomentumX", &fFiberData.MomentumX);
    fFiberTree->Branch("MomentumY", &fFiberData.MomentumY);
    fFiberTree->Branch("MomentumZ", &fFiberData.MomentumZ);

    fCoilDetTree->Branch("DetTime", &fCoilData.DetTime);
    fCoilDetTree->Branch("DetStep", &fCoilData.DetStep);
    fCoilDetTree->Branch("DetStartZpos", &fCoilData.DetStartZpos);
    fCoilDetTree->Branch("DetEndZpos", &fCoilData.DetEndZpos);
    fCoilDetTree->Branch("DetStartEndDiff", &fCoilData.DetStartEndDiff);
    fCoilDetTree->Branch("DetDistance", &fCoilData.DetDistance);
    fCoilDetTree->Branch("DetTimeTravelled", &fCoilData.DetTimeTravelled);
    fCoilDetTree->Branch("DetCopyNo", &fCoilData.CopyNo);
    fCoilDetTree->Branch("EventNo", &fCoilData.EventNo);
    fCoilDetTree->Branch("DetMomentumX", &fCoilData.DetMomentumX);
    fCoilDetTree->Branch("DetMomentumY", &fCoilData.DetMomentumY);
    fCoilDetTree->Branch("DetMomentumZ", &fCoilData.DetMomentumZ);

    fFiberDetTree->Branch("DetTime", &fFiberData.DetTime);
    fFiberDetTree->Branch("DetStep", &fFiberData.DetStep);
    fFiberDetTree->Branch("DetStartZpos", &fFiberData.DetStartZpos);
    fFiberDetTree->Branch("DetEndZpos", &fFiberData.DetEndZpos);
    fFiberDetTree->Branch("DetStartEndDiff", &fFiberData.DetStartEndDiff);
    fFiberDetTree->Branch("DetDistance", &fFiberData.DetDistance);
    fFiberDetTree->Branch("DetTimeTravelled", &fFiberData.DetTimeTravelled);
    fFiberDetTree->Branch("DetCopyNo", &fFiberData.CopyNo);
    fFiberDetTree->Branch("EventNo", &fFiberData.EventNo);
    fFiberDetTree->Branch("DetMomentumX", &fFiberData.DetMomentumX);
    fFiberDetTree->Branch("DetMomentumY", &fFiberData.DetMomentumY);
    fFiberDetTree->Branch("DetMomentumZ", &fFiberData.DetMomentumZ);

    fCoilStepData->Branch("Angle", &fStep.angle);
    fFiberStepData->Branch("Angle", &fStep.angle);
}

ThreadData::~ThreadData()
{
    Close();
}

void ThreadData::AddToTree(const G4Step* step, const int& eventNumber)
{
    int volumeParent = -1;
    fStep.PVName = step->GetPreStepPoint()->GetTouchable()->GetVolume()->GetName();
    if(fStep.PVName == "copperBlockPV") return;
    if(fStep.PVName == "G4CoilCorePV") volumeParent = 0;
    if(fStep.PVName == "G4CoilCladPV") volumeParent = 1;
    if(fStep.PVName == "FiberCorePV") volumeParent = 2;
    if(fStep.PVName == "FiberCladPV") volumeParent = 3;
    
    auto track = step->GetTrack();

    fStep.photonPosX = track->GetPosition().x();
    fStep.photonPosY = track->GetPosition().y();
    fStep.photonPosZ = track->GetPosition().z();
    fStep.stepLength = step->GetStepLength();
    fStep.time = track->GetGlobalTime();
    fStep.step = track->GetCurrentStepNumber();
    fStep.event = eventNumber;
    fStep.volumeNo = volumeParent;
    fStep.copyNo = step->GetPreStepPoint()->GetTouchable()->GetVolume(1)->GetCopyNo();
    fStep.trackStatus = track->GetTrackStatus();

    fStep.momentumX = track->GetMomentumDirection().x();
    fStep.momentumY = track->GetMomentumDirection().y();
    fStep.momentumZ = track->GetMomentumDirection().z();

    if(fStep.PVName == "G4CoilCorePV") {
        if(fStep.step == 1) {
            fCoilData.CopyNo = fStep.copyNo;
            fCoilData.VolumeNo = fStep.volumeNo;
            fCoilData.StartZpos = fStep.photonPosZ;
            fCoilData.StartTime = fStep.time;
            fCoilData.MomentumX = fStep.momentumX;
            fCoilData.MomentumY = fStep.momentumY;
            fCoilData.MomentumZ = fStep.momentumZ;
            fCoilData.distanceTraveled = 0;
        } else if((fStep.trackStatus == fStopAndKill)) {
            fCoilData.endZpos = fStep.photonPosZ;
            fCoilData.endStep = fStep.step;
            fCoilData.endTime = fStep.time;
            fCoilTree->Fill();
            if(fCoilData.endZpos > 490)
            {
                fCoilData.EventNo = fStep.event;
                fCoilData.DetStep = fCoilData.endStep;
                fCoilData.DetTime = fCoilData.endTime;

                fCoilData.DetStartZpos = fCoilData.StartZpos;
                fCoilData.DetEndZpos = fCoilData.endZpos;

                fCoilData.DetStartEndDiff = fCoilData.DetEndZpos - fCoilData.DetStartZpos;
                fCoilData.DetDistance = fCoilData.distanceTraveled;
                fCoilData.DetTimeTravelled = fCoilData.endTime - fCoilData.StartTime;

                fCoilData.DetMomentumX = fCoilData.MomentumX;
                fCoilData.DetMomentumY = fCoilData.MomentumY;
                fCoilData.DetMomentumZ = fCoilData.MomentumZ;

                fCoilData.distanceTraveled = 0;

                fCoilDetTree->Fill();
            }
            fCoilData.distanceTraveled += fStep.stepLength;
        } else {
            fStep.angle = (G4ThreeVector(fStepTemp.momentumX, fStepTemp.momentumY, fStepTemp.momentumZ).angle(G4ThreeVector(fStep.momentumX, fStep.momentumY, fStep.momentumZ)* (-1)))*(180/M_PI);
            fStep.angle = (fStep.angle/2);
            fCoilStepData->Fill();
        }
    } else if (fStep.PVName == "FiberCorePV") {
        if(fStep.step == 1) {
            fFiberData.CopyNo = fStep.copyNo;
            fFiberData.VolumeNo = fStep.volumeNo;
            fFiberData.StartZpos = fStep.photonPosZ;
            fFiberData.StartTime = fStep.time;
            fFiberData.MomentumX = fStep.momentumX;
            fFiberData.MomentumY = fStep.momentumY;
            fFiberData.MomentumZ = fStep.momentumZ;
            fFiberData.distanceTraveled = 0;
        } else if((fStep.trackStatus == fStopAndKill)) {
            fFiberData.endZpos = fStep.photonPosZ;
            fFiberData.endStep = fStep.step;
            fFiberData.endTime = fStep.time;
            fFiberTree->Fill();
            if(fFiberData.endZpos > 490)
            {
                fFiberData.EventNo = fStep.event;
                fFiberData.DetStep = fFiberData.endStep;
                fFiberData.DetTime = fFiberData.endTime;

                fFiberData.DetStartZpos = fFiberData.StartZpos;
                fFiberData.DetEndZpos = fFiberData.endZpos;

                fFiberData.DetStartEndDiff = fFiberData.DetEndZpos - fFiberData.DetStartZpos;
                fFiberData.DetDistance = fFiberData.distanceTraveled;
                fFiberData.DetTimeTravelled = fFiberData.DetTime - fFiberData.StartTime;

                fFiberData.DetMomentumX = fFiberData.MomentumX;
                fFiberData.DetMomentumY = fFiberData.MomentumY;
                fFiberData.DetMomentumZ = fFiberData.MomentumZ;

                fFiberData.distanceTraveled = 0;

                fFiberDetTree->Fill();
            }
            fFiberData.distanceTraveled += fStep.stepLength;
        } else {
            fStep.angle = (G4ThreeVector(fStepTemp.momentumX, fStepTemp.momentumY, fStepTemp.momentumZ).angle(G4ThreeVector(fStep.momentumX, fStep.momentumY, fStep.momentumZ)* (-1)))*(180/M_PI);
            fStep.angle = (fStep.angle/2);
            fFiberStepData->Fill();
        }
    }

    fStepTemp.PVName = fStep.PVName;
    fStepTemp.momentumX = fStep.momentumX;
    fStepTemp.momentumY = fStep.momentumY;
    fStepTemp.momentumZ = fStep.momentumZ;

}

void ThreadData::Close()
{
    fFile->Write();
    fFile->Close();
}

