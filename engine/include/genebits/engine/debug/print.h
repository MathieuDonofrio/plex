#ifndef GENEBITS_ENGINE_DEBUG_PRINT_H
#define GENEBITS_ENGINE_DEBUG_PRINT_H

#include <string>
#include <string_view>

namespace genebits::engine
{
///
/// Enumerator for terminal colors.
///
/// Order of TColor enumerators try to match the order of colors for MSVC.
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

///
/// Prints the string view to the console output (stdout).
///
/// @param[in] string The string to print.
///
void Print(std::string_view string);

///
/// Prints the string to the console output (stdout).
///
/// @param[in] string The string to print.
///
void Print(const std::string& string);

///
/// Prints the character to the console output (stdout).
///
/// @param[in] character The character to print.
///
void Print(char character);

///
/// Prints the color code to the console output (stdout) to set the color.
///
/// @param[in] color The color to print/set.
///
void PrintColor(TColor color);

///
/// Prints the default color code to the console output (stdout) to reset the color.
///
void PrintColorReset();

///
/// Prints a new line ('\n') to the console output (stdout).
///
void PrintLine();

///
/// Flushes the console output (stdout).
///
void PrintFlush();

} // namespace genebits::engine

#endif
