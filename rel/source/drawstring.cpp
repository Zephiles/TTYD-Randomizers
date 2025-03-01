#include "drawstring.h"

#include <ttyd/string.h>
#include <ttyd/fontmgr.h>

#include <cstdint>

namespace drawstring
{
    // Replace the first \n found with \0, and return the index of the character after that
    uint32_t getNextLineIndex(char *str)
    {
        // Set the initial index
        uint32_t i = 0;

        // Loop through the string until \n or \0 is found
        while ((str[i] != '\n') && (str[i] != '\0'))
        {
            i++;
        }

        // Replace \n with \0 and increment the index to the next line
        if (str[i] != '\0')
        {
            str[i] = '\0';
            i++;
        }

        // Return the index of the next line
        // Returns 0 when at the end of the string
        return i;
    }

    void drawStringMultiline(float x, float y, const char *text)
    {
        // Copy the text to a temporary array, as it will be modified
        int32_t textSize = ttyd::string::strlen(text);
        char tempText[textSize + 1];
        ttyd::string::strcpy(tempText, text);

        // Get the index for the next line
        uint32_t index = getNextLineIndex(tempText);

        // Draw the first line
        char *currentLine = tempText;

        do
        {
            // Only draw the line if not pointing to an empty string
            // This will only occur if multiple newlines were directly next to each other
            if (currentLine[0] != '\0')
            {
                ttyd::fontmgr::FontDrawString(x, y, currentLine);
            }

            // Set currentLine to the next line
            currentLine += index;

            // Get the following line index
            index = getNextLineIndex(currentLine);

            // Implement the new line space
            y -= 20;
        } while (index != 0);
    }
} // namespace drawstring
