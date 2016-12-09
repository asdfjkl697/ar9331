#ifndef GSIOTCLIENT_H
#define GSIOTCLIENT_H

#include "gloox/client.h"
#include "gloox/connectionlistener.h"
#include "gloox/adhoccommandprovider.h"
#include "gloox/disco.h"
#include "gloox/adhoc.h"
#include "gloox/tag.h"
#include "gloox/loghandler.h"
#include "gloox/logsink.h"

#include "gloox/subscriptionhandler.h"
#include "gloox/messagehandler.h"
#include "gloox/message.h"

#include "gloox/util.h"
#include "gloox/iqhandler.h" //20160603
#include "GSIOTDevice.h" //20160603
#include "GSIOTConfig.h" //20160603
#include "GSIOTEvent.h" //20160823
#include "DeviceConnection.h" //20160823
#include "ControlMessage.h" //20160819

#include "TimerManager.h" //jyc20160825

#include "XmppGSResult.h"
#include "XmppGSAuth.h"
#include "XmppGSState.h"

#include "XmppGSManager.h"
#include "XmppGSEvent.h"
#include "XmppGSRelation.h"
#include "XmppGSPreset.h"
#include "XmppGSVObj.h"
//#include "XmppGSTrans.h"
#include "XmppGSReport.h"
#include "XmppGSUpdate.h"
#include "IGSMessageHandler.h"

#include "RunCode.h"
#include "DataStoreMgr.h"

#include "typedef.h"

namespace httpreq
{
	#include "HttpRequest.h"
}

#define LOGMSG printf

struct struEventNoticeMsg
{
	std::string id;
	uint32_t starttime;
	JID to_jid;
	std::string strSubject;
	std::string strBody;
	std::string callinfo;

	struEventNoticeMsg()
		: starttime(0)
	{
	}

	struEventNoticeMsg( const std::string &in_id, uint32_t in_starttime, const JID &in_to_jid, const std::string &in_strSubject, const std::string &in_strBody, const char *in_callinfo )
		: id(in_id), starttime(in_starttime), to_jid(in_to_jid), strSubject(in_strSubject), strBody(in_strBody), callinfo(in_callinfo)
	{
	}
};

using namespace gloox;

std::string g_IOTGetVersion();
std::string g_IOTGetBuildInfo();


class GSIOTClient: public IDeviceHandler, IqHandler, SubscriptionHandler, ConnectionListener,
	MessageHandler, ITimerHandler, public TagHandler 
{
private:
	GSIOTConfig *m_cfg; 
	GSIOTEvent *m_event;

	gloox::util::Mutex m_mutex_ctlMessageList;
	std::list<ControlMessage *> ctlMessageList;
	std::list<GSIOTDevice *> IotDeviceList; 
	
	DeviceConnection *deviceClient;
	

	
	gloox::Client *xmppClient;
	TimerManager *timer; 
	IDeviceHandler *m_handler; 
	bool serverPingCount;
	int timeCount;
	
	IGSMessageHandler *m_IGSMessageHandler;
	ITriggerDebugHandler *m_ITriggerDebugHandler; //jyc20160919
	bool m_EnableTriggerDebug; //jyc20160919

	gloox::util::Mutex m_mutex_lstGSMessage;
	std::list<GSMessage*> m_lstGSMessage;

	gloox::util::Mutex m_mutex_lstEventNoticeMsg;
	std::map<std::string,struEventNoticeMsg*> m_lstEventNoticeMsg; // <string id,value>
	
private:
	void Register();

public:
	GSIOTClient(/*ITimerHandler *handler,*/ IDeviceHandler *handler, const std::string &RunParam ); //jyc20160826
	~GSIOTClient(void);

	static ControlBase* CloneControl( const ControlBase *src, bool CreateLock=true );
	static DeviceAddress* CloneDeviceAddress( const DeviceAddress *src );
	static bool Compare_Device( const GSIOTDevice *devA, const GSIOTDevice *devB );
	static bool Compare_Control( const ControlBase *ctlA, const ControlBase *ctlB );
	static bool Compare_Address( const DeviceAddress *AddrA, const DeviceAddress *AddrB );
	static bool Compare_GSIOTObjBase( const GSIOTObjBase *ObjA, const GSIOTObjBase *ObjB );
	static bool Compare_ControlAndAddress( const ControlBase *ctlA, const DeviceAddress *AddrA, const ControlBase *ctlB, const DeviceAddress *AddrB );	

	void RunCodeInit();
	CRunCodeMgr m_RunCodeMgr;

	void PreInit( const std::string &RunParam );
	bool GetPreInitState() const
	{
		return m_PreInitState;
	}

	void CheckSystem();
	void CheckIOTPs();

	void OnDeviceNotify( defDeviceNotify_ notify, GSIOTDevice *iotdevice, DeviceAddress *addr );
	void OnDeviceDisconnect(GSIOTDevice *iotdevice);
	void OnDeviceConnect(GSIOTDevice *iotdevice);
	void OnDeviceData( defLinkID LinkID, GSIOTDevice *iotdevice, ControlBase *ctl, GSIOTObjBase *addr );
	void OnDeviceData_ProcOne( defLinkID LinkID, GSIOTDevice *iotdevice, ControlBase *ctl, DeviceAddress *addr );
	
	void DoAlarmDevice( const GSIOTDevice *iotdevice, const bool AGRunState, const int AlarmGuardGlobalFlag, const bool IsValidCurTime, const std::string &strAlmBody, const std::string &strAlmSubject );
	bool DoControlEvent( const IOTDeviceType DevType, const uint32_t DevID, ControlEvent *ctlevt, const bool isAlarm, const std::string &strAlmBody, const std::string &strAlmSubject, const char *callinfo, const bool isTest=false, const char *teststr=NULL );
	void DoControlEvent_Eventthing( const AutoEventthing *aEvt, const ControlEvent *ctlevt, const char *callinfo, const bool isTest );
	void DoControlEvent_Eventthing_Event( GSIOTDevice *dev, const AutoEventthing *aEvt, const ControlEvent *ctlevt, const char *callinfo, const bool isTest );
	GSAGCurState_ GetAllEventsState() const;
	void SetAllEventsState( const bool AGRunState, const char *callinfo, const bool forcesave );
	
private:
	void SetAllEventsState_do( const bool AGRunState, const bool forcesave, bool isEditCam );

public:
	GSAGCurState_ GetAlarmGuardCurState() const;
	int GetAlarmGuardGlobalFlag() const; //jyc20160919
	void SetAlarmGuardGlobalFlag( int flag );
	
	void SetITriggerDebugHandler( ITriggerDebugHandler *handler );
	bool GetEnableTriggerDebugFlag() const
	{
		return m_EnableTriggerDebug;
	}
	void EnableTriggerDebug( bool enable )
	{
		m_EnableTriggerDebug = enable;
	}
	void AddGSMessage( GSMessage *pMsg );
	GSMessage* PopGSMessage();
	void OnGSMessageProcess();

	/*xmpp handler*/
	virtual void onConnect();
	virtual void onDisconnect( ConnectionError e );
	virtual bool onTLSConnect( const CertInfo& info );
	void handleIqID( const IQ& iq, int context );
	bool handleIq( const IQ& iq );
	void handleSubscription( const Subscription& subscription );
	void handleMessage( const Message& msg, MessageSession* session = 0 );
	
	virtual void handleTag( Tag* tag );

	void handleIq_Get_XmppGSAuth_User( const XmppGSAuth_User *pExXmppGSAuth_User, const IQ& iq, const GSIOTUser *pUser );
	void handleIq_Get_XmppGSAuth( const XmppGSAuth *pExXmppGSAuth, const IQ& iq, const GSIOTUser *pUser );
	void handleIq_Set_XmppGSAuth( const GSMessage *pMsg );
	void handleIq_Set_XmppGSManager( const GSMessage *pMsg );
	void handleIq_Set_XmppGSEvent( const GSMessage *pMsg );
	void handleIq_Set_XmppGSRelation( const GSMessage *pMsg );
	void handleIq_Get_XmppGSRelation( const XmppGSRelation *pXmpp, const IQ& iq, const GSIOTUser *pUser );
	void handleIq_Get_XmppGSReport( const XmppGSReport *pXmpp, const IQ& iq, const GSIOTUser *pUser );

public:
	bool add_GSIOTDevice( GSIOTDevice *pDeviceSrc );
	bool edit_GSIOTDevice( GSIOTDevice *pDeviceSrc );
	bool delete_GSIOTDevice( GSIOTDevice *pDeviceSrc );

private:
	bool edit_RS485DevControl( GSIOTDevice *pLocalDevice, GSIOTDevice *pDeviceSrc );
	bool edit_RFRemoteControl( GSIOTDevice *pLocalDevice, GSIOTDevice *pDeviceSrc );

public:
	
	bool add_ControlEvent( ControlEvent *pSrc );
	bool edit_ControlEvent( ControlEvent *pSrc );
	bool delete_ControlEvent( ControlEvent *pSrc );
	// --------------------------
public:
	static void XmppPrint( const Tag *ptag, const char *callinfo, const Stanza *stanza, bool dodel=true );
	static void XmppPrint( const IQ& iq, const char *callinfo );
	static void XmppPrint( const Message& msg, const char *callinfo );
	void XmppClientSend( const IQ& iq, const char *callinfo );
	void XmppClientSend_msg( const JID &to_jid, const std::string &strBody, const std::string &strSubject, const char *callinfo );
	void XmppClientSend_jidlist( const std::set<std::string> &jidlist, const std::string &strBody, const std::string &strSubject, const char *callinfo );

	/*timer*/
	void OnTimer( int TimerID );
	std::string GetConnectStateStr() const;

	void Run();
	void Connect();
	void Stop(void);
	bool CheckRegistered();
	void LoadConfig();
	bool SetJidToServer( const std::string &strjid, const std::string &strmac );
	void ResetNoticeJid();

	DeviceConnection *GetDeviceConnection() 
	{
		return this->deviceClient;
	}

	GSIOTEvent *GetEvent()
	{
		return this->m_event;
	}

	GSIOTDevice* GetIOTDevice( IOTDeviceType deviceType, uint32_t deviceId ) const;
	std::string GetAddrObjName( const GSIOTAddrObjKey &AddrObjKey ) const;
	std::string GetDeviceAddressName( const GSIOTDevice *device, uint32_t address ) const;
	bool DeleteDevice( GSIOTDevice *iotdevice );
	bool ModifyDevice_Ver( GSIOTDevice *iotdevice, const std::string ver );
	
	void PushControlMesssageQueue( ControlMessage *pCtlMsg );
	bool CheckControlMesssageQueue( GSIOTDevice *device, DeviceAddress *addr, JID jid, std::string id );
	ControlMessage* PopControlMesssageQueue( GSIOTDevice *device, ControlBase *ctl, DeviceAddress *addr, \
	       	 IOTDeviceType specType=IOT_DEVICE_Unknown, IOTDeviceType specExType=IOT_DEVICE_Unknown );
	void CheckOverTimeControlMesssageQueue();

	bool Update_DoUpdateNow( uint32_t &err, std::string runparam = "-update" );
	void Update_DoUpdateNow_fromremote( const JID &fromjid, const std::string& from_id = EmptyString, std::string runparam = "-update" );
	void Update_UpdatedProc();

	bool EventNoticeMsg_Add( struEventNoticeMsg *msg );
	void EventNoticeMsg_Remove( const std::string &id );
	void EventNoticeMsg_Check();
	void EventNoticeMsg_Send( const std::string &tojid, const std::string &subject, const std::string &body, const char *callinfo );


	bool is_running() const
	{
		return m_running;
	}
	
	void DataProc_ThreadCreate();
	bool DataProc();
	void DataSave();
	void DataStatCheck();
	void OnDataProcThreadExit()
	{
		m_isDataProcThreadExit = true;
	}


	
	CDataStoreMgr* GetDataStoreMgr( bool autoCreate=false )
	{
		if( autoCreate && !m_DataStoreMgr )
		{
			m_DataStoreMgr = new CDataStoreMgr();
		}

		return m_DataStoreMgr;
	}

	void AlarmProc_ThreadCreate();
	bool AlarmProc();
	bool AlarmCheck();
	void OnAlarmProcThreadExit()//告警处理线程退出
	{
		m_isAlarmProcThreadExit = true;
	}

	std::string GetSimpleInfo( const GSIOTDevice *const iotdevice );
	std::string GetSimpleInfo_ForSupportAlarm( const GSIOTDevice *const iotdevice );
	std::string getstr_ForRelation( const deflstRelationChild &ChildList );
	
	defGSReturn SendControl( const IOTDeviceType DevType, const GSIOTDevice *device, const GSIOTObjBase *obj, const uint32_t overtime=defNormSendCtlOvertime, const uint32_t QueueOverTime=defNormMsgOvertime, const uint32_t nextInterval=1, const bool isSync=false );
	
private:
	bool m_PreInitState;
	bool m_running;

	time_t m_IOT_starttime;
	std::string m_str_IOT_starttime;
	bool m_xmppReconnect;

	Parser m_parser;
	std::string m_strVerLocal;
	std::string m_strVerNew;
	bool m_retCheckUpdate;
	bool m_isThreadExit;
	bool m_isPlayBackThreadExit;
	bool m_isPlayMgrThreadExit;

public:
	

private:
	void CheckOverTimeControlMesssageQueue_nolock();
	
	uint32_t m_last_checkNetUseable_time;
	bool m_isDataProcThreadExit;
	bool m_isAlarmProcThreadExit;

	// DataStore 
	CDataStoreMgr *m_DataStoreMgr;
	gloox::util::Mutex m_mutex_DataStore;
	std::list<struDataSave*> m_lstDataSaveBuf;
	struDataSave* DataSaveBuf_Pop();
	bool DataSaveBuf_Pop( defvecDataSave &vecDataSave );
	
	// LED Show
	uint32_t m_lastShowLedTick;

	// AC
	uint32_t m_lastcheck_AC;
	bool m_isACProcThreadExit;
};

#endif
