RM = rm
#CC = linux-gcc
OBJS = AddressManager.o common.o DeviceAddress.o EditAttrMgr.o GSIOTClient.o GSIOTControl.o \
GSIOTDevice.o GSIOTDeviceInfo.o GSIOTInfo.o XmppGSResult.o SQLiteHelper.o Column.o Database.o Statement.o \
Transaction.o GSIOTConfig.o DBUpdate.o GSIOTUser.o GSIOTUserMgr.o XmppGSAuth.o XmppGSMessage.o \
DataStoreMgr.o RunCode.o GSIOTHeartbeat.o XmppGSState.o XmppGSManager.o DeviceConnection.o \
TriggerControl.o ControlEvent.o IGSMessageHandler.o CANDeviceControl.o DeviceManager.o \
PresetManager.o CommLinkManager.o rs232.o RS485DevControl.o ModbusProc.o RFRemoteControl.o \
RemoteButtonManager.o RemoteButtonClass.o ControlMessage.o GSIOTEvent.o AutoCallEvent.o AutoControlEvent.o \
AutoEventthing.o AutoNoticeEvent.o AutoSendSMSEvent.o XmppRegister.o  SerialDataBuffer.o SerialMessage.o \
HeartbeatMon.o CommLink.o NetBaseClient.o NetMan.o NetModule.o NetTcpClient.o NetUdpClient.o \
RFDeviceControl.o RFDevice.o RFSignalDefine.o XmppGSChange.o XmppGSEvent.o XmppGSReport.o \
XmppGSRelation.o XmppGSUpdate.o XmppGSVObj.o VObjClass.o TimerManager.o HttpRequest.o gsiot.o

bin = gsiot
CFLAGS += -I.
LIBS += -lpthread
LIBS += -lstdc++
LIBS += -lgloox
LIBS += -lsqlite3

#LIBDIR = -L/lib/
all : $(bin)
$(bin) : $(OBJS)
	$(CXX) $(CFLAGS) -Wall  $(OBJS) $(LIBDIR) -o $(bin) $(LIBS)
clean :
	$(RM) -f $(OBJS)
	$(RM) -f $(bin)
