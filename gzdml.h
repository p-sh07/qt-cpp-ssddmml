#pragma once

#include <windows.h>
#include <Atlconv.h>


#include <fstream>
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
using std::ofstream;
using std::ifstream;
using std::filesystem::path;

namespace fs = std::filesystem;

const path DFLT_DCG_PATH = "mod_options.dgc";
const path DFLT_GZDOOM = "gzdoom";
const path DFLT_CONFIG = DFLT_GZDOOM / "gzdoom_portable.ini";
const std::string DEFAULT_COMMAND = "-config gzdoom_portable.ini -file mods\\SIGIL_v1_21.wad mods\\SIGIL_SHREDS.wad mods\\SIGIL_II_V1_0.WAD mods\\sigil2_ost_thorr.WAD mods\\D64TEX.PK3 mods\\DoomMetalVol5.wad";

struct DooM
{
	std::string label = "Default";

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
		if (fs::current_path().string() == "C:\\Projects\\Qt\\gzdml") {
			cerr << "!changed curr path" << endl;
			fs::current_path(path{ "C:\\Projects\\Qt\\gzdml\\x64\\Debug" });

			bool is_first_launch = CheckIfFirstLaunch();
			file_writer_.open(DFLT_DCG_PATH.string());

			//for a first launch, output an example config
			if (is_first_launch) {

			}
		}

		//set labels for testing
		launch_configs_[0].label = "DOOM";
		launch_configs_[1].label = "DOOM II";
		launch_configs_[2].label = "Some Awesome Mod";
		launch_configs_[3].label = "Brutal DooM Sigil I&II";
		launch_configs_[4].label = "Lost in Darkness";

	}
	GZDML(const path& gzdoom_dir);

	DooM ParseStringInput(std::string_view in);

	void LoadModLaunchConfig(path config_file);
	void SaveModLaunchConfig(path config_file);

	// TODO: use latest id? add new? use back()?
	void AddMod(const path& wad_pk3);

	void AddGamefromParams(const path& iwad, const path& mod, const path& config = DFLT_CONFIG);
	void AddGamefromParams(const path& iwad, const std::vector<path>& mods, const path& config = DFLT_CONFIG);

	void AddADoom(DooM lp);

	void SetGZDoomDir();

	//returns the mod label
	std::string_view SetPrevMod() {
		//loop to end
		if (selected_mod_id_ == 0) {
			selected_mod_id_ = launch_configs_.size()-1;
		}
		else {
			--selected_mod_id_;
		}
		return launch_configs_[selected_mod_id_].label;
	}
	//returns the mod label
	std::string_view SetNextMod() {
		//loop to beginning
		if (selected_mod_id_ >= launch_configs_.size() - 1) {
			selected_mod_id_ = 0;
		}
		else {
			++selected_mod_id_;
		}
		return launch_configs_[selected_mod_id_].label;
	}


	//There is always a default launch config present
	void LaunchGame(size_t id = 0) {
		//cerr << "Attempting to launch: " << gzdoomexe.string() 
		//	 << " curr_path: " << fs::current_path()
		//	 << endl;
		cout << "\nLaunching Mod_ID #" << selected_mod_id_ << endl;
		try {
			PerformDefaultLaunch();
		}
		catch (std::exception& ex) {
			cout << "Error during launch: " << ex.what() << endl;
		}
	}

private:
//PARAMS
	size_t selected_mod_id_ = 0;

	path gzdoomexe_full = "gzdoom/gzdoom.exe";
	path gzdoomexe = "gzdoom.exe";
	path gzdoompath = "gzdoom/";

	path mod_config = DFLT_DCG_PATH;
	//TODO: "fname is: " gzdoomexe_full.filename().string() 
	// " and path is: " gzdoomexe_full.remove_filename() - not working atm

	std::ofstream file_writer_;

	std::vector<DooM> launch_configs_{ 5 };

//METHODS
	bool CheckIfFirstLaunch() {
		//auto dir_it = fs::directory_iterator(fs::current_path());
		//cerr << "looking for cfg:";
		//for (const auto& entry : dir_it) {
		//	if (entry.is_regular_file() && entry.path().filename() == DFLT_DCG_PATH) {

		//	}
		//}
		return !fs::exists(DFLT_DCG_PATH);
	}
	//looks for the gzdoom exe and sets the path variables
	void InitGZDoomPaths();
	//creates all the neccessary directories
	void SetupFolders(); 
	//makes a launch command special string from a std::string or a DooM-config
	LPCWSTR MakeLaunchCommand(const std::string& launch_command) {
		return std::wstring(launch_command.begin(), launch_command.end()).c_str();
	}

	//Tested - working
	void PerformDefaultLaunch() {
		ShellExecuteW(NULL, NULL, gzdoomexe.c_str(), MakeLaunchCommand(DEFAULT_COMMAND), gzdoompath.c_str(), SW_SHOWDEFAULT);

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