#ifndef GENEBITS_ENGINE_DEBUG_COLOR_PRINT_H
#define GENEBITS_ENGINE_DEBUG_COLOR_PRINT_H

#include <iostream>
#include <string_view>

namespace genebits::engine::debug
{
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

///
/// Print the color to the termimal (stdout).
///
/// @param[in] color Color to print.
///
void PrintTerminalColor(TColor color);

///
/// Returns whether the standard output is a terminal that supports colored output.
///
/// @return True if the stdout terminal supports color.
///
bool IsColorTerminal();

} // namespace genebits::engine::debug

#endif
