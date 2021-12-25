#if __linux__
	#define OS_PLATFORM_LINUX
#elif TARGET_OS_MAC
	#define OS_PLATFORM_MAC
#elif defined _WIN32 || defined _WIN64
	#define OS_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <sstream>
	#include <VersionHelpers.h> // Requires windows 8.1 sdk at least
#endif

#include "VersionInfo.h"

std::string VersionInfo::GetVersionInfo() {
#if defined(OS_PLATFORM_LINUX)
	return std::string("Linux");
#elif defined(TARGET_OS_MAC)
	return std::string("Mac OS X");
#elif defined(OS_PLATFORM_WINDOWS)
	
	std::string windowsVersion;

	if (IsWindowsXPOrGreater() && !IsWindowsVistaOrGreater()) {
		windowsVersion = "Windows XP | NucetoSpades 0.5 | https://github.com/Nuceto/NucetoSpades/releases";
	} else if (IsWindowsVistaOrGreater() && !IsWindows7OrGreater()) {
		windowsVersion = "Windows Vista | NucetoSpades 0.5 | https://github.com/Nuceto/NucetoSpades/releases";
	} else if (IsWindows7OrGreater() && !IsWindows8OrGreater()) {
		windowsVersion = "Windows 7 | NucetoSpades 0.5 | https://github.com/Nuceto/NucetoSpades/releases";
	} else if (IsWindows8OrGreater() && !IsWindows8Point1OrGreater()) {
		windowsVersion = "Windows 8 | NucetoSpades 0.5 | https://github.com/Nuceto/NucetoSpades/releases";
	} else if (IsWindows8Point1OrGreater()) {
		windowsVersion = "Windows 8.1 | NucetoSpades 0.5 | https://github.com/Nuceto/NucetoSpades/releases";
	} else {
		// Default to Windows 10
		// See https://github.com/yvt/openspades/pull/528 for reason.
		windowsVersion = "Windows 10 | NucetoSpades 0.5 | https://github.com/Nuceto/NucetoSpades/releases";
	}

	// Might be a greater version, but the new Microsoft
	// API doesn't support checking for specific versions.

	if (IsWindowsServer())
		windowsVersion += " Server | NucetoSpades 0.5 | https://github.com/Nuceto/NucetoSpades/releases";
	return windowsVersion;
#elif defined(__FreeBSD__)
	return std::string("FreeBSD | NucetoSpades 0.5 | https://github.com/Nuceto/NucetoSpades/releases";
#elif defined(__OpenBSD__)
	return std::string("OpenBSD | NucetoSpades 0.5 | https://github.com/Nuceto/NucetoSpades/releases";
#else
	return std::string("Unknown | NucetoSpades 0.5 | https://github.com/Nuceto/NucetoSpades/releases";
#endif
}
