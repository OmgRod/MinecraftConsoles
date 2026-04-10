#include "stdafx.h"
#include "UI.h"
#include "UIControl.h"
#include "..\..\..\Minecraft.World\StringHelpers.h"
#include "..\..\..\Minecraft.World\JavaMath.h"

namespace
{
	void toIggyUtf16(const wchar_t *input, size_t length, vector<IggyUTF16> &output)
	{
		output.clear();
		output.reserve(length + 1);
		for (size_t i = 0; i < length; ++i)
		{
			output.push_back(static_cast<IggyUTF16>(input[i]));
		}
		output.push_back(0);
	}

	void toWString(const IggyUTF16 *input, size_t length, wstring &output)
	{
		output.clear();
		output.reserve(length);
		for (size_t i = 0; i < length; ++i)
		{
			output.push_back(static_cast<wchar_t>(input[i]));
		}
	}
}

UIControl_Base::UIControl_Base()
{
	m_bLabelChanged = false;
	m_label;
	m_id = 0;
}

bool UIControl_Base::setupControl(UIScene *scene, IggyValuePath *parent, const string &controlName)
{
	bool success = UIControl::setupControl(scene,parent,controlName);

	m_setLabelFunc = registerFastName(L"SetLabel");
	m_initFunc = registerFastName(L"Init");
	m_funcGetLabel = registerFastName(L"GetLabel");
	m_funcCheckLabelWidths = registerFastName(L"CheckLabelWidths");

	return success;
}

void UIControl_Base::tick()
{
	UIControl::tick();

	if ( m_label.needsUpdating() || m_bLabelChanged )
	{
		//app.DebugPrintf("Calling SetLabel - '%ls'\n", m_label.c_str());
		m_bLabelChanged = false;

		IggyDataValue result;
		IggyDataValue value[1];
		value[0].type = IGGY_DATATYPE_string_UTF16;
		IggyStringUTF16 stringVal;
		vector<IggyUTF16> labelUtf16;
		toIggyUtf16(m_label.c_str(), m_label.length(), labelUtf16);

		stringVal.string = labelUtf16.data();
		stringVal.length = m_label.length();
		value[0].string16 = stringVal;

		IggyResult out = IggyPlayerCallMethodRS ( m_parentScene->getMovie() , &result, getIggyValuePath() , m_setLabelFunc , 1 , value );

		m_label.setUpdated();
	}
}

void UIControl_Base::setLabel(UIString label, bool instant, bool force)
{
	if( force || ((!m_label.empty() || !label.empty()) && m_label.compare(label) != 0) ) m_bLabelChanged = true;
	m_label = label;

	if(m_bLabelChanged && instant)
	{
		m_bLabelChanged = false;

		IggyDataValue result;
		IggyDataValue value[1];
		value[0].type = IGGY_DATATYPE_string_UTF16;
		IggyStringUTF16 stringVal;
		vector<IggyUTF16> labelUtf16;
		toIggyUtf16(m_label.c_str(), m_label.length(), labelUtf16);

		stringVal.string = labelUtf16.data();
		stringVal.length = m_label.length();
		value[0].string16 = stringVal;

		IggyResult out = IggyPlayerCallMethodRS ( m_parentScene->getMovie() , &result, getIggyValuePath() , m_setLabelFunc , 1 , value );
	}
}

const wchar_t* UIControl_Base::getLabel()
{
	IggyDataValue result;
	IggyResult out = IggyPlayerCallMethodRS(m_parentScene->getMovie(), &result, getIggyValuePath(), m_funcGetLabel, 0, nullptr);

	if(result.type == IGGY_DATATYPE_string_UTF16)
	{
		wstring labelValue;
		toWString(result.string16.string, result.string16.length, labelValue);
		m_label = labelValue;
	}

	return m_label.c_str();
}

void UIControl_Base::setAllPossibleLabels(int labelCount, wchar_t labels[][256])
{
	IggyDataValue result;
	IggyDataValue *value = new IggyDataValue[labelCount];
	IggyStringUTF16 * stringVal = new IggyStringUTF16[labelCount];
	vector<vector<IggyUTF16>> utf16Labels(labelCount);

	for(unsigned int i = 0; i < labelCount; ++i)
	{
		toIggyUtf16(labels[i], wcslen(labels[i]), utf16Labels[i]);
		stringVal[i].string = utf16Labels[i].data();
		stringVal[i].length = utf16Labels[i].empty() ? 0 : static_cast<U32>(utf16Labels[i].size() - 1);
		value[i].type = IGGY_DATATYPE_string_UTF16;
		value[i].string16 = stringVal[i];
	}

	IggyResult out = IggyPlayerCallMethodRS ( m_parentScene->getMovie() , &result, getIggyValuePath() , m_funcCheckLabelWidths , labelCount , value );

	delete [] value;
	delete [] stringVal;
}

bool UIControl_Base::hasFocus()
{
	return m_parentScene->controlHasFocus( this );
}
