#pragma once

#include <afxcoll.h>
#include <string>

using namespace std;

class CLanguageProxy
{
public:
	CLanguageProxy();
	~CLanguageProxy();
	void LoadCrtLanguage(int nLangID); // Called on start-up to load the required language dll

	HINSTANCE m_hDll; // Satellite DLL.
protected:
  CWordArray m_aLanguages;
  LANGID m_nExeLanguage;
  // Try to load the language m_nCurrentLanguage.If it doesn't exist, try to fallback on the neutral or default sublanguage for this language.
};
  