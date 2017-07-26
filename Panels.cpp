#include "SDK.h"
#include "Panels.h"
#include "CDrawX.h"
#include "Fonts.h"
#include "Menu.h"

CScreenSize gScreenSize;
//===================================================================================
void __fastcall Hooked_PaintTraverse( PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	try
	{
		VMTManager& hook = VMTManager::GetHook(pPanels); //Get a pointer to the instance of your VMTManager with the function GetHook.
		hook.GetMethod<void(__thiscall*)(PVOID, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.

		static unsigned int panel;

		if (panel == NULL)
		{
			string szPanelName = gInts.Panels->GetName(vguiPanel);
			if (szPanelName.find("FocusOverlayPanel") != string::npos)
			{
				panel = vguiPanel;
				Intro();
			}
		}
		
		if ( panel == vguiPanel ) //If we're on MatSystemTopPanel, call our drawing code.
		{
			int iNewWidth, iNewHeight;
			gInts.Engine->GetScreenSize(iNewWidth, iNewHeight);
			if (gScreenSize.iScreenWidth != iNewWidth || gScreenSize.iScreenHeight != iNewHeight)
			{
				gInts.Engine->GetScreenSize(gScreenSize.iScreenWidth, gScreenSize.iScreenHeight);
				gDraw.Initialize();
				fonts.init();
			}

			if( gInts.Engine->IsDrawingLoadingImage() || ( ( GetAsyncKeyState(VK_F12) || gInts.Engine->IsTakingScreenshot( ) ) ) )
			{
				return; //We don't want to draw at the menu.
			}

			gInts.Panels->SetMouseInputEnabled(vguiPanel, gMenu.menuToggle ? true : false);
			gMenu.paint();
		}
	}
	catch(...)
	{
		Log::Fatal("Failed PaintTraverse");
	}
}
//===================================================================================
void Intro( void )
{
	try
	{
		gDraw.Initialize(); //Initalize the drawing class.
		Textures.init();
		fonts.init();
		gMenu.init();

		Log::Msg("Injection Successful"); //If the module got here without crashing, it is good day.
	}
	catch(...)
	{
		Log::Fatal("Failed Intro");
	}
}