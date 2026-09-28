/* Copyright (c) 2026 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "xcolorstring.h"
#include <cstdio>

#ifdef QT_GUI_LIB
#include <QColor>
#include <QFontMetricsF>
#endif

XColorString::XColorString()
{
}

XColorString::~XColorString()
{
}

XColorString::CONSOLE_STATE XColorString::initConsole()
{
    return initConsole(XX_TERMINAL_STDOUT);
}

XColorString::CONSOLE_STATE XColorString::initConsole(xx_terminal_stream_t stream)
{
    CONSOLE_STATE state = {};
    state.terminalState = xx_terminal_init(stream);
    state.nOriginalMode = state.terminalState.original_mode;
    state.nCurrentMode = state.terminalState.current_mode;
    state.bIsValid = state.terminalState.valid;
    state.bIsEscapeMode = state.terminalState.type == XX_TERMINAL_TYPE_ANSI;
    state.bIsWinNativeMode = state.terminalState.type == XX_TERMINAL_TYPE_WINDOWS;
    return state;
}

void XColorString::finishConsole(const CONSOLE_STATE &consoleState)
{
    std::fflush(consoleState.terminalState.stream == XX_TERMINAL_STDERR ? stderr : stdout);
    xx_terminal_finish(&consoleState.terminalState);
}

void XColorString::addPart(const QString &sText, const QString &sColorMain, const QString &sColorBackground)
{
    PART part = {};
    part.sText = sText;
    part.colorRecord.sColorMain = sColorMain;
    part.colorRecord.sColorBackground = sColorBackground;

    m_vecParts.append(part);
}

void XColorString::addSpace()
{
    addPart(" ");
}

void XColorString::addString(quint32 nGroupID, const QString &sString)
{
    XOptions::COLOR_RECORD colorRecord = {};
    bool bFound = false;

    qint32 nNumberOfRules = m_lstRules.count();

    for (qint32 i = 0; i < nNumberOfRules; i++) {
        if (m_lstRules.at(i).nGroupID == nGroupID) {
            bool bMatch = false;

            if (m_lstRules.at(i).sString.isEmpty()) {
                bMatch = true;
            } else if (m_lstRules.at(i).bIsCaseSensitive) {
                bMatch = (m_lstRules.at(i).sString == sString);
            } else {
                bMatch = (m_lstRules.at(i).sString.toLower() == sString.toLower());
            }

            if (bMatch) {
                colorRecord = m_lstRules.at(i).colorRecord;
                bFound = true;
                break;
            }
        }
    }

    if (bFound) {
        addPart(sString, colorRecord.sColorMain, colorRecord.sColorBackground);
    } else {
        addPart(sString);
    }
}

void XColorString::addRule(quint32 nGroupID, const QString &sString, const XOptions::COLOR_RECORD &colorRecord, bool bIsCaseSensitive)
{
    RULE rule = {};
    rule.nGroupID = nGroupID;
    rule.sString = sString;
    rule.colorRecord = colorRecord;
    rule.bIsCaseSensitive = bIsCaseSensitive;

    m_lstRules.append(rule);
}

void XColorString::addRule(quint32 nGroupID, const QString &sString, const QString &sColorMain, const QString &sColorBackground, bool bIsCaseSensitive)
{
    XOptions::COLOR_RECORD colorRecord = {};
    colorRecord.sColorMain = sColorMain;
    colorRecord.sColorBackground = sColorBackground;

    addRule(nGroupID, sString, colorRecord, bIsCaseSensitive);
}

QString XColorString::toPlainText()
{
    QString sResult;

    qint32 nNumberOfParts = m_vecParts.count();

    for (qint32 i = 0; i < nNumberOfParts; i++) {
        sResult += m_vecParts.at(i).sText;
    }

    return sResult;
}

void XColorString::printConsole(CONSOLE_STATE *pConsoleState)
{
    if (!pConsoleState) {
        return;
    }

    const xx_terminal_state *pTerminalState = &pConsoleState->terminalState;
    // The C backend writes directly on Windows. Keep earlier CRT output in order.
    std::fflush(pTerminalState->stream == XX_TERMINAL_STDERR ? stderr : stdout);
    bool bColorOutput = xx_is_color_output_enabled();
    qint32 nNumberOfParts = m_vecParts.count();

    for (qint32 i = 0; i < nNumberOfParts; i++) {
        const PART &part = m_vecParts.at(i);
        RGB_COLOR colorMain = parseColor(part.colorRecord.sColorMain);
        RGB_COLOR colorBg = parseColor(part.colorRecord.sColorBackground);
        bool bHasColor = bColorOutput && (colorMain.bValid || colorBg.bValid);
        bool bEscapeColor = bHasColor && pConsoleState->bIsEscapeMode;
        bool bNativeColor = false;
        uint16_t nOriginalAttributes = 0;

        if (bEscapeColor) {
            QByteArray baEscape = QString("\033[%1;%2m").arg(colorToAnsiCode(colorMain, false)).arg(colorToAnsiCode(colorBg, true)).toLatin1();
            xx_terminal_write(pTerminalState, baEscape.constData(), (size_t)baEscape.size());
        } else if (bHasColor && pConsoleState->bIsWinNativeMode && xx_terminal_get_attributes(pTerminalState, &nOriginalAttributes)) {
            quint16 nAttributes = nOriginalAttributes;
            if (colorMain.bValid) {
                nAttributes = (nAttributes & ~0x000F) | colorToConsoleAttribute(colorMain, false);
            }
            if (colorBg.bValid) {
                nAttributes = (nAttributes & ~0x00F0) | colorToConsoleAttribute(colorBg, true);
            }
            bNativeColor = xx_terminal_set_attributes(pTerminalState, nAttributes);
        }

        QByteArray baText = part.sText.toUtf8();
        xx_terminal_write(pTerminalState, baText.constData(), (size_t)baText.size());

        if (bEscapeColor) {
            xx_terminal_print(pTerminalState, "\033[0m");
        } else if (bNativeColor) {
            xx_terminal_set_attributes(pTerminalState, nOriginalAttributes);
        }
    }
    xx_terminal_flush(pTerminalState);
}

quint16 XColorString::colorToConsoleAttribute(const RGB_COLOR &color, bool bBackground)
{
    quint16 nAttribute = 0;
    if (color.nRed > 127) nAttribute |= 0x0004;
    if (color.nGreen > 127) nAttribute |= 0x0002;
    if (color.nBlue > 127) nAttribute |= 0x0001;
    if ((color.nRed + color.nGreen + color.nBlue) / 3 > 192) nAttribute |= 0x0008;
    return bBackground ? nAttribute << 4 : nAttribute;
}

#ifdef QT_GUI_LIB
void XColorString::draw(QPainter *pPainter, QRectF rect) const
{
    QTextOption textOption;
    textOption.setWrapMode(QTextOption::NoWrap);

    draw(pPainter, rect, textOption);
}

void XColorString::draw(QPainter *pPainter, QRectF rect, const QTextOption &textOption) const
{
    if (!pPainter) {
        return;
    }

    pPainter->save();

    qint32 nNumberOfParts = m_vecParts.count();

    for (qint32 i = 0; i < nNumberOfParts; i++) {
        const PART &part = m_vecParts.at(i);
        QRectF rectPart = rect;
        qreal dWidth = QFontMetricsF(pPainter->font()).horizontalAdvance(part.sText);
        rectPart.setWidth(dWidth);

        pPainter->save();

        if (!part.colorRecord.sColorBackground.isEmpty()) {
            pPainter->fillRect(rectPart, QColor(part.colorRecord.sColorBackground));
        }

        if (!part.colorRecord.sColorMain.isEmpty()) {
            pPainter->setPen(QColor(part.colorRecord.sColorMain));
        }

        pPainter->drawText(rectPart, part.sText, textOption);
        pPainter->restore();

        rect.setLeft(rect.left() + dWidth);
    }

    pPainter->restore();
}
#endif

XColorString::RGB_COLOR XColorString::parseColor(const QString &sColor)
{
    RGB_COLOR result = {};
    result.bValid = false;

    if (sColor.isEmpty()) {
        return result;
    }

    QString sColorLower = sColor.toLower().trimmed();

    if (sColorLower.startsWith("#")) {
        return hexToColor(sColorLower);
    }

    static QMap<QString, RGB_COLOR> mapColors;
    if (mapColors.isEmpty()) {
        mapColors["black"] = {0, 0, 0, true};
        mapColors["red"] = {255, 0, 0, true};
        mapColors["green"] = {0, 255, 0, true};
        mapColors["yellow"] = {255, 255, 0, true};
        mapColors["blue"] = {0, 0, 255, true};
        mapColors["magenta"] = {255, 0, 255, true};
        mapColors["cyan"] = {0, 255, 255, true};
        mapColors["white"] = {255, 255, 255, true};
        mapColors["gray"] = {128, 128, 128, true};
        mapColors["darkred"] = {139, 0, 0, true};
        mapColors["darkgreen"] = {0, 100, 0, true};
        mapColors["darkblue"] = {0, 0, 139, true};
        mapColors["orange"] = {255, 165, 0, true};
    }

    if (mapColors.contains(sColorLower)) {
        return mapColors[sColorLower];
    }

    return result;
}

qint32 XColorString::colorToAnsiCode(const RGB_COLOR &color, bool bBackground)
{
    if (!color.bValid) {
        return bBackground ? 49 : 39;
    }

    qint32 nBase = bBackground ? 40 : 30;
    qint32 nBrightBase = bBackground ? 100 : 90;

    qint32 nRed = color.nRed;
    qint32 nGreen = color.nGreen;
    qint32 nBlue = color.nBlue;

    bool bIsBright = (nRed + nGreen + nBlue) > 384;

    if (nRed > 200 && nGreen < 100 && nBlue < 100) {
        return bIsBright ? (nBrightBase + 1) : (nBase + 1);
    } else if (nGreen > 200 && nRed < 100 && nBlue < 100) {
        return bIsBright ? (nBrightBase + 2) : (nBase + 2);
    } else if (nBlue > 200 && nRed < 100 && nGreen < 100) {
        return bIsBright ? (nBrightBase + 4) : (nBase + 4);
    } else if (nRed > 200 && nGreen > 200 && nBlue < 100) {
        return bIsBright ? (nBrightBase + 3) : (nBase + 3);
    } else if (nRed > 200 && nBlue > 200 && nGreen < 100) {
        return bIsBright ? (nBrightBase + 5) : (nBase + 5);
    } else if (nGreen > 200 && nBlue > 200 && nRed < 100) {
        return bIsBright ? (nBrightBase + 6) : (nBase + 6);
    } else if (nRed < 50 && nGreen < 50 && nBlue < 50) {
        return nBase;
    } else if (nRed > 200 && nGreen > 200 && nBlue > 200) {
        return bIsBright ? (nBrightBase + 7) : (nBase + 7);
    }

    return bIsBright ? nBrightBase : nBase;
}

QString XColorString::colorNameToHex(const QString &sColorName)
{
    RGB_COLOR color = parseColor(sColorName);

    if (color.bValid) {
        return QString("#%1%2%3").arg(color.nRed, 2, 16, QChar('0')).arg(color.nGreen, 2, 16, QChar('0')).arg(color.nBlue, 2, 16, QChar('0'));
    }

    return QString();
}

XColorString::RGB_COLOR XColorString::hexToColor(const QString &sHex)
{
    RGB_COLOR result = {};
    result.bValid = false;

    if (sHex.isEmpty()) {
        return result;
    }

    QString sColor = sHex.trimmed();
    if (sColor.startsWith("#")) {
        sColor = sColor.mid(1);
    }

    if (sColor.length() == 6) {
        bool bOk1 = false;
        bool bOk2 = false;
        bool bOk3 = false;

        result.nRed = sColor.mid(0, 2).toInt(&bOk1, 16);
        result.nGreen = sColor.mid(2, 2).toInt(&bOk2, 16);
        result.nBlue = sColor.mid(4, 2).toInt(&bOk3, 16);
        result.bValid = bOk1 && bOk2 && bOk3;
    } else if (sColor.length() == 3) {
        bool bOk1 = false;
        bool bOk2 = false;
        bool bOk3 = false;

        result.nRed = sColor.mid(0, 1).toInt(&bOk1, 16) * 17;
        result.nGreen = sColor.mid(1, 1).toInt(&bOk2, 16) * 17;
        result.nBlue = sColor.mid(2, 1).toInt(&bOk3, 16) * 17;
        result.bValid = bOk1 && bOk2 && bOk3;
    }

    return result;
}
