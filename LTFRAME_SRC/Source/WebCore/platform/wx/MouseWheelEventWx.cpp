/*
 * Copyright (C) 2007 Kevin Ollivier <kevino@theolliviers.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "PlatformWheelEvent.h"

#include "Scrollbar.h"
#include <wtf/CurrentTime.h>

#include <wx/defs.h>
#include <wx/event.h>

namespace WebCore {

PlatformWheelEvent::PlatformWheelEvent(const wxMouseEvent& event, const wxPoint& globalPoint)
    : PlatformEvent(PlatformEvent::Wheel, event.ShiftDown(), event.ControlDown(), event.AltDown(), event.MetaDown(), WTF::currentTime())
    , m_position(event.GetPosition())
    , m_globalPosition(globalPoint)
    , m_granularity(ScrollByPixelWheelEvent)
    , m_deltaX(0) // wx doesn't support horizontal mouse wheel scrolling
    , m_deltaY(event.GetWheelRotation() / event.GetWheelDelta())
    , m_wheelTicksX(m_deltaX)
    , m_wheelTicksY(m_deltaY)
    , m_directionInvertedFromDevice(false)
{
    // FIXME: retrieve the user setting for the number of lines to scroll on each wheel event
    m_deltaY *= static_cast<float>(Scrollbar::pixelsPerLineStep());
}

}
