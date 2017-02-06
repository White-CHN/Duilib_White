// TortoiseSVN - a Windows shell extension for easy version control

// Copyright (C) 2003-2006, 2013 - TortoiseSVN

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "stdafx.h"
#include "stdex_vector.h"
#include "DIB.h"

CDib::CDib()
{
    m_pBits = NULL;
    m_hBitmap = NULL;
    ZeroMemory(&m_BMinfo, sizeof(m_BMinfo));
    DeleteObject();
}

CDib::~CDib()
{
    DeleteObject();
}

int CDib::BytesPerLine(int nWidth, int nBitsPerPixel)
{
    int nBytesPerLine = nWidth * nBitsPerPixel;
    nBytesPerLine = ((nBytesPerLine + 31) & (~31)) / 8;
    return nBytesPerLine;
}

void CDib::DeleteObject()
{
    m_pBits = NULL;
    if(m_hBitmap)
    {
        ::DeleteObject(m_hBitmap);
    }
    m_hBitmap = NULL;

    SecureZeroMemory(&m_BMinfo, sizeof(m_BMinfo));
}

void CDib::Create32BitFromPicture(HDC pDc, int x, int y, int iWidth, int iHeight)
{
    HDC tempDC = CreateCompatibleDC(pDc);
    HBITMAP hBmp = CreateCompatibleBitmap(pDc, iWidth, iHeight);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(tempDC, hBmp);
    StretchBlt(tempDC, 0, 0, iWidth, iHeight, pDc, x, y, iWidth, iHeight, SRCCOPY);

    stdex::vector<DWORD> pBits(iWidth * iHeight);

    BITMAPINFO bi;
    bi.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth         = iWidth;
    bi.bmiHeader.biHeight        = iHeight;
    bi.bmiHeader.biPlanes        = 1;
    bi.bmiHeader.biBitCount      = 32;
    bi.bmiHeader.biCompression   = BI_RGB;
    bi.bmiHeader.biSizeImage     = 0;
    bi.bmiHeader.biXPelsPerMeter = 0;
    bi.bmiHeader.biYPelsPerMeter = 0;
    bi.bmiHeader.biClrUsed       = 0;
    bi.bmiHeader.biClrImportant  = 0;


    SetBitmap(&bi, pBits);

    DWORD* pAr = (DWORD*)GetDIBits();

    // Copy data into the 32 bit dib..
    for(int i = 0; i < iHeight; i++)
    {
        for(int j = 0; j < iWidth; j++)
        {
            pAr[(i * iWidth) + j] = FixColorRef(GetPixel(tempDC, j, iHeight - i - 1));
        }
    }
    ::SelectObject(tempDC, hOldBmp);
    ::DeleteObject(hBmp);
    ::DeleteDC(tempDC);
}

BOOL CDib::SetBitmap(const LPBITMAPINFO lpBitmapInfo, const LPVOID lpBits)
{
    DeleteObject();

    if(!lpBitmapInfo || !lpBits)
    {
        return FALSE;
    }

    HDC hDC = NULL;

    DWORD dwBitmapInfoSize = sizeof(BITMAPINFO);

    memcpy(&m_BMinfo, lpBitmapInfo, dwBitmapInfoSize);

    hDC = ::GetDC(NULL);
    if(!hDC)
    {
        DeleteObject();
        return FALSE;
    }

    m_hBitmap = CreateDIBSection(hDC, &m_BMinfo,
                                 DIB_RGB_COLORS, &m_pBits, NULL, 0);
    ::ReleaseDC(NULL, hDC);
    if(!m_hBitmap)
    {
        DeleteObject();
        return FALSE;
    }

    DWORD dwImageSize = m_BMinfo.bmiHeader.biSizeImage;
    if(dwImageSize == 0)
    {
        int nBytesPerLine = BytesPerLine(lpBitmapInfo->bmiHeader.biWidth,
                                         lpBitmapInfo->bmiHeader.biBitCount);
        dwImageSize = nBytesPerLine * lpBitmapInfo->bmiHeader.biHeight;
    }

    GdiFlush();

    memcpy(m_pBits, lpBits, dwImageSize);

    return TRUE;
}

BOOL CDib::Draw(HDC pDC, POINT ptDest)
{
    if(!m_hBitmap)
    {
        return FALSE;
    }

    SIZE size = GetSize();
    POINT SrcOrigin = {0};

    BOOL resVal = SetDIBitsToDevice(pDC,
                                    ptDest.x, ptDest.y,
                                    size.cx, size.cy,
                                    SrcOrigin.x, SrcOrigin.y,
                                    SrcOrigin.y, size.cy - SrcOrigin.y,
                                    GetDIBits(), &m_BMinfo,
                                    DIB_RGB_COLORS);

    return resVal;
}

COLORREF CDib::FixColorRef(COLORREF clr)
{
    int r = GetRValue(clr);
    int g = GetGValue(clr);
    int b =  GetBValue(clr);

    return RGB(b, g, r);
}

