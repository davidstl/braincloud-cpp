//
//  XMLHTTPRequestLoader.h
//  GameClientLib
//

#ifndef _XMLHTTPREQUESTLOADER_H_
#define _XMLHTTPREQUESTLOADER_H_

#include "braincloud/internal/URLLoader.h"

#include <atomic>
#include <mutex>

#include <atlbase.h>
#include <comutil.h>
#include <msxml6.h>

namespace BrainCloud
{
    class XMLHTTPRequestLoader : public URLLoader
    {
    public:
        // Constructors
        XMLHTTPRequestLoader();

        // Destructor
        virtual ~XMLHTTPRequestLoader();

        // Override the pure virtual methods.
        virtual void    close();
        virtual void    load(URLRequest const &);
        virtual void    load(URLRequest const * r) { if (r != NULL) load(*r); }
        virtual bool    isDone();

        /*
         Time in milliseconds that you allow the libcurl transfer operation to take.
         Normally, name lookups can take a considerable time and limiting operations to less than a few minutes
         risk aborting perfectly normal operations. This option will cause curl to use the SIGALRM to enable time-outing
         system calls.

         If libcurl is built to use the standard system name resolver,
         that portion of the transfer will still use full-second resolution for timeouts with a minimum timeout
         allowed of one second. (Added in 7.16.2)
         */
        virtual void setTimeout(int milliseconds) { _timeoutInterval = milliseconds; }

    protected:

    private:
        static void* loadThread(void*);
        static void loadThreadXMLHTTPRequest(XMLHTTPRequestLoader*);

        static long _timeoutInterval;

        std::atomic<bool> _threadRunning;
        std::mutex _requestMutex;
        CComPtr<IXMLHTTPRequest> _request;
    };
}

#endif  // _XMLHTTPREQUESTLOADER_H_
