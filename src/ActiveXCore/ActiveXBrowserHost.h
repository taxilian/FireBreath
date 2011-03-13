/**********************************************************\ 
Original Author: Richard Bateman (taxilian)

Created:    Oct 30, 2009
License:    Dual license model; choose one of two:
            New BSD License
            http://www.opensource.org/licenses/bsd-license.php
            - or -
            GNU Lesser General Public License, version 2.1
            http://www.gnu.org/licenses/lgpl-2.1.html

Copyright 2009 Richard Bateman, Firebreath development team
\**********************************************************/

#pragma once
#ifndef H_ACTIVEXBROWSERHOST
#define H_ACTIVEXBROWSERHOST

#include "win_common.h"
//#include "global/COM_config.h"
#include <atlctl.h>
#include <map>
#include "BrowserHost.h"
#include "APITypes.h"
#include "FBPointers.h"
#include "SafeQueue.h"
#include "ShareableReference.h"
#include "axSharedContainer.h"
#include "ActiveXFactoryDefinitions.h"

namespace FB {
    class WinMessageWindow;
    namespace ActiveX {
        FB_FORWARD_PTR(ActiveXBrowserHost);
        FB_FORWARD_PTR(IDispatchAPI);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @class  ActiveXBrowserHost
        ///
        /// @brief  Provides a BrowserHost implementation for ActiveX
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        class ActiveXBrowserHost :
            public FB::BrowserHost
        {
        public:
            ActiveXBrowserHost(const axSharedContainerPtr& axObjects);
            virtual ~ActiveXBrowserHost(void);
            virtual bool _scheduleAsyncCall(void (*func)(void *), void *userData) const;

            virtual void *getContextID() const;

            virtual FB::BrowserStreamPtr _createStream(const std::string& url, const FB::PluginEventSinkPtr& callback, 
                                                    bool cache = true, bool seekable = false, 
                                                    size_t internalBufferSize = 128 * 1024 ) const;

            IDispatchEx* getJSAPIWrapper(const FB::JSAPIWeakPtr& api, bool autoRelease = false);

            virtual FB::BrowserStreamPtr _createPostStream(const std::string& url, const FB::PluginEventSinkPtr& callback, 
                                                    const std::string& postdata, bool cache = true, bool seekable = false, 
                                                    size_t internalBufferSize = 128 * 1024 ) const;

        public:
            FB::DOM::DocumentPtr getDOMDocument();
            FB::DOM::WindowPtr getDOMWindow();
            FB::DOM::ElementPtr getDOMElement();
            void evaluateJavaScript(const std::string &script);
            void shutdown();

        public:
            FB::DOM::WindowPtr _createWindow(const FB::JSObjectPtr& obj) const;
            FB::DOM::DocumentPtr _createDocument(const FB::JSObjectPtr& obj) const;
            FB::DOM::ElementPtr _createElement(const FB::JSObjectPtr& obj) const;
            FB::DOM::NodePtr _createNode(const FB::JSObjectPtr& obj) const;

        protected:
            void initDOMObjects(); // This is const so that getDOMDocument/Window can be
            mutable FB::DOM::WindowPtr m_window;
            mutable FB::DOM::DocumentPtr m_document;
            boost::scoped_ptr<FB::WinMessageWindow> m_messageWin;
            const axSharedContainerPtr& m_axObjects;

        private:
            mutable boost::shared_mutex m_xtmutex;
            mutable FB::SafeQueue<IDispatch*> m_deferredObjects;
            typedef std::map<void*, FB::WeakIDispatchRef> IDispatchRefMap;
            mutable IDispatchRefMap m_cachedIDispatch;

        public:
            FB::variant getVariant(const VARIANT *cVar);
            void getComVariant(VARIANT *dest, const FB::variant &var);
            void deferred_release( IDispatch* obj ) const;
            void deferred_release( const IDispatchShareableWeakPtr& weak ) const;
            void DoDeferredRelease() const;
        };
    }
}

#endif // H_ACTIVEXBROWSERHOST

