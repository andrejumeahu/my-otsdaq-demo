simple_plugin(DemoDQMHistosConsumer "processor"
    Configurable
	DemoRootUtilities
    RootUtilities
    DataManager
    WorkLoopManager
#	MessageFacility
	Macros
    ConfigurationInterface
    TableCore
	${ROOT_BASIC_LIB_LIST}
	${MF_MESSAGELOGGER}
  )
  
install_headers()
install_source()
