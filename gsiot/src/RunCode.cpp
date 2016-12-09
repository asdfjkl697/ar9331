#include "RunCode.h"
#include "common.h"
#include <cassert>
#include <set>

static char *s_pStrNull="";

#define defRunCode_CheckOK	0x55AA5A5A
#define StrNull s_pStrNull

enum defUseValType
{
	defUseValType_Default = 1,
	defUseValType_Min,
	defUseValType_Max,
};

// 无特殊说明都是 value1=defRunCodeVal_Enable 开启功能
static struRunCode s_RunCode[defCodeIndex_MAX_+1] = 
{
	{ 0, 0, 0, 0, 0, 0,0,0,0, "Unknown", StrNull, StrNull },						// 

	{ 100, 0, 1, 1024, 768, 0,0,0,0, "SYS_IOTWndMod", StrNull, StrNull },			// 界面显示方式，默认全屏
	{ 101, 1, 2, 0, 0, 0,0,0,0, "SYS_ShowLogWnd", StrNull, StrNull },				// 启动后自动显示日志窗口, val2:SW_SHOWMINIMIZED等等, val3:较早就显示
	{ 102, 0, 0, 0, 0, 0,0,0,0, "SYS_ListRunCode", StrNull, StrNull },				// 打印 运行码列表
	{ 103, 0, 0, 0, 0, 0,0,0,0, "SYS_ShowDebugUI", StrNull, StrNull },				// 打印 显示调试界面
	{ 104, 0, 0, 0, 0, 0,0,0,0, "SYS_PlayBackSound", "录像回放声音", "0:回放时过滤声音, 1:回放时播放声音" },		// 打印 回放时是否过滤声音
	{ 105, 57600, 0, 0, 0, 0,0,0,0, "SYS_COM_baudrate", StrNull, StrNull },			// com baudrate // 115200/57600/19200/9600等等...
	{ 106, 0, 0, 0, 0, 0,0,0,0, "SYS_Talk", "远程通话功能", "0:禁用, 1:启用" },		// 通话功能
	{ 107, 0, 0, 0, 0, 0,0,0,0, "SYS_GSM", "GSM短信通知功能", "0:禁用, 1:启用" },	// GSM功能
	{ 108, 64, 0, 0, 0, 0,0,0,0, "SYS_PlayBack_timefix_a", StrNull, StrNull },		// 回放 音频原始时间修正值，64
	{ 109, 9, 0, 0, 0, 0,0,0,0, "SYS_PlayBack_spanfix", StrNull, StrNull },			// 回放 包发送时间修正参数，可为负数，已测试1/5/9等等...
	{ 110, 1, 0, 0, 0, 0,0,0,0, "SYS_PTZ_TRACE2PTZSel", "跟踪关闭时自动改用定位", StrNull },	// 定位 跟踪关闭时转为定位
	{ 111, 1, 0, 0, 0, 0,0,0,0, "SYS_CheckSystem", StrNull, StrNull },				// 是否检测系统状态，检测当前系统内存
	{ 112, 800, 0, 0, 0, 0,0,0,0, "SYS_CurPsMemOver", StrNull, StrNull },			// 当前进程内存占用上限，单位MB
	{ 113, 1, 0, 0, 0, 0,0,0,0, "SYS_UseUrlDifCnt", StrNull, StrNull },				// url使用差异处理 ios未更新前置0，ios更新后置1
	{ 114, 0, 0, 0, 0, 0,0,0,0, "SYS_CamReEnc", StrNull, StrNull },					// 摄像头音频转码功能支持
	{ 115, 1, 0, 0, 0, 0,0,0,0, "SYS_CamTalk", StrNull, StrNull },					// 支持摄像机对讲功能
	{ 116, 1, 0, 0, 0, 0,0,0,0, "SYS_AutotLockForRun", StrNull, StrNull },			// 启动后自动锁定
	{ 117, 1, 0, 0, 0, 0,0,0,0, "SYS_CheckIOTPs", StrNull, StrNull },				// 是否检测进程自身状态，检测当前进程内存
	{ 118, 92, 0, 0, 0, 0,0,0,0, "SYS_CurSysMemOver", StrNull, StrNull },			// 当前系统内存占用上限，百分比
	{ 119, 87, 0, 0, 0, 0,0,0,0, "SYS_CurSysMemOverLow", StrNull, StrNull },		// 当前系统内存占用空闲时段上限，百分比
	{ 120, 0, 0, 0, 0, 0,0,0,0, "SYS_CamAutoConnect", "摄像机连接方式", "0:全部按照配置连接, 1:全部总是自动连接, 2:全部总是按需连接" },	// 所有摄像机是否保持自动连接，val1: 0按各个配置，1所有总是自动连接，2所有总是按需连接; val4:是否显示在配置界面
	{ 121, 1, 0, 0, 0, 0,0,0,0, "SYS_MotionTrack2ParkAction", StrNull, StrNull },	// 模式, 智能跟踪连带与守望互斥
	{ 122, 1, 0, 0, 0, 0,0,0,0, "SYS_AlarmGuardTimeW7", StrNull, StrNull },			// 布防时间:周日, val1:布防时间标志, 0全天无效(撤防)，1全天生效(布防), 2指定时间段; val2/val3/val4:布防时间段值，值格式: 是否有效标志*100000000+时间段开始时间*10000+时间段结束时间，时间例：有效:08:30-17:00=108301700
	{ 123, 1, 0, 0, 0, 0,0,0,0, "SYS_AutoCheckNetUseable", StrNull, StrNull },		// 自动检测摄像机网络是否畅通
	{ 124, 9, 0, 0, 0, 0,0,0,0, "SYS_RTMPUrlNumMax", StrNull, StrNull },			// CreateRTMPInstance RTMP url数量上限
	{ 125, 1, 0, 0, 0, 0,0,0,0, "SYS_AlarmGuardGlobalFlag", StrNull, StrNull },		// 布防撤防总开关
	{ 131, 1, 0, 0, 0, 0,0,0,0, "SYS_AlarmGuardTimeW1", StrNull, StrNull },			// 布防时间:周一，格式同SYS_AlarmGuardTime周日
	{ 132, 1, 0, 0, 0, 0,0,0,0, "SYS_AlarmGuardTimeW2", StrNull, StrNull },			// 布防时间:周二，格式同SYS_AlarmGuardTime周日
	{ 133, 1, 0, 0, 0, 0,0,0,0, "SYS_AlarmGuardTimeW3", StrNull, StrNull },			// 布防时间:周三，格式同SYS_AlarmGuardTime周日
	{ 134, 1, 0, 0, 0, 0,0,0,0, "SYS_AlarmGuardTimeW4", StrNull, StrNull },			// 布防时间:周四，格式同SYS_AlarmGuardTime周日
	{ 135, 1, 0, 0, 0, 0,0,0,0, "SYS_AlarmGuardTimeW5", StrNull, StrNull },			// 布防时间:周五，格式同SYS_AlarmGuardTime周日
	{ 136, 1, 0, 0, 0, 0,0,0,0, "SYS_AlarmGuardTimeW6", StrNull, StrNull },			// 布防时间:周六，格式同SYS_AlarmGuardTime周日
	{ 141, 1, 0, 0, 0, 0,0,0,0, "SYS_ShowTaskBtnMod", StrNull, StrNull },			// 主页面任务栏按钮显示模式，0动态显示，1总是显示
	{ 142, 1, 0, 0, 0, 0,0,0,0, "SYS_RS485ReadRollMod", StrNull, StrNull },			// RS485遍历读取测试模式 功能启用
	{ 143, 0, 1000, 0, 0, 0,0,0,0, "SYS_BitRateBaseK", StrNull, StrNull },			// 重定义CAudioCfg::BitRate2Bytes K基准值，一般为 1000 or 1024
	{ 144, 0, 0, 0, 0, 0,0,0,0, "SYS_AutoNotiContAddIOTName", "自动生成的通知内容带上主控名", "0:禁用, 1:启用" },		// 自动生成的通知内容是否带上名称
	{ 145, 1, 0, 0, 0, 0,0,0,0, "SYS_AutoNotiContAddTime", "自动生成的通知内容带上时间", "0:禁用, 1:启用" },		// 自动生成的通知内容是否带上时间
	{ 146, 0, 0, 0, 0, 0,0,0,0, "SYS_PlayBackSetSpeed", StrNull, StrNull },			// 是否 回放时设置流量控制
	{ 147, 16, 0, 0, 0, 0,0,0,0, "SYS_PlaybackChannelLimit", StrNull, StrNull },	// 是否 回放会话上限
	{ 148, 1, 0, 0, 0, 0,0,0,0, "SYS_UIAutoGetAllModSysSet", StrNull, StrNull },	// 打开界面时是否自动获取红外设置信息等
	{ 149, 6*3600, 0, 0, 0, 0,0,0,0, "SYS_PlaybackSessionOvertime", StrNull, StrNull },	// 回放会话最长时间，秒
	{ 150, 1, 1, 2, 0, 0,0,0,0, "SYS_CheckDevTimeCfg", "自动校时功能", StrNull },	// 设备校时设置，val1: 启用禁用; 在 val2至val3 时间范围内执行，此时间外不执行
	{ 151, 0, 0, 0, 0, 0,0,0,0, "SYS_CheckDevTimeSave", StrNull, StrNull },			// 设备校时记录，val1: 上次校时日
	{ 152, 0, 0, 0, 0, 0,0,0,0, "SYS_AutoPublishEnable", StrNull, StrNull },		// 保持发布/自动发布功能启用/禁用
	
	{ 157, defLinkID_Local, 0, 0, 0, 0,0,0,0, "SYS_GSM_LinkID", "GSM模块所在链路", "链路ID" },	// GSM模块所在链路

	{ 10100, 1, 0, 0, 0, 0,0,0,0, "PrintRecv_MainPort", StrNull, StrNull },			// 打印 接收字节流详细信息 主端口
	{ 10101, 0, 0, 0, 0, 0,0,0,0, "PrintRecv_RF", StrNull, StrNull },				// 打印 接收字节流详细信息 模块 RXB8_315 & RXB8_433
	{ 10107, 0, 0, 0, 0, 0,0,0,0, "PrintRecv_RS485", StrNull, StrNull },			// 打印 接收字节流详细信息 RS485模块
	{ 10108, 0, 0, 0, 0, 0,0,0,0, "PrintRecv_RF_original", StrNull, StrNull },		// 打印 接收字节流详细信息 模块 RXB8_315_original & RXB8_433_original
	{ 10109, 0, 0, 0, 0, 0,0,0,0, "PrintRecv_IR_code", StrNull, StrNull },			// 打印 接收字节流详细信息 模块 MOD_IR_RX_code
	{ 10110, 0, 0, 0, 0, 0,0,0,0, "PrintRecv_IR_original", StrNull, StrNull },		// 打印 接收字节流详细信息 模块 MOD_IR_RX_original
						 
	{ 10200, 1, 0, 0, 0, 0,0,0,0, "PrintSend_MainPort", StrNull, StrNull },			// 打印 发送字节流详细信息 主端口
	{ 10203, 0, 0, 0, 0, 0,0,0,0, "PrintSend_RF_TX", StrNull, StrNull },			// 打印 发送字节流详细信息 RS485模块
	{ 10207, 0, 0, 0, 0, 0,0,0,0, "PrintSend_RS485", StrNull, StrNull },			// 打印 发送字节流详细信息 RS485模块
	{ 10208, 0, 0, 0, 0, 0,0,0,0, "PrintSend_RF_original", StrNull, StrNull },		// 打印 发送字节流详细信息 RS485模块
						 
	{ 10301, 0, 0, 0, 0, 0,0,0,0, "PrintRecv_val_RF", StrNull, StrNull },			// 打印 接收解析后的值信息 模块 RXB8_315 & RXB8_433
	{ 10307, 1, 0, 0, 0, 0,0,0,0, "PrintRecv_val_RS485", StrNull, StrNull },		// 打印 接收解析后的值信息 RS485模块
	{ 10308, 1, 0, 0, 0, 0,0,0,0, "PrintRecv_val_IR", StrNull, StrNull },			// 打印 接收解析后的值信息 模块 MOD_IR_RX_code & MOD_IR_RX_original

	{ 11001, 1, 300000, 0, 1, 0,0,0,0, "Cam_PrintFrame", StrNull, StrNull },		// 打印 Cam 打印Push的帧, value2间隔时间ms, value3:I帧总是打印, value4:音频帧打印
	{ 11002, 0, 0, 0, 0, 0,0,0,0, "Cam_PrintFrameSize", StrNull, StrNull },			// 打印 Cam 打印Push的帧, value2字节大小超过此门限打印
	{ 11003, 3000, 0, 0, 0, 0,0,0,0, "Cam_PtzAutoMax", StrNull, StrNull },			// Cam 自动控制最大时间ms
	{ 11004, 255, 400, 0, 0, 0,0,0,0, "Cam_PtzAutoNorm", StrNull, StrNull },		// Cam 自动控制普通模式时间ms，value1方向, value2调焦
	{ 11005, 1200, 1600, 0, 0, 0,0,0,0, "Cam_PtzAutoFast", StrNull, StrNull },		// Cam 自动控制快速模式时间ms，value1方向, value2调焦
	{ 11006, 700, 0, 0, 0, 0,0,0,0, "Cam_MakeKeyFrameInterval", StrNull, StrNull },	// Cam MakeKeyFrame最小间隔ms
	{ 11007, 2048*1024, 0, 0, 0, 0,0,0,0, "Cam_VideoBitrateLimit", StrNull, StrNull },	// Cam 视频码率最大值，超出则配置错误

	{ 12001, 1, 300000, 0, 0, 0,0,0,0, "RTMPSend_PrintFrame", StrNull, StrNull },	// 打印 RTMPSend 打印Push的帧, value2间隔时间ms, value3:I帧总是打印
	{ 12002, 220, 0, 0, 0, 0,0,0,0, "RTMPSend_NetSend_WarnTime", StrNull, StrNull },// 打印 RTMPSend，当网络发送耗时过长时打印, value1:耗时门限时间ms
	{ 12003, 32*1024, 0, 0, 0, 0,0,0,0, "RTMPSend_SetChunkSize", StrNull, StrNull },// RTMPSend.RTMP_SetChunkSize 32*1024, 4096
	{ 12004, 8000, 0, 0, 0, 0,0,0,0, "RTMFP_WaitConnTimeout", StrNull, StrNull },	// RTMFP等待连接超时时间ms
	{ 12005, 1, 0, 0, 0, 0,0,0,0, "RTMFP_UrlAddStreamID", StrNull, StrNull },		// RTMFP Url结尾添加StreamID
	{ 12006, 0, 0, 0, 0, 0,0,0,0, "RTMFP_UseSpecStreamID", StrNull, StrNull, "test_sender" },		// RTMFP 使用指定的固定StreamID
	{ 12007, 4, 0, 0, 0, 0,0,0,0, "RTMFP_SessionLimit", StrNull, StrNull },			// RTMFP 点对点连接数量上限，达到上限又有请求时进行RTMP连接
	{ 12008, 0, 0, 0, 0, 0,0,0,0, "SYS_Enable_RTMFP", StrNull, StrNull },			// RTMFP功能启用/禁用，value1实时播放, value2录像回放
	{ 12009, 1, 0, 0, 0, 0,0,0,0, "RTMFP_DelFromUrlStreamID", StrNull, StrNull },	// 进行判断并去掉RTMFP URL结尾的StreamID

	{ 13001, 10000, 0, 0, 0, 0,0,0,0, "xmpp_ConnectInterval", StrNull, StrNull },	// xmpp重连间隔时间
	{ 13002, 1, 0, 0, 0, 0,0,0,0, "RF_Send_CountMin", StrNull, StrNull },			// 无线遥控发送次数 最小值
	{ 13003, 6, 0, 0, 0, 0,0,0,0, "RF_Send_CountDefault", StrNull, StrNull },		// 无线遥控发送次数 默认值
	{ 13004, 1000, 0, 0, 0, 0,0,0,0, "RF_Send_FixAddTime", StrNull, StrNull },		// 无线遥控发送需时的附带增加时间，单位ms
	{ 13005, 1, 0, 0, 0, 0,0,0,0, "COM_SendInterval", StrNull, StrNull },			// 是否启用发送间隔控制

	{ 14001, 1, 1024*1, 0, 0, 0,0,0,0, "APlay_ProbeSize", StrNull, StrNull },		// APlayer ProbeSize参数，value1使用参数, value2参数值, 影响延时

	{ 15001, 0, 0, 0, 0, 0,0,0,0, "SYS_CamAlarmDebugLog", StrNull, StrNull },		// 摄像机告警调试日志打印
	{ 15002, 5*60, 0, 0, 0, 0,0,0,0, "SYS_CamAlarmResumeTime", StrNull, StrNull },	// 摄像机告警恢复时间，单位秒
	{ 15003, 2*60, 0, 0, 0, 0,0,0,0, "SYS_TriggerAlarmInterval", StrNull, StrNull },// 触发器报警间隔，单位秒

	{ 16001, 16, 5, 0, 0, 0,0,0,0, "CommLinkThreadMax", StrNull, StrNull },			// 链路线程最大数量，value1最大数, value2允许进行连接的数量
	{ 16002, 20, 0, 0, 0, 0,0,0,0, "CommLinkConnectInterval", StrNull, StrNull },	// 链路重连间隔，单位秒
	{ 16003, 30, 0, 0, 0, 0,0,0,0, "CommLinkHb_SendInterval", StrNull, StrNull },	// 心跳发送间隔，单位秒
	{ 16004, 150, 0, 0, 0, 0,0,0,0, "CommLinkHb_RecvTimeover", StrNull, StrNull },	// 心跳超时，单位秒

	{ 17001, 9, 3, 0, 0, 0,0,0,0, "Dev_RecvFailedCount", StrNull, StrNull },		// 设备接收失败次数，value1失败次数超过此值直接认为异常, value2失败次数超过此值并且失败时间持续达n秒认为异常
	{ 17002, 10, 50, 0, 0, 0,0,0,0, "Dev_RecvFailedTime", StrNull, StrNull },		// 失败时间持续达n秒，单位秒

	{ 18001, 0, 0, 0, 0, 0,0,0,0, "SYS_Change_Global", StrNull, StrNull },			// 配置变化总体标志，value1变化计数, value2变化时间戳，目前只在内存中存储不保存数据库

	{ 19001, 1000, 0, 0, 0, 0,0,0,0, "SYS_VChgRng_Default", StrNull, StrNull },		// 存储变化量设定值，默认判断变化，1000倍设置
	{ 19002, 490, 0, 0, 0, 0,0,0,0, "SYS_VChgRng_Temperature", StrNull, StrNull },	// 存储变化量设定值，温度变化，1000倍设置 //860
	{ 19003, 1900, 0, 0, 0, 0,0,0,0, "SYS_VChgRng_Humidity", StrNull, StrNull },	// 存储变化量设定值，湿度变化，1000倍设置 //3000
	{ 19004, 240, 100, 0, 0, 0,0,0,0, "SYS_WinTime_Wind", StrNull, StrNull },		// 风速窗口化时间，value1常规值, value2时间点时的最小值，单位秒
	{ 19101, 0, 0, 0, 0, 0,0,0,0, "SYS_MergeWindLevel", StrNull, StrNull },			// 合并风速级别，1级和2级合并
	{ 19102, 1, 0, 0, 0, 0,0,0,0, "SYS_DataSamp_DoBatch", StrNull, StrNull },		// DataSample采集时采用批量读取值方式，否则，单个值分别读取
	{ 19103, 5*60, 0, 0, 0, 0,0,0,0, "SYS_DataSamp_LongTimeNoData", StrNull, StrNull },	// DataSample采集时太长时间未获取数据，单位秒
	{ 19104, 2*60, 30, 0, 0, 0,0,0,0, "SYS_DataSamp_LowSampTime", StrNull, StrNull },	// DataSample采集时太长时间未获取数据则进入低频率采集，单位秒
	{ 19105, 2000, 0, 0, 0, 0,0,0,0, "SYS_QueryLstMax", StrNull, StrNull },			// 查询数据记录的最大数量
	{ 19106, 36, 0, 0, 0, 0,0,0,0, "SYS_DBDataFileMax", StrNull, StrNull },			// 数据库存储文件最大数量至少=2

	{ 20001, 0, 0, 0, 0, 0,0,0,0, "SYS_ReportDef_spanrate", StrNull, StrNull },		// 查询报表参数默认值，变化率，千倍值，1=0.001
	{ 20002, 1, 0, 0, 0, 0,0,0,0, "SYS_ReportDef_ratefortype", StrNull, StrNull },	// 查询报表参数默认值，按照类型分析数据变化 0/1
	{ 20003, 0, 0, 0, 0, 0,0,0,0, "SYS_ReportDef_getfortype", StrNull, StrNull },	// 查询报表参数默认值，按照类型转换值返回 0/1
	
	{ 21001, 0, 0, 0, 0, 0,0,0,0, "LED_Config", "LED显示", StrNull },					// LED - 功能配置，value1启用标志，value2模式0自动
	{ 21002, 30000, 0, 0, 0, 0,0,0,0, "LED_DevParam", StrNull, StrNull, "192.168.1.3" },	// LED - 设备参数，value1端口，str设备IP
	{ 21003, 30, 10, 0, 0, 0,0,0,0, "LED_ShowInterval", StrNull, StrNull },			// LED - 显示间隔，秒; value1最长间隔，至少刷新一次; value2最短间隔，不能快于此值
	{ 21004, 10*60, 1, 0, 0, 0,0,0,0, "LED_ValueOvertime", StrNull, StrNull, "未连接" },	// LED - 值过期异常相关配置，value1多长时间未刷新值为异常，秒; value2异常是否仍显示; str异常显示内容
	{ 21005, 0, 0, 192, 128, 0,0,0,0, "LED_Rect", StrNull, StrNull },				// LED - 显示位置; value1 x; value2 y; value3 宽; value4 高
	{ 21006, 0, 0, 0, 0, 0,0,0,0, "LED_Title", StrNull, StrNull, "" },				// LED - 显示标题; str标题，空为不显示
	{ 21007, 0, 1, 0, 0, 0,0,0,0, "LED_Format", StrNull, StrNull },					// LED - 显示格式; value1文本格式码(参见SDK); value2空行0/1;

	{ 22001, 16, 0, 0, 0, 0,0,0,0, "AC_CtlMAX", StrNull, StrNull },					// 门禁 - 最大数量
	{ 22002, 1, 0, 0, 0, 0,0,0,0, "AC_CheckACMod", StrNull, StrNull },				// 门禁 - 检测门禁模块
	
	{ 23001, 800, 0, 0, 0, 0,0,0,0, "PrePic_Size_NM", StrNull, StrNull },			// 预览图 - NM
	{ 23002, 400, 0, 0, 0, 0,0,0,0, "PrePic_Size_NS", StrNull, StrNull },			// 预览图 - NS
	{ 23003, 128, 0, 0, 0, 0,0,0,0, "PrePic_Size_S", StrNull, StrNull },			// 预览图 - S


	{ 80001, 0, 0, 0, 0, 0,0,0,0, "SYS_IOT_NAME", "设备名称", StrNull, "GSIOT" },	// 控制器设备名称

	//--------------------------------------------------
	{ 0, 0, 0, 0, 0, 0,0,0,0, "_TEST__Spec__", StrNull, StrNull },					// 特殊功能分界线

	{ 90001, 0, 0, 0, 0, 0,0,0,0, "TEST_ModbusMod", StrNull, StrNull },				// 测试模式, modbus透传测试模式
	{ 90002, 0, 0, 0, 0, 0,0,0,0, "Debug_DisableAuth", StrNull, StrNull },			// 调试模式, 屏蔽权限
	{ 90004, 0, 0, 0, 0, 0,0,0,0, "TEST_OpenRFOriginal", StrNull, StrNull },		// 一直打开RF模拟上报通道
	//{ 90005, 0, 0, 0, 0, 0,0,0,0, "TEST_Disable_GSM", StrNull, StrNull },			// 已改用SYS_GSM
	{ 90006, 0, 0, 0, 0, 0,0,0,0, "TEST_TelReplaceSMS", StrNull, StrNull },			// 用拨打电话代替短信测试
	{ 90007, 0, 0, 0, 0, 0,0,0,0, "TEST_DoEvent_NoInterval", StrNull, StrNull },	// 不判断执行间隔，直接执行事件动作
	{ 90010, 0, 0, 0, 0, 0,0,0,0, "TEST_RTMP_TestMod", StrNull, StrNull },			// 测试模式, 采用rtmp测试模式, value2固定使用设备名，否则使用命令传过来的实例名
	{ 90011, 0, 0, 0, 0, 0,0,0,0, "TEST_PlayBack_timefix_pnt", StrNull, StrNull },	// 测试模式, 打印回放 原始时间修正日志，日志量很大
	{ 90012, 0, 0, 0, 0, 0,0,0,0, "TEST_StreamServer", StrNull, StrNull },			// 测试模式, 测试指定流服务器IP, val1:启用标志，val2(ip段1*1000+ip段2), val3(ip段3*1000+ip段4)，例：192168, 000150
	{ 90013, 0, 0, 0, 0, 0,0,0,0, "TEST_DisableHeartbeatMon", StrNull, StrNull },	// 测试模式, 是否禁用心跳监测
	{ 90014, 0, 0, 0, 0, 0,0,0,0, "TEST_DisableAlarmGuard", StrNull, StrNull },		// 测试模式, 是否禁用布防时间功能
	{ 90015, 0, 0, 0, 0, 0,0,0,0, "TEST_DeCheckFilePlayback", StrNull, StrNull },	// 测试模式, 是否禁用回放前检查文件
	{ 90016, 0, 0, 0, 0, 0,0,0,0, "TEST_UseSpecRTMPUrlList", StrNull, StrNull },	// 测试模式, 是否使用指定序号位置的url作为优先url, val1:启用标志，val2:哪个序号的作为优先,下标0开始。
	{ 90017, 0, 0, 0, 0, 0,0,0,0, "Dis_CommLinkTransparent", StrNull, StrNull },	// 测试模式, 禁用链路透传
	{ 90018, 123456, 0, 0, 0, 0,0,0,0, "Dis_ChangeSaveDB", StrNull, StrNull },		// 是否禁用 配置变化保存数据库
	{ 90020, 0, 0, 0, 0, 0,0,0,0, "Dis_RunDataProc", StrNull, StrNull },			// 测试模式 数据采集分析等的测试功能
	{ 90021, 0, 0, 0, 0, 0,0,0,0, "TEST_ForceDataSave", StrNull, StrNull },			// 测试模式, 总是保存数据
	{ 90022, 0, 0, 0, 0, 0,0,0,0, "TEST_DlgQueryMain", StrNull, StrNull },			// 测试模式, 更多查询选项

	{ 90101, 0, 0, 0, 0, 0,0,0,0, "Disable_Recv_RF", StrNull, StrNull },			// 测试模式, 屏蔽功能：(RXB8_315 & RXB8_433)串口接收解码
	{ 90107, 0, 0, 0, 0, 0,0,0,0, "Disable_Recv_RS485", StrNull, StrNull },			// 测试模式, 屏蔽功能：RS485串口接收解码
	{ 90108, 0, 0, 0, 0, 0,0,0,0, "Disable_Recv_RF_original", StrNull, StrNull },	// 测试模式, 屏蔽功能：(RXB8_315_original & RXB8_433_original)串口接收解码
	{ 90110, 0, 0, 0, 0, 0,0,0,0, "Disable_Recv_GSM", StrNull, StrNull },			// 测试模式, 屏蔽功能：Module_GSM串口接收
	{ 90111, 0, 0, 0, 0, 0,0,0,0, "Disable_Recv_IR_code", StrNull, StrNull },		// 测试模式, 屏蔽功能：MOD_IR_RX_code串口接收
	{ 90112, 0, 0, 0, 0, 0,0,0,0, "Disable_Recv_IR_original", StrNull, StrNull },	// 测试模式, 屏蔽功能：MOD_IR_RX_original串口接收

	{ 90203, 0, 0, 0, 0, 0,0,0,0, "Disable_Send_RF_TX", StrNull, StrNull },			// 测试模式, 屏蔽功能：(RXB8_315_TX & RXB8_433_TX)串口发送
	{ 90207, 0, 0, 0, 0, 0,0,0,0, "Disable_Send_RS485", StrNull, StrNull },			// 测试模式, 屏蔽功能：RS485串口发送
	{ 90208, 0, 0, 0, 0, 0,0,0,0, "Disable_Send_RF_original", StrNull, StrNull },	// 测试模式, 屏蔽功能：(RXB8_315_original & RXB8_433_original)串口发送
	{ 90210, 0, 0, 0, 0, 0,0,0,0, "Disable_Send_GSM", StrNull, StrNull },			// 测试模式, 屏蔽功能：Module_GSM串口发送

	{ 90901, 0, 0, 0, 0, 0,0,0,0, "TEST_Develop_NewFunc", StrNull, StrNull },		// 开发测试模式, 正在开发中的新功能测试是否启用
	{ 90911, 0, 0, 0, 0, 0,0,0,0, "TEST_Debug_TempCode", StrNull, StrNull },		// 开发测试模式, 临时调试代码是否启用

	{ 0, 0, 0, 0, 0, 0,0,0,0, "CODEEND", StrNull, StrNull }							// 结尾
};

// 通过code或codestr获取runcode缓存索引号
defCodeIndex_ s_GetRunCodeIndex( int code, const char *codestr )
{
	defCodeIndex_ index = defCodeIndex_Unknown;

	for( int i=0; i<(sizeof(s_RunCode)/sizeof(struRunCode)); ++i )
	{
		if( 0 != s_RunCode[i].code )
		{
			if( code == s_RunCode[i].code )
			{
				index = (defCodeIndex_)i;
				break;
			}

			if( codestr && strlen(codestr)>0 )
			{
				if( 0 == strcmp( codestr, s_RunCode[i].codestr ) )
				{
					index = (defCodeIndex_)i;
					break;
				}
			}
		}
	}

	if( defCodeIndex_Unknown == index )
	{
		//LOGMSGEX( defLOGNAME, defLOG_ERROR, "s_GetRunCodeIndex not found codeindex, code={%d,%s}\r\n", code, codestr?codestr:"" );
	}

	if( index<defCodeIndex_Unknown || index>=defCodeIndex_MAX_ )
	{
		//LOGMSGEX( defLOGNAME, defLOG_ERROR, "s_GetRunCodeIndex codeindex error, code={%d,%s}, index=%d!\r\n", code, codestr?codestr:"", index );
		index = defCodeIndex_Unknown;
	}
	else
	{
		// 校验code和codestr是否一致
		bool isErrorCode = false;
		if( 0 != code )
		{
			if( code != s_RunCode[index].code )
			{
				isErrorCode = true;
			}
		}

		if( codestr && strlen(codestr)>0 )
		{
			if( 0 != strcmp( codestr, s_RunCode[index].codestr ) )
			{
				isErrorCode = true;
			}
		}

		if( isErrorCode )
		{
			//LOGMSGEX( defLOGNAME, defLOG_ERROR, "s_GetRunCodeIndex code & codestr error, code={%d,%s}, index=%d!\r\n", code, codestr?codestr:"", index );
			index = defCodeIndex_Unknown;
		}
	}

	return index;
}

// 有范围限制的进行修正值
static void s_FixRunCodeValue( defCodeIndex_ codeindex, int valueindex, int newval )
{
	if( codeindex<defCodeIndex_Unknown || codeindex>=defCodeIndex_MAX_ )
	{
		return ;
	}

	if( valueindex<defRunCodeValIndex_1 || valueindex>defRunCodeValIndex_4 )
	{
		return ;
	}

	int oldval = 0;
	switch(valueindex)
	{
	case defRunCodeValIndex_1: 
		{
			oldval = s_RunCode[codeindex].value1;
			s_RunCode[codeindex].value1 = newval;
		}

	case defRunCodeValIndex_2: 
		{
			oldval = s_RunCode[codeindex].value2;
			s_RunCode[codeindex].value2 = newval;
		}

	case defRunCodeValIndex_3: 
		{
			oldval = s_RunCode[codeindex].value3;
			s_RunCode[codeindex].value3 = newval;
		}

	case defRunCodeValIndex_4: 
		{
			oldval = s_RunCode[codeindex].value4;
			s_RunCode[codeindex].value4 = newval;
		}
	}

	LOGMSG( ">>>>> s_FixRunCodeValue(%s) oldval=%d, newval=%d\r\n", s_RunCode[codeindex].codestr, oldval, newval );
}

// 有范围限制的进行修正值
static bool s_FixRunCodeValue_range( defCodeIndex_ codeindex, int valueindex, bool havemin=false, int minval=-1, bool havemax=false, int maxval=-1, defUseValType UseValType=defUseValType_Default )
{
	if( codeindex<defCodeIndex_Unknown || codeindex>=defCodeIndex_MAX_ )
	{
		return false;
	}

	if( valueindex<defRunCodeValIndex_1 || valueindex>defRunCodeValIndex_4 )
	{
		return false;
	}

	int curval = s_GetRunCodeValue( codeindex, valueindex );
	if( ( havemin && (curval < minval) ) 
		|| ( havemax && (curval > maxval) ) )
	{
		switch(UseValType)
		{
		case defUseValType_Default:
			s_FixRunCodeValue( codeindex, valueindex, s_GetRunCodeValue_Default(codeindex, valueindex) );
			break;

		case defUseValType_Min:
			s_FixRunCodeValue( codeindex, valueindex, minval );
			break;

		case defUseValType_Max:
			s_FixRunCodeValue( codeindex, valueindex, maxval );
			break;
		}

		return true;
	}

	return false;
}

// 通过index设置runcode的值
bool s_SetRunCodeValue( defCodeIndex_ index, int val1, int val2, int val3, int val4, bool do1, bool do2, bool do3, bool do4 )
{
	if( index<=defCodeIndex_Unknown || index>=defCodeIndex_MAX_ )
	{
		//LOGMSGEX( defLOGNAME, defLOG_ERROR, "s_SetRunCodeValue error! code={%d}, val={%d, %d, %d, %d}\r\n", index, val1, val2, val3, val4 );
		return false;
	}

	if( do1 ) s_RunCode[index].value1 = val1;
	if( do2 ) s_RunCode[index].value2 = val2;
	if( do3 ) s_RunCode[index].value3 = val3;
	if( do4 ) s_RunCode[index].value4 = val4;
	
	bool isWarnSet = false; // 会影响程序运行机制，需要给出警告日志
	switch(index)
	{
	case defCodeIndex_TEST_ModbusMod:
		isWarnSet = true;
		break;

	default:

		if( index > defCodeIndex_TEST__Spec__ )
		{
			isWarnSet = true;
		}

		break;
	}

	// 值范围修正
	switch(index)
	{
	case defCodeIndex_xmpp_ConnectInterval:
		{
			isWarnSet |= s_FixRunCodeValue_range( index, defRunCodeValIndex_1, true, 6, false, -1, defUseValType_Default );
		}
		break;

	case defCodeIndex_CommLinkThreadMax:
		{
			isWarnSet |= s_FixRunCodeValue_range( index, defRunCodeValIndex_1, true, 2, false, -1, defUseValType_Default );
			isWarnSet |= s_FixRunCodeValue_range( index, defRunCodeValIndex_2, true, 1, false, -1, defUseValType_Default );
		}
		break;

	default:
		break;
	}

	LOGMSG( "%sSetcode(%d:%s), val={%d, %d, %d, %d}%s\r\n", isWarnSet?">>>>> ":"", s_RunCode[index].code, s_RunCode[index].codestr, val1, val2, val3, val4, isWarnSet?" <<<<<":"" );

	return true;
}

// 通过codeindex获取runcode的值
int s_GetRunCodeValue( defCodeIndex_ codeindex, int valueindex )
{
	if( codeindex<defCodeIndex_Unknown || codeindex>=defCodeIndex_MAX_ )
	{
		return defRunCodeVal_Disable;
	}

	switch(valueindex)
	{
	case defRunCodeValIndex_1: 
		return s_RunCode[codeindex].value1;

	case defRunCodeValIndex_2: 
		return s_RunCode[codeindex].value2;

	case defRunCodeValIndex_3: 
		return s_RunCode[codeindex].value3;

	case defRunCodeValIndex_4: 
		return s_RunCode[codeindex].value4;
	}

	return defRunCodeVal_Disable;
}

// 通过codeindex获取runcode的值
const std::string& s_GetRunCodeStrVal( defCodeIndex_ codeindex )
{
	if( codeindex<defCodeIndex_Unknown || codeindex>=defCodeIndex_MAX_ )
	{
		return c_NullStr;
	}

	return s_RunCode[codeindex].str_val;
}

// 通过codeindex获取默认值
int s_GetRunCodeValue_Default( defCodeIndex_ codeindex, int valueindex )
{
	if( codeindex<defCodeIndex_Unknown || codeindex>=defCodeIndex_MAX_ )
	{
		return defRunCodeVal_Disable;
	}

	switch(valueindex)
	{
	case defRunCodeValIndex_1: 
		return s_RunCode[codeindex].default_value1;

	case defRunCodeValIndex_2: 
		return s_RunCode[codeindex].default_value2;

	case defRunCodeValIndex_3: 
		return s_RunCode[codeindex].default_value3;

	case defRunCodeValIndex_4: 
		return s_RunCode[codeindex].default_value4;
	}

	return defRunCodeVal_Disable;
}

// 通过codeindex获取默认值
const std::string& s_GetRunCodeStrVal_Default( defCodeIndex_ codeindex )
{
	if( codeindex<defCodeIndex_Unknown || codeindex>=defCodeIndex_MAX_ )
	{
		return c_NullStr;
	}

	return s_RunCode[codeindex].default_str_val;
}

// 通过codeindex获取runcode的值，值无效时采用默认值
int s_GetRunCodeValue_UseDefWhenNul( defCodeIndex_ codeindex, int valueindex, int nulval )
{
	if( codeindex<defCodeIndex_Unknown || codeindex>=defCodeIndex_MAX_ )
	{
		return defRunCodeVal_Disable;
	}

	switch(valueindex)
	{
	case defRunCodeValIndex_1: 
		return ( (nulval==s_RunCode[codeindex].value1) ? s_RunCode[codeindex].default_value1 : s_RunCode[codeindex].value1 );

	case defRunCodeValIndex_2: 
		return ( (nulval==s_RunCode[codeindex].value2) ? s_RunCode[codeindex].default_value2 : s_RunCode[codeindex].value2 );

	case defRunCodeValIndex_3: 
		return ( (nulval==s_RunCode[codeindex].value3) ? s_RunCode[codeindex].default_value3 : s_RunCode[codeindex].value3 );

	case defRunCodeValIndex_4: 
		return ( (nulval==s_RunCode[codeindex].value4) ? s_RunCode[codeindex].default_value4 : s_RunCode[codeindex].value4 );
	}

	return defRunCodeVal_Disable;
}

// 判断运行码对应功能是否开启
bool s_IsRunCodeEnable( defCodeIndex_ codeindex )
{
	int val1 = s_GetRunCodeValue( codeindex, defRunCodeValIndex_1 );

	if( codeindex < defCodeIndex_TEST__Spec__ )
	{
		return ( defRunCodeVal_Disable != val1 );
	}
	else
	{
		return ( defRunCodeVal_Spec_Enable == val1 );
	}

	return false;
}

#define macCheckRunCode(in_codestr)  \
if(  defCodeIndex_##in_codestr == s_GetRunCodeIndex( 0, ""#in_codestr"" ) ) \
{ \
	/*LOGMSG( "s_InitRunCode: "#codestr" code ok.\r\n" );*/ \
	struRunCode &rc = s_RunCode[defCodeIndex_##in_codestr]; \
	if( code_set.find(rc.code) == code_set.end() && codestr_set.find(rc.codestr) == codestr_set.end() ) \
	{ \
		rc.CheckFlag = defRunCode_CheckOK; \
		code_set.insert( rc.code ); \
		codestr_set.insert( rc.codestr ); \
	} \
	else \
	{ \
	\
	} \
} \
else \
{ \
\
}

void s_InitRunCode(void)
{
	//LOGMSGEX( defLOGNAME, defLOG_SYS, "s_InitRunCode and Check..." );

	for( int i=0; i<(sizeof(s_RunCode)/sizeof(struRunCode)); ++i )
	{
		//s_RunCode[i].codestr[64-1] = 0;
		s_RunCode[i].CheckFlag = 0;

		s_RunCode[i].value1 = s_RunCode[i].default_value1;
		s_RunCode[i].value2 = s_RunCode[i].default_value2;
		s_RunCode[i].value3 = s_RunCode[i].default_value3;
		s_RunCode[i].value4 = s_RunCode[i].default_value4;
		s_RunCode[i].str_val = s_RunCode[i].default_str_val;
	}

	std::set<int> code_set;
	std::set<std::string> codestr_set;
	//--------------------------------------------------
	macCheckRunCode(SYS_IOTWndMod);
	macCheckRunCode(SYS_ShowLogWnd);
	macCheckRunCode(SYS_ListRunCode);
	macCheckRunCode(SYS_ShowDebugUI);
	macCheckRunCode(SYS_PlayBackSound);
	macCheckRunCode(SYS_COM_baudrate);
	macCheckRunCode(SYS_Talk);
	macCheckRunCode(SYS_GSM);
	macCheckRunCode(SYS_PlayBack_timefix_a);
	macCheckRunCode(SYS_PlayBack_spanfix);
	macCheckRunCode(SYS_PTZ_TRACE2PTZSel);
	macCheckRunCode(SYS_CheckSystem);
	macCheckRunCode(SYS_CurPsMemOver);
	macCheckRunCode(SYS_UseUrlDifCnt);
	macCheckRunCode(SYS_CamReEnc);
	macCheckRunCode(SYS_CamTalk);
	macCheckRunCode(SYS_AutotLockForRun);
	macCheckRunCode(SYS_CheckIOTPs);
	macCheckRunCode(SYS_CurSysMemOver);
	macCheckRunCode(SYS_CurSysMemOverLow);
	macCheckRunCode(SYS_CamAutoConnect);
	macCheckRunCode(SYS_MotionTrack2ParkAction);
	macCheckRunCode(SYS_AlarmGuardTimeW7);
	macCheckRunCode(SYS_AutoCheckNetUseable);
	macCheckRunCode(SYS_RTMPUrlNumMax);
	macCheckRunCode(SYS_AlarmGuardGlobalFlag);
	macCheckRunCode(SYS_AlarmGuardTimeW1);
	macCheckRunCode(SYS_AlarmGuardTimeW2);
	macCheckRunCode(SYS_AlarmGuardTimeW3);
	macCheckRunCode(SYS_AlarmGuardTimeW4);
	macCheckRunCode(SYS_AlarmGuardTimeW5);
	macCheckRunCode(SYS_AlarmGuardTimeW6);
	macCheckRunCode(SYS_ShowTaskBtnMod);
	macCheckRunCode(SYS_RS485ReadRollMod);
	macCheckRunCode(SYS_BitRateBaseK);
	macCheckRunCode(SYS_AutoNotiContAddIOTName);
	macCheckRunCode(SYS_AutoNotiContAddTime);
	macCheckRunCode(SYS_PlayBackSetSpeed);
	macCheckRunCode(SYS_PlaybackChannelLimit);
	macCheckRunCode(SYS_UIAutoGetAllModSysSet);
	macCheckRunCode(SYS_PlaybackSessionOvertime);
	macCheckRunCode(SYS_CheckDevTimeCfg);
	macCheckRunCode(SYS_CheckDevTimeSave);
	macCheckRunCode(SYS_AutoPublishEnable);
	macCheckRunCode(SYS_GSM_LinkID);

	macCheckRunCode(PrintRecv_MainPort);
	macCheckRunCode(PrintRecv_RF);
	macCheckRunCode(PrintRecv_RS485);
	macCheckRunCode( PrintRecv_RF_original );
	macCheckRunCode( PrintRecv_IR_code );
	macCheckRunCode( PrintRecv_IR_original );

	macCheckRunCode(PrintSend_MainPort);
	macCheckRunCode(PrintSend_RF_TX);
	macCheckRunCode(PrintSend_RS485);
	macCheckRunCode(PrintSend_RF_original);

	macCheckRunCode(PrintRecv_val_RF);
	macCheckRunCode(PrintRecv_val_RS485);
	macCheckRunCode(PrintRecv_val_IR);

	macCheckRunCode(Cam_PrintFrame);
	macCheckRunCode(Cam_PrintFrameSize);
	macCheckRunCode(Cam_PtzAutoMax);
	macCheckRunCode(Cam_PtzAutoNorm);
	macCheckRunCode(Cam_PtzAutoFast);
	macCheckRunCode(Cam_MakeKeyFrameInterval);
	macCheckRunCode(Cam_VideoBitrateLimit);

	macCheckRunCode(RTMPSend_PrintFrame);
	macCheckRunCode(RTMPSend_NetSend_WarnTime);
	macCheckRunCode(RTMPSend_SetChunkSize);
	macCheckRunCode(RTMFP_WaitConnTimeout);
	macCheckRunCode(RTMFP_UrlAddStreamID);
	macCheckRunCode(RTMFP_UseSpecStreamID);
	macCheckRunCode(RTMFP_SessionLimit);
	macCheckRunCode(SYS_Enable_RTMFP);
	macCheckRunCode(RTMFP_DelFromUrlStreamID);

	macCheckRunCode(xmpp_ConnectInterval);
	macCheckRunCode(RF_Send_CountMin);
	macCheckRunCode(RF_Send_CountDefault);
	macCheckRunCode(RF_Send_FixAddTime);
	macCheckRunCode(COM_SendInterval);

	macCheckRunCode(APlay_ProbeSize);

	macCheckRunCode(SYS_CamAlarmDebugLog);
	macCheckRunCode(SYS_CamAlarmResumeTime);
	macCheckRunCode(SYS_TriggerAlarmInterval);

	macCheckRunCode(CommLinkThreadMax);
	macCheckRunCode(CommLinkConnectInterval);
	macCheckRunCode(CommLinkHb_SendInterval);
	macCheckRunCode(CommLinkHb_RecvTimeover);

	macCheckRunCode(Dev_RecvFailedCount);
	macCheckRunCode(Dev_RecvFailedTime);

	macCheckRunCode(SYS_Change_Global);

	macCheckRunCode(SYS_VChgRng_Default);
	macCheckRunCode(SYS_VChgRng_Temperature);
	macCheckRunCode(SYS_VChgRng_Humidity);
	macCheckRunCode(SYS_WinTime_Wind);
	macCheckRunCode(SYS_MergeWindLevel);
	macCheckRunCode(SYS_DataSamp_DoBatch);
	macCheckRunCode(SYS_DataSamp_LongTimeNoData);
	macCheckRunCode(SYS_DataSamp_LowSampTime);
	macCheckRunCode(SYS_QueryLstMax);
	macCheckRunCode(SYS_DBDataFileMax);

	macCheckRunCode(SYS_ReportDef_spanrate);
	macCheckRunCode(SYS_ReportDef_ratefortype);
	macCheckRunCode(SYS_ReportDef_getfortype);

	macCheckRunCode(LED_Config);
	macCheckRunCode(LED_DevParam);
	macCheckRunCode(LED_ShowInterval);
	macCheckRunCode(LED_ValueOvertime);
	macCheckRunCode(LED_Rect);
	macCheckRunCode(LED_Title);
	macCheckRunCode(LED_Format);

	macCheckRunCode(AC_CtlMAX);
	macCheckRunCode(AC_CheckACMod);
	
	macCheckRunCode(PrePic_Size_NM);
	macCheckRunCode(PrePic_Size_NS);
	macCheckRunCode(PrePic_Size_S);


	//--------------------------------------------------
	macCheckRunCode(SYS_IOT_NAME);


	//--------------------------------------------------
	macCheckRunCode(TEST_ModbusMod);
	macCheckRunCode(Debug_DisableAuth);
	macCheckRunCode(TEST_OpenRFOriginal);
	//macCheckRunCode(TEST_Disable_GSM);
	macCheckRunCode(TEST_TelReplaceSMS);
	macCheckRunCode(TEST_DoEvent_NoInterval);
	macCheckRunCode(TEST_RTMP_TestMod);
	macCheckRunCode(TEST_PlayBack_timefix_pnt);
	macCheckRunCode(TEST_StreamServer);
	macCheckRunCode(TEST_DisableHeartbeatMon);
	macCheckRunCode(TEST_DisableAlarmGuard);
	macCheckRunCode(TEST_DeCheckFilePlayback);
	macCheckRunCode(TEST_UseSpecRTMPUrlList);
	macCheckRunCode(Dis_CommLinkTransparent);
	macCheckRunCode(Dis_ChangeSaveDB);
	macCheckRunCode(Dis_RunDataProc);
	macCheckRunCode(TEST_ForceDataSave);
	macCheckRunCode(TEST_DlgQueryMain);

	macCheckRunCode(Disable_Recv_RF);
	macCheckRunCode(Disable_Recv_RS485);
	macCheckRunCode(Disable_Recv_RF_original);
	macCheckRunCode(Disable_Recv_GSM);
	macCheckRunCode(Disable_Recv_IR_code);
	macCheckRunCode(Disable_Recv_IR_original);

	macCheckRunCode(Disable_Send_RF_TX);
	macCheckRunCode(Disable_Send_RS485);
	macCheckRunCode(Disable_Send_RF_original);
	macCheckRunCode(Disable_Send_GSM);

	macCheckRunCode(TEST_Develop_NewFunc);
	macCheckRunCode(TEST_Debug_TempCode);


	for( int i=0; i<(sizeof(s_RunCode)/sizeof(struRunCode)); ++i )
	{
		if( 0 != s_RunCode[i].code )
		{
			if( defRunCode_CheckOK != s_RunCode[i].CheckFlag )
			{
				// 每个code都必须增加macCheckRunCode进行校验
				//LOGMSGEX( defLOGNAME, defLOG_ERROR, "s_InitRunCode: code=%d, codestr=%s check error!!!\r\n\r\n", s_RunCode[i].code, s_RunCode[i].codestr );
				assert(false);
			}
		}
	}

	code_set.clear();
	codestr_set.clear();
	//LOGMSGEX( defLOGNAME, defLOG_SYS, "s_InitRunCode and Check Finish." );
}

void s_ListRunCode( int nfmt )
{
	LOGMSG( "s_ListRunCode Begin...(fmt=1/2)" );
	
	std::string strfmt;

	switch( nfmt )
	{
	case 2:
		LOGMSG( "id;enable;code;codestr;value1;value2;value3;value4;" );
		strfmt = "%d;%d;%d;%s;%d;%d;%d;%d;";
		break;

	case 1:
	default:
		LOGMSG( "id;enable; code,%-32s,% 8s,% 8s,% 8s,% 8s,", "codestr", "value1", "value2", "value3", "value4" );
		strfmt = "% 3d,% 2d,% 8d,%-32s,% 8d,% 8d,% 8d,% 8d,";
		break;
	}

	int printid = 1;
	for( int i=0; i<(sizeof(s_RunCode)/sizeof(struRunCode)); ++i )
	{
		if( 0 != s_RunCode[i].code )
		{
			LOGMSG( strfmt.c_str(), 
				printid++, 1, s_RunCode[i].code, s_RunCode[i].codestr, 
				s_RunCode[i].value1, s_RunCode[i].value2, s_RunCode[i].value3, s_RunCode[i].value4 );
		}
	}

	LOGMSG( "s_ListRunCode End." );
}

const struRunCode& s_RefRunCode( defCodeIndex_ codeindex )
{
	if( codeindex<defCodeIndex_Unknown || codeindex>=defCodeIndex_MAX_ )
	{
		return s_RunCode[defCodeIndex_Unknown];
	}

	return s_RunCode[codeindex];
}

bool s_IsDebugMod()
{
	for( int i=defCodeIndex_TEST__Spec__; i<defCodeIndex_MAX_; ++i )
	{
		if( s_RunCode[i].default_value1 != s_RunCode[i].value1 )
		{
			if( defRunCodeVal_Spec_Enable == s_RunCode[i].value1 )
			{
				return true;
			}
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////

CRunCodeMgr::CRunCodeMgr(void)
	:SQLiteHelper()
{
}

CRunCodeMgr::~CRunCodeMgr(void)
{
}

void CRunCodeMgr::Init()
{
	static bool s_isInitRunCode = false;

	if( s_isInitRunCode )
		return ;

	RUNCODE_Init();
	LoadDB_code();

	if( IsRUNCODEEnable(defCodeIndex_SYS_ListRunCode) )
	{
		s_ListRunCode( RUNCODE_Get(defCodeIndex_SYS_ListRunCode,defRunCodeValIndex_2) );
	}

	s_isInitRunCode = true;
}

void CRunCodeMgr::LoadDB_code()
{
	LOGMSG( "LoadDB_code and SetRunCode...\r\n" );

	SQLite::Statement query(*this->db,"select * from code where enable=1 order by code");
	while(query.executeStep()){
		int col = 0;
		int id = query.getColumn(col++).getInt();
		int enable = query.getColumn(col++).getInt();
		int code = query.getColumn(col++).getInt();

		SQLite::Column colobj = query.getColumn(col++);
		std::string codestr = colobj.isNull()?"":colobj;

		int val1 = query.getColumn(col++).getInt();
		int val2 = query.getColumn(col++).getInt();
		int val3 = query.getColumn(col++).getInt();
		int val4 = query.getColumn(col++).getInt();

		//jyc20160825 can not run have trouble
		//std::string str_val = query.getColumn(col++);
		std::string str_val = "jyc20160825";
		
		s_RunCode[RUNCODE_Index(code, codestr.c_str())].str_val = str_val;
		RUNCODE_Set( RUNCODE_Index(code, codestr.c_str()), val1, val2, val3, val4, true, true, true, true );
	}
}

bool CRunCodeMgr::SaveDB_code( defCodeIndex_ index, bool save_str_val )
{
	if( index<=defCodeIndex_Unknown || index>=defCodeIndex_MAX_ )
	{
		//LOGMSGEX( defLOGNAME, defLOG_ERROR, "SaveDB_code error! code={%d}\r\n", index );
		return false;
	}

	char sqlbuf[1024] = {0};
	snprintf( sqlbuf, sizeof(sqlbuf), "SELECT id FROM [code] WHERE code=%d OR codestr='%s' LIMIT 1", s_RunCode[index].code, s_RunCode[index].codestr );

	SQLite::Statement queryid( *this->db, sqlbuf );

	if(queryid.executeStep())
	{
		int id = queryid.getColumn(0);

		std::string sql = "UPDATE [code] SET enable=?,code=?,codestr=?,value1=?,value2=?,value3=?,value4=? WHERE id=?";
		if( save_str_val )
		{
			sql = "UPDATE [code] SET enable=?,code=?,codestr=?,str_val=? WHERE id=?";
		}

		SQLite::Statement query( *this->db, sql.c_str() );

		int col = 1;

		query.bind(col++, int(1));
		query.bind(col++, s_RunCode[index].code);
		query.bind(col++, s_RunCode[index].codestr);

		if( save_str_val )
		{
			query.bind(col++, s_RunCode[index].str_val);
		}
		else
		{
			query.bind(col++, s_RunCode[index].value1);
			query.bind(col++, s_RunCode[index].value2);
			query.bind(col++, s_RunCode[index].value3);
			query.bind(col++, s_RunCode[index].value4);
		}

		query.bind(col++, id);
		query.exec();
	}
	else
	{
	    SQLite::Statement query( *this->db, "INSERT INTO [code] VALUES(NULL,:enable,:code,:codestr,:value1,:value2,:value3,:value4,:str_val)" );

		int col = 1;

		query.bind(col++, int(1));
		query.bind(col++, s_RunCode[index].code);
		query.bind(col++, s_RunCode[index].codestr);
		query.bind(col++, s_RunCode[index].value1);
		query.bind(col++, s_RunCode[index].value2);
		query.bind(col++, s_RunCode[index].value3);
		query.bind(col++, s_RunCode[index].value4);
		query.bind(col++, s_RunCode[index].str_val);

		query.exec();
	}

	if( save_str_val )
	{

	}
	else
	{

	}

	return true;
}

bool CRunCodeMgr::SetCodeAndSaveDb( defCodeIndex_ index, int val1, int val2, int val3, int val4, bool do1, bool do2, bool do3, bool do4 )
{
	if( !( RUNCODE_Set( index, val1, val2, val3, val4, do1, do2, do3, do4 ) ) )
		return false;

	return this->SaveDB_code( index );
}

bool CRunCodeMgr::SetCodeStrValAndSaveDb( defCodeIndex_ index, const std::string &str_val )
{
	s_RunCode[index].str_val = str_val;
	return this->SaveDB_code( index, true );	
}

