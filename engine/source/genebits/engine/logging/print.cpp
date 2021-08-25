#include "genebits/engine/debug/print.h"

#include <cstdio>

#include "genebits/engine/config/compiler.h"

#if PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h> // for displaying colors
#endif

namespace genebits::engine
{
[[nodiscard]] constexpr decltype(auto) TColorId(const TColor Color) noexcept
{
  switch (Color)
  {
#if PLATFORM_WINDOWS
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
#else
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
#endif
  }
}

void Print(const std::string& string)
{
  fwrite(string.data(), sizeof(char), string.size(), stdout);
}

void Print(const std::string_view string)
{
  fwrite(string.data(), sizeof(char), string.size(), stdout);
}

void Print(const char character)
{
  putc(character, stdout);
}

void PrintColor(const TColor color)
{
#if PLATFORM_WINDOWS
  static const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(handle, static_cast<WORD>(TColorId(color)));
#else
  fwrite("\033[", sizeof(char), 3, stdout);
  fwrite(TColorId(color), sizeof(char), 2, stdout);
  putc('m', stdout);
#endif
}

void PrintColorReset()
{
  PrintColor(TColor::LightGray); // Light gray is the default
}

void PrintLine()
{
  putc('\n', stdout);
}

void PrintFlush()
{
  fflush(stdout);
}
} // namespace genebits::engine
