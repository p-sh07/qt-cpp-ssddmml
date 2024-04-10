#pragma once

#include <windows.h>
#include <Atlconv.h>

#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::filesystem::path;

namespace fs = std::filesystem;

const path DFLT_GZDOOM = "gzdoom";
const path DFLT_CONFIG = DFLT_GZDOOM / "gzdoom_portable.ini";
const std::string DEFAULT_COMMAND = "-config gzdoom_portable.ini -file mods\\SIGIL_v1_21.wad mods\\SIGIL_SHREDS.wad mods\\SIGIL_II_V1_0.WAD mods\\sigil2_ost_thorr.WAD mods\\D64TEX.PK3 mods\\DoomMetalVol5.wad";

struct DooM
{
	path gzdoom_dir = DFLT_GZDOOM;

	path iwad_dir = gzdoom_dir / "iwads";
	path iwad = iwad_dir / "DOOM.WAD";

	path mod_dir = gzdoom_dir / "mods";
	std::vector<path> mod_fnames;

	path config = DFLT_CONFIG;

	//zips dir? multiple dirs?
};

class GZDML
{
public:
	GZDML() {
		//if (fs::current_path().string() == "C:\\Projects\\Qt\\gzdml") {
		//	cerr << "!changed curr path" << endl;
		//	fs::current_path(path{ "C:\\Projects\\Qt\\gzdml\\x64\\Debug" });
		//}
	}
	GZDML(const path& gzdoom_dir);

	DooM ParseStringInput(std::string_view in);

	// TODO: use latest id? add new? use back()?
	void AddMod(const path& wad_pk3);

	void AddGamefromParams(const path& iwad, const path& mod, const path& config = DFLT_CONFIG);
	void AddGamefromParams(const path& iwad, const std::vector<path>& mods, const path& config = DFLT_CONFIG);

	void AddADoom(DooM lp);

	void SetGZDoomDir();

	//There is always a default launch config present
	void LaunchGame(size_t id = 0) {
		cerr << "Attempting to launch: " << gzdoomexe.string() 
			 << " curr_path: " << fs::current_path()
			 << endl;

		try {
			PerformDefaultLaunch();
		}
		catch (std::exception& ex) {
			cout << "Error during launch: " << ex.what() << endl;
		}
	}

private:
	//looks for the gzdoom exe and sets the path variables
	void InitGZDoomPaths();
	//creates all the neccessary directories
	void SetupFolders(); 

	path gzdoomexe_full = "gzdoom/gzdoom.exe";

	path gzdoomexe = "gzdoom.exe";
	path gzdoompath = "gzdoom/";
	//TODO: "fname is: " gzdoomexe_full.filename().string() 
	// " and path is: " gzdoomexe_full.remove_filename()

	std::vector<DooM> launches_{ 1 };

	//Tested - working
	void PerformDefaultLaunch() {
		std::wstring cwstr(DEFAULT_COMMAND.begin(), DEFAULT_COMMAND.end());
		ShellExecuteW(NULL, NULL, gzdoomexe.c_str(), cwstr.c_str(), gzdoompath.c_str(), SW_SHOWDEFAULT);

		//STARTUPINFO si;
		//PROCESS_INFORMATION pi;

		// set the size of the structures
		//ZeroMemory(&si, sizeof(si));
		//si.cb = sizeof(si);
		//ZeroMemory(&pi, sizeof(pi));

		//std::string command = "-config user_config.txt -file mods\\brutalv22test3.pk3 mods\\SIGIL_v1_21.wad mods\\SIGIL_SHREDS.wad mods\\D64TEX.PK3 mods\\DoomMetalVol5.wad";

		//CreateProcess(
		//	gzdoomexe.c_str(),   // the path
		//	NULL,			// Command line
		//	NULL,           // Process handle not inheritable
		//	NULL,           // Thread handle not inheritable
		//	FALSE,          // Set handle inheritance to FALSE
		//	0,              // No creation flags
		//	NULL,           // Use parent's environment block
		//	NULL,           // Use parent's starting directory
		//	&si,            // Pointer to STARTUPINFO structure
		//	&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
		//);
		//// Close process and thread handles.
		//CloseHandle(pi.hProcess);
		//CloseHandle(pi.hThread);

		//ALT, working:
		//ShellExecute(NULL, L"runas", gzdoomexe.c_str(), NULL, L"gzdoom", SW_SHOWDEFAULT);
		//ShellExecute(NULL, NULL, gzdoomexe.c_str(), NULL, L"gzdoom", SW_SHOWDEFAULT);
	}
};

//Launch a program:
/*
//SHELL
HINSTANCE ShellExecuteA(
  [in, optional] HWND   hwnd,
  [in, optional] LPCSTR lpOperation,
  [in]           LPCSTR lpFile,
  [in, optional] LPCSTR lpParameters,
  [in, optional] LPCSTR lpDirectory,
  [in]           INT    nShowCmd
);

//PROCESS
#include <windows.h>

VOID startup(LPCTSTR lpApplicationName)
{
   // additional information
   STARTUPINFO si;
   PROCESS_INFORMATION pi;

   // set the size of the structures
   ZeroMemory( &si, sizeof(si) );
   si.cb = sizeof(si);
   ZeroMemory( &pi, sizeof(pi) );

  // start the program up
  CreateProcess( lpApplicationName,   // the path
	argv[1],        // Command line
	NULL,           // Process handle not inheritable
	NULL,           // Thread handle not inheritable
	FALSE,          // Set handle inheritance to FALSE
	0,              // No creation flags
	NULL,           // Use parent's environment block
	NULL,           // Use parent's starting directory
	&si,            // Pointer to STARTUPINFO structure
	&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);
	// Close process and thread handles.
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
}
*/