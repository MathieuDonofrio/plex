#include "plex/os/color_print.h"

#include <cstdio>

#include "plex/config/compiler.h"

#if PLATFORM_WINDOWS
#include <io.h>
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h> // for displaying colors
#else
#include <unistd.h>
#endif

namespace plex::terminal
{
#if PLATFORM_WINDOWS

[[nodiscard]] constexpr WORD GetWindowsColorCode(Color color) noexcept
{
  switch (color)
  {
  case Color::Black: return 0;
  case Color::DarkBlue: return 1;
  case Color::DarkGreen: return 2;
  case Color::LightBlue: return 3;
  case Color::DarkRed: return 4;
  case Color::Magenta: return 5;
  case Color::Orange: return 6;
  case Color::LightGray: return 7;
  case Color::Gray: return 8;
  case Color::Blue: return 9;
  case Color::Green: return 10;
  case Color::Cyan: return 11;
  case Color::Red: return 12;
  case Color::Pink: return 13;
  case Color::Yellow: return 14;
  case Color::White: return 15;
  default: return 7;
  }
}
#else

constexpr const char* GetANSIColorCode(TerminalColor color) noexcept
{
  switch (color)
  {
  case Color::Black: return "30";
  case Color::DarkBlue: return "34";
  case Color::DarkGreen: return "32";
  case Color::LightBlue: return "36";
  case Color::DarkRed: return "31";
  case Color::Magenta: return "35";
  case Color::Orange: return "33";
  case Color::LightGray: return "37";
  case Color::Gray: return "90";
  case Color::Blue: return "94";
  case Color::Green: return "92";
  case Color::Cyan: return "96";
  case Color::Red: return "91";
  case Color::Pink: return "95";
  case Color::Yellow: return "93";
  case Color::White: return "97";
  default: return "37";
  }
}

#endif

void PrintTerminalColor(Color color)
{
#if PLATFORM_WINDOWS
  static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  fflush(stdout);
  SetConsoleTextAttribute(handle, GetWindowsColorCode(color));
#else
  fwrite("\033[", sizeof(char), 3, stdout);
  fwrite(GetANSIColorCode(color), sizeof(char), 2, stdout);
  putc('m', stdout);
#endif
}

bool IsColorTerminal()
{
#if PLATFORM_WINDOWS
  return 0 != _isatty(_fileno(stdout));
#else
  // For non-Windows platforms use the TERM variable.
  // This list is copied from Google Test.
  constexpr std::string_view supported_term_values[] = {
    "xterm",
    "xterm-color",
    "xterm-256color",
    "screen",
    "screen-256color",
    "tmux",
    "tmux-256color",
    "rxvt-unicode",
    "rxvt-unicode-256color",
    "linux",
    "cygwin",
  };

  const char* const term = std::getenv("TERM");

  bool term_supports_color = false;

  if (term)
  {
    for (auto candidate : supported_term_values)
    {
      if (candidate == term)
      {
        term_supports_color = true;
        break;
      }
    }
  }

  return 0 != isatty(fileno(stdout)) && term_supports_color;
#endif
}
} // namespace plex::terminal