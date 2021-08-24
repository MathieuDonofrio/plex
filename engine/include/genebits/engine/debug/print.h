#ifndef GENEBITS_ENGINE_DEBUG_CONSOLE_COLOR_H
#define GENEBITS_ENGINE_DEBUG_CONSOLE_COLOR_H

#include <string>
#include <string_view>

namespace genebits::engine
{
///
/// Enumerator for terminal colors.
///
enum class TColor
{
  Black,
  DarkBlue,
  DarkGreen,
  LightBlue,
  DarkRed,
  Magenta,
  Orange,
  LightGray,
  Gray,
  Blue,
  Green,
  Cyan,
  Red,
  Pink,
  Yellow,
  White
};

void Print(const std::string& string);

void Print(const std::string_view string);

void Print(const char character);

void PrintColor(const TColor color);

void PrintColorReset();

void PrintLine();

void PrintFlush();

} // namespace genebits::engine

#endif
