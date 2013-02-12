/**
 * Copyright (c) 2010-2011 Andrey AndryBlack Kunitsyn
 * email:support.andryblack@gmail.com
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

#include "mmexporter.h"
#include "../fontconfig.h"
#include "../layoutconfig.h"
#include "../layoutdata.h"

#include <QVector>
#include <QString>
MMExporter::MMExporter(QObject *parent):
    AbstractExporter(parent)
{
    setExtension("xml");
}

bool MMExporter::Export(QByteArray &out) {

    // line 1
    QString res = QObject::tr("info face=\"%1\" ").arg(fontConfig()->family());
    res += QObject::tr("bold=%1 ").arg(fontConfig()->bold());
    res += QObject::tr("italic=%1 ").arg(fontConfig()->italic());
    res += QObject::tr("charset=\"\" ");
    res += QObject::tr("unicode=1 ");
    res += QObject::tr("stretchH=%1 ").arg((int)fontConfig()->width());
    res += QObject::tr("smooth=%1 ").arg(fontConfig()->antialiased()?1:0);
    res += QObject::tr("aa=4 ");
    res += QObject::tr("padding=%1,%2,%3,%4 ").arg(layoutConfig()->offsetTop())
            .arg(layoutConfig()->offsetRight()).arg(layoutConfig()->offsetBottom())
            .arg(layoutConfig()->offsetLeft());
    res += QObject::tr("spacing=%1,%2 ").arg(fontConfig()->lineSpacing())
            .arg(fontConfig()->charSpacing());
    res += QObject::tr("outline=0\n");

    // line 2
    res += QObject::tr("common lineHeight=36 base=29 ");
    res += QObject::tr("scaleW=%1 ").arg(texWidth());
    res += QObject::tr("scaleH=%1 ").arg(texHeight());
    res += QObject::tr("pages=1 packed=0 alphaChnl=0 redChnl=4 greenChnl=4 blueChnl=4\n");

    // line 3
    res += QObject::tr("page id=0 file=\"%1\"\n").arg(texFilename());

    // line 4
    res += QObject::tr("chars count=%1\n").arg(symbols().size());

    foreach (const Symbol& c , symbols()) {
        QString chardef = QObject::tr("char id=%1 ").arg(c.id, -5);
        chardef += QObject::tr("x=%1 ").arg(c.placeX, -5);
        chardef += QObject::tr("y=%1 ").arg(c.placeY, -5);
        chardef += QObject::tr("width=%1 ").arg(c.placeW, -5);
        chardef += QObject::tr("height=%1 ").arg(c.placeH, -5);
        chardef += QObject::tr("xoffset=%1 ").arg(c.offsetX, -5);
        chardef += QObject::tr("yoffset=%1 ").arg(c.offsetY, -5);
        chardef += QObject::tr("xadvance=%1 ").arg(c.advance, -5);
        chardef += QObject::tr("page=0  chnl=15\n");

        res += chardef;

    }

    out = res.toUtf8();
    return true;

}

AbstractExporter* MMExporterFactoryFunc (QObject* parent) {
    return new MMExporter(parent);
}
