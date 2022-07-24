#include "genebits/engine/debug/color_print.h"

#include <cstdlib>

#include "genebits/engine/config/compiler.h"

#if PLATFORM_WINDOWS
#include <io.h>
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h> // for displaying colors
#else
#include <unistd.h>
#endif

namespace genebits::engine::debug
{
#if PLATFORM_WINDOWS

[[nodiscard]] constexpr WORD GetWindowsColorCode(TColor color) noexcept
{
  switch (color)
  {
  case TColor::Black: return 0;
  case TColor::DarkBlue: return 1;
  case TColor::DarkGreen: return 2;
  case TColor::LightBlue: return 3;
  case TColor::DarkRed: return 4;
  case TColor::Magenta: return 5;
  case TColor::Orange: return 6;
  case TColor::LightGray: return 7;
  case TColor::Gray: return 8;
  case TColor::Blue: return 9;
  case TColor::Green: return 10;
  case TColor::Cyan: return 11;
  case TColor::Red: return 12;
  case TColor::Pink: return 13;
  case TColor::Yellow: return 14;
  case TColor::White: return 15;
  default: return 7;
  }
}
#else

constexpr const char* GetANSIColorCode(TColor color) noexcept
{
  switch (color)
  {
  case TColor::Black: return "30";
  case TColor::DarkBlue: return "34";
  case TColor::DarkGreen: return "32";
  case TColor::LightBlue: return "36";
  case TColor::DarkRed: return "31";
  case TColor::Magenta: return "35";
  case TColor::Orange: return "33";
  case TColor::LightGray: return "37";
  case TColor::Gray: return "90";
  case TColor::Blue: return "94";
  case TColor::Green: return "92";
  case TColor::Cyan: return "96";
  case TColor::Red: return "91";
  case TColor::Pink: return "95";
  case TColor::Yellow: return "93";
  case TColor::White: return "97";
  default: return "37";
  }
}

#endif

TColor GetDefaultTerminalColor()
{
  return TColor::LightGray;
}

void PrintTerminalColor(TColor color)
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
} // namespace genebits::engine::debug