#ifndef __INC_CEF_CLIENT_APP_H__
#define __INC_CEF_CLIENT_APP_H__

#include <cef/cef_app.h>
#include <cef/cef_client.h>

class ClientApp : public CefApp, public CefRenderProcessHandler
{
public:
	ClientApp();

	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override
	{
		return this;
	}

	void OnWebKitInitialized() override;

	IMPLEMENT_REFCOUNTING(ClientApp);
};


#endif // __INC_CEF_CLIENT_APP_H__
