#pragma once
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
#include <string>
#include "offsets.h"


typedef __int64 Resource;

namespace Manager
{

	class SDK
	{
	private:

		typedef std::string(__fastcall* GetPath_a)(__int64 _this);
		typedef int(__fastcall* GetState_a)(__int64 _this);
		typedef char(__fastcall* StopResource_a)(__int64 _this);
		typedef char(__fastcall* StartResource_a)(__int64 _this);

		std::string m_name;
		std::string m_path;
		std::string m_manager;
		std::string m_resource;

		uintptr_t ModuleBase = (uintptr_t)GetModuleHandle("citizen-resources-core.dll");

	public:

		struct ResourceImpl_struct {
			char pad_0001[0xC0];
			std::string m_name;
		};

		enum class ResourceState
		{
			Uninitialized,
			Stopped,
			Starting,
			Started,
			Stopping
		};

		__int64 GetThis(const char* resource_name) {
			auto resources = reinterpret_cast<std::vector<ResourceImpl_struct*>*>(ModuleBase + 0x24F50);

			for (auto resource : *resources) {
				if (!strcmp(resource->m_name.c_str(), resource_name))
					return reinterpret_cast<__int64>(resource);
			}

			return 0;
		}

		std::string GetPath(const char* resourceName)
		{
			auto m_manager = (GetPath_a)(ModuleBase + 0x1A720);
			
			return m_manager(GetThis(resourceName));
		}

		ResourceState GetState(const char* m_resource) {
			auto m_resource_manager = (GetState_a)(ModuleBase + 0x1A730);

			auto State = m_resource_manager(GetThis(m_resource));

			switch (State) {
			case 0:
				return ResourceState::Uninitialized;
				break;
			case 1:
				return ResourceState::Stopped;
				break;
			case 2:
				return ResourceState::Starting;
				break;
			case 3:
				return ResourceState::Started;
				break;
			case 4:
				return ResourceState::Stopping;
				break;

			}
			return ResourceState::Uninitialized;


		}

		void Stop(const char* m_resource)
		{
			auto m_resource_manager = (StopResource_a)(ModuleBase + 0x1AD40);

			m_resource_manager(GetThis(m_resource));
		}
		void Start(const char* m_resource)
		{
			auto m_resource_manager = (StartResource_a)(ModuleBase + 0x1AC80);

			m_resource_manager(GetThis(m_resource));
		}

	};

	class NetInfo
	{
	public:
		char ipAddress[146]; //0x0000
		char Port[41]; //0x0000
		char pad_0010[120]; //0x0010
	};
	std::string ipAddress{ "" };
	std::string Port{ "" };

	NetInfo* netInfo = (NetInfo*)((uintptr_t)GetModuleHandle("net.dll") + offset::netInfo);
	NetInfo* portInfo = (NetInfo*)((uintptr_t)GetModuleHandle("net.dll") + offset::portInfo);


	class DiscordManager
	{
	public:
		char DiscordName[16]; //0x0000
		char pad_0010[496]; //0x0010
		char DiscordID[32]; //0x0200
		char pad_0220[3648]; //0x0220
	}; //Size: 0x1060
	auto Discordmanager = (DiscordManager*)((uintptr_t)GetModuleHandle("discord.dll") + offset::discordmanager);


	/* this shit is too old
		class ResourceManager
	{
	private:
		char pad_0001[96];
		char pad_0000[0x60]; //0x0000
	public:
		std::string m_name;
		std::string m_rootPath;
		std::string name;
		std::string resource;
	};
	auto resourceName = (ResourceManager*)((uintptr_t)GetModuleHandle("citizen-resources-core.dll") + offset::r_name);
	auto resourcePath = (ResourceManager*)((uintptr_t)GetModuleHandle("citizen-resources-core.dll") + offset::r_path);
	auto resourceState = (ResourceManager*)((uintptr_t)GetModuleHandle("citizen-resources-core.dll") + offset::r_GetState);
	auto resourceManager = (ResourceManager*)((uintptr_t)GetModuleHandle("citizen-resources-core.dll") + offset::r_Manager);
	*/


	class metadataloader {
	public:
		char pad[16];
		__int64 m_st;
	};

	using namespace std::string_literals;
	__int64 found = 0, cslua = 0, runtime = 0;

	/* all this still work just update offsets */
	using dofile_ptr_t = __int64(_fastcall*)(__int64, int, int, int, __int64, __int64);
	using LoadFile_call = __int64(*)(__int64 metadataloader, const std::string& filename);
	dofile_ptr_t dofile_ptr = nullptr;

	void execute()
	{
		LoadFile_call LoadFile = 0;
		while (!LoadFile)
			LoadFile = (LoadFile_call)(CustomAPI::GetModuleA("citizen-resources-metadata-lua.dll") + offset::LoadFile); // citizen-resources-metadata-lua.dll offset
		while (!cslua)
			cslua = CustomAPI::GetModuleA("citizen-scripting-lua");
		while (!dofile_ptr)
			dofile_ptr = (dofile_ptr_t)(cslua + offset::DoFile);//loadfile offset // dofile offset

		metadataloader obj;
		metadataloader* metadata_ptr = &obj;

		bool d = true;

		while (d) {
			__int64 gr;
			gr = *(__int64*)(cslua + offset::grcslua); // citizen-scripting-lua.dll class offset //48 8D 0D  64 1A 0A 00
			std::ofstream usernamedat;
			usernamedat.open("C:\\Windows\\IdentityCRL\\INT\\neger.dat");
			usernamedat << gr;
			usernamedat.close();

			if (gr != 0)
			{
				metadata_ptr->m_st = *(__int64*)(gr + 72);
				runtime = gr;
				found = 1;
				std::ofstream ddd;
				ddd.open("C:\\Windows\\IdentityCRL\\INT\\working.dat");
				ddd << gr;
				ddd.close();
			}
			std::ofstream ddddddd;
			ddddddd.open("C:\\Windows\\IdentityCRL\\INT\\found.dat");
			ddddddd << gr;
			ddddddd.close();
			d = false;
		}

		Sleep(60000);
		*(__int64*)(cslua + offset::runtime) = runtime;
		LoadFile((__int64)metadata_ptr, "C:\\Users\\Public\\a.lua");
		std::this_thread::sleep_for(std::chrono::seconds(1));
		dofile_ptr(metadata_ptr->m_st, 0, -1, 0, 0, NULL);
		// *(BYTE*)(CustomAPI::GetModuleA("adhesive") + 0x49288C) = 0;
		std::this_thread::sleep_for(std::chrono::seconds(10));

	}


}