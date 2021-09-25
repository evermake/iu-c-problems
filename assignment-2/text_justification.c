#include <stdio.h>
#include <stdbool.h>

#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"
#define MAX_SYMBOLS 300
#define MAX_WORD_LENGTH 20


void concat(char *text, size_t *textLength, const char *string) {
    size_t k = 0;
    char c;
    while ((c = string[k++]) != '\0')
        text[(*textLength)++] = c;
}


void justify_text(
        char (*words)[MAX_WORD_LENGTH + 1],
        const size_t *wordLengths,
        size_t wordsCount,
        char *text,
        size_t maxWidth
) {
    size_t textLength = 0;
    size_t wordsWrittenCount = 0;
    while (wordsCount > wordsWrittenCount) {
        size_t wordsRemainCount = wordsCount - wordsWrittenCount;

        size_t wordsFitCount = 1;
        size_t rowWidth = wordLengths[wordsWrittenCount];
        // Calculate how many words will fit the row
        while (
            // ... + wordsFitCount -> because we need spaces between words
                rowWidth + wordLengths[wordsWrittenCount + wordsFitCount] + wordsFitCount <= maxWidth
                && wordsFitCount < wordsRemainCount
                )
            rowWidth += wordLengths[wordsWrittenCount + wordsFitCount++];

        bool isLastRow = (wordsWrittenCount + wordsFitCount) >= wordsCount;
        bool isOnlyOneWord = wordsFitCount == 1;

        size_t freeSpace = maxWidth - rowWidth;
        if (isLastRow) {
            for (size_t i = wordsWrittenCount; i < wordsWrittenCount + wordsFitCount; ++i) {
                concat(text, &textLength, words[i]);
                text[textLength++] = ' ';
            }
            text[textLength - 1] = '\n';
            wordsWrittenCount += wordsFitCount;
        } else if (isOnlyOneWord) {
            concat(text, &textLength, words[wordsWrittenCount]);

            for (size_t i = 0; i < freeSpace; ++i)
                text[textLength++] = ' ';
            text[textLength++] = '\n';
            ++wordsWrittenCount;
        } else {
            for (size_t i = 0; i < wordsFitCount; ++i) {
                concat(text, &textLength, words[i + wordsWrittenCount]);

                size_t gapsRemainCount = wordsFitCount - i - 1;

                // Add spaces if gaps remain
                if (gapsRemainCount) {
                    size_t gapSize = freeSpace / gapsRemainCount;
                    gapSize += (freeSpace % gapsRemainCount) && 1;

                    for (size_t j = 0; j < gapSize; ++j)
                        text[textLength++] = ' ';

                    freeSpace -= gapSize;
                }
            }
            text[textLength++] = '\n';
            wordsWrittenCount += wordsFitCount;
        }
    }
}


int main() {
    FILE *fi, *fo;

    if ((fi = fopen(INPUT_FILE, "r")) == NULL) {
        printf("Failed to open input file.");
        return 1;
    }

    if ((fo = fopen(OUTPUT_FILE, "w")) == NULL) {
        printf("Failed to open output file.");
        fclose(fi);
        return 1;
    }

    char words[MAX_SYMBOLS][MAX_WORD_LENGTH + 1];
    size_t wordLengths[MAX_SYMBOLS] = {0};
    size_t wordsCount = 0, wordLength = 0;
    bool isWordNow = false;
    char c;

    // Read words from file
    while ((c = (char) fgetc(fi)) != '\n') {
        if (c != ' ') {
            if (!isWordNow)
                isWordNow = true;
            words[wordsCount][wordLength++] = c;
        } else if (isWordNow) {
            isWordNow = false;
            wordLengths[wordsCount++] = wordLength;
            wordLength = 0;
        }
    }
    if (isWordNow)
        wordLengths[wordsCount++] = wordLength;

    // Read maximum row width
    size_t maxWidth;
    fscanf(fi, "%lu", &maxWidth);

    char text[MAX_SYMBOLS] = {0};
    justify_text(words, wordLengths, wordsCount, text, maxWidth);

    // Write result to output file
    fprintf(fo, "%s", text);

    fclose(fi);
    fclose(fo);
}

