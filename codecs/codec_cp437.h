/* Copyright (c) 2024-2026 hors<horsicq@gmail.com>
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
#ifndef CODEC_CP437_H
#define CODEC_CP437_H
#include <QByteArray>
#include <QObject>
#include <QString>

// The IBM437 -> Unicode mapping, available in every build.  The QTextCodec
// below exists only where QTextCodec does (Qt 5, or Qt 6 with Core5Compat),
// but format parsers need the mapping regardless: ZIP stores a member name
// that is not flagged UTF-8 in CP437.
quint16 codec_cp437_toUnicode(quint8 nByte);

// Decode a whole buffer.
//
// bStrictAscii keeps 0x00-0x7F exactly as they are, which is what a stored
// file name needs.  The table deliberately remaps 0x1A, 0x1C and 0x7F the way
// a DOS terminal displayed them, and rewriting those bytes inside a name would
// corrupt it.
QString codec_cp437_decode(const QByteArray &baData, bool bStrictAscii = true);

#if (QT_VERSION_MAJOR < 6) || defined(QT_CORE5COMPAT_LIB)
#include <QTextCodec>

class codec_cp437 : public QTextCodec {
public:
    codec_cp437() = default;

    QByteArray name() const;
    QList<QByteArray> aliases() const;
    int mibEnum() const;

protected:
    QString convertToUnicode(const char *in, int length, ConverterState *state) const;
    QByteArray convertFromUnicode(const QChar *in, int length, ConverterState *state) const;
};
#endif
#endif  // CODEC_CP437_H
