#include "StdAfx.h"
#include "SpeechRecognition.h"

#include <sapi.h>
#include <atlbase.h>
#include "sphelper.h"

static CComPtr<ISpRecognizer>  cpRecognizer;
static CComPtr<ISpRecoContext> cpRecoCtxt;
static CComPtr<ISpRecoGrammar> cpGrammar;
static CComPtr<ISpVoice> cpVoice;

static CComPtr<ISpObjectToken>      cpObjectToken;
static CComPtr<ISpAudio>            cpAudio;

static void SpeechReconitionInit()
{
	HRESULT hr;

	if ( SUCCEEDED(hr = ::CoInitialize(NULL)) )
	{
			//hr = cpRecoCtxt.CoCreateInstance(CLSID_SpSharedRecoContext);
			hr = cpRecognizer.CoCreateInstance(CLSID_SpInprocRecognizer);
            if(SUCCEEDED(hr))
            {
				cpRecognizer->CreateRecoContext(&cpRecoCtxt);

				 // Get the default audio input token.
				hr = SpGetDefaultTokenFromCategoryId(SPCAT_AUDIOIN, &cpObjectToken);

				// Set the audio input to our token.
				hr = cpRecognizer->SetInput(cpObjectToken, TRUE);
				// Set up the inproc recognizer audio input with an audio input object.

				// Create the default audio input object.
				hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOIN, &cpAudio);

				// Set the audio input to our object.
				hr = cpRecognizer->SetInput(cpAudio, TRUE);

				cpRecognizer->SetRecognizer(NULL);

                hr = cpRecoCtxt->GetVoice(&cpVoice);
            }
           
            if (cpRecoCtxt && cpVoice &&
                SUCCEEDED(hr = cpRecoCtxt->SetNotifyWin32Event()) &&
                SUCCEEDED(hr = cpRecoCtxt->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION))) &&
                SUCCEEDED(hr = cpRecoCtxt->SetAudioOptions(SPAO_RETAIN_AUDIO, NULL, NULL)) &&
                SUCCEEDED(hr = cpRecoCtxt->CreateGrammar(0, &cpGrammar)) &&
                SUCCEEDED(hr = cpGrammar->LoadDictation(NULL, SPLO_STATIC)) &&
                SUCCEEDED(hr = cpGrammar->SetDictationState(SPRS_ACTIVE)))
            {
                CComPtr<ISpRecoResult> cpResult;

			}

	}
}

static HRESULT BlockForResult(ISpRecoContext * pRecoCtxt, ISpRecoResult ** ppResult)
{
    HRESULT hr = S_OK;
	CSpEvent event;

	/*while (SUCCEEDED(hr) &&
           SUCCEEDED() &&
           hr == S_FALSE)
    {
        hr = pRecoCtxt->WaitForNotifyEvent(INFINITE);
    }*/

	//hr = event.GetFrom(pRecoCtxt);

	//if ( SUCCEEDED(hr) )
	//{
		//hr = pRecoCtxt->WaitForNotifyEvent(0);

    /*while (SUCCEEDED(hr) &&
           SUCCEEDED(hr = event.GetFrom(pRecoCtxt)) &&
           hr == S_FALSE)
    {
        hr = pRecoCtxt->WaitForNotifyEvent(INFINITE);
    }*/

   
		if ( hr = event.GetFrom(pRecoCtxt) == S_OK )
		{
			*ppResult = event.RecoResult();
			if (*ppResult)
			{
				(*ppResult)->AddRef();
			}
		}
		else
			hr = -1;
	//}
	//else
	//	hr = E_POINTER;

    return hr;
}

#include "Messages.h"

static void Listen()
{
	CComPtr<ISpRecoResult> cpResult;

	if ( SUCCEEDED(BlockForResult(cpRecoCtxt, &cpResult)) )
	{

		cpGrammar->SetDictationState( SPRS_INACTIVE );

		CSpDynamicString dstrText;

		if (SUCCEEDED(cpResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, 
										TRUE, &dstrText, NULL)))
		{
			static std::string		strMyCommand;
			
			strMyCommand = (const char*)CW2A(dstrText);

			std::transform(strMyCommand.begin(), strMyCommand.end(), strMyCommand.begin(), ::tolower);
			//LogToFile("User said %s", pText);

			//cpVoice->Speak( L"I heard", SPF_ASYNC, NULL);
			//cpVoice->Speak( dstrText, SPF_ASYNC, NULL );

			CMessages::AddMessageJumpQ(strMyCommand.c_str(), 3000, 1, true);

			if ( strMyCommand == "hello" )
				cpVoice->Speak( L"Hello sir", SPF_ASYNC, NULL);
			else if ( strMyCommand == "this is great" )
				cpVoice->Speak( L"Thank you sir", SPF_ASYNC, NULL);
			else if ( strMyCommand == "car" )
			{
				cpVoice->Speak( L"Of course sir", SPF_ASYNC, NULL);
				((void(*)(int))0x43A0B0)(410);
			}
			else if ( strMyCommand == "sports car" )
			{
				cpVoice->Speak( L"Of course sir", SPF_ASYNC, NULL);
				((void(*)(int))0x43A0B0)(415);
			}
			else if ( strMyCommand == "what's your name" )
				cpVoice->Speak( L"My name is Domestobot", SPF_ASYNC, NULL);
			else if ( strMyCommand == "domestobot" )
				cpVoice->Speak( L"To your service", SPF_ASYNC, NULL);
			else if ( strMyCommand == "thank you" )
				cpVoice->Speak( L"You're welcome", SPF_ASYNC, NULL);

			cpResult.Release();

		}

		cpGrammar->SetDictationState( SPRS_ACTIVE );
	}
}

void SpeechInject()
{
	/*SpeechReconitionInit();
	Memory::InjectHook(0x53BFB8, Listen);*/
}

void SpeechDeinit()
{
	//cpRecoCtxt.Release();
	cpRecognizer.Release();

	::CoUninitialize();
}