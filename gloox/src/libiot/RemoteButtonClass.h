#pragma once

#include "ControlBase.h"
#include "RFSignalDefine.h"
#include "EditAttrMgr.h"
#include "GSIOTObjBase.h"
#include <bitset>

#define USHORT unsigned short

// 远程操作对象基本通用属性
class GSRemoteObjBaseAttr
{
public:
	GSRemoteObjBaseAttr( void )
		: m_Attribute( 0 )
	{};

	~GSRemoteObjBaseAttr( void ){};

	// 属性有效位位置
	// 注：顺序与配置存储有关，版本确定后，只能往后增加，不能随便调整原有顺序
	enum defAttr
	{
		defAttr_Min_ = 0,
		defAttr_cmd_Open = 0,			// 是否支持打开命令
		defAttr_cmd_Close,				// 是否支持关闭命令
		defAttr_cmd_Get_ObjState,		// 是否支持获取对象状态命令
		defAttr_Max_
	};

	// 属性操作
	bool get_AdvAttr( const defAttr attr ) const
	{
		if( attr<0 || (size_t)attr>=m_Attribute.size() )
		{
			assert( false );
			return false;
		}

		return m_Attribute.test( attr );
	}

	bool set_AdvAttr( const defAttr attr, const bool val )
	{
		if( attr<0 || (size_t)attr>=m_Attribute.size() )
		{
			assert( false );
			return false;
		}

		m_Attribute.set( attr, val );
		return true;
	}

	uint32_t get_AdvAttr_uintfull() const
	{
		return m_Attribute.to_ulong();
	}

	bool set_AdvAttr_uintfull( const uint32_t fullval )
	{
		std::bitset<defAttr_Max_> attrfv( (_ULonglong)fullval );
		m_Attribute = attrfv;
		return true;
	}

	bool hasMoreCmd() const
	{
		return ( get_AdvAttr_uintfull() ? true:false );
	}

private:
	std::bitset<defAttr_Max_> m_Attribute;
};


class RemoteButton : public GSIOTObjBase, public EditAttrMgr
{
public:
	RemoteButton( const RFSignal &signal );
	RemoteButton( const Tag* tag );

	~RemoteButton(void)
	{
	}

	virtual GSIOTObjType GetObjType() const
	{
		return GSIOTObjType_RemoteButton;
	}

	virtual IOTDeviceType GetExType() const
	{
		return IOT_DEVICE_Unknown;
	}

	virtual void Save_RefreshToSave(){};	// 刷新值到即将保存的字段
	virtual void Save_RefreshFromLoad(){};	// 将来自保存的字段刷新为值

	bool doEditAttrFromAttrMgr( const EditAttrMgr &attrMgr, defCfgOprt_ oprt = defCfgOprt_Modify );

	void Reset( bool all=true );

	virtual Tag* tag(const struTagParam &TagParam);

	virtual int GetId() const
	{
		return m_id;
	}

	void SetId( int id )
	{
		m_id = id;
	}
	
	int GetSortNo() const
	{
		return m_sort_no;
	}

	void SetSortNo( int sort_no )
	{
		m_sort_no = sort_no;
	}

	uint32_t GetEnable() const 
	{
		return m_enable;
	}

	void SetEnable( uint32_t enable )
	{
		m_enable = enable;
	}

	void set_enable_col_save( int enable_col_save )
	{
		m_enable = enable_col_save & 0x01;
		m_sort_no = (enable_col_save & 0xFFFE) >> 1;
	}

	int get_enable_col_save() const
	{
		const int sort_no = (this->m_sort_no & 0x7FFF) << 1;
		const int enable = this->m_enable & 0x01;

		return (sort_no+enable);
	}

	RFSignal& GetSignal()
	{
		return this->m_signal;
	}
	
	const RFSignal& GetSignalSafe() const
	{
		return this->m_signal;
	}

	void UpdateSignal( const RFSignal &signal, bool onlysignal )
	{
		this->m_signal.Update( signal, onlysignal );
	}

	void SetName( const std::string& name )
	{
		this->m_name = name;
	}

	virtual const std::string GetObjName() const
	{
		return m_name;
	}

	virtual RemoteButton* clone() const
	{
		return new RemoteButton(*this);
	}

	virtual GSIOTObjBase* cloneObj() const
	{
		return clone();
	}

protected:
	int m_id;
	int m_sort_no;	// 排序号
	std::string m_name;
	uint32_t m_enable;
	RFSignal m_signal;
	GSRemoteObjBaseAttr m_BaseAttr;
};

//
class GSRemoteObj_AC_Door : public RemoteButton
{
public:
	GSRemoteObj_AC_Door( const RFSignal &signal );
	GSRemoteObj_AC_Door( const Tag* tag );

	~GSRemoteObj_AC_Door( void )
	{
	}

	virtual IOTDeviceType GetExType() const
	{
		return IOTDevice_AC_Door;
	}

protected:
	#pragma pack(push, 1)
	struct ObjParam
	{
		USHORT size;
		int DoorNo;
		int OpenDoorTime;			// 开门持续延时s
		uint32_t flagsave_base;		// 存储基本标志
		uint32_t flagsave_ex;		// 存储扩展标志

		ObjParam( void )
		{
			size = sizeof( ObjParam );
			DoorNo = 1;
			OpenDoorTime = 10;
			flagsave_base = 0;
			flagsave_ex = 0;
		}
	}m_param;
	#pragma pack(pop)

	struct ObjState
	{
		long DoorState;

		ObjState( void )
		{
			DoorState = 0;
		}
	}m_state;

public:
	virtual Tag* tag(const struTagParam &TagParam);

	virtual RemoteButton* clone() const
	{
		return new GSRemoteObj_AC_Door( *this );
	}

	virtual void Save_RefreshToSave();
	virtual void Save_RefreshFromLoad();

	void Reset( bool all=true );

	const ObjParam& get_param_safe() const
	{
		return m_param;
	}

	ObjParam& get_param()
	{
		return m_param;
	}

	const ObjState& get_state_safe() const
	{
		return m_state;
	}

	ObjState& get_state()
	{
		return m_state;
	}
};

