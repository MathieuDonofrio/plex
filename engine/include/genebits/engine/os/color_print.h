#ifndef GENEBITS_ENGINE_OS_COLOR_PRINT_H
#define GENEBITS_ENGINE_OS_COLOR_PRINT_H

namespace genebits::engine::terminal
{
enum class Color
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
/// Print the color to the terminal (stdout).
///
/// @param[in] color Color to print.
///
void PrintTerminalColor(Color color);

///
/// Returns whether the standard output is a terminal that supports colored output.
///
/// @return True if the stdout terminal supports color.
///
bool IsColorTerminal();

} // namespace genebits::engine::terminal

#endif
