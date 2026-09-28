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
#include "xconsoloutput.h"
#include "xcolorstring.h"

static void printMessage(xx_terminal_stream_t stream, const QString &sLabel, const QString &sColor, const QString &sMessage)
{
    XColorString::CONSOLE_STATE state = XColorString::initConsole(stream);
    XColorString colorString;
    colorString.addPart(sLabel, sColor);
    colorString.addSpace();
    colorString.addPart(sMessage);
    colorString.addPart("\n");
    colorString.printConsole(&state);
    XColorString::finishConsole(state);
}

XConsoleOutput::XConsoleOutput(QObject *pParent) : QObject(pParent)
{
}

void XConsoleOutput::errorMessage(const QString &sMessage)
{
    printMessage(XX_TERMINAL_STDERR, "[ERROR]", "red", sMessage);
}

void XConsoleOutput::warningMessage(const QString &sMessage)
{
    printMessage(XX_TERMINAL_STDOUT, "[WARNING]", "yellow", sMessage);
}

void XConsoleOutput::infoMessage(const QString &sMessage)
{
    printMessage(XX_TERMINAL_STDOUT, "[INFO]", "green", sMessage);
}
