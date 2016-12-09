#include "DeviceAddress.h"
#include "common.h"
//#include <windows.h> //20160603
#include <time.h>


DeviceAddress::DeviceAddress(uint32_t address)
	:m_address(address),m_type(IOT_DEVICE_Unknown),m_readType(IOT_Unknow),m_dataType(IOT_DataUnknow),
	m_dataObjLen(0),m_dataObject(NULL),m_curValue("0"),m_defaultValue("0"),m_minValue("0"),m_maxValue("0"),
	m_lastUpdate(0)//, m_enable(defDeviceEnable)
{
	m_timecurValue = 0;
	ResetDataAnalyse();
	InitNewMutex();
}

DeviceAddress::DeviceAddress(uint32_t address, std::string name, IOTDeviceType type, DataType dataType, IOTDeviceReadType readType,
	std::string curValue, std::string maxValue, std::string minValue, std::string defaultValue,
	uint8_t *dataObject, uint32_t dataObjLen )
	:m_address(address),m_name(name),m_type(type),m_readType(readType),m_dataType(dataType),
	m_dataObjLen(dataObjLen),m_dataObject(dataObject),m_curValue(curValue),m_defaultValue(defaultValue),m_maxValue(maxValue),m_minValue(minValue),
	m_lastUpdate(0)//, m_enable(defDeviceEnable)
{
	m_timecurValue = 0;
	ResetDataAnalyse();
	InitNewMutex();
}

DeviceAddress::DeviceAddress(const Tag* tag)
	://ControlBase(tag),
	m_address(0),m_type(IOT_DEVICE_Unknown),m_readType(IOT_Unknow),m_dataType(IOT_DataUnknow),
	m_dataObjLen(0),m_dataObject(NULL),m_curValue("0"),m_defaultValue("0"),m_minValue("0"),m_maxValue("0"),
	m_lastUpdate(0)//, m_enable(defDeviceEnable)
{
	m_timecurValue = 0;
	ResetDataAnalyse();
	InitNewMutex();

	if( !tag || tag->name() != "address")
      return;

	if(tag->hasAttribute("data"))
	   this->m_address = atoi(tag->findAttribute("data").c_str());
	if(tag->hasAttribute("name"))
		this->m_name = UTF8ToASCII(tag->findAttribute("name"));
	if(tag->hasAttribute("type"))
	this->m_type = (IOTDeviceType)atoi(tag->findAttribute("type").c_str());
	if(tag->hasAttribute("readtype"))
	this->m_readType = (IOTDeviceReadType)atoi(tag->findAttribute("readtype").c_str());
	if(tag->hasAttribute("datatype"))
	this->m_dataType = (DataType)atoi(tag->findAttribute("datatype").c_str());
	if(tag->hasAttribute("cur_value"))
	this->m_curValue = tag->findAttribute("cur_value");
	if(tag->hasAttribute("defualt_value"))
	this->m_defaultValue = tag->findAttribute("defualt_value");
	if(tag->hasAttribute("min_value"))
	this->m_minValue = tag->findAttribute("min_value");
	if(tag->hasAttribute("max_value"))
	this->m_maxValue = tag->findAttribute("max_value");

	this->UntagEditAttr( tag );
}

DeviceAddress::~DeviceAddress(void)
{
	macCheckAndDel_Array(m_dataObject);

	if( m_pmutex_addr )
	{
		delete m_pmutex_addr;
		m_pmutex_addr = NULL;
	}
}

void DeviceAddress::ResetDataAnalyse()
{
	m_data_MultiReadCount = 3;
	m_lastSampTime = 0;

	m_lastSaveTime = 0;
	m_lastSaveValue = m_curValue;

	m_ValueWindow_StartTs = 0;
	m_ValueWindow_MaxValue = m_curValue;

	m_data_abnormal_count = 0;
	m_curMaxValue = "";
	m_curMinValue = "";
	m_timecurMaxValue = 0;
	m_timecurMinValue = 0;
}

bool DeviceAddress::doEditAttrFromAttrMgr( const EditAttrMgr &attrMgr, defCfgOprt_ oprt )
{
	if( attrMgr.GetEditAttrMap().empty() )
		return false;

	bool doUpdate = false;
	std::string outAttrValue;

	if( attrMgr.FindEditAttr( "name", outAttrValue ) )
	{
		doUpdate = true;
		this->SetName( outAttrValue );
	}

	return doUpdate;
}

void DeviceAddress::InitNewMutex()
{
	// 只有在新建实例时才会调用，所以这里是不判断是否已分配，总是强制分配
	m_pmutex_addr = new gloox::util::Mutex();
}

uint8_t *DeviceAddress::GetCurToByte(uint8_t *data)
{
	switch(this->m_dataType)
	{
	case IOT_Integer:
		return Int32ToByte(data, atoi(this->m_curValue.c_str()));
    case IOT_String:
		*data++= this->m_curValue.length();
		memcpy(data,this->m_curValue.c_str(),this->m_curValue.length());
		return data+ this->m_curValue.length();
    case IOT_Boolean:
		if(this->m_curValue == "1"){
			*data ++= 0x01;
		}else{
			*data ++= 0x00;
		}
		return data;
	case IOT_Byte:
		*data ++= atoi(this->m_curValue.c_str());
		return data;
	case IOT_Int16:
		return Int16ToByte(data, atoi(this->m_curValue.c_str()));
	case IOT_Long:
		return LongToByte(data, atol(this->m_curValue.c_str()));
	case IOT_Double:
		return DoubleToByte(data, atof(this->m_curValue.c_str()));
	case IOT_Float:
		return FloatToByte(data, (float)atof(this->m_curValue.c_str()));
	}
	return data;
}

uint8_t *DeviceAddress::GetObjectData(uint8_t *data)
{
	if(this->m_dataObjLen>0 && this->m_dataObject ){
		memcpy(data,this->m_dataObject,this->m_dataObjLen);
		data+=this->m_dataObjLen;
	}
	return data;
}

void DeviceAddress::SetObjectData(uint8_t *data,uint32_t len)
{
	macCheckAndDel_Array(m_dataObject);

	if(len>0){
		//this->m_dataObject = (uint8_t *)malloc(len);
		this->m_dataObject = new uint8_t[len];
		this->m_dataObjLen = len;

		memcpy(m_dataObject,data,len);
	}
}

Tag* DeviceAddress::tag(const struTagParam &TagParam)
{
	Tag* i = new Tag( "address" );
	i->addAttribute("data",(int)this->m_address);

	if( TagParam.isValid && TagParam.isResult )
	{
		this->tagEditAttr( i, TagParam );

		return i;
	}

	if( 1==TagParam.fmt )
	{
		i->addAttribute("cur_value",this->GetCurValue());
		return i;
	}
	
	i->addAttribute("name",ASCIIToUTF8(this->m_name));
	if( IOT_DEVICE_Unknown != this->m_type ) { i->addAttribute("type",this->m_type); }
	if( IOT_Unknow != this->m_readType ) { i->addAttribute("readtype",this->m_readType); }
	if( IOT_DataUnknow != this->m_dataType ) { i->addAttribute("datatype",this->m_dataType); }
	i->addAttribute("cur_value",this->GetCurValue());
	if( this->m_defaultValue != "0" ) { i->addAttribute("defualt_value",this->m_defaultValue); }
	if( this->m_minValue != "0" ) { i->addAttribute("min_value",this->m_minValue); }
	if( this->m_maxValue != "0" ) { i->addAttribute("max_value",this->m_maxValue); }

	int attr = 0;
	if( this->GetAttrObj().get_AdvAttr( DeviceAddressAttr::defAttr_IsReSwitch ) )
		attr = DeviceAddressAttr::defAttr_IsReSwitch;
	else if( this->GetAttrObj().get_AdvAttr( DeviceAddressAttr::defAttr_IsAutoBackSwitch ) )
		attr = DeviceAddressAttr::defAttr_IsAutoBackSwitch;

	if( attr ) { i->addAttribute( "attr", attr ); }

	return i;
}

bool DeviceAddress::SetMultiReadCount( int MultiReadCount )
{
	gloox::util::MutexGuard( this->m_pmutex_addr );

	m_data_MultiReadCount = MultiReadCount;

	return true;
}

int DeviceAddress::GetMultiReadCount()
{
	gloox::util::MutexGuard( this->m_pmutex_addr );

	return m_data_MultiReadCount;
}

// 是否连接读取并递减次数
int DeviceAddress::PopMultiReadCount()
{
	gloox::util::MutexGuard( this->m_pmutex_addr );

	if( m_data_MultiReadCount > 99 )
	{
		m_data_MultiReadCount = 99;
	}

	if( m_data_MultiReadCount > 0 )
	{
		return ( m_data_MultiReadCount-- );
	}

	return 0;
}

void DeviceAddress::NowSampTick()
{
	gloox::util::MutexGuard( this->m_pmutex_addr );

	m_lastSampTime = timeGetTime();

	if( 0 == m_lastSampTime )
	{
		m_lastSampTime++;
	}
}


std::string DeviceAddress::GetCurValue( bool *isOld, uint32_t *noUpdateTime, const uint32_t oldtimeSpan, bool *isLowSampTime, const bool curisTimePoint,
	time_t *timecurValue, time_t *timecurMaxValue,	time_t *timecurMinValue )
{
	gloox::util::MutexGuard( this->m_pmutex_addr );

	// 是否长时间没有更新
	uint32_t tick = timeGetTime()-m_lastUpdate;
	if( isOld )
	{
		*isOld = false;
		if( 0 == m_lastUpdate || tick>oldtimeSpan )
		{
			*isOld = true;
		}

		const int DataSamp_LongTimeNoData = RUNCODE_Get(defCodeIndex_SYS_DataSamp_LongTimeNoData);
		if( m_lastSampTime
			&& tick>(DataSamp_LongTimeNoData*1000)
			)
		{
			if( isLowSampTime )
			{
				*isLowSampTime = true;

				const int DataSamp_LowSampTime = RUNCODE_Get( defCodeIndex_SYS_DataSamp_LowSampTime, curisTimePoint ? defRunCodeValIndex_2:defRunCodeValIndex_1 );
				if( timeGetTime()-m_lastSampTime<(DataSamp_LowSampTime*1000) )
				{
					// 太长时间未获取数据则进入低频率采集
					*isOld = false;
				}
				else
				{
					LOGMSG( "DataSamp_LowSampTime(%d,%d) DataSamp_LongTimeNoData=%ds, DataSamp_LowSampTime=%ds", m_type, this->GetAddress(), DataSamp_LongTimeNoData, DataSamp_LowSampTime );
				}
			}
		}
	}
	
	if( noUpdateTime ) *noUpdateTime = tick;
	if( timecurValue ) *timecurValue = m_timecurValue;
	if( timecurMaxValue ) *timecurMaxValue = m_timecurMaxValue;
	if( timecurMinValue ) *timecurMinValue = m_timecurMinValue;

	return this->m_curValue;
}


std::string DeviceAddress::GetCurMaxValue( time_t *timecurMaxValue )
{
	gloox::util::MutexGuard( this->m_pmutex_addr );

	if( timecurMaxValue ) *timecurMaxValue = m_timecurMaxValue;

	return this->m_curMaxValue;
}

std::string DeviceAddress::GetCurMinValue( time_t *timecurMinValue )
{
	gloox::util::MutexGuard( this->m_pmutex_addr );

	if( timecurMinValue ) *timecurMinValue = m_timecurMinValue;

	return this->m_curMinValue;
}

bool DeviceAddress::SetCurValue( const std::string& newValue, const time_t newValTime, const bool annlyse, std::string *strlog )
{
	gloox::util::MutexGuard( this->m_pmutex_addr );
	
	const float newCurValueF = atof(newValue.c_str());
	if( annlyse )
	{
		// 赋初值，只赋值，不做任何状态更新
		if( 0 == m_lastUpdate )
		{
			this->m_curValue = newValue;
			m_lastUpdate++;
			return false;
		}

		if( abs( newCurValueF-atof(m_curValue.c_str()) ) > 5.0f )
		{
			m_data_MultiReadCount = 5; // 大幅度变化是连续读取值

			// 数据异常变化
			if( m_data_abnormal_count>1 )
			{
				// 上次已经出现过，认为是正常变化了大幅度数值
				m_data_abnormal_count = 0;
			}
			else
			{
				// 不更新数值，需重新采集
				m_data_abnormal_count++;

				if( strlog )
				{
					char chlog[1024] = {0};
					snprintf( chlog, sizeof(chlog), "SetCurValue(%d,%d) data_abnormal_count=%d, old=%s, new=%s", m_type, this->GetAddress(), m_data_abnormal_count, m_curValue.c_str(), newValue.c_str() );
					*strlog = chlog;
				}

				return false;
			}
		}

		if( m_data_abnormal_count )
		{
			m_data_abnormal_count = 0;
		}
	}

	this->m_curValue = newValue;
	m_lastUpdate = timeGetTime();
	m_timecurValue = newValTime;

	if( 0 == m_lastUpdate )
	{
		m_lastUpdate++;
	}
	m_lastSampTime = m_lastUpdate;

	if( 0==m_timecurMaxValue || newCurValueF>atof(m_curMaxValue.c_str()) )
	{
		m_timecurMaxValue = m_timecurValue;
		m_curMaxValue = m_curValue;
	}

	if( 0==m_timecurMinValue || newCurValueF>atof(m_curMinValue.c_str()) )
	{
		m_timecurMinValue = m_timecurValue;
		m_curMinValue = m_curValue;
	}

	return true;
}

// 数据分析
// 数据存储方式： 见defDataFlag_。
// 温湿度等常规监测量是发生变化存储，判断门限参考SYS_VChgRng_***。
// 风速这种按照窗口化处理后判断变化存储，门限参考SYS_WinTime_Wind，并对小值做合并处理参考SYS_MergeWindLevel/g_WindSpeedLevel()等。
// 存储时间点的判断，在时间点之前2分钟以内已经存储的，认为这个存储已经接近时间点，算作时间点存储，算完成了时间点存储。从时间点整点开始记，一个时间范围内都算作时间点存储。相关参考g_isTimePoint、g_TransToTimePoint等。
void DeviceAddress::DataAnalyse( const std::string& newValue, const time_t newValTime, bool *doSave, time_t *SaveTime, std::string *SaveValue, defDataFlag_* dataflag, std::string *strlog )
{
	gloox::util::MutexGuard( this->m_pmutex_addr );

	std::string newFinishValue = newValue;

	if( doSave && SaveTime && SaveValue && dataflag && g_isNeedSaveType(m_type) )
	{
		bool isFinishValue = false;
		switch( m_type )
		{
		case IOT_DEVICE_Wind:
			{
				std::string WindowFinishValue;

				// 此类型的数据，采集到的值先做窗口化处理
				// 窗口化数值处理：找到这段时间内更符合的值
				if( ValueWindow( newValue, newValTime, WindowFinishValue, RUNCODE_Get(defCodeIndex_SYS_WinTime_Wind), RUNCODE_Get(defCodeIndex_SYS_WinTime_Wind,defRunCodeValIndex_2) ) )
				{
					isFinishValue = true;
					newFinishValue = WindowFinishValue;
				}
			}
			break;

		case IOT_DEVICE_Temperature:
		case IOT_DEVICE_Humidity:
		default:
			{
				// 此类型的数据，采集到的值直接可用
				isFinishValue = true;
			}
			break;
		}

		if( isFinishValue )
		{
			if( m_lastSaveTime )
			{
				// 先进行存储时间点相关的判断
				// 当前时间所在时间点和上次存储的时间所在时间点不同。即，当前存储时间点还未存储过值。
				if( g_TransToTimePoint(newValTime, m_type, false)!=g_TransToTimePoint(m_lastSaveTime, m_type, true) )
				{
					*doSave = true;

					// 如果当前时间在时间点则标志位时间点存储，否则为普通存储
					if( g_isTimePoint(newValTime,m_type) )
					{
						*dataflag = defDataFlag_TimePoint;
					}

					if( strlog && (*doSave) )
					{
						char chlog[1024] = {0};
						snprintf( chlog, sizeof(chlog), "ValueDoSave for TimePoint (%d,%d) old=%s, new=%s", m_type, this->GetAddress(), m_lastSaveValue.c_str(), newFinishValue.c_str() );
						*strlog = chlog;
					}
				}

				// 是否达到需要存储的变化量
				if( !(*doSave) )
				{
					const float oldSaveValueF = atof(m_lastSaveValue.c_str());
					const float newFinishValueF = atof(newFinishValue.c_str());

					switch( m_type )
					{
					case IOT_DEVICE_Wind:
						{
							// 风速级别发生变化
							*doSave = ( g_WindSpeedLevel( oldSaveValueF, true ) != g_WindSpeedLevel( newFinishValueF, true ) );
							*dataflag = defDataFlag_Changed;

							if( strlog && (*doSave) )
							{
								char chlog[1024] = {0};
								snprintf( chlog, sizeof(chlog), "ValueDoSave for change (%d,%d) old=%s, new=%s", m_type, this->GetAddress(), m_lastSaveValue.c_str(), newFinishValue.c_str() );
								*strlog = chlog;
							}
						}
						break;

					case IOT_DEVICE_Temperature:
					case IOT_DEVICE_Humidity:
					default:
						{
							const float VChgRng = g_SYS_VChgRng(m_type);

							// 普通变化范围比较
							*doSave = ( abs( oldSaveValueF - newFinishValueF ) >= VChgRng );
							*dataflag = defDataFlag_Changed;

							if( strlog && (*doSave) )
							{
								char chlog[1024] = {0};
								snprintf( chlog, sizeof(chlog), "ValueDoSave for change (%d,%d) old=%s, new=%s, VChgRng=%.3f", m_type, this->GetAddress(), m_lastSaveValue.c_str(), newFinishValue.c_str(), VChgRng );
								*strlog = chlog;
							}
						}
						break;
					}
				}
			}
			else
			{
				*doSave = true;
				*dataflag = defDataFlag_First;

				if( strlog && (*doSave) )
				{
					char chlog[1024] = {0};
					snprintf( chlog, sizeof(chlog), "ValueDoSave for first (%d,%d) new=%s", m_type, this->GetAddress(), newFinishValue.c_str() );
					*strlog = chlog;
				}
			}

			if( *doSave )
			{
				m_lastSaveTime = newValTime;
				m_lastSaveValue = newFinishValue;

				*doSave = true;
				*SaveTime = m_lastSaveTime;
				*SaveValue = m_lastSaveValue;
			}
		}

//#ifdef _DEBUG
//		switch( m_type )
//		{
//		case IOT_DEVICE_Wind:
//			{
//				*dataflag = (defDataFlag_)(g_WindSpeedLevel( atof(newFinishValue.c_str()), false )*100 + (*dataflag));
//			}
//			break;
//		}
//#endif
	}
}

// 最后一次存储值
void DeviceAddress::SetLastSave( const time_t lastSaveTime, const std::string lastSaveValue )
{
	gloox::util::MutexGuard( this->m_pmutex_addr );

	m_lastSaveTime = lastSaveTime;
	m_lastSaveValue = lastSaveValue;
}

time_t DeviceAddress::GetLastSaveTime() const
{
	gloox::util::MutexGuard( this->m_pmutex_addr );

	return m_lastSaveTime;
}

std::string DeviceAddress::GetLastSaveValue() const
{
	gloox::util::MutexGuard( this->m_pmutex_addr );

	return m_lastSaveValue;
}

DeviceAddress* DeviceAddress::clone() const
{
	DeviceAddress *pNew = new DeviceAddress(*this);
	pNew->InitNewMutex();
	return pNew;
}

//
bool DeviceAddress::ValueWindow( const std::string& newValue, const time_t newValTime, std::string& WindowFinishValue, uint32_t WindowTime, uint32_t WindowTimeMin )
{
	const uint32_t curts = timeGetTime();

	if( 0==m_ValueWindow_StartTs )
	{
		m_ValueWindow_StartTs = curts;
		m_ValueWindow_MaxValue = newValue;
	}
	else
	{
		if( atof(newValue.c_str())>atof(m_ValueWindow_MaxValue.c_str()) )
		{
			m_ValueWindow_MaxValue = newValue;
		}

		const uint32_t span = curts - m_ValueWindow_StartTs;

		if( span >= (WindowTime*1000)
			||
			( span>(WindowTimeMin*1000) && g_isTimePoint(newValTime,m_type) && g_TransToTimePoint(newValTime, m_type, false)!=g_TransToTimePoint(m_lastSaveTime, m_type, true) )
			)
		{
			WindowFinishValue = m_ValueWindow_MaxValue;

			m_ValueWindow_StartTs = 0;
			m_ValueWindow_MaxValue = "0";

			LOGMSG( "ValueWindow(%d,%d) %s, WindowTime(%ds, min=%ds)", m_type, this->GetAddress(), WindowFinishValue.c_str(), WindowTime, WindowTimeMin );
			return true;
		}
	}

	return false;
}
