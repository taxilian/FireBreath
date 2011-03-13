/**********************************************************\ 
Original Author: Richard Bateman (taxilian)

Created:    Feb 23, 2011
License:    Dual license model; choose one of two:
            New BSD License
            http://www.opensource.org/licenses/bsd-license.php
            - or -
            GNU Lesser General Public License, version 2.1
            http://www.gnu.org/licenses/lgpl-2.1.html

Copyright 2011 Richard Bateman, Firebreath development team
\**********************************************************/

#include "axSharedContainer.h"
#include "ActiveXBrowserHost.h"

using namespace FB::ActiveX;

void axSharedContainer::releaseAll()
{
    m_spClientSite.Release();
    m_serviceProvider.Release();
    m_webBrowser.Release();
    m_htmlDocDisp.Release();
    m_htmlDoc.Release();
    m_htmlWin.Release();
    m_htmlWinDisp.Release();
    // TODO: Do we really need to do this? I thought we did when I started, but I've
    // been thinking about it and we probably don't. I'll keep it here for now and 
    // just not call it, but need to pull it out before I merge back into trunk
    // unless it turns out that we do need it.
    ActiveXBrowserHostPtr host(m_weakHost.lock());
    if (host) {
        for (std::list<IDispatchShareablePtr>::iterator it = m_IDispatchObjects.begin();
            it != m_IDispatchObjects.end(); ++it) {
                // Make sure we release the objects with the host
                host->deferred_release((*it)->getPtr());
                m_IDispatchObjects.erase(it++);
        }
    }
}