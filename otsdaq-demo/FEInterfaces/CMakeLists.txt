include_directories(${MPI_INCLUDE_PATH})
include_directories(${CMAKE_BINARY_DIR}/artdaq)

#cet_make(LIBRARY_NAME FEInterfaces
#		 LIBRARIES
#         FECore		 
#         ${CETLIB}
#)
#
#install_headers()
#install_source()
    
#simple_plugin(myNewInterface "interface"
#  DAQHardware
#  FECore
#  WorkLoopManager
#  MessageFacility
#  NetworkUtilities
#  ConfigurationInterface
#  TableCore
#  $ENV{XERCES_C_LIB}/libxerces-c.so
#  ${CETLIB}
#  ${CETLIB_EXCEPT}
#  )
  
simple_plugin(FEOtsEthernetProgramInterface "interface"
	otsdaq-components_FEInterfaces_FEOtsUDPTemplateInterface_interface
  DAQHardware
  FECore
  Configurable
  WorkLoopManager
#  MessageFacility
   Macros
  NetworkUtilities
  ConfigurationInterface
  TableCore
#  otsdaq-demo_FEInterfaces_FEOtsUDPTemplateInterface_interface
#  ${CETLIB}
#  ${CETLIB_EXCEPT}
	${MF_MESSAGELOGGER}
	${XDAQ_XERCES-C}
	${Boost_SYSTEM_LIBRARY}
	pthread
  )
  
