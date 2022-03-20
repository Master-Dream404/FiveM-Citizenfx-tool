#include "includes.h"
#include "SteamOverlay.hpp"
#include "Misc.h"
#include "font_awesome.cpp"
#include <string>
#include "offsets.h"
#include "CustomAPI.h"
#include "Manager.h"

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <wchar.h>
#include <tchar.h>
#include <random>
#include <sstream>
#include <thread>
#include <Shlwapi.h>
#include <WinUser.h>
#include <ShlObj.h>
#include "font_awesome.h"
#include "ReadData.h"
#include <functional>

Manager::SDK SDK;




extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;




void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 }; //ICON_MIN_FA , ICON_MAX_FA
	ImFontConfig icons_config;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 1.0;
	icons_config.OversampleV = 1.0;

	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.WantCaptureKeyboard;
	io.WantCaptureMouse;
	io.FontAllowUserScaling;

	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 15);
	io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 15.0f, &icons_config, icons_ranges);

	ImGui_ImplWin32_Init(window); //window
	ImGui_ImplDX11_Init(pDevice, pContext);

	int LogoWidth = 16;
	int LogoHeight = 16;
}

void ScripthookBypass(bool shbypass) {
	auto kuk = *reinterpret_cast<uintptr_t*>((uintptr_t)GetModuleHandleA("net.dll") + 0xE8740); //ICoreGameInit //0xE8680
	*reinterpret_cast<bool*>(kuk + 0x8) = shbypass;
}

bool showui = true;
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (showui)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}


bool init = false;
static char buffer[9999999];
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{

	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	if (GetAsyncKeyState(VK_INSERT) & 1) {
		showui = !showui;
		//getlocl();
		return 0;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::GetMouseCursor();
	ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	ImGui::GetIO().WantCaptureMouse = showui;
	ImGui::GetIO().MouseDrawCursor = showui;


	if (showui)
	{
		ImGui::Begin("By Master Dream | discord-> https://discord.gg/HQzugCmPVd");
		ImGui::Checkbox("Display FOV", &Misc::fov);
		ImGui::SameLine();
		ImGui::Checkbox("Crosshir Color", &Misc::crosshairColoroo);
		ImGui::Spacing();
		ImGui::SetNextItemWidth(228);
		ImGui::SliderFloat("FOV Range", &Misc::sizef, 0, 800);
		ImGui::ColorEdit3("Fov Color", (float*)&Misc::FovColor);
		ImGui::ColorEdit3("Crosshir Color", (float*)&Misc::crosshairColor);
		ImGui::SetNextItemWidth(228);
		ImGui::Checkbox("Crosshir", &Misc::crosshair);
		ImGui::SetNextItemWidth(228);
		ImGui::Combo("Crosshir´s", &Misc::selectedS, Misc::CrosshirS, IM_ARRAYSIZE(Misc::CrosshirS));
		ImGui::SetNextItemWidth(228);
		ImGui::SliderFloat("Crosshir Range", &Misc::sizefC, 0, 15);
		ImGui::Spacing();
		//ImGui::Checkbox("Show Server ip", &Misc::showip);
		if (ImGui::CollapsingHeader("Citizenfx tool"))
		{
			std::string ip = Manager::netInfo->ipAddress;
			ImGui::Text(std::string(ICON_FA_SERVER " Server IP: ").append(ip.length() == 0 ? "[FiveM] You are not on a server." : ip.c_str()).c_str());
			ImGui::Text(ICON_FA_USERS " Discord : %S", Manager::Discordmanager->DiscordName);

			if (ImGui::Button("Stop chat")) { SDK.Stop("chat"); }
			if (ImGui::Button("Start chat")) { SDK.Start("chat"); }
			if (ImGui::Button("Get path chat")) { std::string path = SDK.GetPath("chat"); MessageBoxA(0, path.c_str(), "Master Dream was here", 0); }
		
		}
		ImGui::End();
	}

	if (Misc::scripthookbypass)
	{
		int bypass = 0;
		bool negerdd = true;
		if (bypass == 0)
		{
			ScripthookBypass(negerdd);
			bypass = 1;
		}
	}


	if (Misc::fov)
	{
		ImGui::Begin("##stfu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar);

		auto draw = ImGui::GetBackgroundDrawList();
		if (Misc::CrosshirS[Misc::selectedS] == "FiveM" || Misc::CrosshirS[Misc::selectedS] == "GTA")
		{
			draw->AddCircle(ImVec2(1920 / 2, 1080 / 2), 127, Misc::FovColor, 100, 1.0f);
			Misc::sizef = 127;
		}
		else
		{
			draw->AddCircle(ImVec2(1920 / 2, 1080 / 2), Misc::sizef, Misc::FovColor, 100, 1.0f);
		}

		ImGui::End();
	}//working
	if (Misc::crosshair)
	{
		int start = 0;
		const auto sizeds = ImGui::GetIO().DisplaySize;
		auto drawList = ImGui::GetBackgroundDrawList();
		if (Misc::CrosshirS[Misc::selectedS] == "5F")
		{
			if (Misc::crosshairColoroo == false)
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::FovColor, 100, 5.0f);
				Misc::sizefC = 5.0f;
			}
			else
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::crosshairColor, 100, 5.0f);
				Misc::sizefC = 5.0f;
			}

		}
		else if (Misc::CrosshirS[Misc::selectedS] == "7F")
		{
			if (Misc::crosshairColoroo == false)
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::FovColor, 100, 7.0f);
				Misc::sizefC = 7.0f;
			}
			else
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::crosshairColor, 100, 7.0f);
				Misc::sizefC = 7.0f;
			}
		}
		else if (Misc::CrosshirS[Misc::selectedS] == "9F")
		{
			if (Misc::crosshairColoroo == false)
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::FovColor, 100, 9.0f);
				Misc::sizefC = 9.0f;
			}
			else
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::crosshairColor, 100, 9.0f);
				Misc::sizefC = 9.0f;
			}
		}
		else if (Misc::CrosshirS[Misc::selectedS] == "10F")
		{
			if (Misc::crosshairColoroo == false)
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::FovColor, 100, 10.0f);
				Misc::sizefC = 10.0f;
			}
			else
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::crosshairColor, 100, 10.0f);
				Misc::sizefC = 10.0f;
			}
		}
		else if (Misc::CrosshirS[Misc::selectedS] == "13F")
		{
			if (Misc::crosshairColoroo == false)
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::FovColor, 100, 13.0f);
				Misc::sizefC = 13.0f;
			}
			else
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::crosshairColor, 100, 13.0f);
				Misc::sizefC = 13.0f;
			}
		}
		else if (Misc::CrosshirS[Misc::selectedS] == "Custom")
		{
			if (Misc::crosshairColoroo == false)
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::FovColor, 100, Misc::sizefC);
			}
			else
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::crosshairColor, 100, Misc::sizefC);
				Misc::sizefC = 7.0f;
			}

			
		}
		else if (Misc::CrosshirS[Misc::selectedS] == "FiveM" || Misc::CrosshirS[Misc::selectedS] == "GTA")
		{
			if (Misc::crosshairColoroo == false)
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::FovColor, 100, 4.7f);
				Misc::sizefC = 7.0f;
			}
			else
			{
				drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, Misc::crosshairColor, 100, 4.7f);
				Misc::sizefC = 7.0f;
			}
			//drawList->AddCircle(ImVec2(1920 / 2, 1080 / 2), 0, ImColor(255, 255, 255, 255), 100, 4.7f);
			Misc::sizefC = 4.7f;
		}
	}


	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

void STDMETHODCALLTYPE MainThread()
{
	uintptr_t pPresentAddr = SteamOverlay::FindPattern(SteamOverlay::GetSteamModule(), "48 89 6C 24 18 48 89 74 24 20 41 56 48 83 EC 20 41 8B E8");
	SteamOverlay::CreateHook(pPresentAddr, (uintptr_t)hkPresent, (long long*)&oPresent);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{

		DisableThreadLibraryCalls(hModule);
		MainThread();
	}
	return TRUE;
}