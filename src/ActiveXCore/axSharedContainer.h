#pragma once
#ifndef axSharedContainer_h__
#define axSharedContainer_h__

/**********************************************************\ 
Original Author: Richard Bateman (taxilian)

Created:    Feb 23, 2011
License:    Dual license model; choose one of two:
            New BSD License
            http://www.opensource.org/licenses/bsd-license.php
            - or -
            GNU Lesser General Public License, version 2.1
            http://www.gnu.org/licenses/lgpl-2.1.html

Copyright 2010 Richard Bateman, Firebreath development team
\**********************************************************/

#include "win_common.h"
#include "win_targetver.h"
#include <atlctl.h>
#include <boost/enable_shared_from_this.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "FBPointers.h"
#include "ShareableReference.h"
#include <shlguid.h>

namespace FB { namespace ActiveX {

    FB_FORWARD_PTR(ActiveXBrowserHost);
    typedef FB::ShareableReference<IDispatch> IDispatchShareable;
    typedef boost::shared_ptr<IDispatchShareable> IDispatchShareablePtr;
    typedef boost::weak_ptr<IDispatchShareable> IDispatchShareableWeakPtr;

    FB_FORWARD_PTR(axSharedContainer);
    class axSharedContainer : public boost::enable_shared_from_this<axSharedContainer>, boost::noncopyable {
    public:
        void releaseAll()
        {
            m_spClientSite.Release();
            m_serviceProvider.Release();
            m_webBrowser.Release();
            m_htmlDocDisp.Release();
            m_htmlDoc.Release();
            m_htmlWin.Release();
            m_htmlWinDisp.Release();
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
        void release(const IDispatchShareablePtr& obj)
        {
            // This one is called by the browserhost, so we don't need to call deferred_release
            // it has already been called
            m_IDispatchObjects.remove(obj);
        }
        void setSite(CComQIPtr<IOleClientSite>& site)
        {
            m_spClientSite = site;
            m_serviceProvider = site;
            m_serviceProvider->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, reinterpret_cast<void**>(&m_webBrowser));
            if (m_webBrowser) {
                m_webBrowser->get_Document(&m_htmlDocDisp);
                m_htmlDoc = m_htmlDocDisp;
                m_htmlDoc->get_parentWindow(&m_htmlWin);
                m_htmlWinDisp = m_htmlWin;
            }
        }
    private:
        ActiveXBrowserHostWeakPtr m_weakHost;
        std::list<IDispatchShareablePtr> m_IDispatchObjects;
        CComQIPtr<IServiceProvider> m_serviceProvider;
        CComQIPtr<IWebBrowser2> m_webBrowser;
    	CComQIPtr<IOleClientSite> m_spClientSite;
        CComQIPtr<IHTMLDocument2> m_htmlDoc;
        CComQIPtr<IDispatch> m_htmlDocDisp;
        CComQIPtr<IHTMLWindow2> m_htmlWin;
        CComQIPtr<IDispatch> m_htmlWinDisp;
    };
}}
#endif // axSharedContainer_h__
