#include "CoilTree.hh"

CoilTree::CoilTree()
{    
    fThreadsData.resize(fParameters.nThreads, nullptr);
    fEventCounter = 0;
}

CoilTree::~CoilTree()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* time_info = std::localtime(&now_time_t);
    std::ostringstream filenameStream;
    filenameStream << std::put_time(time_info, "%Y-%m-%d_%H-%M-%S");
    std::string filetime = filenameStream.str();
    std::string fileName = "../../data/temp/"+std::to_string((int)fParameters.energy)+"GeV_"+fParameters.particle+"_"+std::to_string(fParameters.nEvents)+"_"+filetime+".root";
    
    std::ofstream READMEfile;
    READMEfile.open("../../data/README/"+std::to_string((int)fParameters.energy)+"GeV_"+fParameters.particle+"_"+std::to_string(fParameters.nEvents)+"_"+filetime+"README.txt");

    READMEfile << "Batch Mode: " << fParameters.batchMode << std::endl;
    READMEfile << "Particle: " << fParameters.particle << std::endl;
    READMEfile << "Number of Threads: " << fParameters.nThreads << std::endl;
    READMEfile << "Number of Events: " << fParameters.nEvents << std::endl;
    READMEfile << "Energy: " << fParameters.energy << std::endl;
    READMEfile << "Coil Density: " << fParameters.coilDensity << std::endl;
    READMEfile << "Copper Block Size: " << fParameters.copperBlockSize << std::endl;
    READMEfile << "Random Position: " << fParameters.randomPosition << std::endl;
    READMEfile << "Random Angle: " << fParameters.randomAngle << std::endl;
    READMEfile<< "Print Number: " << fParameters.printNumber << std::endl;

    READMEfile.close();

    TFile* fFile = new TFile((fileName).c_str(), "RECREATE");
    TTree* fCoilTree = new TTree("CoilTree", "Photon Step Data in Coil Tree");
    TTree* fCoilDetTree = new TTree("CoilDetTree", "Photon Step Data in Coil Detector Tree");
    TTree* fFiberTree = new TTree("FiberTree", "Photon Step Data in Fiber Tree");
    TTree* fFiberDetTree = new TTree("FiberDetTree", "Photon Step Data in Fiber Detector Tree");
    TTree* fCoilStepData = new TTree("CoilStepData", "Photon Step Data");
    TTree* fFiberStepData = new TTree("FiberStepData", "Photon Step Data");
    

    fCoilTree->Branch("CopyNo", &fCoilData.CopyNo);
    fCoilTree->Branch("VolumeNo", &fCoilData.VolumeNo);
    fCoilDetTree->Branch("EventNo", &fCoilData.EventNo);
    fFiberTree->Branch("CopyNo", &fFiberData.CopyNo);
    fFiberTree->Branch("VolumeNo", &fFiberData.VolumeNo);
    fFiberDetTree->Branch("EventNo", &fFiberData.EventNo);

    fCoilTree->Branch("StartZpos", &fCoilData.StartZpos);
    fCoilTree->Branch("StartTime", &fCoilData.StartTime);
    fFiberTree->Branch("StartZpos", &fFiberData.StartZpos);
    fFiberTree->Branch("StartTime", &fFiberData.StartTime);

    fCoilTree->Branch("endZpos", &fCoilData.endZpos);
    fCoilTree->Branch("endStep", &fCoilData.endStep);
    fCoilTree->Branch("endTime", &fCoilData.endTime);
    fFiberTree->Branch("endZpos", &fFiberData.endZpos);
    fFiberTree->Branch("endStep", &fFiberData.endStep);

    fCoilTree->Branch("MomentumX", &fCoilData.MomentumX);
    fCoilTree->Branch("MomentumY", &fCoilData.MomentumY);
    fCoilTree->Branch("MomentumZ", &fCoilData.MomentumZ);
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
    fFiberDetTree->Branch("DetMomentumX", &fFiberData.DetMomentumX);
    fFiberDetTree->Branch("DetMomentumY", &fFiberData.DetMomentumY);
    fFiberDetTree->Branch("DetMomentumZ", &fFiberData.DetMomentumZ);

    fCoilStepData->Branch("Angle", &fStep.angle);
    fFiberStepData->Branch("Angle", &fStep.angle);

    for(auto threadData : fThreadsData)
    {

        if(threadData != nullptr)
        {
            int ID = threadData->GetThreadID();
            delete threadData;
            TFile* File = new TFile(("data/Thread" + std::to_string(ID) + ".root").c_str(), "READ");
            TTree* CoilTree = (TTree*) File->Get("CoilTree");
            TTree* CoilDetTree = (TTree*) File->Get("CoilDetTree");
            TTree* FiberTree = (TTree*) File->Get("FiberTree");
            TTree* FiberDetTree = (TTree*) File->Get("FiberDetTree");
            TTree* CoilStepData = (TTree*) File->Get("CoilStepData");
            TTree* FiberStepData = (TTree*) File->Get("FiberStepData");

            CoilTree->SetBranchAddress("CopyNo", &fCoilData.CopyNo);
            CoilTree->SetBranchAddress("VolumeNo", &fCoilData.VolumeNo);
            CoilDetTree->SetBranchAddress("EventNo", &fCoilData.EventNo);
            FiberTree->SetBranchAddress("CopyNo", &fFiberData.CopyNo);
            FiberTree->SetBranchAddress("VolumeNo", &fFiberData.VolumeNo);
            FiberDetTree->SetBranchAddress("EventNo", &fFiberData.EventNo);

            CoilTree->SetBranchAddress("StartZpos", &fCoilData.StartZpos);
            CoilTree->SetBranchAddress("StartTime", &fCoilData.StartTime);
            FiberTree->SetBranchAddress("StartZpos", &fFiberData.StartZpos);
            FiberTree->SetBranchAddress("StartTime", &fFiberData.StartTime);

            CoilTree->SetBranchAddress("endZpos", &fCoilData.endZpos);
            CoilTree->SetBranchAddress("endStep", &fCoilData.endStep);
            CoilTree->SetBranchAddress("endTime", &fCoilData.endTime);
            FiberTree->SetBranchAddress("endZpos", &fFiberData.endZpos);
            FiberTree->SetBranchAddress("endStep", &fFiberData.endStep);

            CoilTree->SetBranchAddress("MomentumX", &fCoilData.MomentumX);
            CoilTree->SetBranchAddress("MomentumY", &fCoilData.MomentumY);
            CoilTree->SetBranchAddress("MomentumZ", &fCoilData.MomentumZ);
            FiberTree->SetBranchAddress("MomentumX", &fFiberData.MomentumX);
            FiberTree->SetBranchAddress("MomentumY", &fFiberData.MomentumY);
            FiberTree->SetBranchAddress("MomentumZ", &fFiberData.MomentumZ);
            

            CoilDetTree->SetBranchAddress("DetTime", &fCoilData.DetTime);
            CoilDetTree->SetBranchAddress("DetStep", &fCoilData.DetStep);
            CoilDetTree->SetBranchAddress("DetStartZpos", &fCoilData.DetStartZpos);
            CoilDetTree->SetBranchAddress("DetEndZpos", &fCoilData.DetEndZpos);
            CoilDetTree->SetBranchAddress("DetStartEndDiff", &fCoilData.DetStartEndDiff);
            CoilDetTree->SetBranchAddress("DetDistance", &fCoilData.DetDistance);
            CoilDetTree->SetBranchAddress("DetTimeTravelled", &fCoilData.DetTimeTravelled);
            CoilDetTree->SetBranchAddress("DetMomentumX", &fCoilData.DetMomentumX);
            CoilDetTree->SetBranchAddress("DetMomentumY", &fCoilData.DetMomentumY);
            CoilDetTree->SetBranchAddress("DetMomentumZ", &fCoilData.DetMomentumZ);

            CoilDetTree->SetBranchAddress("DetCopyNo", &fCoilData.CopyNo);
            FiberDetTree->SetBranchAddress("DetTime", &fFiberData.DetTime);
            FiberDetTree->SetBranchAddress("DetStep", &fFiberData.DetStep);
            FiberDetTree->SetBranchAddress("DetStartZpos", &fFiberData.DetStartZpos);
            FiberDetTree->SetBranchAddress("DetEndZpos", &fFiberData.DetEndZpos);
            FiberDetTree->SetBranchAddress("DetStartEndDiff", &fFiberData.DetStartEndDiff);
            FiberDetTree->SetBranchAddress("DetDistance", &fFiberData.DetDistance);
            FiberDetTree->SetBranchAddress("DetTimeTravelled", &fFiberData.DetTimeTravelled);
            FiberDetTree->SetBranchAddress("DetCopyNo", &fFiberData.CopyNo);
            FiberDetTree->SetBranchAddress("DetMomentumX", &fFiberData.DetMomentumX);
            FiberDetTree->SetBranchAddress("DetMomentumY", &fFiberData.DetMomentumY);
            FiberDetTree->SetBranchAddress("DetMomentumZ", &fFiberData.DetMomentumZ);

            CoilStepData->SetBranchAddress("Angle", &fStep.angle);
            FiberStepData->SetBranchAddress("Angle", &fStep.angle);

            for (Long64_t i = 0; i < CoilTree->GetEntries(); ++i) {
                CoilTree->GetEntry(i);
                fCoilTree->Fill();
            }

            for (Long64_t i = 0; i < CoilDetTree->GetEntries(); ++i) {
                CoilDetTree->GetEntry(i);
                fCoilDetTree->Fill();
            }

            for (Long64_t i = 0; i < FiberTree->GetEntries(); ++i) {
                FiberTree->GetEntry(i);
                fFiberTree->Fill();
            }

            for (Long64_t i = 0; i < FiberDetTree->GetEntries(); ++i) {
                FiberDetTree->GetEntry(i);
                fFiberDetTree->Fill();
            }

            for (Long64_t i = 0; i < CoilStepData->GetEntries(); ++i) {
                CoilStepData->GetEntry(i);
                fCoilStepData->Fill();
            }

            for (Long64_t i = 0; i < FiberStepData->GetEntries(); ++i) {
                FiberStepData->GetEntry(i);
                fFiberStepData->Fill();
            }

            File->Close();
        }
    }
    fFile->Write();
    fFile->Close();
}

void CoilTree::AddThreadData(unsigned int threadID)
{
    if (fThreadsData[threadID] == nullptr) fThreadsData[threadID] = new ThreadData(threadID);
}

void CoilTree::SetParameters(const std::string& macFile)
{
    std::ifstream file(macFile);
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file: " << macFile << std::endl;
        return;
    }
    std::string line;

    while(std::getline(file, line))
    {
        if(line.find("#$$$") == 0)
        {
            double posX, posY, posZ;
            double angleX, angleY, angleZ;

            std::string parameter = line.substr(4, line.find("=") - 4);
            std::string value = line.substr(line.find("=") + 1, line.size() - line.find("=") - 1);
            if(parameter == " batchMode") fParameters.batchMode = std::stoi(value);
            else if(parameter == " particle") fParameters.particle = value;
            else if(parameter == " nThreads") fParameters.nThreads = std::stoi(value);
            else if(parameter == " nEvents") fParameters.nEvents = std::stoi(value);
            else if(parameter == " energy") fParameters.energy = std::stod(value);
            else if(parameter == " coilDensity") fParameters.coilDensity = std::stoi(value) * cm;
            else if(parameter == " copperBlockSize") fParameters.copperBlockSize = std::stod(value) * cm;
            else if(parameter == " randomPosition") fParameters.randomPosition = std::stoi(value);
            else if(parameter == " randomDirection") fParameters.randomAngle = std::stoi(value);
            else if(parameter == " positionX") posX = std::stod(value) * cm;
            else if(parameter == " positionY") posY = std::stod(value) * cm;
            else if(parameter == " positionZ") posZ = std::stod(value) * cm;
            else if(parameter == " directionX") angleX = std::stod(value);
            else if(parameter == " directionY") angleY = std::stod(value);
            else if(parameter == " directionZ") angleZ = std::stod(value);
            else if(parameter == " printNumber") fParameters.printNumber = std::stoi(value);

            fParameters.position = {posX, posY, posZ};
            fParameters.angle = {angleX, angleY, angleZ};
        }
    }
    std::cout << "File Name: " << macFile << std::endl;
    std::cout << "Batch Mode: " << fParameters.batchMode << std::endl;
    std::cout << "Particle: " << fParameters.particle << std::endl;
    std::cout << "Number of Threads: " << fParameters.nThreads << std::endl;
    std::cout << "Number of Events: " << fParameters.nEvents << std::endl;
    std::cout << "Energy: " << fParameters.energy << std::endl;
    std::cout << "Coil Density: " << fParameters.coilDensity << std::endl;
    std::cout << "Copper Block Size: " << fParameters.copperBlockSize << std::endl;
    std::cout << "Random Position: " << fParameters.randomPosition << std::endl;
    std::cout << "Random Angle: " << fParameters.randomAngle << std::endl;
    std::cout << "Position: " << fParameters.position << std::endl;
    std::cout << "Angle: " << fParameters.angle << std::endl;
    std::cout << "Print Number: " << fParameters.printNumber << std::endl;

    file.close();
}
