#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <atlconv.h>
#include <boost\lexical_cast.hpp>
#include <boost\algorithm\string.hpp>
#include "TypeDef.h"
#include "../tinxml/tinyxml.h"
#include "utils.h"

using namespace std;
using namespace boost;

class ConfigManager
{
public:
	ConfigManager():m_configFilePath(ConfigManager::GetConfigFilePath())
	{
		m_commonSetting.clear();
		m_chSettings.clear();
		m_bLoaded = false;
		m_nLangID = LANG_CHS;
		m_bAutoResetShiftStats = false;
	}
	ConfigManager(wstring strConfigFilePath):m_configFilePath(strConfigFilePath)
	{
		m_commonSetting.clear();
		m_chSettings.clear();
		m_bLoaded = false;
	}

	~ConfigManager(){}
	void ReloadConfigFile()
	{
		m_bLoaded = false;
		m_commonSetting.clear();
		m_vShift.clear();
		m_modelSetting.clear();
		m_chSettings.clear();
		ParseConfigFile();
	}
	bool ParseConfigFile()
	{
		if(m_bLoaded == true)
			return true;

		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Ansi(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile())
				return false;

			TiXmlNode* node = config.FirstChild("configuration");
			if(node == nullptr)
				return false;

			ParseBasicSettings(node);
			ParseShiftSettings(node);
			ParseModelSetting(node);
			ParseChannelSettings(node);
			m_bLoaded = true;
		}
		return true;
	}
	wstring GetBasicSetting(wstring key)
	{
		if(!m_bLoaded)
			ParseConfigFile();

		wstring ret(L"");
		if(m_commonSetting.count(key) > 0)
	    {
		    ret = m_commonSetting[key];
	    }
	   return ret;
	}
	int GetLanguageID()
	{
		wstring strLang = GetBasicSetting(L"LANGUAGE");
		if(strLang == L"CHS")
			m_nLangID = LANG_CHS;
		if(strLang == L"ENU")
			m_nLangID = LANG_ENU;
		if(strLang == L"RUS")
			m_nLangID = LANG_RUS;
		return m_nLangID;
	}
	int GetChannelCount()
	{
		return (int)m_chSettings.size();
	}
	void SetBatchNumber(CString cstrbatchnumber)
	{
		if(CheckFileExists(m_configFilePath))
		{
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return;

			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return;

			auto pBS = pNode->FirstChild("BasicSetting");
			if(pBS == nullptr) return;

			TiXmlElement* pBSElement = pBS->ToElement();
			if(pBSElement == nullptr) return;
			wstring wstrbatchnumber = cstrbatchnumber.GetString();
			std::string strbatchnumber = utils::ws2s(wstrbatchnumber);

			bool bHasBatchNumber = false;
			for(auto pElement = pBSElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				std::string strName;
				pElement->QueryStringAttribute("key",&strName);
				if(strName == "BATCHNUMBER")
				{
					pElement->SetAttribute("value", strbatchnumber);
					bHasBatchNumber = true;
				}
			}

			//没有字段时 自动添加批次号字段！
			if (!bHasBatchNumber)
			{
				TiXmlElement it("add");
				it.SetAttribute("key", "BATCHNUMBER");
				it.SetAttribute("value", strbatchnumber);
				pBS->InsertEndChild(it);
			}

			config.SaveFile();
//			ReloadConfigFile();   最后保存批次号字符串，不需要多次载入配置文件
		}
	}

	void SetUploadTime(CString cstrlastuploadtime)
	{
		if(CheckFileExists(m_configFilePath))
		{
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return;

			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return;

			auto pBS = pNode->FirstChild("BasicSetting");
			if(pBS == nullptr) return;

			TiXmlElement* pBSElement = pBS->ToElement();
			if(pBSElement == nullptr) return;
			wstring wstrlastuploadtime = cstrlastuploadtime.GetString();
			std::string strlastuploadtime = utils::ws2s(wstrlastuploadtime);

			bool bHasBatchNumber = false;
			for(auto pElement = pBSElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				std::string strName;
				pElement->QueryStringAttribute("key",&strName);
				if(strName == "lastuploadtime")
				{
					pElement->SetAttribute("value", strlastuploadtime);
					bHasBatchNumber = true;
				}
			}

			//没有字段时 自动添加批次号字段！
			if (!bHasBatchNumber)
			{
				TiXmlElement it("add");
				it.SetAttribute("key", "lastuploadtime");
				it.SetAttribute("value", strlastuploadtime);
				pBS->InsertEndChild(it);
			}

			config.SaveFile();
			//			ReloadConfigFile();   最后保存批次号字符串，不需要多次载入配置文件
		}
	}

	void SetSysForeignKey(CString strSysForeignKey)
	{
		if(CheckFileExists(m_configFilePath))
		{
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return;

			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return;

			auto pBS = pNode->FirstChild("BasicSetting");
			if(pBS == nullptr) return;

			TiXmlElement* pBSElement = pBS->ToElement();
			if(pBSElement == nullptr) return;
			wstring wstrSysForeignKey = strSysForeignKey.GetString();
			std::string strSysForeignKey = utils::ws2s(wstrSysForeignKey);

			bool bHasSysForeignKey = false;
			for(auto pElement = pBSElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				std::string strName;
				pElement->QueryStringAttribute("key",&strName);
				if(strName == "foreign_key")
				{
					pElement->SetAttribute("value", strSysForeignKey);
					bHasSysForeignKey = true;
				}
			}

			//没有字段时 自动添加批次号字段！
			if (!bHasSysForeignKey)
			{
				TiXmlElement it("add");
				it.SetAttribute("key", "foreign_key");
				it.SetAttribute("value", strSysForeignKey);
				pBS->InsertEndChild(it);
			}

			config.SaveFile();
			//			ReloadConfigFile();   最后保存批次号字符串，不需要多次载入配置文件
		}
	}

	void SetLanguageID(int nLangID)
	{
		m_nLangID = nLangID;
		if(CheckFileExists(m_configFilePath))
		{
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return;

			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return;

			auto pBS = pNode->FirstChild("BasicSetting");
			if(pBS == nullptr) return;

			TiXmlElement* pBSElement = pBS->ToElement();
			if(pBSElement == nullptr) return;

			std::string strLang;
			if(nLangID == LANG_CHS) strLang = "CHS";
			if(nLangID == LANG_ENU) strLang = "ENU";
			if(nLangID == LANG_RUS) strLang = "RUS";

			for(auto pElement = pBSElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				std::string strName;
				pElement->QueryStringAttribute("key",&strName);
				if(strName == "LANGUAGE")
				{
					pElement->SetAttribute("value", strLang);
				}
			}

			config.SaveFile();
			ReloadConfigFile();
		}
	}
	void SetSerialPort(int nPort)
	{
		if(nPort == -1) return;
		if(CheckFileExists(m_configFilePath))
		{
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile())
			{
				return;
			}
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return;

			auto pBS = pNode->FirstChild("BasicSetting");
			if(pBS == nullptr) return;

			TiXmlElement* pBSElement = pBS->ToElement();
			if(pBSElement == nullptr) return;

			std::string strPort = boost::lexical_cast<string>(nPort);

			bool bAbsent = true;
			for(auto pElement = pBSElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				std::string strName;
				pElement->QueryStringAttribute("key",&strName);
				if(strName == "LC_PORT")
				{
					pElement->SetAttribute("value", strPort);
					bAbsent = false;
				}
			}
			if(bAbsent)
			{
				TiXmlElement lcNode("add");
				lcNode.SetAttribute("key","LC_PORT");
				lcNode.SetAttribute("value",nPort);
				pBSElement->InsertEndChild(lcNode);
			}
			config.SaveFile();
		}
	}
	std::map<int, CHANNEL_CONFIG>* GetChannelConfigPtr(bool bLoad = false)
	{
		if (bLoad)
		{
			m_bLoaded = false;
		}
		
		if(!m_bLoaded)
		{
			ParseConfigFile();
		}
		return &m_chSettings;
	}
	std::map<wstring, TASK_MODEL>* GetTaskTemplateArrayPtr()
	{
		if(!m_bLoaded)
		{
			ParseConfigFile();
		}
		return &m_modelSetting;
	}
	std::vector<SHIFT>* GetShiftArrayPtr()
	{
		if(!m_bLoaded)
		{
			ParseConfigFile();
		}
		return &m_vShift;
	}
	std::vector<int>* GetProductIdArrayPtr()
	{
		if(!m_bLoaded)
		{
			ParseConfigFile();
		}
		return &m_vProductIDs;
	}

	CHANNEL_CONFIG GetChannelConfig(int nChannelIndex)
	{
		return m_chSettings[nChannelIndex];
	}

	void SetChannelConfig(int nChannelIndex,CHANNEL_CONFIG config)
	{
		m_chSettings[nChannelIndex] = config;
	}

	bool UpdateTaskCameraSetting(int chIndex, wstring Taskname,string strTaskCameraSetting)
	{


		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					auto pTaskGpNode = pElement->FirstChildElement("TaskGroup");
					if(pTaskGpNode == nullptr)  return false;

					auto pGpArray = pTaskGpNode->ToElement();
					if(pGpArray == nullptr) return false;

					for(auto p = pGpArray->FirstChildElement(); p; p = p->NextSiblingElement())
					{
						std::string ret;
						p->QueryValueAttribute("name", &ret);
						wstring retName = utils::Utf8ToUnicode(ret);
						if(retName == Taskname)
						{
							p->SetAttribute("DelayGainExposure",strTaskCameraSetting);
						}
					}
				}
			}
			config.SaveFile();
		}
		ReloadConfigFile();
		return true;
	}

	bool UpdateCameraSetting(int chIndex, CAMERA_SETTING* pSetting,bool bTaskParamAttach = false)
	{
		if(pSetting == nullptr) return false;
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{

				for(auto pCameraNode = pElement->FirstChildElement("Camera"); pCameraNode; pCameraNode = pCameraNode->NextSiblingElement("Camera"))
				{
						if(pCameraNode == nullptr)
						{
							// TODO: Add camera setting
						}
						auto pChildNode = pCameraNode->FirstChildElement("Name");
						wstring name  = utils::Utf8ToUnicode(pChildNode->FirstChild()->Value());
						
						if(pChildNode == nullptr) return false;
						if (name != pSetting->m_name)
						{
							continue;
						}

						string strValue = W2CA(pSetting->m_name.c_str());
						pChildNode->FirstChild()->SetValue(strValue);
					
						pChildNode = pCameraNode->FirstChildElement("TriggerMode");
						if(pChildNode == nullptr) return false;
						strValue = (pSetting->m_bTriggerMode == true?"Y":"N");
						pChildNode->FirstChild()->SetValue(strValue);
					
						// Frame Rate
						if(!UpdateChildNode(pCameraNode,"FrameRate",pSetting->m_nFrameRate)) return false;

						pChildNode = pCameraNode->FirstChildElement("TriggerActivation");
						if(pChildNode == nullptr) return false;
						strValue = (pSetting->m_triggerAct == FALL_EDGE?"FALL":"RISE");
						pChildNode->FirstChild()->SetValue(strValue);

						if (!bTaskParamAttach)
						{
							if(!UpdateChildNode(pCameraNode,"Gain",pSetting->m_gain)) return false;
							if(!UpdateChildNode(pCameraNode,"ExposureTime",pSetting->m_ExposureTime)) return false;
							if(!UpdateChildNode(pCameraNode,"DelayType",(int)pSetting->m_delayType)) return false;
							if(!UpdateChildNode(pCameraNode,"DelayAbs",pSetting->m_delayValue)) return false;
						}
						if(!UpdateChildNode(pCameraNode,"GevSCPSPacketSize",pSetting->m_nPacketSz)) return false;
						if(!UpdateChildNode(pCameraNode,"DropFrameUcl",pSetting->m_nDroppedImgUcl)) return false;

						config.SaveFile();
						break;
				}
				}
			}
			ReloadConfigFile();
			return true;
		}
		return false;
	}

	bool AddCameraSetting(int chIndex, CAMERA_SETTING* pSetting)
	{
		if(pSetting == nullptr) return false;
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				
				if(chIndex == boost::lexical_cast<int>(index))
				{
					TiXmlElement newNode("Camera");

					string strValue = W2CA(pSetting->m_name.c_str());
					if(!UpdateChildNode(&newNode,"Name", strValue)) return false;

					strValue = (pSetting->m_bTriggerMode == true?"Y":"N");
					if(!UpdateChildNode(&newNode,"TriggerMode", strValue)) return false;


					if(!UpdateChildNode(&newNode,"FrameRate", pSetting->m_nFrameRate)) return false;

					strValue = (pSetting->m_triggerAct == FALL_EDGE?"FALL":"RISE");
					if(!UpdateChildNode(&newNode,"TriggerActivation", strValue)) return false;
					if(!UpdateChildNode(&newNode,"Gain", pSetting->m_gain)) return false;
					if(!UpdateChildNode(&newNode,"DelayAbs", pSetting->m_delayValue)) return false;
					if(!UpdateChildNode(&newNode,"DelayType",pSetting->m_delayType)) return false;
					if(!UpdateChildNode(&newNode,"GevSCPSPacketSize", pSetting->m_nPacketSz)) return false;
					if(!UpdateChildNode(&newNode,"DropFrameUcl", pSetting->m_nDroppedImgUcl)) return false;
					if(!UpdateChildNode(&newNode,"ExposureTime", pSetting->m_ExposureTime)) return false;

					auto pCameraNode = pElement->FirstChildElement("Camera");
					if (pCameraNode!=NULL)
					{
						while (pCameraNode->NextSiblingElement("Camera"))
						{
							pCameraNode = pCameraNode->NextSiblingElement("Camera");
						}

						pElement->InsertAfterChild(pCameraNode,newNode);
					}
					else
					{
						pCameraNode = pElement->FirstChildElement("Light");
						pElement->InsertBeforeChild(pCameraNode,newNode);
					}
					
					config.SaveFile();
					break;
					}
				}
			
			ReloadConfigFile();
			return true;
		}
		return false;
	}
	bool DelCameraSetting(int chIndex, const wstring & CameraName)
	{
		if(CameraName.empty()) return false;
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					for (auto pCameraNode = pElement->FirstChildElement("Camera");pCameraNode;pCameraNode = pCameraNode->NextSiblingElement("Camera"))
					{
						auto pCameNameNode = pCameraNode->FirstChildElement("Name");
						wstring name  = utils::Utf8ToUnicode(pCameNameNode->FirstChild()->Value());

						if (name == CameraName)
						{
							pElement->RemoveChild(pCameraNode);
							break;
						}
					}
					config.SaveFile();
					break;
				}
			}
			ReloadConfigFile();
			return true;
		}
		return false;
	}
	bool AddLightSetting(TiXmlElement* pChNode, LIGHT_SETTING* pSetting)
	{
		TiXmlElement lightNode("Light");
		string strIDs;
		size_t sz = pSetting->m_vLcID.size();
		for(size_t t = 0; t < sz; t++)
		{
			strIDs.append(boost::lexical_cast<string>(pSetting->m_vLcID[t]));
			if(t != sz - 1)
			{
				strIDs.append(",");
			}
		}
		if(!strIDs.empty())
		{
			lightNode.SetAttribute("id", strIDs);
		}
		string strTags;
		sz = pSetting->m_vTags.size();
		for(size_t t = 0; t < sz; t++)
		{
			string strTag = utils::Unicode2Utf8(pSetting->m_vTags[t]);
			strTags.append(strTag);
			if(t != sz - 1)
			{
				strTags.append(",");
			}
		}
		if(!strTags.empty())
		{
			lightNode.SetAttribute("tag", strTags);
		}

		for(auto it = pSetting->m_dictParamGroup.begin() ; it != pSetting->m_dictParamGroup.end(); it++)
		{
			TiXmlElement paramGroup("ParameterGroup");
			paramGroup.SetAttribute("name", utils::Unicode2Utf8(it->first));

			std::vector<LC> vLcParam = it->second;

			for(size_t t = 0; t < vLcParam.size(); t++)
			{
				TiXmlElement controller("controller");
				controller.SetAttribute("id", vLcParam[t].m_nID);
				controller.SetAttribute("mode",vLcParam[t].m_nMode);
				if (vLcParam[t].m_nMode != 2)
				{
					controller.SetAttribute("value",vLcParam[t].PackageParam());
				}
				else
				{
					controller.SetAttribute("value",vLcParam[t].PackageParam1());
				}
			
				paramGroup.InsertEndChild(controller);
			}
			lightNode.InsertEndChild(paramGroup);
		}
		auto cameraNode = pChNode->FirstChildElement("Camera");
		if(cameraNode != nullptr)
		{
			pChNode->InsertAfterChild(cameraNode, lightNode);
		}
		else
		{
			pChNode->InsertEndChild(lightNode);
		}
		return true;
	}
	bool UpdateLightControllerParam(int chIndex, LIGHT_SETTING* pSetting, bool bReload = false)
	{
		if(pSetting == nullptr) return false;
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			for(auto pChNode = pPSE->FirstChildElement(); pChNode; pChNode = pChNode->NextSiblingElement())
			{
				pChNode->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					auto pLightNode = pChNode->FirstChildElement("Light");
					if(pLightNode != nullptr)
					{
						pChNode->RemoveChild(pLightNode);
					}
					AddLightSetting(pChNode,pSetting); 

					config.SaveFile();
					if(bReload)
						ReloadConfigFile();
					break;
				}
			}
		}
		return true;
	}
	bool DeleteLightControllerParam(int chIndex, const wstring& wstrTaskname)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			for(auto pChNode = pPSE->FirstChildElement(); pChNode; pChNode = pChNode->NextSiblingElement())
			{
				pChNode->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					auto pLightNode = pChNode->FirstChildElement("Light");
					if(pLightNode != nullptr)
					{
						auto pParamGroup = pLightNode->ToElement();
						if(pParamGroup != nullptr)
						{
							for(auto it = pParamGroup->FirstChildElement(); it; it = it->NextSiblingElement())
							{
								string strGroupName;
								it->QueryStringAttribute("name",&strGroupName);
								wstring wstrGpName = utils::Utf8ToUnicode(strGroupName);
								if(wstrGpName == wstrTaskname)
								{
									pLightNode->RemoveChild(it);
									break;
								}
							}
						}
					}
				}
			}
		config.SaveFile();
		}
		return true;
	}
	bool CreateLigthControllerParam(int chIndex, const wstring& wstrTaskname)
	{
		bool bOk = false;
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			
			for(auto pChNode = pPSE->FirstChildElement(); pChNode; pChNode = pChNode->NextSiblingElement())
			{
				pChNode->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					auto pLightNode = pChNode->FirstChildElement("Light");
					if(pLightNode != nullptr)
					{
						TiXmlElement paramGroup("ParameterGroup");
						paramGroup.SetAttribute("name", utils::Unicode2Utf8(wstrTaskname));
						pLightNode->InsertEndChild(paramGroup);
						bOk = true;
						break;
					}
				}
			}
			config.SaveFile();
		}
		return bOk;
	}
	bool CopyLightControllerParam(int chIndex, const wstring& srcTaskName, const wstring& newTaskName)
	{
		if(m_chSettings.find(chIndex) == m_chSettings.end())
			return false;
		LIGHT_SETTING& ls = m_chSettings[chIndex].m_lightCtrlSetting;
		if(ls.m_dictParamGroup.find(srcTaskName) != ls.m_dictParamGroup.end())
		{
			auto vLc = ls.m_dictParamGroup[srcTaskName];
			ls.m_dictParamGroup[newTaskName] = vLc;
			if(UpdateLightControllerParam(chIndex, &ls, false))
			{
				return true;
			}
		}
		return false;
	}
	bool UpdateRejectorSetting(int chIndex, REJECTOR_SETTING* pSetting)
	{
		if(pSetting == nullptr) return false;
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					auto pRejectorNode = pElement->FirstChildElement("Rejector");
					if(pRejectorNode == nullptr)
					{
						//TODO: Add rejector setting
					}
					string strValue;
					strValue = utils::Unicode2Utf8(pSetting->m_wstrDeviceIp);
					if(!UpdateChildNode(pRejectorNode,"DeviceIP", strValue)) return false;

// 					if(!UpdateChildNode(pRejectorNode,"RejectorPort",pSetting->m_nRejectPort)) return false;
// 					if(!UpdateChildNode(pRejectorNode,"DropDetectPort",pSetting->m_nDropImgPort)) return false;


					if(!UpdateChildNode(pRejectorNode,"CameraGrabTime",pSetting->m_CameraGrabTime)) return false;
					if(!UpdateChildNode(pRejectorNode,"TriggerIngoreTime",pSetting->m_TriggerIngoreTime)) return false;

					if(!UpdateChildNode(pRejectorNode,"SignalIngoreTime",pSetting->m_SignalIngoreTime)) return false;
					if(!UpdateChildNode(pRejectorNode,"ContinueRejectAlarm",pSetting->m_nContinueRejectAlarm)) return false;

			//		if(pSetting->m_mode == DELAY_TIME)     strValue = "T";
			//		if(pSetting->m_mode == DELAY_DISTANCE) strValue = "D";
			//		if(!UpdateChildNode(pRejectorNode,"Mode", strValue)) return false;

					if(!UpdateChildNode(pRejectorNode,"Delay",pSetting->m_delay)) return false;
					if(!UpdateChildNode(pRejectorNode,"SignalWidth",pSetting->m_signalWidth)) return false;
					
					strValue = "N";
					if(pSetting->m_bReReject)
						strValue = "Y";
					if(!UpdateChildNode(pRejectorNode,"ReRejectFlag",strValue)) return false;
					if(!UpdateChildNode(pRejectorNode,"ReSiToSol",pSetting->m_nReSitoSol)) return false;
					if(!UpdateChildNode(pRejectorNode,"ReDelay",pSetting->m_nReDelay)) return false;
					if(!UpdateChildNode(pRejectorNode,"ReSignalWidth",pSetting->m_nReSignalWidth)) return false;

					// Rejection Test
					strValue = "N";
					if(pSetting->m_bTestRejector)
						strValue = "Y";
					if(!UpdateChildNode(pRejectorNode, "TestFlag", strValue)) return false;
					if(!UpdateChildNode(pRejectorNode,"TestMode",pSetting->m_nTestMode)) return false;
					if(!UpdateChildNode(pRejectorNode,"RejectCount",pSetting->m_nNumerator)) return false;
					if(!UpdateChildNode(pRejectorNode,"RejectInterval",pSetting->m_nDenominator)) return false;
					if(!UpdateChildNode(pRejectorNode,"CDC_SA",(int)pSetting->m_sa)) return false;
					if(!UpdateChildNode(pRejectorNode,"CDC",pSetting->m_nCDC)) return false;

                    // Last N
                    if(!UpdateChildNode(pRejectorNode,"LastN",pSetting->m_nLastN)) return false;
                    ostringstream oss;
                    oss << pSetting->m_fLastNRatio;
                    string strLastNRatio = oss.str();
                    if(!UpdateChildNode(pRejectorNode,"LastNRatioUCL",strLastNRatio)) return false;
                    if(!UpdateChildNode(pRejectorNode,"LastNAlarmFlag",(pSetting->m_bEnableLastNAlarm?1:0))) return false;
				}
			}
			config.SaveFile();
			ReloadConfigFile();
			return true;
		}
		return false;
	}
	bool UpdateUiSetting(int chIndex, UI_SETTING* pSetting)
	{
		if(pSetting == nullptr)
			return false;
		//TODO:
		return false;
	}
	bool AddTask(int chIndex, TASK& task, bool bReload = true, bool bCrtTask = false)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return false ;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;

			std::string index;
			TiXmlElement* pTaskGpNode = nullptr;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					pTaskGpNode = pElement->FirstChildElement("TaskGroup");
					if(pTaskGpNode == nullptr)  return false;

					if(bCrtTask)
					{
						string strName = utils::Unicode2Utf8(task.m_wstrTaskName);
						pTaskGpNode->SetAttribute("crt_task_name", strName);
					}
					
					// Create New Task Node
					TiXmlElement taskNode("task");
					string taskName = utils::Unicode2Utf8(task.m_wstrTaskName);
					taskNode.SetAttribute("name", taskName);
					if(!task.m_strModelName.empty())
					{
						std::string strModelName = utils::Unicode2Utf8(task.m_strModelName);
						taskNode.SetAttribute("model_name",strModelName);
					}
					taskNode.SetAttribute("product_id", task.m_nProductId);
					taskNode.SetAttribute("blowthreshold", task.m_blowThreshold);

//  					//任务排盖延时
//  				    taskNode.SetAttribute("rjectdelay", task.m_rjectDelay);

					//任务相机参数
					wstring wstrTaskCameraSetting;
					string strTaskCameraSetting;
			
					size_t sz = task.m_vecTaskCameraSetting.size();

					for(size_t t = 0; t < sz; t++)
					{
						if (task.m_vecTaskCameraSetting[t].m_delayType == DELAY_TIME)
						{
							strTaskCameraSetting.append("T");
						}
						else
						{
							strTaskCameraSetting.append("D");
						}
						strTaskCameraSetting.append(",");
						strTaskCameraSetting.append(boost::lexical_cast<string>(task.m_vecTaskCameraSetting[t].m_delayValue));
						strTaskCameraSetting.append(",");
						strTaskCameraSetting.append(boost::lexical_cast<string>(task.m_vecTaskCameraSetting[t].m_gain));
						strTaskCameraSetting.append(",");
						strTaskCameraSetting.append(boost::lexical_cast<string>(task.m_vecTaskCameraSetting[t].m_ExposureTime));
						if(t != sz - 1)
						{
							strTaskCameraSetting.append(";");
						}
					}

					if(!strTaskCameraSetting.empty())
					{
						taskNode.SetAttribute("DelayGainExposure", strTaskCameraSetting);
					}
					if(task.m_bImgInside)
					{
						taskNode.SetAttribute("image_inside","true");
					}
					if(task.m_bSelfLearning)
					{
						taskNode.SetAttribute("self_learning", "true");
						taskNode.SetAttribute("sample_count", task.m_nSampleCount);
						if(task.m_wstrSlParam != L"")
						{
							string strParam = utils::Unicode2Utf8(task.m_wstrSlParam.c_str());
							taskNode.SetAttribute("param", strParam);
						}
					}
					std::for_each(task.m_vItParam.begin(), task.m_vItParam.end(), [&](IT_PARAM& funcInfo)
					{
						TiXmlElement it("interface");
						string strIndex = utils::Unicode2Utf8(funcInfo.m_itIndex);
						it.SetAttribute("index", strIndex);
						string strEable = (funcInfo.m_bEnable ? "true" : "false");
						it.SetAttribute("enable", strEable);

						string strName = utils::Unicode2Utf8(funcInfo.m_itName);
						it.SetAttribute("name", strName);

						string strParam;
						int sz = (int)funcInfo.m_vParam.size();
						for(int i = 0; i < sz; i++)
						{
							string strToken = utils::Unicode2Utf8(funcInfo.m_vParam[i]);
							strParam.append(strToken);
							if(i != sz - 1)
								strParam.append(",");
						}
						it.SetAttribute("param", strParam);
						if(!funcInfo.m_itNickname.empty())
						{
							string strNickname = utils::Unicode2Utf8(funcInfo.m_itNickname);
							it.SetAttribute("nickname", strNickname);
						}
						taskNode.InsertEndChild(it);
					});
					auto pFirstNode = pTaskGpNode->FirstChildElement();
					if(pFirstNode != nullptr)
						pTaskGpNode->InsertBeforeChild(pFirstNode,taskNode);
					else
						pTaskGpNode->InsertEndChild(taskNode);
				}
			}
			config.SaveFile();
			if(bReload)
			{
				ReloadConfigFile();
			}
			return true;
		}
		return false;
	}
	bool CreateNewTask(int nChIndex, wstring wstrTaskName, wstring wstrNewModelName)
	{
		if(m_modelSetting.empty())
			ParseConfigFile();

		if(!CreateLigthControllerParam(nChIndex, wstrTaskName))
		{
			return false;
		}

		TASK_MODEL model;
		for(auto it = m_modelSetting.begin(); it != m_modelSetting.end(); it++)
		{
			if(it->second.m_strModelName == wstrNewModelName)
			{
				model = it->second;
			}
		}

		TASK_MODEL taskModel   = model;
		TASK newTask;
		newTask.m_wstrTaskName   = wstrTaskName;
		newTask.m_bImgInside    = false;
		newTask.m_bSelfLearning = false;
		newTask.m_strModelName  = taskModel.m_strModelName;
		newTask.m_nProductId    = taskModel.m_nProductId;
		newTask.m_nSampleCount  = -1;

		PrepareTaskFuncAndParameter(newTask, taskModel);
		bool bCrtTask = false;
		TASK* pChCrtTask = m_chSettings[nChIndex].GetCurrentTask();
		if(pChCrtTask  != nullptr && pChCrtTask->m_wstrTaskName == wstrTaskName)
		{
			bCrtTask = true;
		}
		return AddTask(nChIndex, newTask,true,bCrtTask);
	}
	bool ModifySlTask(int chIndex, TASK& slTask)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return false ;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;

			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					auto pTaskGpNode = pElement->FirstChildElement("TaskGroup");
					if(pTaskGpNode == nullptr) return  false;

					auto pGpArray = pTaskGpNode->ToElement();
					if(pGpArray == nullptr) return false;

					TiXmlElement* pTarget = nullptr;
					for(auto p = pGpArray->FirstChildElement(); p; p = p->NextSiblingElement())
					{
						bool bSlFlag = false;
						p->QueryBoolAttribute("self_learning", &bSlFlag);

						if(bSlFlag)
						{
							p->SetAttribute("sample_count", slTask.m_nSampleCount);
							p->Clear();
							std::for_each(slTask.m_vItParam.begin(), slTask.m_vItParam.end(), [&](IT_PARAM& funcInfo)
							{
								TiXmlElement it("interface");
								string strIndex = utils::Unicode2Utf8(funcInfo.m_itIndex);
								it.SetAttribute("index", strIndex);
								string strName = utils::Unicode2Utf8(funcInfo.m_itName);
								it.SetAttribute("name", strName);
								if(!funcInfo.m_itNickname.empty())
								{
									string strNickname = utils::Unicode2Utf8(funcInfo.m_itNickname);
									it.SetAttribute("nickname", strNickname);
								}
								int sz = (int)funcInfo.m_vParam.size();
								if(sz != 0)
								{
									string strParam = utils::Unicode2Utf8(funcInfo.m_vParam[0]);
									it.SetAttribute("param", strParam);
									p->InsertEndChild(it);
								}
							});
							config.SaveFile();
						}
					}
				}
			}
			ReloadConfigFile();
		}
		return true;
	}
	bool DeleteTask(int chIndex, wstring strTaskName, bool bReload = true, bool bChangeCrtTask = false)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return false ;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;

			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					auto pTaskGpNode = pElement->FirstChildElement("TaskGroup");
					if(pTaskGpNode == nullptr)  return false;
					string crtTaskName, newTaskName;
					pTaskGpNode->QueryValueAttribute("crt_task_name", &crtTaskName);
					wstring wCrtTaskName = utils::Utf8ToUnicode(crtTaskName);

					auto pGpArray = pTaskGpNode->ToElement();
					if(pGpArray == nullptr) return false;

					TiXmlElement* pTarget = nullptr;
					for(auto p = pGpArray->FirstChildElement(); p; p = p->NextSiblingElement())
					{
						string ret;
						p->QueryValueAttribute("name", &ret);
						wstring retName = utils::Utf8ToUnicode(ret);
						if(retName == strTaskName)
						{
							pTarget = p;
						}
					}
					if(wCrtTaskName == strTaskName && bChangeCrtTask)
					{
						string strNewCrtTaskName = GetSubstituteCurrentTaskName(chIndex,crtTaskName);
						pTaskGpNode->SetAttribute("crt_task_name", strNewCrtTaskName);
					}
					pTaskGpNode->RemoveChild(pTarget);
					config.SaveFile();
					if(bReload)
					{
						ReloadConfigFile();
					}
					return true;
				}
			}
		}
		return false;
	}
	bool AddTaskTemplate(TASK_MODEL& newModel)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return false ;
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pTM = pNode->FirstChild("TaskModel");
			if(pTM == nullptr) return false;

			TiXmlElement modelNode("model");

			string modelName = utils::Unicode2Utf8(newModel.m_strModelName);
			modelNode.SetAttribute("name", modelName);
			modelNode.SetAttribute("product_id", newModel.m_nProductId);
		//	modelNode.SetAttribute("blowthreshold", "3");

			std::for_each(newModel.m_vItParam.begin(), newModel.m_vItParam.end(), [&](IT_PARAM& funcInfo)
			{
				TiXmlElement it("interface");
				string strIndex = utils::Unicode2Utf8(funcInfo.m_itIndex);
				it.SetAttribute("index", strIndex);
				string strName = utils::Unicode2Utf8(funcInfo.m_itName);
				it.SetAttribute("name", strName);
				if(!funcInfo.m_itNickname.empty())
				{
					string strNickname = utils::Unicode2Utf8(funcInfo.m_itNickname);
					it.SetAttribute("nickname", strNickname);
				}
				string strParam;
				int sz = (int)funcInfo.m_vParam.size();
				for(int i = 0; i < sz; i++)
				{
					string strToken = utils::Unicode2Utf8(funcInfo.m_vParam[i]);
					if(strToken.find(".reg") != string::npos ||
					   strToken.find(".shm") != string::npos)
					{
						strParam.append(" ");
					}
					else
					{
						strParam.append(strToken);
					}
					if(i != sz - 1)
						strParam.append(",");
				}
				it.SetAttribute("param", strParam);
				modelNode.InsertEndChild(it);
			});
			pTM->InsertEndChild(modelNode);

			config.SaveFile();
			ReloadConfigFile();
		}
		return true;
	}
	bool DeleteTaskTemplate(wstring strModelName)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return false ;
		
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pTM = pNode->FirstChild("TaskModel");
			if(pTM == nullptr) return false;

			auto pTmGp = pTM->ToElement();
			if(pTmGp == nullptr) return false;
			TiXmlElement* pTarget = nullptr;
			for(auto it = pTmGp->FirstChildElement(); it; it = it->NextSiblingElement())
			{
				string strName;
				it->QueryStringAttribute("name", &strName);
				wstring wstrName = utils::Utf8ToUnicode(strName);
				if(wstrName == strModelName)
				{
					pTarget = it;
				}
			}
			pTM->RemoveChild(pTarget);
			config.SaveFile();
			ReloadConfigFile();
		}
		return true;
	}
	bool ModifyTaskTemplate(const TASK_MODEL& oldModel, TASK_MODEL& newModel)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return false ;
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pTM = pNode->FirstChild("TaskModel");
			if(pTM == nullptr) return false;

			TiXmlElement* pTmElement = pTM->ToElement();
			if(pTmElement == nullptr) return false;
			for(auto pElement = pTmElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				string strName;
				pElement->QueryStringAttribute("name", &strName);
				wstring wstrName = utils::Utf8ToUnicode(strName);
				if(wstrName == oldModel.m_strModelName)
				{
					if(wstrName != newModel.m_strModelName)
					{
						string newName = utils::Unicode2Utf8(newModel.m_strModelName);
						pElement->SetAttribute("name", newName);
					}
					int nProductId = -1;
					pElement->QueryIntAttribute("product_id", &nProductId);
					if(nProductId != newModel.m_nProductId)
					{
						pElement->SetAttribute("product_id", newModel.m_nProductId);
					}
					// Delete original algorithm interfaces
					pElement->Clear();
					std::for_each(newModel.m_vItParam.begin(), newModel.m_vItParam.end(), [&](IT_PARAM& funcInfo)
					{
						TiXmlElement it("interface");
						string strIndex = utils::Unicode2Utf8(funcInfo.m_itIndex);
						it.SetAttribute("index", strIndex);
						string strName = utils::Unicode2Utf8(funcInfo.m_itName);
						it.SetAttribute("name", strName);
						if(!funcInfo.m_itNickname.empty())
						{
							string strNickname = utils::Unicode2Utf8(funcInfo.m_itNickname);
							it.SetAttribute("nickname", strNickname);
						}
						string strParam;
						int sz = (int)funcInfo.m_vParam.size();
						for(int i = 0; i < sz; i++)
						{
							string strToken = utils::Unicode2Utf8(funcInfo.m_vParam[i]);
							strParam.append(strToken);
							if(i != sz - 1)
								strParam.append(",");
						}
						it.SetAttribute("param", strParam);
						pElement->InsertEndChild(it);
					});
				}
			}
			config.SaveFile();
			ReloadConfigFile();
		}
		return true;
	}
	bool SynchronizeParamFromTask2Template(int chIndex, wstring wstrTaskName, wstring wstrModelName)
	{
		if(m_chSettings.find(chIndex) == m_chSettings.end())
		{
			return false;
		}

		if(!DeleteTaskTemplate(wstrModelName))
		{
			return false;
		}
		TASK_MODEL newModel;
		CHANNEL_CONFIG chCofig = m_chSettings[chIndex];
		TASK task = chCofig.GetTask(wstrTaskName);

		newModel.m_nProductId   = task.m_nProductId;
		newModel.m_strModelName = wstrModelName;
		newModel.m_vItParam     = task.m_vItParam;

		if(!AddTaskTemplate(newModel))
		{
			return false;
		}
		return true;
	}
	bool UpdateTask(int chIndex, TASK& task, bool bChangeCrtTask = false)
	{
		if(task.m_bSelfLearning)
		{
			return true;
		}
		// Step 1: delete the original algorithm group.
		bool ret = DeleteTask(chIndex, task.m_wstrTaskName,false, false);
		// Step 2: Add a new algorithm group
		ret = AddTask(chIndex, task);
		return ret;
	}
	bool UpdateSpcSettingSetting(int chIndex, SPC_SETTING* pSpcSetting)
	{
		if(pSpcSetting == nullptr) return false;
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					auto pSpcNode = pElement->FirstChildElement("SPC_Chart");
					if(pSpcNode == nullptr)  return false;

					auto pChildNode = pSpcNode->FirstChildElement("X_UNIT");
					if(pChildNode == nullptr) return false;
					string strValue = utils::Unicode2Utf8(pSpcSetting->m_strUnitX);
					pChildNode->FirstChild()->SetValue(strValue);
					// X axis length
					pChildNode = pSpcNode->FirstChildElement("X_LEN");
					if(pChildNode == nullptr) return false;
					string strLen = boost::lexical_cast<string>(pSpcSetting->m_nXLen);
					pChildNode->FirstChild()->SetValue(strLen);
					// Y axis unit
					pChildNode = pSpcNode->FirstChildElement("Y_UNIT");
					if(pChildNode == nullptr) return false;
					string strUnitY = utils::Unicode2Utf8(pSpcSetting->m_strUnitY);
					pChildNode->FirstChild()->SetValue(strUnitY);

					config.SaveFile();
				}
			}
			ReloadConfigFile();
			return true;
		}
		return false;
	}
	bool UpdateCrtTaskName(int chIndex, wstring& crtTaskName)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					auto pTaskGpNode = pElement->FirstChildElement("TaskGroup");
					if(pTaskGpNode == nullptr)  return false;
					if(!crtTaskName.empty())
					{
						string tmpName = utils::Unicode2Utf8(crtTaskName);
						pTaskGpNode->SetAttribute("crt_task_name",tmpName);
					}
					else
						pTaskGpNode->RemoveAttribute("crt_task_name");
					
				}
			}
			config.SaveFile();
		}
		return true;
	}
	bool UpdateErrorSetting(int chIndex,CString str)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					std::wstring wstr = str.GetString();
					string tmpwstr = utils::Unicode2Utf8(wstr);
					pElement->SetAttribute("warningflag",tmpwstr);
				}
			}
			config.SaveFile();
			return true;
		}
		return false;
	}
		

	bool UpdateTaskBlowThreshold(int chIndex,wstring Taskname, int newThreshold)
	{
		char buffer[20];
		_itoa(newThreshold,buffer, 10);
		string iBlowThreshold(buffer);

		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return false;
			std::string strChIndex = boost::lexical_cast<string>(chIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;
			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(chIndex == boost::lexical_cast<int>(index))
				{
					auto pTaskGpNode = pElement->FirstChildElement("TaskGroup");
					if(pTaskGpNode == nullptr)  return false;

					auto pGpArray = pTaskGpNode->ToElement();
					if(pGpArray == nullptr) return false;

					for(auto p = pGpArray->FirstChildElement(); p; p = p->NextSiblingElement())
					{
						std::string ret;
						p->QueryValueAttribute("name", &ret);
						wstring retName = utils::Utf8ToUnicode(ret);
						if(retName == Taskname)
 						{
 					//		string tmpName = utils::Unicode2Utf8(iBlowThreshold);
 							p->SetAttribute("blowthreshold",iBlowThreshold);
 						}
					}
				}
			}
			config.SaveFile();
		}
		return true;
	}

	bool CreateNewCrownCapTask(const wstring strTaskName, int bImgInside)
	{
		wstring strTaskNameCh1 = strTaskName + L"_CH1";
		wstring strTaskNameCh2 = strTaskName + L"_CH2";

		TASK_MODEL tmCh1, tmCh2;
		bool bModelOk_ch1 = false;
		bool bModelOk_ch2 = false;
		if(FALSE == bImgInside)
		{
			wstring wstrModelCh1 = L"内无图任务模板_CH1";
			if(m_modelSetting.find(wstrModelCh1) != m_modelSetting.end())
			{
				tmCh1 = m_modelSetting[wstrModelCh1];
				bModelOk_ch1 = true;
			}
		}
		else
		{
			wstring wstrModelCh1 = L"内有图任务模板_CH1";
			if(m_modelSetting.find(wstrModelCh1) != m_modelSetting.end())
			{
				tmCh1 = m_modelSetting[wstrModelCh1];
				bModelOk_ch1 = true;
			}
		}

		wstring wstrModelCh2 = L"瓶盖外面检测模板_CH2";
		if(m_modelSetting.find(wstrModelCh2) != m_modelSetting.end())
		{
			tmCh2 = m_modelSetting[wstrModelCh2];
			bModelOk_ch2 = true;
		}
		bool bRet = true;
		if(bModelOk_ch1 && bModelOk_ch2)
		{
			//int retIndex1 = -1;
			//int retIndex2 = -1;
			//if(tmCh1.NeedLoadSelfLearningResult(retIndex1))
			//{
			//	tmCh1.ReviseSelfLearningParameter(retIndex1, strTaskNameCh1);
			//}
			//if(tmCh2.NeedLoadSelfLearningResult(retIndex2))
			//{
			//	tmCh2.ReviseSelfLearningParameter(retIndex2, strTaskNameCh2);
			//}

			//TASK task_ch1 = GenerateTaskFromModel(strTaskNameCh1, tmCh1, (bImgInside == 1));
			//bRet = AddTask(1, task_ch1);
			//if(!bRet) return false;

			//TASK task_ch2 = GenerateTaskFromModel(strTaskNameCh2, tmCh2);
			////TODO:
			//bRet = AddTask(2, task_ch2);
			//if(!bRet) return false;
		}
		return true;
	}
	bool DeleteCrownCapTask(const wstring& strTaskName)
	{
		wstring strNameCh1 = strTaskName + L"_CH1";
		wstring strNameCh2 = strTaskName + L"_CH2";

		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return false ;
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;

			TiXmlElement* pTarget1 = nullptr;
			TiXmlElement* pTarget2 = nullptr;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				auto pTaskGpNode = pElement->FirstChildElement("TaskGroup");
				if(pTaskGpNode == nullptr)  return false;
				string crtTaskName, newCrtTaskName;
				pTaskGpNode->QueryValueAttribute("crt_task_name", &crtTaskName);
				wstring wCrtTaskName = utils::Utf8ToUnicode(crtTaskName);

				auto pGpArray = pTaskGpNode->ToElement();
				if(pGpArray == nullptr) return false;

				for(auto p = pGpArray->FirstChildElement(); p; p = p->NextSiblingElement())
				{
					string ret;
					p->QueryValueAttribute("name", &ret);
					wstring retName = utils::Utf8ToUnicode(ret);
					if(retName == strNameCh1)
					{
						pTarget1 = p;
						if(wCrtTaskName == strNameCh1)
						{
							pTaskGpNode->RemoveAttribute("crt_task_name");
						}
					}
					if(retName == strNameCh2)
					{
						pTarget2 = p;
						if(wCrtTaskName == strNameCh2)
						{
							pTaskGpNode->RemoveAttribute("crt_task_name");
						}
					}
				}

				if(pTarget1 != nullptr)
				{
					pTaskGpNode->RemoveChild(pTarget1);
					pTarget1 = nullptr;
				}
				if(pTarget2 != nullptr)
				{
					pTaskGpNode->RemoveChild(pTarget2);
					pTarget2 = nullptr;
				}
			}
			config.SaveFile();
			ReloadConfigFile();
			return true;
		}
		return false;
	}
	bool RetrainningTask(const wstring& strTaskName, int bImgInside)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return false ;
	
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return false;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return false;

			int chIndex;
			TiXmlElement* pTaskGpNode = nullptr;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryIntAttribute("index",&chIndex);
				pTaskGpNode = pElement->FirstChildElement("TaskGroup");

				wstring strName;
				if(chIndex == 1)
				{
					strName = strTaskName + L"_CH1";
					if(bImgInside == 1)
						UpdateChannelSelfLearningParam(pTaskGpNode, strName);
					else
					{
						string taskName = utils::Unicode2Utf8(strName);
						pTaskGpNode->SetAttribute("crt_task_name", taskName);
					}
				}
				if(chIndex == 2)
				{
					strName = strTaskName + L"_CH2";
					UpdateChannelSelfLearningParam(pTaskGpNode, strName);
				}
			}
			config.SaveFile();
			ReloadConfigFile();
		}
		return true;
	}
	bool IsTaskInused(const wstring& wstrTaskName)
	{
		if(m_chSettings.empty())
			return false;
		for(auto it = m_chSettings.begin(); it != m_chSettings.end(); it++)
		{
			CHANNEL_CONFIG chConfig = it->second;
			std::vector<wstring> vToken = utils::SplitString(chConfig.m_crtTaskName, L"_");
			if(!vToken.empty() && vToken[0] == wstrTaskName)
				return true;
		}
		return false;
	}
	bool IsTaskNameExisted(const wstring& wstrTaskName)
	{
		if(m_chSettings.empty())
			return false;
		std::vector<wstring> vToken;
		for(auto it = m_chSettings.begin(); it != m_chSettings.end(); it++)
		{
			CHANNEL_CONFIG chConfig = it->second;
			for(auto t = chConfig.m_vTask.begin(); t != chConfig.m_vTask.end(); t++)
			{
				vToken.clear();
				TASK task = *t;
				vToken = utils::SplitString(task.m_wstrTaskName, L"_");
				if(!vToken.empty() && vToken[0] == wstrTaskName)
					return true;
			}
		}
		return false;
	}
	bool IsTaskModelExisted(const wstring& wstrModelName)
	{
		if(m_modelSetting.empty())
			return false;
		for(auto it = m_modelSetting.begin(); it != m_modelSetting.end(); it++)
		{
			if(it->second.m_strModelName == wstrModelName)
				return true;
		}
		return false;
	}
	wstring GetModelNameOfThisTask(int nChIndex, wstring wstrTaskName)
	{
		wstring wstrRet(L"");
		if(m_chSettings.find(nChIndex) != m_chSettings.end())
		{
			CHANNEL_CONFIG chConfig = m_chSettings[nChIndex];

			for(auto it = chConfig.m_vTask.begin(); it != chConfig.m_vTask.end(); it++)
			{
				if(wstrTaskName == it->m_wstrTaskName)
				{
					wstrRet = it->m_strModelName;
				}
			}
		}
		return wstrRet;
	}
	bool GetTask(int nChIndex, wstring wstrTaskName, TASK& retTask)
	{
		if(m_chSettings.find(nChIndex) != m_chSettings.end())
		{
			CHANNEL_CONFIG chConfig = m_chSettings[nChIndex];
			for(auto it = chConfig.m_vTask.begin(); it != chConfig.m_vTask.end(); it++)
			{
				if(wstrTaskName == it->m_wstrTaskName)
				{
					retTask = *it;
					return true;
				}
			}
		}
		return false;
	}
	bool AddNewShift(const SHIFT& newShift)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return false ;
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pShiftNode = pNode->FirstChild("ShiftSetting");
			if(pShiftNode == nullptr) return false;

			TiXmlElement shiftNode("shift");
			string strShiftName = utils::Unicode2Utf8(newShift.m_wstrShiftName);
			shiftNode.SetAttribute("name", strShiftName);
			shiftNode.SetAttribute("bgn_time", newShift.m_bgnTime);
			shiftNode.SetAttribute("end_time", newShift.m_endTime);

			pShiftNode->InsertEndChild(shiftNode);
			config.SaveFile();
			ReloadConfigFile();
			return true;
		}
		return false;
	}
	bool UpdateShift(const SHIFT& oldShift, const SHIFT& newShift)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return false ;
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pShiftNode = pNode->FirstChild("ShiftSetting");
			if(pShiftNode == nullptr)
				return false;

			auto pShiftElement = pShiftNode->ToElement();
			if(pShiftElement == nullptr)
				return false;
			wstring wstrShiftName = oldShift.m_wstrShiftName;
			for(auto it = pShiftElement->FirstChildElement(); it; it = it->NextSiblingElement())
			{
				string strName;
				it->QueryStringAttribute("name", &strName);
				wstring wstrName = utils::Utf8ToUnicode(strName);
				if(wstrName == wstrShiftName)
				{
					string strNewShiftName = utils::Unicode2Utf8(newShift.m_wstrShiftName);
					it->SetAttribute("name", strNewShiftName.c_str());
					it->SetAttribute("bgn_time", newShift.m_bgnTime);
					it->SetAttribute("end_time", newShift.m_endTime);
				}
			}
			config.SaveFile();
			ReloadConfigFile();
			return true;
		}
		return false;
	}
	bool DeleteShift(const wstring& wstrShiftName)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return false ;
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return false;
			auto pShiftNode = pNode->FirstChild("ShiftSetting");
			if(pShiftNode == nullptr) return false;

			auto pShiftElement = pShiftNode->ToElement();
			if(pShiftElement == nullptr) return false;
			TiXmlElement* pTarget = nullptr;
			for(auto it = pShiftElement->FirstChildElement(); it; it = it->NextSiblingElement())
			{
				string strName;
				it->QueryStringAttribute("name", &strName);
				wstring wstrName = utils::Utf8ToUnicode(strName);
				if(wstrName == wstrShiftName)
				{
					pTarget = it;
				}
			}
			pShiftNode->RemoveChild(pTarget);
			config.SaveFile();
			ReloadConfigFile();
			return true;
		}
		return false;
	}
	void SaveResetShiftStatsFlag()
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) 
				return;
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) 
				return;

			auto pShiftNode = pNode->FirstChild("ShiftSetting");
			if(pShiftNode == nullptr)
				return;

			auto pShiftElement = pShiftNode->ToElement();
			if(pShiftElement == nullptr)
				return;

			string strFlag = (m_bAutoResetShiftStats ? "true" : "false");

			pShiftElement->SetAttribute("auto_reset",strFlag);
			config.SaveFile();
		}
	}
	void UpateSelfLearningParam(CHANNEL_CONFIG& chConfig, const wstring& wstrParam)
	{
		int nChIndex         = chConfig.m_chIndex;
		wstring wstrTaskName = chConfig.m_crtTaskName;
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile()) return ;
			std::string strChIndex = boost::lexical_cast<string>(nChIndex);
			TiXmlNode* pNode = config.FirstChild("configuration");
			if(pNode == nullptr) return ;
			auto pPS = pNode->FirstChild("PanelSetting");
			if(pPS == nullptr) return ;

			TiXmlElement* pPSE = pPS->ToElement();
			if(pPSE == nullptr) return ;

			std::string index;
			for(auto pElement = pPSE->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("index",&index);
				if(nChIndex == boost::lexical_cast<int>(index))
				{
					auto pTaskGpNode = pElement->FirstChildElement("TaskGroup");
					if(pTaskGpNode == nullptr)  return;

					auto pGpArray = pTaskGpNode->ToElement();
					if(pGpArray == nullptr) return;

					string strTaskName = utils::Unicode2Utf8(wstrTaskName);
					for(auto p = pGpArray->FirstChildElement(); p; p = p->NextSiblingElement())
					{
						string strName;
						p->QueryValueAttribute("name", &strName);
						if(strName == strTaskName)
						{
							string strParam("");
							p->QueryStringAttribute("param", &strParam);
							if(strParam != "")
							{
								std::vector<wstring> vNewParam = utils::SplitString(wstrParam, L",");
								std::vector<string> vOldParam = utils::SplitString(strParam, ",");
								std::string strNewParam;
								size_t sz = vOldParam.size();
								for(size_t t = 0; t < sz; t++)
								{
									string strNewValue = utils::Unicode2Utf8(vNewParam[t]);
									size_t pos = vOldParam[t].find("@");
									vOldParam[t].replace(pos + 1,string::npos, strNewValue); 
									strNewParam.append(vOldParam[t]);
									if(t != sz - 1)
									{
										strNewParam.append(",");
									}
								}
								strNewParam = strNewParam.substr(0, strNewParam.size() - 1);
								p->SetAttribute("param",strNewParam);
								TASK* pSlTask = chConfig.GetCurrentTask();
								if(pSlTask != nullptr && pSlTask->m_bSelfLearning)
								{
									pSlTask->m_wstrSlParam = utils::Utf8ToUnicode(strNewParam);
								}
				                config.SaveFile();
							    ReloadConfigFile();
							}
						}
					}
				}
			}
		}
	}
	bool CopyModel(wstring wstrSrcName, wstring wstrNewName)
	{
		if(m_modelSetting.find(wstrSrcName) == m_modelSetting.end())
		{
			return false;
		}
		TASK_MODEL tm = m_modelSetting[wstrSrcName];
		tm.m_strModelName = wstrNewName;

		if(!AddTaskTemplate(tm))
		{
			return false;
		}
		return true;
	}
	std::set<wstring> GetDependencyModel(TASK crtTask)
	{
		std::set<wstring> retSet;
		size_t sz = crtTask.m_vItParam.size();
		for(size_t t = 0; t < sz; t++)
		{
			IT_PARAM itParam = crtTask.m_vItParam[t];
			size_t tmpSz = itParam.m_vParam.size();
			for(size_t i = 0; i < tmpSz; i++)
			{
				wstring wstrParam = itParam.m_vParam[i];
				if(wstrParam.find(L"\\") != wstring::npos)
				{
// 					size_t pos = wstrParam.find_last_of(L"\\");
// 					wstring wstrPath = wstrParam.substr(0, pos);
					retSet.insert(wstrParam);
				}
			}
		}

		return retSet;
	}
	bool CopyDependencyDirectory(wstring wstrSrcChTaskName, std::set<wstring> srcModelPath, wstring wstrNewChTaskName)
	{
		if (!srcModelPath.empty())
		{
			for(auto it = srcModelPath.begin(); it != srcModelPath.end(); it++)
			{
				wstring wstrSrcModelPath = *it;
				size_t pos = wstrSrcModelPath.find(wstrSrcChTaskName);
				if(pos != wstring::npos)
				{
					//是否当前任务目录文件
					bool b_CurDirFile = false; 
					wstring strName =  wstrSrcModelPath.substr(pos+wstrSrcChTaskName.length()+1);
					size_t posFile = strName.find_last_of(L"\\");
					if (posFile == wstring::npos)
					{
						b_CurDirFile = true;
					}

					if (!b_CurDirFile)
					{
						size_t pos = wstrSrcModelPath.find_last_of(L"\\");
						wstrSrcModelPath = wstrSrcModelPath.substr(0, pos);
					}

					wstring wstrNewModelPath = wstrSrcModelPath;

					size_t index = wstrNewModelPath.find_last_of(_T("\\"));
					wstrNewModelPath = wstrNewModelPath.substr(0, index);

					wstrNewModelPath.replace(pos, wstrSrcChTaskName.size(),wstrNewChTaskName);

					if(utils::CheckFileExists(wstrSrcModelPath) && utils::CreateFullDirectory(wstrNewModelPath))
					{
						if(!utils::CopyFiles(wstrSrcModelPath, wstrNewModelPath))
						{
							return false;
						}
					}

					//复制任务功能(多相机工位扩展)
					for (int i = 0 ; i < 6 ;i++)
					{
						CString strtmp;
						strtmp.Format(_T("#%d"),i);
						wstring NewwstrSrcModelPath = wstrSrcModelPath +strtmp.GetString();
				
						if(utils::CheckFileExists(NewwstrSrcModelPath) && utils::CreateFullDirectory(wstrNewModelPath))
						{
							if(!utils::CopyFiles(NewwstrSrcModelPath, wstrNewModelPath))
							{
								return false;
							}
						}
					}
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			wstring exepath;
			utils::GetExePath(exepath);
			wstring CopyTaskpath =  exepath +L"\\data\\"+ wstrNewChTaskName;
			wstring srcTaskpath =  exepath +L"\\data\\"+ wstrSrcChTaskName;
			if(utils::CheckFileExists(srcTaskpath) &&utils::CreateFullDirectory(CopyTaskpath))
			{	
				utils::CopyFiles(srcTaskpath+L"\\CheckerBoardImg.tiff", CopyTaskpath);
				utils::CopyFiles(srcTaskpath+L"\\Map.tiff", CopyTaskpath);
				utils::CopyFiles(srcTaskpath+L"\\MulImg.tiff", CopyTaskpath);
				utils::CopyFiles(srcTaskpath+L"\\MapOrigin.tiff", CopyTaskpath);
			}

			//CAN自学习复制任务功能(多相机工位扩展)
			for (int i = 0 ; i < 6 ;i++)
			{
				CopyTaskpath =  exepath +L"\\data\\"+ wstrNewChTaskName;
				srcTaskpath =  exepath +L"\\data\\"+ wstrSrcChTaskName;

				CString strtmp;
				strtmp.Format(_T("%d"),i);
				int posDes = CopyTaskpath.find_last_of(_T("\\"));
				CopyTaskpath.insert(posDes,strtmp.GetString());
				int posSrc =  srcTaskpath.find_last_of(_T("\\"));
				srcTaskpath.insert(posSrc,strtmp.GetString());

				if(utils::CheckFileExists(srcTaskpath) && utils::CreateFullDirectory(CopyTaskpath))
				{	
					utils::CopyFiles(srcTaskpath+L"\\CheckerBoardImg.tiff", CopyTaskpath);
					utils::CopyFiles(srcTaskpath+L"\\Map.tiff", CopyTaskpath);
					utils::CopyFiles(srcTaskpath+L"\\MulImg.tiff", CopyTaskpath);
					utils::CopyFiles(srcTaskpath+L"\\MapOrigin.tiff", CopyTaskpath);
				}
			}
		}
		return true;
	}
	TASK ReplaceDependencyFiles(TASK crtTask, wstring wstrSrcChTaskName, wstring wstrNewChTaskName)
	{
		TASK retTask = crtTask;

		size_t sz = retTask.m_vItParam.size();
		for(size_t t = 0; t < sz; t++)
		{
			IT_PARAM& itParam = retTask.m_vItParam[t];
			size_t len = itParam.m_vParam.size();
			for(size_t i = 0; i < len; i++)
			{
				wstring wstrParam = itParam.m_vParam[i];
				auto it = wstrParam.find(wstrSrcChTaskName);
				if(it != wstring::npos)
				{
					itParam.m_vParam[i].replace(it, wstrSrcChTaskName.size(), wstrNewChTaskName);
				}
			}
		}

		return retTask;
	}
	bool DuplicateTask(int nCrtChIndex, wstring wstrSrcTaskName, std::map<int, wstring> dictTarget)
	{
		if(dictTarget.empty())	
		{
			return false;
		}
		// Delete Task
		for(auto it = dictTarget.begin(); it != dictTarget.end(); it++)
		{
			int nIndex = it->first;
			wstring wstrTaskName = it->second;
			DeleteTask(nIndex, wstrTaskName,false, false);
		}

		wstring wstrSrcChTaskName = L"CH";
		wstrSrcChTaskName.append(boost::lexical_cast<wstring>(nCrtChIndex));
		wstrSrcChTaskName.append(L"\\");
		wstrSrcChTaskName.append(wstrSrcTaskName);

		TASK retTask;
		if(GetTask(nCrtChIndex,wstrSrcTaskName,retTask))
		{
			std::set<wstring> retModelPath = GetDependencyModel(retTask);
			for(auto it = dictTarget.begin(); it != dictTarget.end(); it++)
			{
				int nIndex = it->first;
				wstring wstrTaskName = it->second;

				wstring wstrNewChTaskName = L"CH";
				wstrNewChTaskName.append(boost::lexical_cast<wstring>(nIndex));
				wstrNewChTaskName.append(L"\\");
				wstrNewChTaskName.append(wstrTaskName);

				if(CopyDependencyDirectory(wstrSrcChTaskName, retModelPath, wstrNewChTaskName))
				{
					TASK newTask = ReplaceDependencyFiles(retTask, wstrSrcChTaskName, wstrNewChTaskName); 
					newTask.m_wstrTaskName = wstrTaskName;

					if(!AddTask(nIndex, newTask, false))
					{
						return false;
					}
					if(nIndex == nCrtChIndex) // 
					{
						if(!CreateLigthControllerParam(nIndex, newTask.m_wstrTaskName))
						{
							//return false;
						}
						if(!CopyLightControllerParam(nIndex,wstrSrcTaskName,newTask.m_wstrTaskName))
						{
							
							//return false;
						}
					}
				}
				else
				{
					return false;
				}
			}
		}
		ReloadConfigFile();
		return true;
	}
	bool CheckTaskNameUniqueness(wstring wstrTaskName)
	{
		if(m_setTaskName.empty())
		{
			return true;
		}
		boost::algorithm::to_lower(wstrTaskName);
		return (m_setTaskName.find(wstrTaskName) == m_setTaskName.end());
	}
	bool SaveLaneName(int chIndex, wstring wstrLaneName)
	{
		if(CheckFileExists(m_configFilePath))
		{
			USES_CONVERSION;
			string strConfigFilePath = utils::Unicode2Utf8(m_configFilePath);
			TiXmlDocument config(strConfigFilePath);
			if(!config.LoadFile())
				return false;

			TiXmlNode* node = config.FirstChild("configuration");
			if(node == nullptr)
				return false;

			TiXmlNode* pPanelNode = node->FirstChild("PanelSetting");
			if(pPanelNode != nullptr)
			{
				TiXmlElement* pPanelSettingElement = pPanelNode->ToElement();
				if(pPanelSettingElement != nullptr)
				{
					std::string strIndex, strProductID, strMaxSpeed;
					for(auto pElement = pPanelSettingElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
					{
						pElement->QueryValueAttribute("index",&strIndex);
						if(!strIndex.empty())
						{
							int crtIndex = boost::lexical_cast<int>(strIndex);
							if(crtIndex == chIndex)
							{
								string strLaneName = utils::Unicode2Utf8(wstrLaneName);
								pElement->SetAttribute("lane_name",strLaneName);
								config.SaveFile();
								return true;
							}
						}
					}
				}
			}
		}
		return false;
	}
	int GetChannelMaxSpeed(int nChIndex)
	{
		return m_chSettings[nChIndex].m_nMaxSpeed;
	}
private:
	static std::wstring GetConfigFilePath()
	{
		TCHAR  moduleFileName[MAX_PATH];  
		::GetModuleFileName(NULL,moduleFileName, MAX_PATH);
		wstring filePath(moduleFileName);
		filePath.append(L".config");
		return filePath;
	}
	void ParseBasicSettings(TiXmlNode* node)
	{
		// basic setting
		TiXmlNode* basicNode = node->FirstChild("BasicSetting");
		if(basicNode == nullptr)
			return;

		string key;
		string strValue;
		TiXmlElement* pAppElement = basicNode->ToElement();
		if(pAppElement != nullptr)
		{
			for(TiXmlElement* pElement = pAppElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("key",&key);
				pElement->QueryValueAttribute("value",&strValue);
				if(!key.empty() && !strValue.empty())
				{
					wstring wsKey   = utils::Utf8ToUnicode(key);
					wstring wsValue = utils::Utf8ToUnicode(strValue);
					m_commonSetting[wsKey] = wsValue;
				}
			}
			m_vProductIDs.clear();
			if(m_commonSetting.find(L"PRODUCT") != m_commonSetting.end())
			{
				wstring strProductIDs = m_commonSetting[L"PRODUCT"];
				if(strProductIDs.find(L";") == wstring::npos)
				{
					m_vProductIDs.push_back(boost::lexical_cast<int>(strProductIDs));
				}
				else
				{
					std::vector<wstring> vStrIDs = utils::SplitString(strProductIDs, L";");
					for(size_t t = 0 ; t < vStrIDs.size(); t++)
					{
						m_vProductIDs.push_back(boost::lexical_cast<int>(vStrIDs[t]));
					}
				}
			}
		}
	}
	void ParseShiftSettings(TiXmlNode* node)
	{
		TiXmlNode* pShiftNode = node->FirstChild("ShiftSetting");
		if(pShiftNode == nullptr)
			return;

		TiXmlElement* pRootElement = pShiftNode->ToElement();
		if(pRootElement)
		{
			pRootElement->QueryBoolAttribute("auto_reset",&m_bAutoResetShiftStats);
		}
		string strShiftName;
		long   nBgnTime = 0;
		long   nEndTime = 0;
		TiXmlElement* pAppElement = pShiftNode->ToElement();
		if(pAppElement != nullptr)
		{
			for(TiXmlElement* pElement = pAppElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
			{
				pElement->QueryValueAttribute("name",&strShiftName);
				pElement->QueryValueAttribute("bgn_time",&nBgnTime);
				pElement->QueryValueAttribute("end_time",&nEndTime);
				if(!strShiftName.empty())
				{
					wstring wstrShiftName   = utils::Utf8ToUnicode(strShiftName);
					SHIFT shift;
					shift.m_wstrShiftName = wstrShiftName;
					shift.m_bgnTime = nBgnTime;
					shift.m_endTime = nEndTime;
					m_vShift.push_back(shift);
				}
			}
		}
	}
	void ParseCameraSetting(CHANNEL_CONFIG& config,TiXmlElement* pRoot)
	{
		config.m_vecCameraSetting.clear();
		CAMERA_SETTING cameraSetting;

		for(auto pCameraElement = pRoot->FirstChildElement("Camera"); pCameraElement; pCameraElement = pCameraElement->NextSiblingElement("Camera"))
		{
			cameraSetting.m_chIndex = config.m_chIndex;
			auto pNode = pCameraElement->FirstChildElement("Name");


			if(pNode != nullptr)
			{
				cameraSetting.m_name = utils::Utf8ToUnicode(pNode->FirstChild()->Value());
			}
			pNode = pCameraElement->FirstChildElement("TriggerMode");
			if(pNode != nullptr)
			{
				bool bTriggerMode = true;
				std::string strValue = pNode->FirstChild()->Value();
				if(strValue == "N")
					bTriggerMode = false;
				cameraSetting.m_bTriggerMode = bTriggerMode;
			}
			ReadChildNode(pCameraElement,"FrameRate",cameraSetting.m_nFrameRate);

			pNode = pCameraElement->FirstChildElement("TriggerActivation");
			if(pNode != nullptr)
			{
				std::string strRet = pNode->FirstChild()->Value();
				if(strRet == "FALL")
					cameraSetting.m_triggerAct = FALL_EDGE;
				else
					cameraSetting.m_triggerAct = RISE_EDGE;
			}

			ReadChildNode(pCameraElement,"Gain",cameraSetting.m_gain);
			ReadChildNode(pCameraElement,"ExposureTime",cameraSetting.m_ExposureTime);
			//default Exposure Time of Camera
			if (cameraSetting.m_ExposureTime == 0)
			{
				cameraSetting.m_ExposureTime = 100;
			}
			int nDelayType = 0;
			ReadChildNode(pCameraElement,"DelayType",nDelayType);
			cameraSetting.m_delayType = (eDELAY_TYPE)nDelayType;
			ReadChildNode(pCameraElement,"DelayAbs",cameraSetting.m_delayValue);
			ReadChildNode(pCameraElement,"GevSCPSPacketSize",cameraSetting.m_nPacketSz);
			ReadChildNode(pCameraElement,"DropFrameUcl",cameraSetting.m_nDroppedImgUcl);

			ReadChildNode(pCameraElement,"LineSelector",cameraSetting.m_LineSelector);
			ReadChildNode(pCameraElement,"LineSource",cameraSetting.m_LineSource);

			config.m_vecCameraSetting.push_back(cameraSetting);
			config.m_cameraSetting = config.m_vecCameraSetting[0];
		}
		if(config.m_vecCameraSetting.size() == 1)
		{
			config.m_bSingleProcessor = true;
		}
	}
	void ParseLightCtrlSetting(CHANNEL_CONFIG& config, TiXmlElement* pRoot)
	{
		config.m_lightCtrlSetting.m_nChIndex = config.m_chIndex;
		TiXmlElement* pLightElement = pRoot->FirstChildElement("Light");
		if(pLightElement != nullptr)
		{
			std::string strIDs;
			pLightElement->QueryStringAttribute("id", &strIDs);
			if(!strIDs.empty())
			{
				std::vector<string> vStrID = utils::SplitString(strIDs, ",");
				for(size_t t = 0; t < vStrID.size(); t++)
				{
					int nID = boost::lexical_cast<int>(vStrID[t]);
					config.m_lightCtrlSetting.m_vLcID.push_back(nID);
				}
			}

			std::string strTags;
			pLightElement->QueryStringAttribute("tag", &strTags);
			if(!strTags.empty())
			{
				std::vector<string> vTags = utils::SplitString(strTags, ",");
				for(size_t t = 0; t < vTags.size(); t++)
				{
					wstring wstrTag = utils::Utf8ToUnicode(vTags[t]);
					config.m_lightCtrlSetting.m_vTags.push_back(wstrTag);
				}
			}
			auto pParamGroup = pLightElement->ToElement();
			if(pParamGroup != nullptr)
			{
				for(auto it = pParamGroup->FirstChildElement(); it; it = it->NextSiblingElement())
				{
					string strGroupName;
					it->QueryStringAttribute("name",&strGroupName);
					wstring wstrGpName = utils::Utf8ToUnicode(strGroupName);
					std::vector<LC> vLcParamGp;
					auto pLcParam = it->ToElement();
					if(pLcParam != nullptr)
					{
						int CHid = 0;
						for(auto itParam = pLcParam->FirstChildElement(); itParam; itParam = itParam->NextSiblingElement())
						{
							LC lcParam;
							lcParam.m_vChParam.clear();
							itParam->QueryIntAttribute("id", (int*)&lcParam.m_nID);
							itParam->QueryIntAttribute("mode", (int*)&lcParam.m_nMode);
							string strValue;
							itParam->QueryStringAttribute("value", &strValue);
							if(!strValue.empty())
							{
								std::vector<string> vChValue = utils::SplitString(strValue,";");
								std::vector<wstring> vTags = utils::SplitString(config.m_lightCtrlSetting.m_vTags[CHid++], L"$",token_compress_off,false);
								if(vChValue.size() == 4)
								{
									for(size_t t = 0; t < 4; t++)
									{
 										if (vTags[t]!=L"")
										{
 											lcParam.m_nActiveID +=(1<<(3-t));
 										}
				
										LC_CH_PARAM chParam;
										std::vector<string> vValue = utils::SplitString(vChValue[t], "$");
										if(vValue.size() == 3)
										{
											chParam.m_nLum      = boost::lexical_cast<unsigned int>(vValue[0]);
											chParam.m_nDelay    = boost::lexical_cast<unsigned int>(vValue[1]);
											chParam.m_nDuration = boost::lexical_cast<unsigned int>(vValue[2]);
											lcParam.m_vChParam.push_back(chParam);
										}
									}
								}

								if(vChValue.size() == 8)
								{
									LC_CH_PARAM chParam[4];
									for(size_t t = 0; t < 4; t++)
									{
										if (vTags[t]!=L"")
										{
											lcParam.m_nActiveID +=(1<<(3-t));
										}
										std::vector<string> vValue = utils::SplitString(vChValue[t], "$");
										if(vValue.size() == 3)
										{
											chParam[t].m_nLum      = boost::lexical_cast<unsigned int>(vValue[0]);
											chParam[t].m_nDelay    = boost::lexical_cast<unsigned int>(vValue[1]);
											chParam[t].m_nDuration = boost::lexical_cast<unsigned int>(vValue[2]);
										}
									}

									for(size_t t = 4; t < 8; t++)
									{
										std::vector<string> vValue = utils::SplitString(vChValue[t], "$");
										if(vValue.size() == 3)
										{
											chParam[t-4].m_nLum1      = boost::lexical_cast<unsigned int>(vValue[0]);
											chParam[t-4].m_nDelay1    = boost::lexical_cast<unsigned int>(vValue[1]);
											chParam[t-4].m_nDuration1 = boost::lexical_cast<unsigned int>(vValue[2]);
											lcParam.m_vChParam.push_back(chParam[t-4]);
										}
			
									}
								} 
										
							}
							vLcParamGp.push_back(lcParam);
						}
						config.m_lightCtrlSetting.m_dictParamGroup[wstrGpName] = vLcParamGp;
					}
				}
			}
		}
	}
	void ParseRejectorSetting(CHANNEL_CONFIG& config, TiXmlElement* pRoot)
	{
		TiXmlElement* pIoRoot = pRoot->FirstChildElement("Rejector");
		if(pIoRoot != nullptr)
		{
			wstring value;
			auto pNode = pIoRoot->FirstChildElement("DeviceIP");
			if(pNode != nullptr)
			{
				value = utils::Utf8ToUnicode(pNode->FirstChild()->Value());
			}
			else
			{
				value = L"192.168.200.";
				int nAppend = 159 + config.m_chIndex;
				value.append(boost::lexical_cast<wstring>(nAppend));
			}
			config.m_rejectorSetting.m_wstrDeviceIp = value;
			ReadChildNode(pIoRoot, "RejectorPort",config.m_rejectorSetting.m_nRejectPort);
			ReadChildNode(pIoRoot, "DropDetectPort",config.m_rejectorSetting.m_nDropImgPort);

			ReadChildNode(pIoRoot,"CameraGrabTime",config.m_rejectorSetting.m_CameraGrabTime);
			ReadChildNode(pIoRoot,"SignalIngoreTime",config.m_rejectorSetting.m_SignalIngoreTime);			
			ReadChildNode(pIoRoot,"ContinueRejectAlarm",config.m_rejectorSetting.m_nContinueRejectAlarm);
			ReadChildNode(pIoRoot,"TriggerIngoreTime",config.m_rejectorSetting.m_TriggerIngoreTime);
			ReadChildNode(pIoRoot,"TriggerLightWidth",config.m_rejectorSetting.m_TriggerLightWidth);
			

			pNode = pIoRoot->FirstChildElement("Mode");
			if(pNode != nullptr)
			{
				value = utils::Utf8ToUnicode(pNode->FirstChild()->Value());
				eDELAY_TYPE mode = DELAY_DISTANCE;
				if(value == L"T")
					mode = DELAY_TIME;
				config.m_rejectorSetting.m_mode = mode;
			}
			ReadChildNode(pIoRoot, "Delay",config.m_rejectorSetting.m_delay);
			ReadChildNode(pIoRoot, "SignalWidth",config.m_rejectorSetting.m_signalWidth);
			//ReRejector.

			string retStr;
			pNode = pIoRoot->FirstChildElement("ReRejectFlag");
			if(pNode != nullptr)
			{
				retStr = pNode->FirstChild()->Value();
				config.m_rejectorSetting.m_bReReject = (retStr == "Y");
			}

			pNode = pIoRoot->FirstChildElement("ReRejectFunc");
			if(pNode != nullptr)
			{
				retStr = pNode->FirstChild()->Value();
				config.m_rejectorSetting.m_bReRejectFunc = (retStr == "Y");
			}

			ReadChildNode(pIoRoot, "ReSiToSol",config.m_rejectorSetting.m_nReSitoSol);
			ReadChildNode(pIoRoot, "ReDelay",config.m_rejectorSetting.m_nReDelay);
			ReadChildNode(pIoRoot, "ReSignalWidth",config.m_rejectorSetting.m_nReSignalWidth);

			// Rejector Test
			pNode = pIoRoot->FirstChildElement("TestFlag");
			if(pNode != nullptr)
			{
				retStr = pNode->FirstChild()->Value();
				config.m_rejectorSetting.m_bTestRejector = (retStr == "Y");
			}
			ReadChildNode(pIoRoot, "TestMode",config.m_rejectorSetting.m_nTestMode);
			ReadChildNode(pIoRoot, "RejectCount",config.m_rejectorSetting.m_nNumerator);
			ReadChildNode(pIoRoot, "RejectInterval",config.m_rejectorSetting.m_nDenominator);
			int nSA = 0;
			ReadChildNode(pIoRoot, "CDC_SA",nSA);
			config.m_rejectorSetting.m_sa = (eSOLENOID_ACTION)nSA;
			ReadChildNode(pIoRoot, "CDC",config.m_rejectorSetting.m_nCDC);

            // Last N
            int nLastN = 5000;
            ReadChildNode(pIoRoot,"LastN",nLastN);
            config.m_rejectorSetting.m_nLastN = nLastN;
            string strLastNRatio = "";
            ReadChildNode(pIoRoot,"LastNRatioUCL",strLastNRatio);
            if(strLastNRatio != "")
            {
                config.m_rejectorSetting.m_fLastNRatio = boost::lexical_cast<float>(strLastNRatio);
            }
            int nFlag = 0;
            ReadChildNode(pIoRoot,"LastNAlarmFlag",nFlag);
            config.m_rejectorSetting.m_bEnableLastNAlarm = (nFlag == 1);
		}
	}
	void ParseTaskSetting(CHANNEL_CONFIG& config, TiXmlElement* pRoot)
	{
		auto pTaskGpNode = pRoot->FirstChildElement("TaskGroup");
		
		if(pTaskGpNode != nullptr)
		{
			string strCrtTaskName;
			pTaskGpNode->QueryValueAttribute("crt_task_name",&strCrtTaskName);
			if(!strCrtTaskName.empty())
			{
				config.m_crtTaskName = utils::Utf8ToUnicode(strCrtTaskName);
			}
		}
		for(auto pTaskNode = pTaskGpNode->FirstChildElement(); pTaskNode; pTaskNode = pTaskNode->NextSiblingElement())
		{
			std::string taskName;
			pTaskNode->QueryValueAttribute("name",&taskName);
			TASK task;
			task.m_wstrTaskName = utils::Utf8ToUnicode(taskName);
			wstring wstrTmpName = task.m_wstrTaskName;
			boost::algorithm::to_lower(wstrTmpName);
			m_setTaskName.insert(wstrTmpName);

			std::string strModelName("");
			pTaskNode->QueryStringAttribute("model_name", &strModelName);
			if(!strModelName.empty())
			{
				task.m_strModelName  = utils::Utf8ToUnicode(strModelName);
			}

			int nTimingUcl = 80;
			pTaskNode->QueryIntAttribute("timing_ucl", &nTimingUcl);
			task.m_nTimingUcl  = nTimingUcl;

			std::string strProductId("");
			pTaskNode->QueryValueAttribute("product_id", &strProductId);
			wstring wstrId = utils::Utf8ToUnicode(strProductId);
			int nProductID = boost::lexical_cast<int>(wstrId);

			//bug 460 #1
			std::string strblowThreshold("");
			pTaskNode->QueryValueAttribute("blowthreshold", &strblowThreshold);
			if (strblowThreshold.empty())
			{
				task.m_blowThreshold = 3;
			}
			else
			{
				wstring blowThreshold  = utils::Utf8ToUnicode(strblowThreshold);
				int iblowThreshold = boost::lexical_cast<int>(blowThreshold);
				task.m_blowThreshold = iblowThreshold;
			}

// 			//解析任务排盖延时
// 			int rjectDelay;
// 			pTaskNode->QueryIntAttribute("rjectdelay", &rjectDelay);
// 			task.m_rjectDelay = rjectDelay;

			//解析任务相机参数
			std::string  strDGE("");
			pTaskNode->QueryValueAttribute("DelayGainExposure",&strDGE);
			wstring wstrDGE = utils::Utf8ToUnicode(strDGE);
			
			if (!wstrDGE.empty())
			{
				std::vector<wstring> vecALLCameraParam = utils::SplitString(wstrDGE, L";");
				for (int i = 0 ; i < vecALLCameraParam.size(); i++)
				{
					std::vector<wstring> vCameraParam = utils::SplitString(vecALLCameraParam[i], L",");

					if (vCameraParam.size() <= 2 )
					{
						continue;
					}
					
					CAMERA_SETTING cameraSetting;
					if (vCameraParam[0]== L"D")
					{
						cameraSetting.m_delayType = DELAY_DISTANCE;
					}
					else if(vCameraParam[0]== L"T")
					{
						cameraSetting.m_delayType = DELAY_TIME;
					}
					cameraSetting.m_delayValue = boost::lexical_cast<unsigned int>(vCameraParam[1]);
					cameraSetting.m_gain = boost::lexical_cast<unsigned int>(vCameraParam[2]);
					cameraSetting.m_ExposureTime = boost::lexical_cast<unsigned int>(vCameraParam[3]);

					task.m_vecTaskCameraSetting.push_back(cameraSetting);
				}
			}
	
			
			bool bSelfLearningGp = false;
			pTaskNode->QueryBoolAttribute("self_learning",&bSelfLearningGp);
			task.m_bSelfLearning = bSelfLearningGp;

			if(std::find(m_vProductIDs.begin(),m_vProductIDs.end(),nProductID) != m_vProductIDs.end() || bSelfLearningGp)
			{
				task.m_nProductId = nProductID;
				bool bImgInside = false;
				pTaskNode->QueryBoolAttribute("image_inside", &bImgInside);
				task.m_bImgInside = bImgInside;

				if(bSelfLearningGp)
				{
					unsigned int nSampleCount = 0;
					pTaskNode->QueryUnsignedAttribute("sample_count", &nSampleCount);
					task.m_nSampleCount = nSampleCount;

					string strSlParam;
					pTaskNode->QueryStringAttribute("param", &strSlParam);
					task.m_wstrSlParam = utils::Utf8ToUnicode(strSlParam);
				}

				auto its = pTaskNode->ToElement();
				if(its != nullptr)
				{
					int index = 0;
					for(auto it = its->FirstChildElement(); it; it = it->NextSiblingElement())
					{
						IT_PARAM itParam;
						std::string itID;
						it->QueryValueAttribute("index", &itID);
						itParam.m_itIndex = utils::Utf8ToUnicode(itID);

						bool bEnable = true;
						it->QueryBoolAttribute("enable", &bEnable);
						itParam.m_bEnable = bEnable;

						std::string itName;
						it->QueryValueAttribute("name",&itName);
						itParam.m_itName = utils::Utf8ToUnicode(itName);

						std::string strParam;
						it->QueryValueAttribute("param",&strParam);
						itParam.m_vParam = ParseParamFromString(strParam);

						std::string strNickname("");
						it->QueryStringAttribute("nickname",&strNickname);
						if(!strNickname.empty())
						{
							itParam.m_itNickname = utils::Utf8ToUnicode(strNickname);
						}
						else
						{
							itParam.m_itNickname = itParam.m_itName;
						}
						task.m_vItParam.push_back(itParam);
						index++;
					}
				}
				config.m_vTask.push_back(task);
			}
		}
	}
	void ParseChannelUiSetting(CHANNEL_CONFIG& config, TiXmlElement* pRoot)
	{
		TiXmlElement* pDispElement = pRoot->FirstChildElement("DispConfig");
		if(pDispElement != nullptr)
		{
			ReadChildNode(pDispElement,"Width",config.m_uiSetting.m_width);
			ReadChildNode(pDispElement,"Height",config.m_uiSetting.m_height);
		}

//	    多相机工位调试通道显示，已经适配比例
// 		CRect rcWorkArea;
// 		SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);
// 		float fVRate = 1080.0f / rcWorkArea.Height();
// 		float fHRate = 1920.0f / rcWorkArea.Width();
// 		int width  = int(config.m_uiSetting.m_width / fHRate);
// 		config.m_uiSetting.m_width = width;
// 		int height = int(config.m_uiSetting.m_height / fVRate);
// 		config.m_uiSetting.m_height = height;
	}
	void ParseSpcChartSetting(CHANNEL_CONFIG& config, TiXmlElement* pRoot)
	{
		TiXmlElement* pDispElement = pRoot->FirstChildElement("SPC_Chart");
		if(pDispElement != nullptr)
		{
			auto pNode = pDispElement->FirstChildElement("X_UNIT");
			if(pNode != nullptr)
			{
				string strUnitX = pNode->FirstChild()->ValueStr();
				config.m_spcSetting.m_strUnitX = utils::Utf8ToUnicode(strUnitX);
			}
			ReadChildNode(pDispElement,"X_LEN",config.m_spcSetting.m_nXLen);
			pNode = pDispElement->FirstChildElement("Y_UNIT");
			if(pNode != nullptr)
			{
				string strUnitY = pNode->FirstChild()->ValueStr();
				config.m_spcSetting.m_strUnitY = utils::Utf8ToUnicode(strUnitY);
			}
		}
	}
	void ParseChannelSettings(TiXmlNode* node)
	{
		m_setTaskName.clear();
		TiXmlNode* pPanelNode = node->FirstChild("PanelSetting");
		if(pPanelNode != nullptr)
		{
			TiXmlElement* pPanelSettingElement = pPanelNode->ToElement();
			if(pPanelSettingElement != nullptr)
			{
				std::string strIndex, strWarningflag;
				for(auto pElement = pPanelSettingElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
				{
					CHANNEL_CONFIG config;

					pElement->QueryValueAttribute("index",&strIndex);
					if(!strIndex.empty())
					{
						config.m_chIndex = boost::lexical_cast<int>(strIndex);
					}

					pElement->QueryValueAttribute("warningflag",&strWarningflag);
					if(!strWarningflag.empty())
					{
						config.m_chWarningflag =  utils::Utf8ToUnicode(strWarningflag);
					}

					pElement->QueryIntAttribute("mainchannel",&config.m_mainchannel);
					pElement->QueryIntAttribute("subchannel",&config.m_subchannel);

					pElement->QueryIntAttribute("rejectmintime",&config.m_RejectMinTime);

					pElement->QueryBoolAttribute("Splicing",&config.m_bSplicing);
					pElement->QueryBoolAttribute("Modelfiles",&config.m_bModelfiles);
					pElement->QueryBoolAttribute("single_processor",&config.m_bSingleProcessor);
					pElement->QueryIntAttribute("save_image_type",&config.m_SaveImageType);
					pElement->QueryIntAttribute("max_speed",&config.m_nMaxSpeed);
					if(config.m_nMaxSpeed <= 0)
					{
						config.m_nMaxSpeed = 50;
					}
		

					string strChNickname("");
					pElement->QueryValueAttribute("lane_name",&strChNickname);
					if(strChNickname != "")
					{
						config.m_wstrLaneName = utils::Utf8ToUnicode(strChNickname);
					}
					ParseCameraSetting(config, pElement);
					ParseLightCtrlSetting(config, pElement);
					ParseRejectorSetting(config, pElement);
					ParseTaskSetting(config, pElement);
					ParseChannelUiSetting(config, pElement);
					ParseSpcChartSetting(config, pElement);
					m_chSettings[config.m_chIndex] = config;
				}
			}
		}
	}
	void ParseModelSetting(TiXmlNode* node)
	{
		TiXmlNode* pTM = node->FirstChild("TaskModel");
		if(pTM == nullptr) return;

		TiXmlElement* pTmElement = pTM->ToElement();
		if(pTmElement == nullptr) return;

		std::string strTmName;
		for(auto pElement = pTmElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
		{
			TASK_MODEL crtTM;
			pElement->QueryValueAttribute("name",&strTmName);
			crtTM.m_strModelName = utils::Utf8ToUnicode(strTmName);

			int productId = -1;
			pElement->QueryIntAttribute("product_id", &productId);
			if(std::find(m_vProductIDs.begin(), m_vProductIDs.end(), productId) == m_vProductIDs.end())
			{
				continue;
			}
			crtTM.m_nProductId = productId;

			auto pItGp = pElement->ToElement();
			if(pItGp == nullptr) return;

			int index = 0;
			for(auto it = pItGp->FirstChildElement(); it; it = it->NextSiblingElement())
			{
				IT_PARAM itParam;
				std::string itID;
				it->QueryValueAttribute("index", &itID);
				itParam.m_itIndex = utils::Utf8ToUnicode(itID);

				std::string itName;
				it->QueryValueAttribute("name",&itName);
				itParam.m_itName = utils::Utf8ToUnicode(itName);

				std::string strParam;
				it->QueryValueAttribute("param",&strParam);
				itParam.m_vParam = ParseParamFromString(strParam);

				std::string strNickname("");
				it->QueryStringAttribute("nickname",&strNickname);

				if(!strNickname.empty())
				{
					itParam.m_itNickname = utils::Utf8ToUnicode(strNickname);
				}
				crtTM.m_vItParam.push_back(itParam);
				index++;
			}
			m_modelSetting[crtTM.m_strModelName] = crtTM;
		}
	}
	bool CheckFileExists(const wstring& path, bool bOutputMsg = true)
	{
		if(INVALID_FILE_ATTRIBUTES == (::GetFileAttributes(path.c_str())))
		{
			if(bOutputMsg == true)
				printf("File does not exists : %s\n", path.c_str());
			return false;
		}
		return true;
	}
	vector<wstring> ParseParamFromString(std::string strValue)
	{
		std::wstring wStr = utils::Utf8ToUnicode(strValue);
		std::vector<wstring> retParam;
		boost::split(retParam, wStr, boost::is_any_of(L","),token_compress_on);
		return retParam;
	}
	bool UpdateFuncParameters(TiXmlElement* pFuncNode, FUNC_INFO* pFuncInfo)
	{
		if(pFuncNode == nullptr || pFuncInfo == nullptr)
			return false;
		string strOldParam;
		pFuncNode->QueryValueAttribute("param", &strOldParam);
		std::vector<string> vStrParam = utils::SplitString(strOldParam, ",");
		utils::TrimString(vStrParam);

		USES_CONVERSION;

		string strNewParam;
		CString strV;
		if(pFuncInfo->m_bTestFunc)
		{
			if(vStrParam.empty()) return false;
			string strTh = vStrParam[0];
			std::vector<string> vRet = utils::SplitString(strTh, "$");
			utils::TrimString(vRet);
			if(vRet.empty()) return false;
			strNewParam.append(vRet[0]);
			strNewParam.append("$");

			THRESHOLD threshold = pFuncInfo->m_threshold;
			if(threshold.m_compType > LESS_EQUAL)
			{
				CString strMin;
				CString strMax;
				if(threshold.m_type == VT_R4)
				{
					strMin.Format(_T("%.2f"), threshold.m_lcl);
					strMin.Format(_T("%.2f"), threshold.m_ucl);
				}
				else
				{
					strMin.Format(_T("%d"), (long)threshold.m_lcl);
					strMin.Format(_T("%d"), (long)threshold.m_ucl);
				}
				strNewParam.append(CT2CA(strMin));
				strNewParam.append("-");
				strNewParam.append(CT2CA(strMax));
			}
			else
			{
				if(threshold.m_type == VT_R4)
					strV.Format(_T("%.2f"), threshold.m_specV);
				else
					strV.Format(_T("%d"), (long)threshold.m_specV);
				strNewParam.append(CT2CA(strV));
				strNewParam.append(",");
			}
		}
		else
			strNewParam.append("N/A,");

		auto vParam = pFuncInfo->m_vParam;
		for(int i = 0; i < vParam.size(); i++)
		{
			if(vParam[i].m_type == VT_R4)
			{
				strV.Format(_T("%.3f"), vParam[i].m_value);
			}
			if(vParam[i].m_type == VT_I4 || vParam[i].m_type == VT_UI1)
			{
				strV.Format(_T("%d"), (long)vParam[i].m_value);
			}
			if(vParam[i].m_type == VT_BSTR)
			{
				strV = vParam[i].m_wstrFile.c_str();
				if(strV == _T(""))
				{
					strV = _T(" ");
				}
			}
		    wstring wstrTmp = CT2CW(strV);
			string strItem = utils::Unicode2Utf8(wstrTmp);
			strNewParam.append(strItem);
			if(i != vParam.size() - 1)
				strNewParam.append(",");
		}
		pFuncNode->SetAttribute("param",strNewParam);

		return true;
	}
	bool UpdateChannelSelfLearningParam(TiXmlElement* pTaskGpNode, wstring strTaskName)
	{
		if(pTaskGpNode == nullptr)
			return false;
		auto pTaskNode = pTaskGpNode->ToElement();

		if(pTaskNode == nullptr)
			return false;

		TiXmlElement* pTarget = nullptr;
		for(auto p = pTaskNode->FirstChildElement(); p; p = p->NextSiblingElement())
		{
			bool ret = false;
			p->QueryBoolAttribute("self_learning", &ret);
			if(ret)
			{
				string slTaskName;
				p->QueryStringAttribute("name", & slTaskName);
				pTaskGpNode->SetAttribute("crt_task_name", slTaskName);

				auto pItArray = p->ToElement();
				if(pItArray != nullptr)
				{
					for(auto pIt = pItArray->FirstChildElement(); pIt;  pIt = pIt->NextSiblingElement())
					{
						string strRet;
						pIt->QueryStringAttribute("name", &strRet);
					}
				}
			}
		}
		return false;
	}
	TASK GenerateTaskFromModel(const wstring& taskName, const TASK_MODEL& taskModel, bool bImgInside = false)
	{
		TASK retTask;
		retTask.m_bImgInside = bImgInside;
		retTask.m_bSelfLearning = false;
		retTask.m_nProductId = taskModel.m_nProductId;
		retTask.m_nSampleCount = -1;
		retTask.m_wstrTaskName = taskName;
		retTask.m_vItParam = taskModel.m_vItParam;

		return retTask;
	}
	void PrepareTaskFuncAndParameter(TASK& crtTask, const TASK_MODEL& taskModel)
	{
		int index = 0;
		std::vector<IT_PARAM> vItParam = taskModel.m_vItParam;
		for(size_t i = 0; i < vItParam.size(); i++)
		{
			IT_PARAM itParam = vItParam[i];
			itParam.m_itIndex = boost::lexical_cast<wstring>(index); index++;
			crtTask.m_vItParam.push_back(itParam);
		}
	}
	bool UpdateChildNode(TiXmlElement* pParentNode, string strNodeName, int nValue)
	{
		if(pParentNode == nullptr) return false;

		std::string strValue;
		TiXmlElement* pChildNode = pParentNode->FirstChildElement(strNodeName);
		if(pChildNode == nullptr)
		{
			TiXmlElement childNode(strNodeName);
			strValue = boost::lexical_cast<string>(nValue);
			TiXmlText value(strValue.c_str());
			childNode.InsertEndChild(value);
			pParentNode->InsertEndChild(childNode);
		}
		else
		{
			strValue = boost::lexical_cast<string>(nValue);
			pChildNode->FirstChild()->SetValue(strValue);
		}
		return true;
	}
	bool UpdateChildNode(TiXmlElement* pParentNode, string strNodeName, string strValue)
	{
		if(pParentNode == nullptr) return false;

		TiXmlElement* pChildNode = pParentNode->FirstChildElement(strNodeName);
		if(pChildNode == nullptr)
		{
			TiXmlElement childNode(strNodeName);
			TiXmlText value(strValue.c_str());
			childNode.InsertEndChild(value);
			pParentNode->InsertEndChild(childNode);
		}
		else
		{
			pChildNode->FirstChild()->SetValue(strValue);
		}
		return true;
	}
	void ReadChildNode(TiXmlElement* pParentNode, string strNodeName, int& nRetValue)
	{
		if(pParentNode == nullptr) return;

		TiXmlElement* pChildNode = pParentNode->FirstChildElement(strNodeName);
		if(pChildNode != nullptr)
		{
			wstring wstrValue = utils::Utf8ToUnicode(pChildNode->FirstChild()->Value());
			nRetValue = boost::lexical_cast<int>(wstrValue);
		}
	}
    void ReadChildNode(TiXmlElement* pParentNode, string strNodeName, string& strValue)
    {
        strValue = "";
        if(pParentNode == nullptr) return;

        TiXmlElement* pChildNode = pParentNode->FirstChildElement(strNodeName);
        if(pChildNode != nullptr)
        {
            strValue = pChildNode->FirstChild()->Value();
        }
    }
	string GetSubstituteCurrentTaskName(int nChIndex, string strCrtTaskName)
	{
		string strRet = "";
		if(m_chSettings.find(nChIndex) != m_chSettings.end())
		{
			wstring wstrTaskName = utils::Utf8ToUnicode(strCrtTaskName);
			CHANNEL_CONFIG chConfig = m_chSettings[nChIndex];
			for(auto it = chConfig.m_vTask.begin();it != chConfig.m_vTask.end();it++)
			{
				if(wstrTaskName != (*it).m_wstrTaskName)
				{
					strRet = utils::Unicode2Utf8((*it).m_wstrTaskName);
					break;
				}
			}
		}
		return strRet;
	}
private:
	int							m_nLangID;
	wstring						m_configFilePath;
    bool						m_bLoaded;
	
	map<wstring, wstring>		m_commonSetting;
	vector<SHIFT>				m_vShift;
	map<wstring, TASK_MODEL>    m_modelSetting;
	map<int, CHANNEL_CONFIG>	m_chSettings;
public:
	set<wstring>                m_setTaskName;
	std::vector<int>            m_vProductIDs;
	bool                        m_bAutoResetShiftStats;
private:
	ConfigManager(const ConfigManager&);
	ConfigManager& operator=(const ConfigManager&);
};

#endif
