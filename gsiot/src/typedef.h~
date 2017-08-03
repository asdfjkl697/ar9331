#ifndef TYPEDEF_H
#define TYPEDEF_H

#define GSIOT_VERSION	"1.7.3"	// update 201602����ʽ��n.n.n �ַ�
#define GSIOT_DBVER		15060800	// ��ݿ�汾

//#define OS_UBUNTU_FLAG 1
#define OS_OPENWRT 1

#ifdef OS_UBUNTU_FLAG
#define ROOTDIR  "/home/chen/"
#else
#define ROOTDIR  "/root/"
#endif
	

#ifdef WIN32  
#include <WinSock2.h>
#include <Windows.h>  
#include <stdio.h>  
#else   //20160527
#define  MAX_PATH 260  
#include <unistd.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <stdarg.h>  
#endif 

#include <stdint.h>
#include <list>
#include <map>
#include <set>
#include <vector>
#include "gloox/stanzaextension.h"

//#include "..//..//include//memmacro.h"

#include <string> //20150526
using  std::string;


const std::string c_NullStr = "";
const std::string c_ZeroStr = "0";
const std::string c_DefaultVer = "1.0";

#define XMPP_SERVER_DOMAIN "gsss.cn"
#define XMPP_GSIOTUser_Admin "admin@gsss.cn" // ȫСд
#define XMPP_GSIOTUser_Guest "guest@gsss.cn" // ȫСд
#define XMPP_GSIOTUser_DefaultDomain "@gsss.cn" // �û�����Ĭ��ֵ

#define defRTMFPSession_strjid "RTMFP@local.net/rtmfpthd"	// RTMFP ���ػỰ��
#define defAutoPublish_strjid "AutoPublish@local.net/publishthd"	// ���ַ���/�Զ�����

#define defIotStoreDir_Main	"E:\\iotstore"
#define defIotStoreDir_Data	"data"
#define defIotStoreDir_Pic	"pic"

enum defPicPreSize
{
	defPicPreSize_Unknown = 0,
	defPicPreSize_Full,
	defPicPreSize_Norm,
	defPicPreSize_NS,
	defPicPreSize_S,

	defPicPreSize_MAX,
};

//static char *str_PicPreSize[] = //20160527
static string str_PicPreSize[] =
{
	"nil",
	"full",
	"nm",
	"ns",
	"s",
	""
};


#define XMPP_MESSAGE_PREHEAD	"<SYS>"

const std::string XMLFUNC_media_rtmfp = "media:rtmfp";

const std::string XMLNS_GSIOT = "http://www.gswww.cn/protocol/iot";
const std::string XMLNS_GSIOT_RESULT = "http://www.gswww.cn/protocol/result";
const std::string XMLNS_GSIOT_CONTROL = "http://www.gswww.cn/protocol/device#control";
const std::string XMLNS_GSIOT_DEVICE = "http://www.gswww.cn/protocol/device#info";
const std::string XMLNS_GSIOT_HEARTBEAT = "http://www.gswww.cn/protocol/heartbeat";
const std::string XMLNS_GSIOT_AUTHORITY = "http://www.gswww.cn/protocol/authority";
const std::string XMLNS_GSIOT_AUTHORITY_USER = "http://www.gswww.cn/protocol/authority#user";
const std::string XMLNS_GSIOT_MANAGER = "http://www.gswww.cn/protocol/device#manager";
const std::string XMLNS_GSIOT_EVENT = "http://www.gswww.cn/protocol/event";
const std::string XMLNS_GSIOT_STATE = "http://www.gswww.cn/protocol/device#state";
const std::string XMLNS_GSIOT_TALK = "http://www.gswww.cn/protocol/talk";
const std::string XMLNS_GSIOT_PLAYBACK = "http://www.gswww.cn/protocol/playback";
const std::string XMLNS_GSIOT_RELATION = "http://www.gswww.cn/protocol/relation";
const std::string XMLNS_GSIOT_Preset = "http://www.gswww.cn/protocol/preset";
const std::string XMLNS_GSIOT_VObj = "http://www.gswww.cn/protocol/vobj";
const std::string XMLNS_GSIOT_Trans = "http://www.gswww.cn/protocol/trans";
const std::string XMLNS_GSIOT_Report = "http://www.gswww.cn/protocol/report";
const std::string XMLNS_GSIOT_Change = "http://www.gswww.cn/protocol/change";
const std::string XMLNS_GSIOT_MESSAGE = "http://www.gswww.cn/protocol/message";
const std::string XMLNS_GSIOT_UPDATE = "http://www.gswww.cn/protocol/update";

#define defDeviceTypeTag_media			"media"
#define defDeviceTypeTag_camera			"camera"
#define defDeviceTypeTag_rfdevice		"rfdevice"
#define defDeviceTypeTag_candevice		"candevice"
#define defDeviceTypeTag_rs485device	"rs485device"
#define defDeviceTypeTag_rfremote		"rfremote"

typedef int defLinkID;
#define defLinkID_Local		defLinkID(0)		// ��������ģ�� ��·ID
#define defLinkID_Null		defLinkID(-1)		// ����·
#define defLinkID_All		defLinkID(-9)		// ������·

#define defDeviceEnable		1
#define defDeviceDisable	0

#define defAlarmGuardTime_UnAllDay		0 // ȫ�쳷��
#define defAlarmGuardTime_AllDay		1 // ȫ�첼��
#define defAlarmGuardTime_SpecT			2 // ָ��ʱ���

#define macAlarmGuardTime_InvaildAG(flag,begintime,endtime) ( !(flag) || (begintime)==(endtime) ) // ʱ����Ƿ���Ч

#define defAlarmGuard_AGTimeCount		3 // ʱ�������

// ����״̬
enum defUseable
{
	// С��0 �쳣
	defUseable_ErrConfig	= -4,	// �쳣 - ���ô���
	defUseable_ErrNoData	= -3,	// �쳣 - ����ͨ�����
	defUseable_ErrNet		= -2,	// �쳣 - �����ж�
	defUseable_Err			= -1,	// �쳣

	defUseable_Null			= 0,	// δ֪״̬����ʼ״̬��Ĭ��״̬

	// ����0 ��
	defUseable_OK			= 1,	// ��
};

// ����ģʽ
enum defTransMod
{
	defTransMod_Transparent		= 0, // ͸��
	defTransMod_GSIOT			= 1, // ���ư�Э��
};

// ����״̬
enum defConnectState
{
	defConnectState_Null		= 0,	// δ֪
	defConnectState_Connecting,			// ������
	defConnectState_Connected,			// ������
	defConnectState_Disconnected,		// �����ж�
};

// �澯״̬
enum defAlarmState
{
	// С��0 �澯
	defAlarmState_NormAlarm		= -1,	// �澯

	defAlarmState_UnInit		= 0,	// δ֪״̬����ʼ״̬��Ĭ��״̬

	// ����0 ��
	defAlarmState_OK			= 1,	// ��
};
#define macAlarmState_IsAlarm(st) (st<0)
#define macAlarmState_IsOK(st) (st>0)

enum defGSReturn
{
	defGSReturn_SuccExecuted	= 2,		// ��ִ�У�û�н��
	defGSReturn_Success			= 1,		// �ɹ�
	defGSReturn_Null			= 0,		// û�з��أ�����ȷ���ɹ�ʧ�ܵ������û�н��
	defGSReturn_Err				= -1,		// ����һ���Դ�������ȷ����ԭ��ʱ���ش˴�����
	defGSReturn_NoExist			= -2,		// Ŀ�겻���ڶ���ʧ�ܣ���������ͷ������
	defGSReturn_IsExist			= -3,		// Ŀ���Ѵ��ڶ���ʧ�ܣ���������û�ʱ�û��Ѵ���
	defGSReturn_IsLock			= -4,		// Ŀ���ѱ�������ʹ��
	defGSReturn_NoAuth			= -5,		// ��Ȩ��
	defGSReturn_ResLimit		= -6,		// ��Դ�����Ѵ�����
	defGSReturn_FunDisable		= -7,		// �������Ρ����ܽ��á������ѹرյ�
	defGSReturn_IsSelf			= -8,		// ���ܶ��������
	defGSReturn_ObjDisable		= -9,		// �������Ρ�������õȣ�
	defGSReturn_ConnectObjErr	= -10,		// ���Ӷ���ʧ�ܣ�������������ͷʧ��
	defGSReturn_ConnectSvrErr	= -11,		// ���ӷ�����ʧ�ܣ�����������ý�������ʧ��
	defGSReturn_CreateObjErr	= -12,		// ��������ʧ�ܣ����紴��¼��طŻỰ
	defGSReturn_NoData			= -13,		// ����ݴ���
	defGSReturn_NotFoundContent	= -14,		// �Ҳ������ݣ������Ҳ������������¼��
	defGSReturn_TimeOut			= -15,		// ��ʱ
	defGSReturn_UnSupport		= -16,		// ���ܲ�֧��
	defGSReturn_ErrUser			= -17,		// �û������
	defGSReturn_ErrPassword		= -18,		// �������
	defGSReturn_ErrParam		= -19,		// ����������
	defGSReturn_ErrConfig		= -20,		// ������Ϣ�쳣�������ô���
	defGSReturn_NameEmpty		= -21,		// ���Ϊ�մ���
	defGSReturn_ObjEditDisable	= -22,		// Ŀ������ֹ�޸�
	defGSReturn_SameName		= -23,		// ��������Ѵ��ڴ���
	defGSReturn_SaveFailed		= -24,		// �洢ʧ��
	defGSReturn_DBNoExist		= -25,		// Ŀ����ݿⲻ����
	defGSReturn_DBNoRec			= -26,		// Ŀ����ݼ�¼������
	defGSReturn_OverTimeRange	= -27,		// ʱ�䳬����Χ
	defGSReturn_TimeNotYet		= -28,		// ʱ��δ��
};
#define macGSIsReturnNull(ret) (defGSReturn_Null==(ret))
#define macGSSucceeded(ret) (ret>0)
#define macGSFailed(ret) (ret<0)

#define defGSReturnStr_Succeed		"succeed"
#define defGSReturnStr_Fail			"fail"

#define defDoInterval_MAX				60480000// ִ�м�����ֵ������
#define defDoInterval_UseSysDefault		0		// ����ϵͳĬ��
#define macIsUseSysDefault_DoInterval( DoInterval ) ( DoInterval > defDoInterval_MAX )
#define macUseDoInterval( DoInterval ) ( DoInterval>defDoInterval_MAX ? defDoInterval_MAX:DoInterval )

#define defEventLevel_Default 5

enum defDataFlag_
{
	defDataFlag_Invalid		= 0,	// ��Ч
	defDataFlag_First		= 1,	// �������һ�δ洢
	defDataFlag_Norm		= 2,	// ��ͨ
	defDataFlag_TimePoint	= 3,	// ʱ���洢
	defDataFlag_Changed		= 4,	// ����仯�洢
	defDataFlag_Manual		= 5,	// �ֶ��洢
};

enum defCfgOprt_
{
	defCfgOprt_Unknown = 0,	// δ֪
	defCfgOprt_Add,			// ֻ������ӣ��Ѵ�����ʧ��
	defCfgOprt_Modify,		// ֻ�����޸ģ���������ʧ��
	defCfgOprt_Delete,
	defCfgOprt_AddModify,	// ��ӻ��޸ģ��Ѵ����޸ģ����������
	defCfgOprt_BatchModify,	// �����޸�
	defCfgOprt_GetSelf,		// ֻ��ȡ������Ϣ�������ȡ�û���Ϣʱ
	defCfgOprt_GetSimple,	// ��ȡ�򻯵���Ϣ
};

enum defNotify_
{
	defNotify_Unknown = 0,
	defNotify_Add,
	defNotify_Update,
	defNotify_Delete
};

enum defTalkSendCmd_
{
	defTalkSendCmd_Open,
	defTalkSendCmd_Play,
	defTalkSendCmd_Close,
};

//�������
enum defCmd
{
	defCmd_Null		= 0,					// ��
	defCmd_Default	= 1,					// Ĭ��

	defCmd_Set_Spec_Begin_		= 100,		//--
	defCmd_Set_Open				= 101,		// �򿪣���ơ�����
	defCmd_Set_Close			= 102,		// �رգ���صơ�����
	defCmd_Set_Spec_End_		= 499,		//--

	defCmd_Get_Spec_Begin_		= 500,		//--
	defCmd_Get_ObjState			= 501,		// ��ȡ����״̬�����ȡ��״̬����״̬�ȵ�
	defCmd_Get_ConnectState		= 502,		// ��ȡ����״̬
	defCmd_Get_Spec_End_		= 899,		//--

	defCmd_Sys_Spec_			= 900,		//--
	defCmd_Sys_Reboot			= 901,		// �����Ž�����/������
};
inline bool g_IsReadOnlyCmd( const defCmd cmd )
{
	return ( cmd>=defCmd_Get_Spec_Begin_ && cmd<=defCmd_Get_Spec_End_ );
}

enum GSIOTExtensionType
{
	ExtIot = gloox::ExtUser+1,
	ExtIotResult,
	ExtIotControl,
	ExtIotDeviceInfo,
	ExtIotHeartbeat,
	ExtIotAuthority,
	ExtIotAuthority_User,
	ExtIotManager,
	ExtIotEvent,
	ExtIotState,
	ExtIotChange,
	ExtIotTalk,
	ExtIotPlayback,
	ExtIotRelation,
	ExtIotPreset,
	ExtIotVObj,
	ExtIotTrans,
	ExtIotReport,
	ExtIotMessage,
	ExtIotUpdate
};

enum IOTDeviceType
{
    IOT_DEVICE_Unknown = 0,	// ������豸
	IOT_DEVICE_Video,		// ��Ƶ
	IOT_DEVICE_Audio,		// ����
	IOT_DEVICE_Switch,		// ����
	IOT_DEVICE_Temperature,	// �¶�
	IOT_DEVICE_Humidity,	// ʪ��
	IOT_DEVICE_Trigger,		// ������
	IOT_DEVICE_Camera,		// �����
	IOT_DEVICE_Remote,		// ң��
	IOT_DEVICE_RFDevice,	// �����豸
	IOT_DEVICE_CANDevice,	// CAN�豸
	IOT_DEVICE_Event,		// ���¼�
	IOT_DEVICE_RS485,		// RS485
	IOT_DEVICE_Wind,		// ����
	IOT_DEVICE_CO2=15,         //14
	IOT_DEVICE_PM25,		//jyc20170304 add
	IOT_DEVICE_HCHO=19,
	IOT_DEVICE_Custom=100,	// �Զ���

	IOTDevice_ExSpec_Begin_		= 500,	//-- ������
	IOTDevice_AC_Ctl			= 501,	// �Ž�����/������
	IOTDevice_AC_Door			= 502,	// �Ž� - �Ŷ���
	IOTDevice_Combo_Ctl			= 503,	// ��Ͽ���
	IOTDevice_ExSpec_End_		= 900,	//-- ������

	IOT_DEVICE_All=999,		// �����豸

	IOT_Obj_SYS				=7000,	// �������� - ϵͳ����
	IOT_Obj_User			=7001,	// �������� - �û�
	IOT_Obj_CommLink		=7002,	// �������� - ��·

	IOT_VObj_scene			=8001,	// ������� - ���������Խ����ֲ�ͬ���������һ�������£�����һ��"����"��
	IOT_VObj_union			=8002,	// ������� - ���󼯺ϡ����Խ����ֲ�ͬ���󼯺ϵ�һ����������У�����һ��������ʹ�õ��������(�����豸)����Ϊ"���󼯺�"��
	
	// "����"��"���󼯺�"��������λ���кܴ����"���󼯺�"��Сһ��������豸���"����"�Ǹ������⻷�����"���󼯺�"�ɱ�����"����"�
	// * �����Ԥ��"���󼯺�"���԰�"���󼯺�"����������Ŀǰ�ݲ�ʵ�ֺ�ʹ�á�

	IOT_Module_Unknown		=90000, // ����ģ�鶨��
	IOT_Module_authority,			// Ȩ�޹���
	IOT_Module_record,				// ¼��
	IOT_Module_system,				// ϵͳ����
	IOT_Module_manager,				// �豸����
	IOT_Module_event,				// �¼�����
	IOT_Module_talk,				// Զ��ͨ��
	IOT_Module_reboot,				// Զ������
	IOT_Module_acctl,				// �Ž����
	IOT_Module_all			=90999	// ���й���ģ��
};

#define defIOTDeviceAddr_DevSelf	0	// ����ַ���豸���ͺ�IDһ��ʱ����ֵ��ʾ�豸����
#define defIOTDeviceAddr_AllAddr	-1	// ����ַ���豸���ͺ�IDһ��ʱ����ֵ��ʾ�豸�����е�ַ
#define defIOTDeviceAddr_Other		-2	// ����ַ���豸���ͺ�IDһ��ʱ����ֵ��ʾ�豸�����������

// һ���Է���resultЭ��module����
enum defNormResultMod_
{
	defNormResultMod_null						= 0,	// ��

	// ����ռ�=XMLNS_GSIOT_CONTROL
	defNormResultMod_control_camplay			= 1,	// ��Ƶ����
	defNormResultMod_control_MotionTrack		= 2,	// �򿪹ر����ܸ��ٲ���
	defNormResultMod_control_PTZ_ParkAction		= 3,	// �򿪹ر��������
};

enum defAuthFlag
{
	defAuthFlag_Empty		= 0x00
};

// �û�Ȩ��
//0 ��Ȩ��,1ֻ��,2д��3��д
enum defUserAuth
{
	defUserAuth_Null = 0,
	defUserAuth_RO = 1,
	defUserAuth_WO,
	defUserAuth_RW
};

// ����Ȩ�޵��ж�ֵ
#define defAllAuth_DeviceType		IOT_DEVICE_All
#define defAllAuth_DeviceID			0
#define defAllAuth_DeviceAuth		defUserAuth_RW
#define defAllAuth_DeviceAuth_Null	defUserAuth_Null
#define defAllAuth_DeviceShowName	"�����豸"

// ����Ȩ�޵��ж�ֵ
#define defAllAuth_ModuleType		IOT_Module_all
#define defAllAuth_ModuleID			0
#define defAllAuth_ModuleAuth		defUserAuth_RW
#define defAllAuth_ModuleAuth_Null	defUserAuth_Null

// ����ģ���IDĬ��ֵ
#define defAuth_ModuleDefaultID		0

enum IOTDeviceReadType
{
	IOT_Unknow,
	IOT_READ,
	IOT_WRITE,
	IOT_READWRITE
};

enum SerialPortModule
{
	MOD_NULL			= 0x00,

	RXB8_315			= 0x01,
	RXB8_433			= 0x02,
	RXB8_315_TX			= 0x03,
	RXB8_433_TX			= 0x04,
	CC1101_433			= 0x05,
	CAN_Chip			= 0x06,
	Module_RS485		= 0x07,
	RXB8_315_original	= 0x08,
	RXB8_433_original	= 0x09,
	Module_GSM			= 0x0A,
	Read_IOT_Ver		= 0x0B,

	// V2
	MOD_IR_RX_original	= 0x30,	// �������	ֻ��
	MOD_IR_RX_code		= 0x31,	// ������ձ���	ֻ��
	MOD_IR_TX_original	= 0x3A,	// ���ⷢ��	ֻд
	MOD_IR_TX_code		= 0x3B,	// ���ⷢ�����	ֻд
	MOD_SYS_set			= 0xE0, // ϵͳ����
	MOD_SYS_get			= 0xEA, // ϵͳ���÷���
};

enum DataType
{
	IOT_DataUnknow  = 0x00,
	IOT_Integer = 0x01,
    IOT_String	= 0x02,
    IOT_Boolean = 0x03,
	IOT_Byte	= 0x04,
	IOT_Int16	= 0x05,
	IOT_Long	= 0x06,
	IOT_Double	= 0x07,
	IOT_Float	= 0x08,
	IOT_Object  = 0x09
};

enum EventType
{
	Unknown_Event	= 0,
	SMS_Event		= 1,
	EMAIL_Event,
	NOTICE_Event,
	CONTROL_Event,
	Eventthing_Event,
	CALL_Event
};

// Ʒ�Ƴ���
enum defFactory
{
	defFactory_Unknown = 0,	// ��
	CameraType_Unkown = defFactory_Unknown,
	
	SSD1935			= 1,
	TI368			= 2,
	CameraType_hik	= 3,		// ����
	CameraType_dh	= 4,		// ��
	
	defFactory_ZK	= 5,			// �пأ��Ž�

	defFactory_HIK	=CameraType_hik,	// ������ȡ������ ����
	defFactory_DH	=CameraType_dh,		// ������ȡ������ ��
};
typedef defFactory IPCameraType;


enum defRecMod
{
	defRecMod_NoRec = 0,	// ��¼��
	defRecMod_OnCamera,		// ��������豸��
	defRecMod_OnReordSvr	// ��¼���������
};

// ��̨���ܱ�־
enum GSPtzFlag
{
	GSPtzFlag_Null = 0,					// ��
	GSPtzFlag_forward_direction = 1,	// ����
	GSPtzFlag_negative_direction		// ������
};

// �������ܱ�־
enum GSFocalFlag
{
	GSFocalFlag_Null = 0,				// ��
	GSFocalFlag_Enable = 1				// ����
};

// Ԥ�õ����� 33���ϡ�100��ͷ��Щ��Щ���������ֵ��
// ��1-8�������������壬���ھ���ʹ��ʱ���趨����Ϊ������;���������ڶ�ʱ������
#define defGSPresetIndex_Min 201		// Ԥ�õ�������Сֵ
#define defGSPresetIndex_Max 232		// Ԥ�õ��������ֵ

#define GSPTZ_STOP_OFFSET		100
#define GSPTZ_FUNCTION_OFFSET	200
#define GSPTZ_SYSCMD_OFFSET		900
enum GSPTZ_CtrlCmd
{
	GSPTZ_Null	  = -1,		// 
	GSPTZ_STOPAll = 0,		// ��ֹ̨ͣ��ͨ��ֹͣ
	GSPTZ_UP,				// ��̨����
	GSPTZ_DOWN,				// ��̨����
	GSPTZ_RIGHT,			// ��̨����
	GSPTZ_RIGHT_UP,			// ��̨����
	GSPTZ_RIGHT_DOWN,		// ��̨����
	GSPTZ_LEFT,				// ��̨����
	GSPTZ_LEFT_UP,			// ��̨����
	GSPTZ_LEFT_DOWN,		// ��̨����
	GSPTZ_AUTO,				// ��̨�Զ�

	GSPTZ_ZOOM_IN =11,		// ������(���ʱ��) 
	GSPTZ_ZOOM_OUT,			// �����С(���ʱ�С) 
	GSPTZ_FOCUS_NEAR,		// ����ǰ��
	GSPTZ_FOCUS_FAR,		// ������
	GSPTZ_IRIS_OPEN,		// ��Ȧ����
	GSPTZ_IRIS_CLOSE,		// ��Ȧ��С

	//----------------------------------------
	// �����Ӧֹͣ
	GSPTZ_STOP_UP = GSPTZ_STOP_OFFSET+1,		// ��̨����
	GSPTZ_STOP_DOWN,			// ��̨����
	GSPTZ_STOP_RIGHT,			// ��̨����
	GSPTZ_STOP_RIGHT_UP,		// ��̨����
	GSPTZ_STOP_RIGHT_DOWN,		// ��̨����
	GSPTZ_STOP_LEFT,			// ��̨����
	GSPTZ_STOP_LEFT_UP,			// ��̨����
	GSPTZ_STOP_LEFT_DOWN,		// ��̨����
	GSPTZ_STOP_AUTO,			// ��̨�Զ�

	GSPTZ_STOP_ZOOM_IN = GSPTZ_STOP_OFFSET+11,	// ������(���ʱ��) 
	GSPTZ_STOP_ZOOM_OUT,		// �����С(���ʱ�С) 
	GSPTZ_STOP_FOCUS_NEAR,		// ����ǰ��
	GSPTZ_STOP_FOCUS_FAR,		// ������
	GSPTZ_STOP_IRIS_OPEN,		// ��Ȧ����
	GSPTZ_STOP_IRIS_CLOSE,		// ��Ȧ��С

	//----------------------------------------
	// Ԥ�õ�
	GSPTZ_Add_Preset = GSPTZ_STOP_OFFSET+21,		// ���Ԥ�õ�
	GSPTZ_Del_Preset,								// ɾ��Ԥ�õ�
	GSPTZ_Goto_Preset,								// ת��Ԥ�õ�
	GSPTZ_Rename_Preset,							// �޸�Ԥ�õ����
	GSPTZ_SetNew_Preset,							// ����Ԥ�õ㣬�����µ�λ�õ�һ���Ѵ��ڵ�Ԥ�õ�

	//----------------------------------------
	// ��๦������
	GSPTZ_MOTION_TRACK_Enable = GSPTZ_FUNCTION_OFFSET+1,	// ���ܸ��ٹ��� ��
	GSPTZ_MOTION_TRACK_Disable,								// ���ܸ��ٹ��� �ر�
	GSPTZ_MANUALTRACE,										// �ֶ����ٿ���
	GSPTZ_MANUALPTZSel,										// �ֶ���λ
	GSPTZ_MANUALZoomRng,									// �ֶ�����ѡ��Ŵ���С

	GSPTZ_PTZ_ParkAction_Enable,							// ������ ��
	GSPTZ_PTZ_ParkAction_Disable,							// ������ �ر�

	GSPTZ_DoPrePic,									// ���Ԥ��ͼ

	//----------------------------------------
	// ϵͳ������
	GSPTZ_CameraReboot = GSPTZ_SYSCMD_OFFSET+1,		// ���������
};

enum RFCommand
{
	Heartbeat_Request       = 0x0101,
	Heartbeat_Response      = 0x0102,
	Device_Error            = 0x0103,
	Device_Broadcast        = 0x0202,
	Device_Info_Request     = 0x0301,
	Device_Info_Response    = 0x0302,
	Device_Address_Request  = 0x0401,
	Device_Address_Response = 0x0402,
	Device_Write_Request    = 0x0501,
	Device_Write_Response   = 0x0502,
	Device_Setting_Request  = 0x0601,
	Device_Setting_Response = 0x0602
};

enum CANCommand
{
	CAN_Heartbeat_Request = 0x10,
	CAN_Heartbeat_Response = 0x11,
	CAN_Info_Request = 0x20,
	CAN_Info_Response = 0x21,
	CAN_Address_Request = 0x30,
	CAN_Address_Response= 0x31,
	CAN_Address_Read_Request = 0x40,
	CAN_Address_Read_Response = 0x41,
	CAN_Address_Write_Request = 0x50,
	CAN_Address_Write_Response = 0x51,
	CAN_Address_Setting_Request = 0x60,
	CAN_Address_Setting_Response = 0x61,
	CAN_Address_Packet_Ext = 0x70,
	CAN_Address_Error = 0x80
};

enum GSIOTObjType
{
	GSIOTObjType_Unknown = 0,
	GSIOTObjType_GSIOTDevice = 1,
	GSIOTObjType_DeviceAddress,
	GSIOTObjType_RemoteButton,
	GSIOTObjType_PresetObj,
};

// �طſ�����
enum GSPlayBackCode_
{
	GSPlayBackCode_Stop			= -1,
	GSPlayBackCode_NULL			= 0,
	GSPlayBackCode_GetState		= 1,

	GSPlayBackCode_PLAYPAUSE	= 11,	// ��ͣ����
	GSPlayBackCode_PLAYRESTART,			// �ָ����ţ�ֻ�ܴ���ͣʱ��״̬�ָ�����ţ���ǰ״̬������ͣʱ������Ч
	GSPlayBackCode_PLAYNORMAL,			// ���ٶȣ��κ�״̬�ָ������ģʽ
	GSPlayBackCode_PLAYFAST,			// ��ţ������ٶ���1�����ٶȼ�����2-��1-����-��1-��2�����ᳬ�����޺�����
	GSPlayBackCode_PLAYSLOW,			// ��ţ������ٶȽ���1��

	GSPlayBackCode_PLAYGETPOS	= 21,	// ���Ž�Ȱٷֱ�
	GSPlayBackCode_PLAYSETPOS,			// ��ת�����Ž�Ȱٷֱ�λ�ü����
	GSPlayBackCode_SETSPEED,			// ���������ٶȣ��ٶȵ�λ��kbps����СΪ256kbps
	GSPlayBackCode_PlaySetTime,			// ����ʱ�䶨λ
	GSPlayBackCode_SkipTime,			// ��ǰ����n�룬����n���ָ������ٶȲ���
};

enum GSDevViewMod_
{
	GSDevViewMod_Default = 0,
	GSDevViewMod_Normal = 1,
	GSDevViewMod_Debug = 123456
};


// ϵͳ�����ۺ����в����������������Ŀǰ�Ƿ��ڲ�����Ч״̬
// AlarmGuardCurState
enum GSAGCurState_
{
	GSAGCurState_UnArmed		= 0,  // ����
	GSAGCurState_AllArmed		= 1,  // ȫ����������Ч��
	GSAGCurState_PartOfArmed	= 2,  // ���ִ�������������Ч��
	GSAGCurState_WaitTimeArmed	= 3,  // ��ʱδ��Ч�ȴ������ﵽ���Զ���Ч��
};

struct struGSTime
{
    unsigned short Year;
    unsigned char Month;
    unsigned char Day;
    unsigned char Hour;
    unsigned char Minute;
    unsigned char Second;
    unsigned short Milliseconds;

	struGSTime(
		const unsigned short in_Year=0, const unsigned char in_Month=0, const unsigned char in_Day=0,
		const unsigned char in_Hour=0, const unsigned char in_Minute=0, const unsigned char in_Second=0, const unsigned short in_Milliseconds=0 )
		: Year(in_Year), Month(in_Month), Day(in_Day), Hour(in_Hour), Minute(in_Minute), Second(in_Second), Milliseconds(in_Milliseconds)
	{
	}

	void AddSelfMonth()
	{
		Month++;
		if( Month > 12 )
		{
			Month = 1;
			Year++;
		}
	}
	
	void SubtractSelfMonth()
	{
		Month--;
		if( Month < 1 )
		{
			Month = 12;
			Year--;
		}
	}
	bool IsValidYearMonthOnly() const
	{
		return (
			Year>0 && Year<2999
			&& Month>0 && Month<13
			);
	}

	bool IsValidDateOnly() const
	{
		return (
			IsValidYearMonthOnly()
			&& Day>0 && Day<32
			);
	}

	bool IsValid() const
	{
		return (
			IsValidDateOnly()
			&& Hour>=0 && Hour<25
			&& Minute>=0 && Minute<61
			&& Second>=0 && Second<61
			);
	}
};

struct GSIOTAddrObjKey
{
	IOTDeviceType dev_type;
	int dev_id;
	IOTDeviceType address_type;
	int address_id;

	GSIOTAddrObjKey( const IOTDeviceType in_dev_type, const int in_dev_id, const IOTDeviceType in_address_type, const int in_address_id )
		: dev_type(in_dev_type), dev_id(in_dev_id), address_type(in_address_type), address_id(in_address_id)
	{
	}

	GSIOTAddrObjKey()
		: dev_type(IOT_DEVICE_Unknown), dev_id(0), address_type(IOT_DEVICE_Unknown), address_id(0)
	{
	}

	std::string get_str( const bool simple=false, const std::string &strpre=c_NullStr ) const
	{
		char buf[32] = {0};

		if( simple )
			snprintf( buf, sizeof(buf), "%s%d.%d.%d.%d", strpre.c_str(), dev_type, dev_id, address_type, address_id );
		else
			snprintf( buf, sizeof(buf), "%sdev(%d,%d) addr(%d,%d)", strpre.c_str(), dev_type, dev_id, address_type, address_id );

		return std::string(buf);
	}
};

typedef struct _IotSwitch
{
	int state_default;
	int state;
}IotSwitch;

typedef struct _IotTemperature
{
	float fahrenheit;
	float celsius;
}IotTemperature;

typedef struct _IotMedia
{
	std::string address;
	std::string protocol;
	std::string state;
}IotVideo,IotAudio;

typedef struct _IotConfig
{
	std::string serialnumber;
	std::string jid;
	std::string password;
}IotConfig;

typedef struct _DeviceCamera
{
	int id;
	int type;
	std::string url;	
	int isStop;
}DeviceCamera;

typedef struct _rtsignal
{
	uint32_t one_low_time;
	uint32_t one_high_time;
	uint32_t zero_low_time;
	uint32_t zero_high_time;
	uint32_t pulse_split_time;
	uint32_t code;
	uint8_t pulse_bit_size;
	uint32_t trigger_count;
}REMOTESignal;

typedef struct _ipcambroadcast
{
	std::string deviceID;
	std::string ipAddr;
	std::string macAddr;
	std::string mask;
	std::string gateway;
	std::string dns;
	std::string dns2;
	uint16_t tcpPort;
	uint16_t webPort;
	bool enableDHCP;
	std::string name;
}IPCAMERABROADCAST;

struct x264_nal_t
{
    int i_ref_idc;  /* nal_priority_e */
    int i_type;     /* nal_unit_type_e */
    int b_long_startcode;
    int i_first_mb; /* If this NAL is a slice, the index of the first MB in the slice. */
    int i_last_mb;  /* If this NAL is a slice, the index of the last MB in the slice. */

    /* Size of payload in bytes. */
    int     i_payload;
    /* If param->b_annexb is set, Annex-B bytestream with startcode.
     * Otherwise, startcode is replaced with a 4-byte size.
     * This size is the size used in mp4/similar muxing; it is equal to i_payload-4 */
    unsigned char *p_payload;

	x264_nal_t()
	{
		i_ref_idc = 0;
		i_type = 0;
		b_long_startcode = 0;
		i_first_mb = 0;
		i_last_mb = 0;

		i_payload = 0;
		p_payload = NULL;
	}
};

// tagʱ�Ĳ�����Ϣ
struct struTagParam 
{
	bool isValid; // ������Чʱ����Ĭ��tag��������Чʱ��������Ϣtag
	bool isResult;
	int fmt;
	defUserAuth Auth;

	struTagParam( bool in_isValid=false, bool in_isResult=false, int in_fmt=0, defUserAuth in_Auth=defUserAuth_RW )
		:isValid(in_isValid), isResult(in_isResult), fmt(in_fmt), Auth(in_Auth)
	{
	}
};

#endif
