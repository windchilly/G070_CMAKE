#include "SGUI_Common.h"

void SGUI_Common_AdaptDisplayInfo(SGUI_RECT* pstDisplayArea, SGUI_POINT* pstInnerPos)
{
    if((NULL != pstDisplayArea) && (NULL != pstInnerPos))
    {
        if(SGUI_RECT_X_START(*pstDisplayArea) < 0)
        {
            SGUI_RECT_X_START(*pstInnerPos) += SGUI_RECT_X_START(*pstDisplayArea);
            SGUI_RECT_WIDTH(*pstDisplayArea) += SGUI_RECT_X_START(*pstDisplayArea);
            SGUI_RECT_X_START(*pstDisplayArea) = 0;
        }
        if(SGUI_RECT_Y_START(*pstDisplayArea) < 0)
        {
            SGUI_RECT_Y_START(*pstInnerPos) += SGUI_RECT_Y_START(*pstDisplayArea);
            SGUI_RECT_HEIGHT(*pstDisplayArea) += SGUI_RECT_Y_START(*pstDisplayArea);
            SGUI_RECT_Y_START(*pstDisplayArea) = 0;
        }
    }
}

SGUI_INT SGUI_Common_IntegerToStringWithDecimalPoint(SGUI_INT iInteger, SGUI_INT iDecimalPlaces, SGUI_SZSTR pszStringBuffer, SGUI_INT iAlignment, SGUI_CHAR cFillCharacter)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_CHAR                   szNumberString[SGUI_NUMBER_STR_LENGTH_MAX] = {0x00};
    SGUI_UINT                   uiSourceNumber;
    SGUI_INT                    iSignBit;
    SGUI_INT                    iNumberStringLength;
    SGUI_INT                    iDecimalLength;
    SGUI_INT                    iOutputLength;
    SGUI_SZSTR                  pcSrcPtr;
    SGUI_SZSTR                  pcDestPtr;

    // Judged sign
    if(iInteger < 0)
    {
        iSignBit = 1;
        uiSourceNumber = -iInteger;
    }
    else
    {
        iSignBit = 0;
        uiSourceNumber = iInteger;
    }

    // Insert dot to string.
    if(iDecimalPlaces > 0)
    {
        iDecimalLength = iDecimalPlaces;
        // Convert number to string, ignore sign.
        iNumberStringLength = SGUI_Common_IntegerToString(uiSourceNumber, szNumberString, 10, 0, cFillCharacter);
        // Set pointer.
        pcSrcPtr = szNumberString + iNumberStringLength - 1;
        if(iDecimalLength < iNumberStringLength)
        {
            iOutputLength = iNumberStringLength + 1;
        }
        else
        {
            iOutputLength = iDecimalLength + 2;
        }
        pcDestPtr = pszStringBuffer + iOutputLength + iSignBit;
        // Process decimal string character.
        *pcDestPtr = '\0';
        pcDestPtr--;
        while((pcSrcPtr >= szNumberString) && (iDecimalLength > 0))
        {
            *pcDestPtr = *pcSrcPtr;
            pcDestPtr--;
            pcSrcPtr--;
            iDecimalLength--;
        }
        while(iDecimalLength > 0)
        {
            *pcDestPtr = '0';
            pcDestPtr--;
            iDecimalLength --;
        }
        *pcDestPtr = '.';
        pcDestPtr--;
        // Process integer string character.
        if(iDecimalPlaces >= iNumberStringLength)
        {
            *pcDestPtr = '0';
            pcDestPtr--;
        }
        else
        {
            while(pcSrcPtr >= szNumberString)
            {
                *pcDestPtr = *pcSrcPtr;
                pcDestPtr--;
                pcSrcPtr--;
            }
        }
        if(iSignBit > 0)
        {
            *pcDestPtr = '-';
        }
        // Alignment
        iOutputLength = iOutputLength + iSignBit;
        if(iAlignment > 0)
        {
            pcSrcPtr = pszStringBuffer + iOutputLength - 1;
            if(iOutputLength > iAlignment)
            {
                iAlignment = iOutputLength;
            }
            else
            {
                iOutputLength = iAlignment;
            }
            pcDestPtr = pszStringBuffer + iAlignment;
            *pcDestPtr = '\0';
            pcDestPtr--;

            while(pcDestPtr >= pszStringBuffer)
            {
                if(pcSrcPtr >= pszStringBuffer)
                {
                    *pcDestPtr = *pcSrcPtr;
                    pcSrcPtr--;
                }
                else
                {
                    *pcDestPtr = cFillCharacter;
                }
                pcDestPtr--;
            }
        }
        else
        {
            iAlignment = -iAlignment;
            pcSrcPtr = pszStringBuffer;
            pcDestPtr = pszStringBuffer + iOutputLength;
            if(iOutputLength > iAlignment)
            {
                iAlignment = 0;
            }
            else
            {
                iAlignment = iAlignment - iOutputLength;
                iOutputLength = iOutputLength + iAlignment;
            }
            while(iAlignment --)
            {
                *pcDestPtr = cFillCharacter;
                pcDestPtr ++;
            }
            *pcDestPtr = '\0';
        }

    }
    else
    {
        // Convert number to string.
        iOutputLength = SGUI_Common_IntegerToString(iInteger, pszStringBuffer, 10, iAlignment, cFillCharacter);
    }

    return iOutputLength;
}

SGUI_SIZE SGUI_Common_IntegerToString(SGUI_INT iInteger, SGUI_SZSTR pszStringBuffer, SGUI_UINT uiBase, SGUI_INT iAlignment, SGUI_CHAR cFillCharacter)
{
    SGUI_CHAR                   szNumberString[SGUI_NUMBER_STR_LENGTH_MAX] = {0x00};
    SGUI_UINT                   uiSourceNumber;
    SGUI_UINT                   uiSignBit;
    SGUI_INT                    iNumberStringLength;
    SGUI_SZSTR                  pcSrcPtr;
    SGUI_SZSTR                  pcDestPtr;
    SGUI_SIZE                   uiDigitBit;

    pcSrcPtr = szNumberString + SGUI_NUMBER_STR_LENGTH_MAX - 1;
    pcDestPtr = szNumberString;
    *pcSrcPtr = '\0';
    pcSrcPtr--;
    iNumberStringLength = 0;
    // Judged sign
    if(iInteger < 0)
    {
        if(uiBase == 16)
        {
            uiSignBit = 0;
            uiSourceNumber = (SGUI_UINT)iInteger;
        }
        else
        {
            uiSignBit = 1;
            uiSourceNumber = -iInteger;
        }
    }
    else
    {
        uiSignBit = 0;
        uiSourceNumber = (SGUI_UINT)iInteger;
    }

    // Convert number to string.
    while(uiSourceNumber != 0)
    {
        uiDigitBit = uiSourceNumber % uiBase;
        if(uiDigitBit < 10)
        {
            *pcSrcPtr = uiDigitBit + '0';
        }
        else
        {
            *pcSrcPtr = uiDigitBit - 10 + 'A';
        }
        uiSourceNumber = uiSourceNumber / uiBase;
        pcSrcPtr--;
        iNumberStringLength++;
    }
    if(uiSignBit == 1)
    {
        *pcSrcPtr = '-';
        iNumberStringLength++;
    }
    else
    {
        pcSrcPtr++;
    }

    while((pcDestPtr - iNumberStringLength) != szNumberString)
    {
        *pcDestPtr = *pcSrcPtr;
        pcSrcPtr++;
        pcDestPtr++;
    }
    if(iNumberStringLength == 0)
    {
        pcDestPtr = szNumberString;
        *pcDestPtr = '0';
        pcDestPtr++;
        iNumberStringLength++;
    }
    *pcDestPtr = '\0';
    // Move to out put buffer
    if(iAlignment > 0)
    {
        pcSrcPtr = szNumberString + iNumberStringLength-1;
        if(iNumberStringLength > iAlignment)
        {
            iAlignment = iNumberStringLength;
        }
        else
        {
            iNumberStringLength = iAlignment;
        }
        pcDestPtr = pszStringBuffer + iAlignment;
        *pcDestPtr = '\0';
        pcDestPtr--;

        while(pcDestPtr >= pszStringBuffer)
        {
            if(pcSrcPtr >= szNumberString)
            {
                *pcDestPtr = *pcSrcPtr;
                pcSrcPtr--;
            }
            else
            {
                *pcDestPtr = cFillCharacter;
            }
            pcDestPtr--;
        }
    }
    else
    {
        iAlignment = -iAlignment;
        if(iNumberStringLength > iAlignment)
        {
            iAlignment = iNumberStringLength;
        }
        else
        {
            iNumberStringLength = iAlignment;
        }
        pcDestPtr = pszStringBuffer;
        pcSrcPtr = szNumberString;
        while(iAlignment > 0)
        {
            if(*pcSrcPtr != '\0')
            {
                *pcDestPtr = *pcSrcPtr;
                pcSrcPtr++;
            }
            else
            {
                *pcDestPtr = cFillCharacter;
            }
            pcDestPtr++;
            iAlignment--;
        }
        *pcDestPtr = '\0';
    }
    return iNumberStringLength;
}

SGUI_UINT SGUI_Common_ConvertStringToUnsignedInteger(SGUI_SZSTR szString, SGUI_CHAR** ppcEndPointer, SGUI_UINT uiBase)
{
    SGUI_UINT32                 uiResult;
    SGUI_UINT                   uiBitValue;
    SGUI_SZSTR                  pcCurPtr;

    uiResult =              0;
    pcCurPtr =              szString;

    if(NULL != pcCurPtr)
    {
        // Skip header space.
        while(' ' == *pcCurPtr)
        {
            pcCurPtr++;
        }
        // Convert string.
        if(uiBase == 16)
        {
            if((*(pcCurPtr) == '0') && ((*(pcCurPtr+1) == 'x') || (*(pcCurPtr+1) == 'X')))
            {
                pcCurPtr += 2;
            }
            while(('\0' != (*pcCurPtr)) && (true == SGUI_ISHEXDIGIT(*pcCurPtr)))
            {
                uiBitValue = SGUI_ISDIGIT(*pcCurPtr)?((*pcCurPtr)-'0'):(10+SGUI_TOUPPER(*pcCurPtr)-'A');
                uiResult = uiResult*uiBase + uiBitValue;
                pcCurPtr++;
            }
        }
        else if((uiBase == 8) || (uiBase == 10))
        {
            while(('\0' != (*pcCurPtr)) && (true == SGUI_ISDIGIT(*pcCurPtr)))
            {
                uiBitValue =(*pcCurPtr)-'0';
                uiResult = uiResult*uiBase + uiBitValue;
                pcCurPtr++;
            }
        }
    }
    if(NULL != ppcEndPointer)
    {
        *ppcEndPointer = pcCurPtr;
    }
    return uiResult;
}

SGUI_INT SGUI_Common_ConvertStringToInteger(SGUI_SZSTR szString, SGUI_CHAR** ppcEndPointer, SGUI_UINT uiBase)
{
    SGUI_INT                    iResult;
    SGUI_INT                    iSign;
    SGUI_SZSTR                  pcCurPtr;

    iResult =               0;
    iSign =                 1;
    pcCurPtr =              szString;

    if(NULL != pcCurPtr)
    {
        // Skip header space.
        while(' ' == *pcCurPtr)
        {
            pcCurPtr++;
        }
        // Check sign.
        if((10 == uiBase) || (8 == uiBase))
        {
            if('+' == *pcCurPtr)
            {
                pcCurPtr++;
            }
            else if('-' == *pcCurPtr)
            {
                pcCurPtr++;
                iSign = -1;
            }
        }
        // Convert string without sign.
        iResult = SGUI_Common_ConvertStringToUnsignedInteger(pcCurPtr, ppcEndPointer, uiBase);
        if((iResult > 0) && (-1 == iSign))
        {
            iResult = -iResult;
        }
    }
    return iResult;
}

