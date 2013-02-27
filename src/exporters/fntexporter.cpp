/**
 * Copyright (c) 2010-2011 Stardrad Yin
 * email:yin8086@gmail.com
 *
 * Report bugs and download new versions at http://code.google.com/p/fontbuilder
 *
 * This software is distributed under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "fntexporter.h"
#include "../fontconfig.h"
#include "../layoutconfig.h"
#include "../layoutdata.h"

#include <QVector>
#include <QString>
FntExporter::FntExporter(QObject *parent):
    AbstractExporter(parent)
{
    setExtension("xml");
}

bool FntExporter::Export(QByteArray &out) {

    // line 1
    QString res = QString("info face=\"%1\" ").arg(fontConfig()->family());
    res += QString("bold=%1 ").arg(fontConfig()->bold());
    res += QString("italic=%1 ").arg(fontConfig()->italic());
    res += QString("charset=\"\" ");
    res += QString("unicode=1 ");
    res += QString("stretchH=%1 ").arg((int)fontConfig()->width());
    res += QString("smooth=%1 ").arg(fontConfig()->antialiased()?1:0);
    res += QString("aa=4 ");
    res += QString("padding=%1,%2,%3,%4 ").arg(layoutConfig()->offsetTop())
            .arg(layoutConfig()->offsetRight()).arg(layoutConfig()->offsetBottom())
            .arg(layoutConfig()->offsetLeft());
    res += QString("spacing=%1,%2 ").arg(fontConfig()->lineSpacing())
            .arg(fontConfig()->charSpacing());
    res += QString("outline=0\n");

    // line 2
    res += QString("common lineHeight=36 base=29 ");
    res += QString("scaleW=%1 ").arg(texWidth());
    res += QString("scaleH=%1 ").arg(texHeight());
    res += QString("pages=1 packed=0 alphaChnl=1 redChnl=0 greenChnl=0 blueChnl=0\n");

    // line 3
    res += QString("page id=0 file=\"%1\"\n").arg(texFilename());

    // line 4
    res += QString("chars count=%1\n").arg(symbols().size());

    int offset = metrics().ascender;
    QString kernStrs;
    int numKerns(0);
    foreach (const Symbol& c , symbols()) {
        QString chardef = QString("char id=%1 ").arg(c.id, -5);
        chardef += QString("x=%1 ").arg(c.placeX, -5);
        chardef += QString("y=%1 ").arg(c.placeY, -5);
        chardef += QString("width=%1 ").arg(c.placeW, -5);
        chardef += QString("height=%1 ").arg(c.placeH, -5);
        chardef += QString("xoffset=%1 ").arg(c.offsetX, -5);
        chardef += QString("yoffset=%1 ").arg(offset - c.offsetY, -5);
        chardef += QString("xadvance=%1 ").arg(c.advance, -5);
        chardef += QString("page=0  chnl=15\n");

        res += chardef;


        typedef QMap<ushort,int>::ConstIterator Kerning;
        for(Kerning k = c.kerning.begin(); k != c.kerning.end(); k++) {
            kernStrs += QString("kerning first=%1 ").arg(c.id);
            kernStrs += QString("second=%1 ").arg(k.key());
            kernStrs += QString("amount=%1\n").arg(k.value());
            numKerns++;
        }

    }

    if(numKerns > 0) {
        res += QString("kernings count=%1\n").arg(numKerns);
        res += kernStrs;
    }

    out = res.toUtf8();
    return true;

}

AbstractExporter* FntExporterFactoryFunc (QObject* parent) {
    return new FntExporter(parent);
}
