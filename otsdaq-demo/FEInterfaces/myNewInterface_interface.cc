#include "otsdaq-demo/FEInterfaces/myNewInterface.h"
#include "otsdaq-core/MessageFacility/MessageFacility.h"
#include "otsdaq-core/Macros/CoutHeaderMacros.h"
#include "otsdaq-core/Macros/InterfacePluginMacros.h"
#include <iostream>
#include <set>

using namespace ots;

#undef 	__MF_SUBJECT__
#define __MF_SUBJECT__ "FE-myNewInterface"


//========================================================================================================================
myNewInterface::myNewInterface(const std::string& interfaceUID, const ConfigurationTree& theXDAQContextConfigTree, const std::string& interfaceConfigurationPath)
: Socket            	(
		theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("HostIPAddress").getValue<std::string>()
		, theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("HostPort").getValue<unsigned int>())
, FEVInterface      	(interfaceUID, theXDAQContextConfigTree, interfaceConfigurationPath)
, OtsUDPHardware    	(theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("InterfaceIPAddress").getValue<std::string>()
		, theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("InterfacePort").getValue<unsigned int>())
, OtsUDPFirmwareDataGen	(theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("FirmwareVersion").getValue<unsigned int>())
{
	universalAddressSize_ = 8;
	universalDataSize_    = 8;

	//register FE Macro Functions
	registerFEMacroFunction("testFunction",	//feMacroName
			static_cast<FEVInterface::frontEndMacroFunction_t>(&myNewInterface::testFunction), //feMacroFunction
			std::vector<std::string>{"arg1","arg2","arg3"}, //namesOfInputArgs
			std::vector<std::string>{"oarg1","oarg2"},		//namesOfOutputArgs
			255); //requiredUserPermissions
	registerFEMacroFunction("testFunction2",
			static_cast<FEVInterface::frontEndMacroFunction_t>(&myNewInterface::testFunction),
			std::vector<std::string>{"targ1"},
			std::vector<std::string>{"toarg1"},
			1);



	std::vector<frontEndMacroInArg_t> argsIn;
	argsIn.push_back(frontEndMacroInArg_t("arg1","val1"));
	argsIn.push_back(frontEndMacroInArg_t("arg2","val2"));
	argsIn.push_back(frontEndMacroInArg_t("arg3","val3"));


	__MOUT__ << std::endl;
	__MOUT__ << argsIn[0].first << std::endl;

	__MOUT__ << std::endl;

	__MOUT__ << __COUT_HDR_P__ << "# of args = " << argsIn.size() << std::endl;
	for(auto &argIn:argsIn)
		__MOUT__ << argIn.first << ": " << argIn.second << std::endl;


	std::vector<std::string> returnStrings;
	std::vector<frontEndMacroOutArg_t> argsOut;

	std::string outputArgs = "oarg1,oarg2,";
	{
		std::istringstream inputStream(outputArgs);
		std::string argName;
		while (getline(inputStream, argName, ','))
		{
			__MOUT__ << "argName " << argName << std::endl;

			returnStrings.push_back(std::string("test"));
			argsOut.push_back(FEVInterface::frontEndMacroOutArg_t(
					argName,
					returnStrings[returnStrings.size()-1]));
			//
			//			__MOUT__ << argsOut[argsOut.size()-1].first << std::endl;
			//			__MOUT__ << argsOut[argsOut.size()-1].second << std::endl;
		}
	}

	auto mapOfFEMacroIt = mapOfFEMacroFunctions_.find("testFunction");
	if(mapOfFEMacroIt != mapOfFEMacroFunctions_.end())
	{
		(this->*(mapOfFEMacroIt->second.macroFunction_))(argsIn,argsOut);
		__MOUT__ << "Made it " << std::endl;
		for(auto &arg:argsOut)
			__MOUT__ << arg.first << ": " << arg.second << std::endl;
	}

	//std::function<void(std::vector<std::pair<std::string,std::string> >)> my = &myNewInterface::testFunction;
}

//========================================================================================================================
myNewInterface::~myNewInterface(void)
{}


//========================================================================================================================
void myNewInterface::testFunction(frontEndMacroInArgs_t argsIn, frontEndMacroOutArgs_t argsOut)
{
	__MOUT__ << "in...\n" << __COUT_HDR_P__ << "# of input args = " << argsIn.size() << std::endl;
	__MOUT__ << "in...\n" << __COUT_HDR_P__ << "# of output args = " << argsOut.size() << std::endl;
	for(auto &argIn:argsIn)
		__MOUT__ << argIn.first << ": " << argIn.second << std::endl;


	for(unsigned int i=0;i<argsOut.size();++i)
		argsOut[i].second = argsOut[i].first + "-NewValue";
}

//========================================================================================================================
void myNewInterface::configure(void)
{
	__MOUT__ << "configure" << std::endl;
	__MOUT__ << "Clearing receive socket buffer: " << OtsUDPHardware::clearReadSocket() << " packets cleared." << std::endl;

	std::string writeBuffer;
	//	std::string readBuffer;

	__MOUT__ << "Setting Destination IP: " <<
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToIPAddress").getValue<std::string>()
			<< std::endl;
	__MOUT__ << "And Destination Port: " <<
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToPort").getValue<unsigned int>()
			<< std::endl;

	writeBuffer.resize(0);
	OtsUDPFirmwareCore::setDataDestination(writeBuffer,
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToIPAddress").getValue<std::string>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToPort").getValue<uint64_t>()
	);
	OtsUDPHardware::write(writeBuffer);

//	__MOUT__ << "CCD Test CLOCK Freq" <<
//			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("CCDClockFrequency").getValue<uint32_t>() << std::endl;
//
//	int CcdClockAddress = 0x4;
//	int CddClockValue = theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("CCDClockFrequency").getValue<uint32_t>();
//
//
//	writeBuffer.resize(0);
//	OtsUDPFirmware::write(writeBuffer,CcdClockAddress,CddClockValue);
//	OtsUDPHardware::write(writeBuffer);

	//
	//
	//	__MOUT__ << "Reading back burst dest MAC/IP/Port: "  << std::endl;
	//	writeBuffer.resize(0);
	//	OtsUDPFirmware::readBurstDestinationMAC(writeBuffer);
	//	OtsUDPHardware::read(writeBuffer,readBuffer);
	//	writeBuffer.resize(0);
	//	OtsUDPFirmware::readBurstDestinationIP(writeBuffer);
	//	OtsUDPHardware::read(writeBuffer,readBuffer);
	//	writeBuffer.resize(0);
	//	OtsUDPFirmware::readBurstDestinationPort(writeBuffer);
	//	OtsUDPHardware::read(writeBuffer,readBuffer);
	//
	//
	__MOUT__ << "Done with configuring."  << std::endl;
}

//========================================================================================================================
//void myNewInterface::configureDetector(const DACStream& theDACStream)
//{
//	__MOUT__ << "\tconfigureDetector" << std::endl;
//}

//========================================================================================================================
void myNewInterface::halt(void)
{
	__MOUT__ << "\tHalt" << std::endl;
	stop();
}

//========================================================================================================================
void myNewInterface::pause(void)
{
	__MOUT__ << "\tPause" << std::endl;
	stop();
}

//========================================================================================================================
void myNewInterface::resume(void)
{
	__MOUT__ << "\tResume" << std::endl;
	start("");
}

//========================================================================================================================
void myNewInterface::start(std::string )//runNumber)
{
	__MOUT__ << "\tStart" << std::endl;
	OtsUDPHardware::write(OtsUDPFirmware::startBurst());
}

//========================================================================================================================
void myNewInterface::stop(void)
{
	__MOUT__ << "\tStop" << std::endl;
	OtsUDPHardware::write(OtsUDPFirmware::stopBurst());
}

//========================================================================================================================
bool myNewInterface::running(void)
{
	__MOUT__ << "\running" << std::endl;

	//		//example!
	//		//play with array of 8 LEDs at address 0x1003


	bool flashLEDsWhileRunning = false;
	if(flashLEDsWhileRunning)
	{
		std::string writeBuffer;
		int state = -1;
		while(WorkLoop::continueWorkLoop_)
		{
			//while running
			//play with the LEDs at address 0x1003

			++state;
			if(state < 8)
			{
				writeBuffer.resize(0);
				OtsUDPFirmware::write(writeBuffer, 0x1003,1<<state);
				OtsUDPHardware::write(writeBuffer);
			}
			else if(state%2 == 1 && state < 11)
			{
				writeBuffer.resize(0);
				OtsUDPFirmware::write(writeBuffer, 0x1003, 0xFF);
				OtsUDPHardware::write(writeBuffer);
			}
			else if(state%2 == 0 && state < 11)
			{
				writeBuffer.resize(0);
				OtsUDPFirmware::write(writeBuffer, 0x1003,0);
				OtsUDPHardware::write(writeBuffer);
			}
			else
				state = -1;

			sleep(1);
		}
	}

	return false;
}

//========================================================================================================================
//NOTE: buffer for address must be at least size universalAddressSize_
//NOTE: buffer for returnValue must be max UDP size to handle return possibility
int ots::myNewInterface::universalRead(char *address, char *returnValue)
{
	__MOUT__ << "address size " << universalAddressSize_ << std::endl;

	__MOUT__ << "Request: ";
	for(unsigned int i=0;i<universalAddressSize_;++i)
		printf("%2.2X",(unsigned char)address[i]);
	std::cout << std::endl;

	std::string readBuffer(universalDataSize_,0); //0 fill to correct number of bytes

	//OtsUDPHardware::read(FSSRFirmware::universalRead(address), readBuffer) < 0;
	if(OtsUDPHardware::read(OtsUDPFirmware::universalRead(address), readBuffer) < 0) // data reply
	{
		__MOUT__ << "Caught it! This is when it's getting time out error" << std::endl;
		return -1;
	}
	__MOUT__ << "Result SIZE: " << readBuffer.size() << std::endl;
	std::memcpy(returnValue,readBuffer.substr(2).c_str(),universalDataSize_);
	return 0;
}

//========================================================================================================================
//NOTE: buffer for address must be at least size universalAddressSize_
//NOTE: buffer for writeValue must be at least size universalDataSize_
void ots::myNewInterface::universalWrite(char* address, char* writeValue)
{
	__MOUT__ << "address size " << universalAddressSize_ << std::endl;
	__MOUT__ << "data size " << universalDataSize_ << std::endl;
	__MOUT__ << "Sending: ";
	for(unsigned int i=0;i<universalAddressSize_;++i)
		printf("%2.2X",(unsigned char)address[i]);
	std::cout << std::endl;

	OtsUDPHardware::write(OtsUDPFirmware::universalWrite(address,writeValue)); // data request
}

DEFINE_OTS_INTERFACE(myNewInterface)
