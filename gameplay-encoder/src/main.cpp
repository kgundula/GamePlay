#include "Base.h"
#include "DAESceneEncoder.h"
#include "FBXSceneEncoder.h"
#include "TTFFontEncoder.h"
#include "GPBDecoder.h"
#include "EncoderArguments.h"

using namespace gameplay;

/**
 * Prompts the user for a font size until a valid font size is entered.
 * 
 * @return A valid font size.
 */
static unsigned int promptUserFontSize()
{
    static const int lowerBound = 8;
    static const int upperBound = 500;
    unsigned int fontSize = 0;
    char buffer[80];
    do
    {
        printf("Enter font size (between %d and %d):\n", lowerBound, upperBound);
        std::cin.getline(buffer, 80);
        int i = atoi(buffer);
        if (i >= lowerBound && i <= upperBound)
        {
            fontSize = (unsigned int)i;
        }
    } while (fontSize == 0);
    return fontSize;
}

/**
 * Main application entry point.
 *
 * @param argc The number of command line arguments
 * @param argv The array of command line arguments.
 *
 * usage:   gameplay-encoder[options] <file_list>
 * example: gameplay-encoder C:/assets/seymour.dae
 * example: gameplay-encoder -i boy seymour.dae
 *
 * @stod: Improve argument parsing.
 */
int main(int argc, const char** argv)
{
    EncoderArguments arguments(argc, argv);

    if (arguments.parseErrorOccured())
    {
        arguments.printUsage();
        return 0;
    }

    // Check if the file exists.
    if (!arguments.fileExists())
    {
        LOG(1, "Error: File not found: %s\n", arguments.getFilePathPointer());
        return -1;
    }

    // File exists
    LOG(1, "Encoding file: %s\n", arguments.getFilePathPointer());

    switch (arguments.getFileFormat())
    {
    case EncoderArguments::FILEFORMAT_DAE:
        {
            std::string realpath(arguments.getFilePath());
            DAESceneEncoder daeEncoder;
            daeEncoder.write(realpath, arguments);
            break;
        }
    case EncoderArguments::FILEFORMAT_FBX:
        {
#ifdef USE_FBX
            std::string realpath(arguments.getFilePath());
            FBXSceneEncoder fbxEncoder;
            fbxEncoder.write(realpath, arguments);
            break;
#else
            LOG(1, "Error: FBX not enabled. Install the FBX SDK and use the preprocessor definition USE_FBX.\n");
            return -1;
#endif
        }
    case EncoderArguments::FILEFORMAT_TTF:
        {
            unsigned int fontSize = arguments.getFontSize();
            if (fontSize == 0)
            {
                fontSize = promptUserFontSize();
            }
            std::string id = getBaseName(arguments.getFilePath());
            writeFont(arguments.getFilePath().c_str(), arguments.getOutputFilePath().c_str(), fontSize, id.c_str(), arguments.fontPreviewEnabled());
            break;
        }
    case EncoderArguments::FILEFORMAT_GPB:
        {
            std::string realpath(arguments.getFilePath());
            GPBDecoder decoder;
            decoder.readBinary(realpath);
            break;
        }
   default:
        {
            LOG(1, "Error: Unsupported file format: %s\n", arguments.getFilePathPointer());
            return -1;
        }
    }

    return 0;
}
